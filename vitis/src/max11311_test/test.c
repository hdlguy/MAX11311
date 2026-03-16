#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"
#include "sleep.h"
#include "max11311.h"


int main()
{
	xil_printf("\n\rmax11311_test\n\r");
	
	u32 Control;

	// enable SPI master mode.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_MASTER_MODE_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);


	// configure temperature monitors
	max11311_write(0, 0x18, 0x0003);
	
	// device control
	max11311_write(0, 0x10, 0x0143);
	
	// port[0:5] configuration
	max11311_write(0, 0x22, 0x7100);
	max11311_write(0, 0x23, 0x7100);
	max11311_write(0, 0x24, 0x7100);
	max11311_write(0, 0x25, 0x7100);
	max11311_write(0, 0x26, 0x7100);
	max11311_write(0, 0x27, 0x7100);
	
	// port[6:11] configuration
	max11311_write(0, 0x2b, 0x7100);
	max11311_write(0, 0x2c, 0x7100);
	max11311_write(0, 0x2d, 0x7100);
	max11311_write(0, 0x2e, 0x7100);
	max11311_write(0, 0x2f, 0x7100);
	max11311_write(0, 0x30, 0x7100);

		
	uint16_t rval, adcval[12];
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

		adcval[0] = max11311_read(0, 0x42);
		adcval[0] = max11311_read(0, 0x43);
		adcval[0] = max11311_read(0, 0x44);
		adcval[0] = max11311_read(0, 0x45);
		adcval[0] = max11311_read(0, 0x46);
		adcval[0] = max11311_read(0, 0x47);
		
		adcval[0] = max11311_read(0, 0x4b);
		adcval[0] = max11311_read(0, 0x4c);
		adcval[0] = max11311_read(0, 0x4d);
		adcval[0] = max11311_read(0, 0x4e);
		adcval[0] = max11311_read(0, 0x4f);
		adcval[0] = max11311_read(0, 0x50);

		for (int i=0; i<12; i++) { xil_printf("0x%04x ", adcval[i]);} xil_printf("\n\r");
		
		usleep(1000000);
		whilecount++;
	}
	
	xil_printf("rval = 0x%04x\n\r", rval);

}	
