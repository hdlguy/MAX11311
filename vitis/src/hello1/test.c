#include <stdlib.h>
#include <stdint.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xgpio_l.h"
#include "sleep.h"

int main()
{
    // get pointers to WDT and GPIO
    uint32_t* gpio_ptr = (uint32_t *) XPAR_XGPIO_0_BASEADDR;

    xil_printf("\n\rHello World!\n\r");

    uint32_t whilecount=0;
    while(1) {

        xil_printf("0x%08x: hello\n\r", whilecount);

        // increment the LEDs
        gpio_ptr[XGPIO_DATA_OFFSET/4] = 0x00ff & whilecount;    

        // delay for about 1 second
        //for(int i=0; i<10000000; i++); 
        usleep(1000000);

        whilecount++;
    }
    
    return 0;
}

