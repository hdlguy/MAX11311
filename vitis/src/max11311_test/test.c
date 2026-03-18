#include "xparameters.h"
#include "xstatus.h"
#include "xspi_l.h"
#include "xil_printf.h"
#include "sleep.h"
#include "max11311.h"
// This little program runs on the Microblaze and accesses the MAX11311 on the PMOD eval board.
// It configures the part and then reads the ID and internal temperature once per second.
// The ports are externally jumpered P0->P1, ... P10->P11. Ports 0,2,...10 are configured as DACs and 
// P1, P3, ... P11 are configured as ADCs. The DACs are written incrementing values and the ADCs read back.

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
	
	// port[6:11] configuration
	max11311_write(0, 0x2b, 0x5100);
	max11311_write(0, 0x2c, 0x7100);
	max11311_write(0, 0x2d, 0x5100);
	max11311_write(0, 0x2e, 0x7100);
	max11311_write(0, 0x2f, 0x5100);
	max11311_write(0, 0x30, 0x7100);

		
	uint16_t rval, adcval[12], dacval[12];
	int16_t temp;
	uint32_t whilecount=0;
	for(;;){
		
		xil_printf("\n\rwhilecount = 0x%08x\n\r", whilecount);
		
		// read the device ID
		rval = max11311_read(0, 0x00);
		xil_printf("MAX11311 ID = 0x%04x\n\r", rval);
		
		// read back Device Control
		rval = max11311_read(0, 0x10);
		xil_printf("MAX11311 Dev Control = 0x%04x\n\r", rval);

		// read the internal temperature
		rval = max11311_read(0, 0x08);
		temp = ((int16_t)(rval*16))/16; // convert to signed
		xil_printf("MAX11311 int temp = 0x%04x = %d.%dC\n\r", rval, temp/8, 125*(temp%8));

		// read and print the adc values
		adcval[0] = max11311_read(0, 0x42);
		adcval[1] = max11311_read(0, 0x43);
		adcval[2] = max11311_read(0, 0x44);
		adcval[3] = max11311_read(0, 0x45);
		adcval[4] = max11311_read(0, 0x46);
		adcval[5] = max11311_read(0, 0x47);
		
		adcval[6] = max11311_read(0, 0x4b);
		adcval[7] = max11311_read(0, 0x4c);
		adcval[8] = max11311_read(0, 0x4d);
		adcval[9] = max11311_read(0, 0x4e);
		adcval[10] = max11311_read(0, 0x4f);
		adcval[11] = max11311_read(0, 0x50);

		xil_printf("ADC: ");
		for (int i=0; i<12; i++) { xil_printf("0x%04x ", adcval[i]);} xil_printf("\n\r");
		
		// write the DAC values
		for (int i=0; i<12; i++) { dacval[i] = (whilecount & 0x0fff); }
		max11311_write(0, 0x62, dacval[0]);
		max11311_write(0, 0x63, dacval[1]);
		max11311_write(0, 0x64, dacval[2]);
		max11311_write(0, 0x65, dacval[3]);
		max11311_write(0, 0x66, dacval[4]);
		max11311_write(0, 0x67, dacval[5]);		

		max11311_write(0, 0x6b, dacval[6]);
		max11311_write(0, 0x6c, dacval[7]);
		max11311_write(0, 0x6d, dacval[8]);
		max11311_write(0, 0x6e, dacval[9]);
		max11311_write(0, 0x6f, dacval[10]);
		max11311_write(0, 0x70, dacval[11]);

		// read back the DAC values and print
		dacval[0] = max11311_read(0, 0x62);
		dacval[1] = max11311_read(0, 0x63);
		dacval[2] = max11311_read(0, 0x64);
		dacval[3] = max11311_read(0, 0x65);
		dacval[4] = max11311_read(0, 0x66);
		dacval[5] = max11311_read(0, 0x67);
		
		dacval[6] = max11311_read(0, 0x6b);
		dacval[7] = max11311_read(0, 0x6c);
		dacval[8] = max11311_read(0, 0x6d);
		dacval[9] = max11311_read(0, 0x6e);
		dacval[10] = max11311_read(0, 0x6f);
		dacval[11] = max11311_read(0, 0x70);

		xil_printf("DAC: ");
		for (int i=0; i<12; i++) { xil_printf("0x%04x ", dacval[i]);} xil_printf("\n\r");
		
		usleep(2000000);
		whilecount++;
	}
	
	xil_printf("rval = 0x%04x\n\r", rval);

}	

