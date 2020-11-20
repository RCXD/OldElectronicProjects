/*
 * Project02.c
 *
 * Created: 2020-11-15 오전 11:09:43
 * Author : tpxdkss
 * 
 * Project Overview
	* Master: ATmega128A
	* Slave: FND 4 digits(ctrl: C0~C3, data: E0~E7)
	* 
 */


//Includings//
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL//8000000UL
#include <util/delay.h>
#include <avr/common.h>

//Fonts, Data
unsigned char FND_fonts[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98};
//unsigned char GLCD_fonts[]={}; //use ICRAM, CGRAM instead of this

//switching var, flags
unsigned char TimerFlag = 0; //TimerFlag off -> FND 1 cycle




int main(void) {
	
	WDTCR &= ~(1<<WDE);
	//float bpm = 0.0;
	//int adc = 0;
	DDRA = 0xff;
	PORTA = 0xff;
	_delay_ms(1000);
	int n=3;
	for(int i = 0; i<2*n;i++){
		_delay_ms(500);
		PORTA ^= 0xff;
	}
	for(int i = 0; i<2*n;i++){
		_delay_ms(300);
		PORTA ^= 0xff;
	}
	//FND init
	DDRC = 0xff;
	DDRE = 0xff;
	PORTC = 0xff;
	PORTE = 0x00;
	
	//Timer init
	TCCR0 = 0b00000111;
	TCNT0 = 112;
	
	int loop;
	int state = 0;
	int data = 1234;
	while (1)
	{
		for (loop = 1; loop <= 2; loop++){
			while(!(TIFR & 0b00000001));
			TIFR |= 0b00000001;
			TCNT0 = 112;
		}
		//PORTA = SEG[n++];
		//if(n>= 10) n= 0;
		

		//adc = ADC Read();
		//bpm = Calculate(adc);
		//GLCD_update(data, bpm);
		switch(state){
			case 0:
			PORTC = 0x04;
			PORTE = FND_fonts[(int)data%1000/100];
			state++;
			break;
			case 1:
			PORTC = 0x02;
			PORTE = FND_fonts[(int)data%100/10];
			state++;
			break;
			case 2:
			PORTC = 0x01;
			PORTE = FND_fonts[(int)data%10];
			state = 0;
			break;
		}
	}
	
	return 0;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

/*
 * GD RAM(Graphic Data RAM)
 * X=0~15(line) (b15~b0 Each) //AC3:AC0: 16개
 * Y = 0~63 // AC6~AC0: 128개
 */


/////////////////////////////////////////////////////////