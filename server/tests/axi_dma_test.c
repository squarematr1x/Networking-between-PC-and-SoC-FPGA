#include <stdio.h>
#include "../platform.h"
#include "xil_printf.h"
#include "../axi_dma.h"

#define N_CASES 10
#define P_LEN 1444

void axi_dma_test() {
	u32* test_in  = (u32*)RX_BUFFER_BASE;
	u32* test_out = (u32*)TX_BUFFER_BASE;
	u32 expected[N_CASES];
	int n_passed = 0;

	axi_dma_init();

	// ========== Test cases ==========
	// All zeros
	test_out[0] = 0x00000000;
	// One zero (G channel)
	test_out[1] = 0xA33200A2;
	// All ones
	test_out[2] = 0x01010101;
	// Single channel is one (B channel)
	test_out[3] = 0xAF01213E;
	// All 255
	test_out[4] = 0xFFFFFFFF;
	// One channel 255 (R channel)
	test_out[5] = 0x781202FF;
	// All channel values between 0..255
	test_out[6] = 0xFFA3EEA9;
	// G-channel overflow
	test_out[7] = 0xFF02FAE9;
	// B-channel overflow
	test_out[8] = 0xFFCE0335;
	// Negative integer value as input
	test_out[9] = -9913;

	// ======= Expected values ========
	expected[0] = 0xFF000000;
	expected[1] = 0xFF9600A2;
	expected[2] = 0xFF030201;
	expected[3] = 0xFF03423E;
	expected[4] = 0xFFFDFEFF;
	expected[5] = 0xFF3604FF;
	expected[6] = 0xFFE9DCA9;
	expected[7] = 0xFF06F4E9;
	expected[8] = 0xFF6A0635;
	expected[9] = 0xFFFDB247;

	axi_dma_send_to_stream(P_LEN);
	axi_dma_receive_from_stream();
	axi_dma_reset();

	// ======= Compare values =========
	for (int i = 0; i < N_CASES; i++) {
		if (test_in[i] == expected[i]) {
			n_passed++;
			xil_printf("Test #%d passed.\r\n", i);
		} else
			xil_printf("Test #%d failed: 0x%08lx != 0x%08lx\r\n", i, test_in[i], expected[i]);
	}

	xil_printf("\n%d/%d test passed.\r\n", n_passed, N_CASES);
}

