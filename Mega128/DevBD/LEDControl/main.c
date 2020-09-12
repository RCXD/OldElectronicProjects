/*
 * LEDControl.c
 * Reference : 쉽게 배우는 AVR ATmega128 마이크로컨트롤러 (ISBN 978-89-7050-810-8)
 * Created: 2020-09-12 오후 12:17:41
 * Author : tpxdk
 * Compiler : GCC
 */ 

#include <avr/io.h>
#include <util/delay.h>
//# define F_CPU 1000000UL	//default
#define F_CPU 8000000UL		//Fuse programming is necessary
int main(void)
{
	/* Setup */
	DDRA = 0xff;
    while (1) 
    {
		
		PORTA = ~0xff;		//need to reverse : 1 is off
		_delay_ms(1000);
		PORTA = ~0x00;
		_delay_ms(1000);
    }
}

