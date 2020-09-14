/*
 * StepMtrControl.c
 *
 * Created: 2020-09-13 오후 11:59:15
 * Author : tpxdk
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define STEP
#include "MESLBoardHeader.h" // How to add??


int main(void)
{
	//step_init(HALF);
	//step_init(QUARTER);
	step_init(EIGHTH);
	//step_init(SIXTEENTH);

    _delay_ms(1000);
	//step_single(1, 100);

	
	step_single(5);
	_delay_ms(1000);
    while (1) 
    {
		step_single(5);
		_delay_ms(1000);
    }
}

