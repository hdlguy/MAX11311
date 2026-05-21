#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"
#include "sleep.h"
#include "max11311.h"
// This little program runs on the Microblaze and accesses the MAX11311 on the MAX11311PMB# Peripheral Module.
// It configures the part and then periodically reads the ID and internal temperature.
// The ports are externally jumpered P0->P1, ... P10->P11. 
// Ports P0, P2, and P4 are configured as DACs. P1, P3, and P5 are configured as ADCs. 
// Those DACs are written incrementing values and the ADCs read back.
// Ports P6, P8, and P10 are configured as GPO. Ports P7, P9, and P11 are configured as GPI.
// Incrementing values are written to those three GPO ports.

int main()
{
	xil_printf("\n\r\n\rmax11311_test\n\r");
	
	u32 Control;

	// enable SPI master mode.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control |= XSP_CR_MASTER_MODE_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);

	// configure temperature monitors
	max11311_write(0, 0x18, 0x0003);
	
	// write device control
	max11311_write(0, 0x10, 0x8000); // reset
	max11311_write(0, 0x10, 0x0143);
	
	// port[0:5] configuration, 0x5100=DAC, 0x7100=ADC
	max11311_write(0, 0x22, 0x5100);
	max11311_write(0, 0x23, 0x7100);
	max11311_write(0, 0x24, 0x5100);
	max11311_write(0, 0x25, 0x7100);
	max11311_write(0, 0x26, 0x5100);
	max11311_write(0, 0x27, 0x7100);
	
    // first we have to set the dac values for the gpo and gpi, 0x500 makes the GPO = 3.3V.
	max11311_write(0, 0x6b, 0x500);
	max11311_write(0, 0x6c, 0x500);
	max11311_write(0, 0x6d, 0x500);
	max11311_write(0, 0x6e, 0x500);
	max11311_write(0, 0x6f, 0x500);
	max11311_write(0, 0x70, 0x500);
    usleep(1000);

	// port[6:11] configuration, 0x3000=GPO, 0x1000=GPI, thresholds are set by DAC values
	max11311_write(0, 0x2b, 0x3100);
	max11311_write(0, 0x2c, 0x1100);
	max11311_write(0, 0x2d, 0x3100);
	max11311_write(0, 0x2e, 0x1100);
	max11311_write(0, 0x2f, 0x3100);
	max11311_write(0, 0x30, 0x1100);

		
    // loop and print values
	uint16_t rval;
	uint32_t whilecount=0;
	for(;;){
		
		// xil_printf("\n\rwhilecount = 0x%08x\n\r", whilecount);
		
		// read the device ID
		rval = max11311_read(0, 0x00);
		// xil_printf("MAX11311 ID = 0x%04x\n\r", rval);
		
		// write device control
		max11311_write(0, 0x10, 0x0143);

		// read back Device Control
		rval = max11311_read(0, 0x10);
		// xil_printf("MAX11311 Dev Control = 0x%04x\n\r", rval);

		// read the internal temperature
		rval = max11311_read(0, 0x08);
		// temp = ((int16_t)(rval*16))/16; // convert 12-bit signed word to 16-bit signed by doing an unsigned multiply 16 and then a signed divide by 16.
		// xil_printf("MAX11311 int temp = 0x%04x = %d.%dC\n\r", rval, temp/8, 125*(temp%8));
		
		usleep(100000);
		whilecount++;
	}
	
	xil_printf("rval = 0x%04x\n\r", rval);

}	

