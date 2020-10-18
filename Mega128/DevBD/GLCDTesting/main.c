/*
 * ATmega_GLCD_TextFont
 * http://electronicwings.com
 */ 

#define F_CPU 8000000UL			/* Define CPU clock Freq 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include delay header file */
#include <stdio.h>			/* Include std i/o library file */
#include "Font_Header.h"

#ifdef ORIGINAL
#define Data_Port	 PORTA		/* Define data port for GLCD */
#define Command_Port	 PORTC		/* Define command port for GLCD */
#define Data_Port_Dir	 DDRA		/* Define data port for GLCD */
#define Command_Port_Dir DDRC		/* Define command port for GLCD */

#define RS		 PC0		/* Define control pins */
#define RW		 PC1
#define EN		 PC2
#define CS1		 PC3
#define CS2		 PC4
#define RST		 PC5
#endif

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

#define TotalPage	 8

void GLCD_Command(char Command)		/* GLCD command function */
{
	Data_Port = Command;		/* Copy command on data pin */
	Command_Port &= ~(1 << RS);	/* Make RS LOW for command register*/
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Data(char Data)		/* GLCD data function */
{
	Data_Port = Data;		/* Copy data on data pin */
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Init()			/* GLCD initialize function */
{
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
}

void GLCD_ClearAll()			/* GLCD all display clear function */
{
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

void GLCD_String(char page_no, char *str)/* GLCD string write function */
{
	unsigned int i, column;
	unsigned int Page = ((0xB8) + page_no);
	unsigned int Y_address = 0;
	float Page_inc = 0.5;
	
	Command_Port |= (1 << CS1);	/* Select Left half of display */
	Command_Port &= ~(1 << CS2);
	
	GLCD_Command(Page);
	for(i = 0; str[i] != 0; i++)	/* Print char in string till null */
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
	GLCD_Command(0x40);	/* Set Y address (column=0) */
}

int main(void)
{
	GLCD_Init();		/* Initialize GLCD */
	GLCD_ClearAll();	/* Clear all GLCD display */
	GLCD_String(0,"Atmel");	/* Print String on 0th page of display */
	while(1);
}