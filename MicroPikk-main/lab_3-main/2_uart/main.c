#include <stdint.h>
#include "uart.h"
#include "gpio.h"


int main(){
	// Configure LED Matrix
	for (int i = 17; i <= 20; i++)
	{
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	// Configure buttons -> see button_init()
	button_init();
    uart_init(); 
	int sleep = 0;

	while (1)
	{

		/* Check if button 1 is pressed;
		 * turn on LED matrix if it is. */
		if (!(GPIO->IN & (1 << 13)))
		{
			GPIO->OUT = 1 << 17;
            uart_send('A'); 
		}

		/* Check if button 2 is pressed;
		 * turn off LED matrix if it is. */
		if (!(GPIO->IN & (1 << 14)))
		{
			GPIO->OUT = 1 << 18;
            uart_send('B'); 
		}

        //check if something is read 
        char letter = uart_read(); 
        if (letter != '\0') {
            GPIO->OUT = 0 << 17 | 0 << 18 | 0 << 19 | 0 << 20;  
        }

        if (!(GPIO->IN & (1 << 15))) {
            iprintf("The average grade in TTK%d was in %d was: %c\n\r",4235,2022,'B'); 
        }
        
		sleep = 10000;
		while (--sleep)
			; // Delay
	}
	return 0;


}