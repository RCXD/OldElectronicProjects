/*
 * LEDControl.c
 * Reference : 쉽게 배우는 AVR ATmega128 마이크로컨트롤러 (ISBN 978-89-7050-810-8)
 * Created: 2020-09-12 오후 12:17:41
 * Author : tpxdk
 * Compiler : GCC
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	/* Setup */
	DDRA = 0xff;
	PORTA = 0xff;
    while (1) 
    {
		PORTA = 0x00;
		_delay_ms(990);

		PORTA = 0xff;
		_delay_ms(10);
		/*
		PORTA &= ~0x45;		
		_delay_ms(500);
		PORTA |= 0xff;
		_delay_ms(500);
		
		PORTA &= ~0xff;
		_delay_ms(1000);
		PORTA &= ~0x0f;
		_delay_ms(1000);
		PORTA &= ~0xf0;
		_delay_ms(1000);
		PORTA |= 0xff;
		
		_delay_ms(1000);
		PORTA &= ~((1<<PORTA0)|(1<<PORTA7));	//Turn on
		_delay_ms(1000);
		PORTA |= (1<<PORTA0)|(1<<PORTA7);	//Turn off
		_delay_ms(1000);
		*/
    }
}
