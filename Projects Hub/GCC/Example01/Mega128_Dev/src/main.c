/**
 * \file
 *
 * 
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	
	//board_init();
	
	
	/* Insert application code here, after the board has been initialized. */
	DDRA=0xff;
	PORTA=0xff;
	
	
	while(1){
		//DDRA=0b00001111;
		//PORTA=0b0001111;
		PORTA = 0xff;
		_delay_ms(500);
		PORTA = 0x00;
		_delay_ms(500);
		
	}
}
