#include "axi_dma.h"

void axi_dma_init() {
	// Run/stop bit to 1
	Xil_Out32(S2MM_DMACR, Xil_In32(S2MM_DMACR) | 0x1);

	// Enable interrupts here
	// ...

	// Destination address for received data:
	Xil_Out32(S2MM_DA, RX_BUFFER_BASE);

	// Start receiving
	Xil_Out32(S2MM_LENGTH, MAX_IMG_SIZE*N_CHANNELS);

	// Start mm2s channel:
	Xil_Out32(MM2S_DMACR, Xil_In32(MM2S_DMACR) | 0x1);

	// Handle interrupts here
	// ...

	// Set source address
	Xil_Out32(MM2S_SA, TX_BUFFER_BASE);
}

void axi_dma_send_to_stream(u16_t len) {
	Xil_DCacheFlush();

	// Trigger transfer by setting length to MM2S_LENGTH
	Xil_Out32(MM2S_LENGTH, len);
}

void axi_dma_receive_from_stream() {
	// Poll IRQ bit (bit 12) in S2MM to know if any data received
	while(!(Xil_In32(S2MM_DMASR) & 0x1000)) {}

	Xil_DCacheFlush();
}

void axi_dma_reset() {
	// Reset IRQ bit (12) by writing 1 to it (as in specs)
	Xil_Out32(S2MM_DMASR, Xil_In32(S2MM_DMASR) | 0x1000);
	Xil_Out32(S2MM_LENGTH, MAX_IMG_SIZE*N_CHANNELS);
}
