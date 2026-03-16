#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"
#include "sleep.h"
#include "max11311.h"


int main()
{
	xil_printf("max11311_test\n\r");
	
	u32 BaseAddress = SPI_BASEADDR;
	u32 Control;

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
