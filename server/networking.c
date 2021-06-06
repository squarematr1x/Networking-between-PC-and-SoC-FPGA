/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif
#include "axi_dma.h"

#define MEMSANITYCHECK 0

static u32* data_out;	// Unprocessed packet
static u32* data_in;	// Processed packet
static u32* image;		// Entire received image (can be used for histogram etc.)

static u32 last_i = 0;
static u32 total_bytes = 0;
static u32 n_images = 0;

void print_app_header() {
#if (LWIP_IPV6==0)
	xil_printf("\n\r\n\r-----lwIP TCP ICAT3170 server ------\n\r");
#endif
}

void set_buffers() {
	data_in  = (u32*)RX_BUFFER_BASE;
	data_out = (u32*)TX_BUFFER_BASE;
}

/* Append new data to the end of previously received data */
void append8(u8* raw_data, u8* new_data, u16_t n) {
	for (int i = 0; i < n; i++) {
		raw_data[last_i] = new_data[i];
		last_i++;
	}
}

/* Copy new_data to image */
void copy32(u32* data, u32* new_data, u16_t n) {
	for (int i = 0; i < n; i++) {
		data[i] = new_data[i];

	}
}

/* Convert u8 image to u32 image (not in use) */
void img8_to_img32(u32* image_data, u8* raw_data, u32 n) {
	for (int i = 0; i < n; i++) {
		int j = 4*i;
		u32 pixel = 0;

		pixel |= (u32)raw_data[j];
		pixel |= (u32)raw_data[j+1] << 8;
		pixel |= (u32)raw_data[j+2] << 16;
		pixel |= (u32)raw_data[j+3] << 24;

		image_data[i] = pixel;
	}
}

void transfer_data() {
	return;
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
	/* Do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);

		return ERR_OK;
	}

	/* Indicate that the packet has been received */
	tcp_recved(tpcb, p->len);
	total_bytes += p->len;

#ifdef __DEBUG__
	xil_printf("Info: pbuf length: %d\n\r", p->len);

	if (p->len % 4 != 0)
		xil_printf("Warning: RGBA pixel value is split between two packets!\r\n");
#endif

	copy32(data_out, (u32*)p->payload, p->len);

	/* Echo back the processed payload */
	/* In this case, we assume that the payload is < TCP_SND_BUF */
	if (tcp_sndbuf(tpcb) > p->len) {
		// Send pixels to FPGA
		axi_dma_send_to_stream(p->len);
		axi_dma_receive_from_stream();
		axi_dma_reset();

		// Send processed pixels back to PC
		err = tcp_write(tpcb, (void*)data_in, p->len, 1);
	}
	else
		xil_printf("No space in tcp_sndbuf\n\r");

	/* Last received TCP package*/
	if (p->flags) {
		// u32 image_size = total_bytes/N_CHANNELS;
		// If first image
		if (n_images == 0) {
			// Calculate histogram
			// Use variables image and image_size here?
			// ...
		}

		n_images++;
		last_i = 0;
		total_bytes = 0;
	}

	/* Free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* Set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* Just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* Increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}

int networking_start()
{
	set_buffers();
	axi_dma_init();

	Xil_ICacheEnable();
	Xil_DCacheEnable();

	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* Create new TCP PCB structure */
	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* Bind to specified @port */
	err = tcp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* We do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* Listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* Specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);


	return 0;
}
