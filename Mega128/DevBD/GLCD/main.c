/*
 * GLCD.c
 *
 * Created: 2020-10-11 오후 6:11:49
 * Author : Tpxdkss
 * Prject Summary
 1. Based on ATmega128 Mission(GLCD-ECG-FND)
 2. Todo
	1) Get Ready to Transfer FND-ECG to GCC
	2) ADC Init
	3) ADC out
	4) Plotting ECG
	5) Timer-Interrupt
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//initiation//
void BD_init(void);							//예비
void GLCD_init(void);						//GLCD 초기화: 제어포트, 데이터 포트
void TI_init(void);

//Core functions//
void GLCD_Plot(char y[128]);				//데이터셋 출력
void GLCD_Command(char Command);			//GLCD 커맨드 입력 - 참조:https://www.electronicwings.com/avr-atmega/graphical-lcd-128x64-interfacing-with-atmega1632
void GLCD_Data(char Data);					//GLCD 데이터 입력
void GLCD_ClearAll(void);					//GLCD 클리어
void TI_set(void/*int duty*/);				//타이머인터럽트 세팅

//Options : Based on Mission//
void FND_init(void);						//FND 초기화: DDRx, PORTx
void FND_show(void);						//FND에 값 출력: FND_seg_num함수 활용
void FND_seg_num(int seg, int font);

//Utilities//
void LED_Debug(void);
void FND_Arm(void);
void setFreq(void/*F_CPU*/);

int main(void)
{
    BD_init();
	GLCD_init();
	TI_init();
	
    while (1) 
    {
		_delay_us(10);
		
    }
}

//Interrupt Subroutines//
ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	PORTD ^= (1 << LED);
	TCNT1 = 63974;   // for 1 sec at 16 MHz
}

//initiation//
void BD_init(void){
	return;
}
void GLCD_init(void){
	return;
}
void TI_init(void){
	DDRD = (0x01 << LED);     //Configure the PORTD4 as output
		
	//TCNT1 = 63974;   // for 1 sec at 16 MHz
	

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
	return;
}

//Core functions//
void GLCD_Plot(char y[128]){
	  
	return;
}
void GLCD_Command(char Command){
	return;
}
void GLCD_Data(char Data){
	return;
}
void GLCD_ClearAll(void){
	return;
}
void TI_set(void/*int duty*/){
	return;
}

//Options : Based on Mission//
void FND_init(void){
	return;
}
void FND_show(void){
	return;
}
void FND_seg_num(int seg, int font){
	return;
}

//Utilities//
void LED_Debug(void){
	return;
}
void FND_Arm(void){
	return;
}
void setFreq(void/*F_CPU*/){
	return;
}

