#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"
#include "sleep.h"

#ifndef SDT
#define SPI_BASEADDR		XPAR_SPI_0_BASEADDR
#else
#define SPI_BASEADDR		XPAR_XSPI_0_BASEADDR
#endif


void max11311_write(uint8_t dev, uint8_t regnum, uint16_t val){
	uint8_t wbuf[3], rbuf[3];
	
	wbuf[0] = (regnum<<1) | (0x00); // write bit
	wbuf[1] = (val>>8) & 0x00ff; // MSB
	wbuf[2] = (val>>0) & 0x00ff; // LSB

// xil_printf("wbuf[0:2] = %02x %02x %02x\n\r", wbuf[0], wbuf[1], wbuf[2]);
// xil_printf("%02x %02x %04x\n\r", dev, regnum, val);
		
	
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[0]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[1]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[2]);
		
	XSpi_WriteReg(SPI_BASEADDR, XSP_SSR_OFFSET, 0xffffffff ^ (0x00000001<<dev));
	
	// * Enable the device.
	uint32_t Control;
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);

	while (!(XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_TX_EMPTY_MASK));

	int NumBytesRcvd = 0;
	while ((XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_RX_EMPTY_MASK) == 0) {
		rbuf[NumBytesRcvd++] = XSpi_ReadReg((SPI_BASEADDR), XSP_DRR_OFFSET);
	}

	// disable the device.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control &= ~XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);	
		
}

uint16_t max11311_read(uint8_t dev, uint8_t regnum){
	uint8_t wbuf[16], rbuf[16];
	
	wbuf[0] = (regnum << 1) | (0x01); // read bit
	wbuf[1] = 0;
	wbuf[2] = 0;
	
// xil_printf("wbuf[0:2] = %02x %02x %02x\n\r", wbuf[0], wbuf[1], wbuf[2]);
// xil_printf("%02x %02x\n\r", dev, regnum);
	
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[0]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[1]);
	XSpi_WriteReg((SPI_BASEADDR), XSP_DTR_OFFSET, wbuf[2]);
		
	XSpi_WriteReg(SPI_BASEADDR, XSP_SSR_OFFSET, 0xffffffff ^ (0x00000001<<dev));
	
	// * Enable the device.
	uint32_t Control;
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);

	while (!(XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_TX_EMPTY_MASK));

	int NumBytesRcvd = 0;
	while ((XSpi_ReadReg(SPI_BASEADDR, XSP_SR_OFFSET) & XSP_SR_RX_EMPTY_MASK) == 0) {
		rbuf[NumBytesRcvd++] = XSpi_ReadReg((SPI_BASEADDR), XSP_DRR_OFFSET);
	}
	
// xil_printf("NumBytesRcvd = %d\n\r", NumBytesRcvd);	
	
	// disable the device.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control &= ~XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);	
		
	
	uint16_t retval;
	retval = (((uint16_t)(rbuf[1])) << 8) | rbuf[2];  // MSB is in rbuf[1]. LSB is in rbuf[2].
	
	return(retval);		
}


// int XSpi_LowLevelExample() //(u32 BaseAddress)
int main()
{
	xil_printf("max11311_test\n\r");
	
	u32 BaseAddress = SPI_BASEADDR;
	u32 Control;
	// int NumBytesSent;
	// int NumBytesRcvd;
	// int Nbuf = 16;
	// uint8_t wbuf[Nbuf], rbuf[Nbuf];

	// enable master mode.
	Control = XSpi_ReadReg(BaseAddress, XSP_CR_OFFSET);
	Control |= XSP_CR_MASTER_MODE_MASK;
	XSpi_WriteReg(BaseAddress, XSP_CR_OFFSET, Control);


	// configure temperature monitors
	max11311_write(0, 0x18, 0x0003);
	// device control
	max11311_write(0, 0x10, 0x0143);

		
	uint16_t rval;
	int temp;
	uint32_t whilecount=0;
	for(;;){
		
		xil_printf("\n\rwhilecount = 0x%08x\n\r", whilecount);
		
		rval = max11311_read(0, 0x00);
		xil_printf("MAX11311 ID = 0x%04x\n\r", rval);
		
		rval = max11311_read(0, 0x10);
		xil_printf("MAX11311 Dev Control = 0x%04x\n\r", rval);

		rval = max11311_read(0, 0x08);
		temp = ((int)(rval*16))/16; // convert to signed
		xil_printf("MAX11311 int temp = 0x%04x = %d.%dC\n\r", rval, temp/8, 125*(temp%8));
		
		usleep(1000000);
		whilecount++;
	}
	
	xil_printf("rval = 0x%04x\n\r", rval);

}	


// 	// send three bytes
// 	XSpi_WriteReg((BaseAddress), XSP_DTR_OFFSET, 0xfa);
// 	XSpi_WriteReg((BaseAddress), XSP_DTR_OFFSET, 0xf3);
// 	XSpi_WriteReg((BaseAddress), XSP_DTR_OFFSET, 0x55);
// 	NumBytesSent = 3;
	

// 	// slave select	
// 	XSpi_WriteReg(BaseAddress, XSP_SSR_OFFSET, 0xfffffffe);


// 	// Enable the device.
// 	Control = XSpi_ReadReg(BaseAddress, XSP_CR_OFFSET);
// 	Control |= XSP_CR_ENABLE_MASK;
// 	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
// 	XSpi_WriteReg(BaseAddress, XSP_CR_OFFSET, Control);


// 	 // Initialize the rx buffer with zeroes so that it can be used to receive data.
// 	for (int i = 0; i < Nbuf; i++) { rbuf[i] = 0x0; }

// 	// Wait for the transmit FIFO to transition to empty before checking
// 	while (!(XSpi_ReadReg(BaseAddress, XSP_SR_OFFSET) & XSP_SR_TX_EMPTY_MASK));


// 	//now receive the data just looped back until the receiver is empty. */
// 	while ((XSpi_ReadReg(BaseAddress, XSP_SR_OFFSET) & XSP_SR_RX_EMPTY_MASK) == 0) {
// 		rbuf[NumBytesRcvd++] = XSpi_ReadReg((BaseAddress), XSP_DRR_OFFSET);
// 	}

// 	// disable the device.
// 	Control = XSpi_ReadReg(BaseAddress, XSP_CR_OFFSET);
// 	Control &= ~XSP_CR_ENABLE_MASK;
// 	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
// 	XSpi_WriteReg(BaseAddress, XSP_CR_OFFSET, Control);

// 	// If no data was sent or the data that was sent was not received, then return an error
// 	if ((NumBytesSent != NumBytesRcvd) || (NumBytesSent == 0)) {
// 		xil_printf("FAIL: NumBytesRcvd = %d\n\r", NumBytesRcvd);
// 		return XST_FAILURE;
// 	} else {
// 		xil_printf("PASS: NumBytesRcvd = %d\n\r", NumBytesRcvd);
// 		return XST_SUCCESS;
// 	}
// }
