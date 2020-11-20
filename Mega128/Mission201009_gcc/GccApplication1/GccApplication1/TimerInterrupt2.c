/*
 * TimerInterrupt2.c
 *
 * Created: 2020-11-17 오전 4:07:07
 *  Author: tpxdk
 */ 

//Includings//
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL//8000000UL
#include <util/delay.h>
#include <avr/common.h>

unsigned char SEG[16] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
						 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

int n, loop;
ISR(TIMER0_OVF_vect)
{
	if(loop < 2) loop++;
	else{
		loop=0;
		PORTE = SEG[n++];
		if(n>=10) n=0;
	}
	TCNT0 = 112;
}

int main(void){
	DDRE = 0xff;
	DDRC = 0xff;
	PORTC = 0xff;
	PORTE = 0x00;
	TCCR0 = 0b00000111;
	TCNT0 = 112;
	TIMSK = 0b00000001;
	SREG = 0x80;
	
	while(1){
		PORTE ^= 0xff;
		_delay_ms(100);
	}
	
}