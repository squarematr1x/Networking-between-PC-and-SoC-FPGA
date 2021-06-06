#ifndef __DMA_H_
#define __DMA_H_

#include "xil_cache.h"
#include "xil_io.h"
#include "xparameters.h"
#include "lwip/arch.h"

#ifndef DDR_BASE_ADDR

#define MEM_BASE_ADDR		0x01000000
#else
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x1000000)
#endif

#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)

#define DMA_BASE			XPAR_AXI_DMA_0_BASEADDR

#define MM2S_DMACR			(DMA_BASE + 0x00) 		// MM2S control
#define MM2S_SA				(DMA_BASE + 0x18) 		// MM2S source address
#define MM2S_SA_MSB			(DMA_BASE + 0x1C) 		// ...and MSB
#define MM2S_LENGTH			(DMA_BASE + 0x28) 		// MM2S transfer length
#define S2MM_DMACR			(DMA_BASE + 0x30) 		// S2MM control
#define S2MM_DMASR			(DMA_BASE + 0x34) 		// S2MM status
#define S2MM_DA				(DMA_BASE + 0x48) 		// S2MM destination address
#define S2MM_DA_MSB			(DMA_BASE + 0x4C) 		// ...and MSB
#define S2MM_LENGTH			(DMA_BASE + 0x58) 		// S2MM buffer length

#define MAX_IMG_SIZE (1920*1080)
#define N_CHANNELS 4

void axi_dma_init();
void axi_dma_send_to_stream(u16_t len);
void axi_dma_receive_from_stream();
void axi_dma_reset();

#endif
