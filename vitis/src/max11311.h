

void max11311_write(uint8_t dev, uint8_t regnum, uint16_t val){
	uint8_t wbuf[3];
	
	wbuf[0] = (regnum<<1) | (0x00); // write bit
	wbuf[1] = (val>>8) & 0x00ff;
	wbuf[2] = (val>>0) & 0x00ff;

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
		Buffer[NumBytesRcvd++] = XSpi_ReadReg((SPI_BASEADDR), XSP_DRR_OFFSET);
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
	
	// disable the device.
	Control = XSpi_ReadReg(SPI_BASEADDR, XSP_CR_OFFSET);
	Control &= ~XSP_CR_ENABLE_MASK;
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_WriteReg(SPI_BASEADDR, XSP_CR_OFFSET, Control);	
		
	
	uint16_t retval;
	retval = (((uint16_t)(rbuf[1])) << 8) | rbuf[0];
	
	return(retval);		
}


