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


///References///
/*
 *
 */

/////////////////////////////////////////////////////////
///Main///

//Includings//
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

//void SPI_init();
//void ADC_init();
//void GLCD_init();
//void Timer0_init();
//void FND_init();
//int ADC_read();
//float Calculate(int data);
//void GLCD_update(int data, float bpm);
//void FND_update(int data);
//void set_up();

//Fonts, Data
unsigned char FND_fonts[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98};
//unsigned char GLCD_fonts[]={}; //use ICRAM, CGRAM instead of this

//switching var, flags
unsigned char TimerFlag = 0; //TimerFlag off -> FND 1 cycle
void setTimerFlag(){ TimerFlag = 1;}

//void debug_init();
//void debug(int n);

/*
void set_up(){
	//SPI_init();
	//ADC_init();
	//GLCD_init();
	Timer0_init();
	FND_init();
}
*/


//Timer Interrupt > FND
ISR(TIMER0_OVF_vect)
{
	static unsigned int loop = 0; //Timer Interrupt loop variable
	if(loop < 100) loop++;
	else {
		loop = 0;
		setTimerFlag();
	}
	TCNT0 = 60;
	//if(num_count > 0xfe) num_count = 0;
}


void debug_init(){
	DDRA = 0xff;
	PORTA = 0xff;
}
void debug(int n){
	for(int i = 0; i<2*n;i++){
		_delay_ms(500);
		PORTA ^= 0xff;
	}
	for(int i = 0; i<2*n;i++){
		_delay_ms(300);
		PORTA ^= 0xff;
	}
}

void SPI_init(){
	SPCR = 0x5c; //SPI enable, master mode, sck: fosc/4
	SPSR = 0x00; //
	//return;
}
void SPI_master(unsigned char data){
	SPDR = data; //데이터 전송
	while(!(SPSR & 0x80)); //전송완료 대기
}
void ADC_init(){
	//return;	
}
void GLCD_init(){
	//wait time > 40ms
	//return;
}
void Timer0_init(void){
	TCCR0 = 0b00000111;
	TCNT0 = 60;
	TIMSK = (1<<TOIE0);
	sei();
	//return;
}
void FND_init(void){
	//PE0~7 Setup
	DDRE |= (1<<DDRE0)|(1<<DDRE1)|(1<<DDRE2)|(1<<DDRE3)|(1<<DDRE4)|(1<<DDRE5)|(1<<DDRE6)|(1<<DDRE7);
	DDRC = (1<<DDRC0)|(1<<DDRC1)|(1<<DDRC2)|(1<<DDRC3);
	PORTE = 0x00;
	PORTC = 0xff;
	_delay_ms(1);
	//return;
}
int ADC_read(){
	int adc = 0;
	return adc;
}
float Calculate(int data){
	float bpm = 0;
	return bpm;
}
void GLCD_update(int data, float bpm){
	//Set position
	//SPI_master();
	//SPI_plot(arr[]);
	
	//GLCD Plotting Queue
	//arr[128] -> sepr 16, 16, 16, 16, ..., 16
	
	
	//delay: 72us(540kHz)
	//return;
}
void FND_update(int data){
	static unsigned int state = 0;
	static unsigned int num = 0;
	num = data;
	if(TimerFlag){
		switch(state){
			case 0:
			PORTC = 0x04;
			PORTE = FND_fonts[(int)num%1000/100];
			state++;
			break;
			case 1:
			PORTC = 0x02;
			PORTE = FND_fonts[(int)num%100/10];
			state++;
			break;
			case 2:
			PORTC = 0x01;
			PORTE = FND_fonts[(int)num%10];
			state = 0;
			break;
		}
	}
	//return;
}
/////////////////////////////////////////////////////////

/*
 * GD RAM(Graphic Data RAM)
 * X=0~15(line) (b15~b0 Each) //AC3:AC0: 16개
 * Y = 0~63 // AC6~AC0: 128개
 */






/////////////////////////////////////////////////////////
int main(void) {
	//float bpm = 0.0;
	//int adc = 0;
	DDRA = 0xff;
	PORTA = 0xff;
	int n=3;
	for(int i = 0; i<2*n;i++){
		_delay_ms(500);
		PORTA ^= 0xff;
	}
	for(int i = 0; i<2*n;i++){
		_delay_ms(300);
		PORTA ^= 0xff;
	}
	DDRC = 0xff;
	DDRE = 0xff;
	PORTC = 0xff;
	PORTE = 0x00;
	
	FND_init();
	Timer0_init();
	//set_up();
	
	while (1)
	{
		int data = 1234;//Temp
		//adc = ADC Read();
		//bpm = Calculate(adc);
		//GLCD_update(data, bpm);
		if(TimerFlag){
			FND_update(data);
			TimerFlag = 0;
		}
	}
	
	return 0;
}
/////////////////////////////////////////////////////////