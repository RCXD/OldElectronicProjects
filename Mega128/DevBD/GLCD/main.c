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
 3. Peripheral info
	1) GLCD : parallel, SGB1286411, KS0108 Driver
 */ 

//Basic avr//
#include <avr/io.h>
#include <avr/interrupt.h>

//utilities//
#define F_CPU 8000000UL
#include <util/delay.h>

//visual in/out//
#include <stdio.h>			/* Include std i/o library file */
//#include "Font_Header.h"	//unnecessary

/* GLCD Pinout Information
 * DevB/D Pinout : VDD, VSS, VO, DB0~DB7, /CS1, /CS2, RST, R/W, RS, E, VEE, LEDA, LEDK
 *	Based on S6B0108B
 * SGB1286411 Pinout(Assume) : VSS, VDD, VO, RS, R/W, E, DB0~DB7, CS1, CS2, /RST, VEE, A, K
 *  Based on KS0108
 */

/* GLCD pinout Matching Table
 * pin_map					PC0	PC1	PC2	PC3	PC4	PC5	PC6	PC7	PF0	PF1	PF2	PF3	PF4	PF5
 * pin_num		1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20
 * DevBD		Vdd	Vss	Vo	DB0	DB1	DB2	DB3	DB4	DB5	DB6	DB7	/CS1/CS2/RSTR/W	RS	E	Vee	A	K
 * Matching		-											Rev	Rev
 * SGB1286411	Vdd	Vss	Vo	DB0	DB1	DB2	DB3	DB4	DB5	DB6	DB7	CS1	CS2	/RSTR/W	RS	E	VoutA	K	
 * Matching: DevBD -> SGB1286411
 * Method : DevBD -> Jumper Connector -> GLCD(Bread Board)
 */

/* KS0108B Driver(ref : pdf document)
 * Output Resistor
 * RS	R/W		Function
 * L	L		Instruction
 * L	H		Status read(busy check)
 * H	L		Data Write
 * H	H		Data Read
 * 
 * Busy Flag
 * After E fall, T Busy : 4.3~12.9 us
 * 
 */

//Deb B/D Configuration//
//#define 	ORIGINAL				//Reference: https://www.electronicwings.com/avr-atmega/graphical-lcd-128x64-interfacing-with-atmega1632
#define		MESL_BME_KKU			//KKU MESL BME lab Board

//GLCD B/D Configuration//
//#define 	ABG128064H05			//DebBD Original Schematic
#define 	SGB1286411				//extra GLCD
//#define 	LCD12864				//DFrobot Arduino LCD12864

//GLCD Driver Configuration//
#define 	S6B0108B				//ABG128064H05
//#define	KS0108B					//SGB1286411
//#define 	ST7920					//LCD12864

//GLCD Configuration//
#ifdef MESL_BME_KKU
#define Data_Port			PORTC	//For DevBD
#define Command_Port		PORTF	//For DevBD
#define Data_Port_Dir		DDRC	//For DevBD
#define Command_Port_Dir 	DDRF	//For DevBD
#define RS		PF4
#define RW		PF3
#define EN		PF5
#define CS1		PF0
#define CS2		PF1
#define RST		PF2
#endif

//???
#define TotalPage	 8		//Purpose?

#ifdef ORIGINAL
#define Data_Port	 PORTA			// Define data port for GLCD 
#define Command_Port	 PORTC		// Define command port for GLCD 
#define Data_Port_Dir	 DDRA		// Define data port for GLCD 
#define Command_Port_Dir DDRC		// Define command port for GLCD 
#define RS		 PC0				// Define control pins 
#define RW		 PC1
#define EN		 PC2
#define CS1		 PC3
#define CS2		 PC4
#define RST		 PC5

#define TotalPage	 8
#endif

//initiation//
void BD_init(void);							//예비
void GLCD_init(void);						//GLCD 초기화: 제어포트, 데이터 포트 - Mode : parallel
void TI_init(void);

//Core functions//
void GLCD_Plot(char y[128]);				//데이터셋 출력
void GLCD_Command(char Command);			//GLCD 커맨드 입력 - 참조:https://www.electronicwings.com/avr-atmega/graphical-lcd-128x64-interfacing-with-atmega1632
void GLCD_Data(char Data);					//GLCD 데이터 입력
void GLCD_ClearAll(void);					//GLCD 클리어
void TI_set(void/*int duty*/);				//타이머인터럽트 세팅

//Options : Based on Mission//
void FND_init(void);						//FND 초기화: DDRx, PORTx
void ADC_init(void);
void FND_show(void);						//FND에 값 출력: FND_seg_num함수 활용
void FND_seg_num(int seg, int font);

//Utilities//
void LED_Debug(void);
void FND_Arm(void);
void setFreq(void/*F_CPU*/);
void GLCD_String(char page_no, char *str);

int main(void)
{
    BD_init();
	GLCD_init();
	GLCD_ClearAll();
	TI_init();
	
    while (1) 
    {
		_delay_us(10);	
    }
}

//Interrupt Subroutines//
ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	//PORTD ^= (1 << LED);					//visual output
	//TCNT1 = 63974;   // for 1 sec at 16 MHz // (65536-63974)*1024 ~= 16MHz
	TCNT1 = 57724;		//(65536-57724)*1024 ~= 8MHz
}

//initiation//
void BD_init(void){
	return;
}

/* Bit Operation Practice....
00111100
and
11001111
==
00001100
turn off 5, 6
*/

void GLCD_init(void){
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	Command_Port = (1 << RST) | ()
	#ifdef SGB1286411
	/* Select both left & right half of display & Keep reset pin high */
	Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
	#endif
}
void TI_init(void){
	//DDRD = (0x01 << LED);					//Configure the PORTD4 as output
		
	//TCNT1 = 63974;   // for 1 sec at 16 MHz
	TCNT1 = 57724;		//for 1 sec at 8MHz

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescaler
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
	return;
}

//Core functions//
void GLCD_Plot(char y[128]){
	  
	return;
}
void GLCD_Command(char Command){
	Data_Port = Command;		/* Copy command on data pin */
	Command_Port &= ~(1 << RS);	/* Make RS LOW for command register*/
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}
void GLCD_Data(char Data){
	Data_Port = Data;		/* Copy data on data pin */
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}
void GLCD_ClearAll(void){
	int i,j;
	/* Select both left & right half of display */
	Command_Port |= (1 << CS1) | (1 << CS2);
	for(i = 0; i < TotalPage; i++)
	{
		GLCD_Command((0xB8) + i);/* Increment page */
		for(j = 0; j < 64; j++)
		{
			GLCD_Data(0);	/* Write zeros to all 64 column */
		}
	}
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
}
void TI_set(void/*int duty*/){
	return;
}

//Options : Based on Mission//
void FND_init(void){
	return;
}
void ADC_init(void){
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

/* GLCD_String Function: need Font_Header.h from https://www.electronicwings.com/avr-atmega/graphical-lcd-128x64-interfacing-with-atmega1632
void GLCD_String(char page_no, char *str) // GLCD string write function 
{
	unsigned int i, column;
	unsigned int Page = ((0xB8) + page_no);
	unsigned int Y_address = 0;
	float Page_inc = 0.5;
	
	Command_Port |= (1 << CS1);	  // Select Left half of display 
	Command_Port &= ~(1 << CS2);
	
	GLCD_Command(Page);
	for(i = 0; str[i] != 0; i++)	// Print char in string till null 
	{
		if (Y_address > (1024-(((page_no)*128)+FontWidth)))
		break;
		if (str[i]!=32)
		{
			for (column=1; column<=FontWidth; column++)
			{
				if ((Y_address+column)==(128*((int)(Page_inc+0.5))))
				{
					if (column == FontWidth)
					break;
					GLCD_Command(0x40);
					Y_address = Y_address + column;
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command(Page + Page_inc);
					Page_inc = Page_inc + 0.5;
				}
			}
		}
		if (Y_address>(1024-(((page_no)*128)+FontWidth)))
		break;
		if((font[((str[i]-32)*FontWidth)+4])==0 || str[i]==32)
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_Data(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
		}
		else
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_Data(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
			GLCD_Data(0);
			Y_address++;
			if((Y_address)%64 == 0)
			{
				Command_Port ^= (1 << CS1);
				Command_Port ^= (1 << CS2);
				GLCD_Command((Page+Page_inc));
				Page_inc = Page_inc + 0.5;
			}
		}
	}
	GLCD_Command(0x40);	// Set Y address (column=0) 
}
*/