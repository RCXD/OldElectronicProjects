/* General Information
 * File info : Headers for MESL.BME.KKU Development Board for ATmega128A-AU
 * Board Version : 1.4
 * Date : 2020-09-13
 * Version : 0.0.1
 * Author : TPXDKSS
 * Reference : SanKen Elctronics SLA7062M datasheet
 */

/* Board Information
 * Contents : 
 *  8 LEDs, 4 FNDs, 4x4 Tact SWs, 5V Regulator w/ 7805, IR communicator,
 *  Single DC Motor Driver w/ H Bridge, Single Step Motor Driver,
 *  Single Slot for Character LCD or Graphic LCD, Serial communicator(?)
 *  (?) : Not sure
 * 
 * Specific Pinout : 
 * 1. LEDs
 *  PA0 ~ PA7 to D2 ~ D9, down to up
 *  External Pull-up(off signal will turn on the LEDs)
 *      Pull-up resistor : RN1(Network Resistor)
 * 2. Switch matrix
 *  PD0 ~ PD3 : Row0 to Row3
 *  PD4 ~ PD7 : Col0 to Col3
 *  Set Row pins as Output, Column pins as Input
 *  PD4~7 are external pull-down(High for positive Signal, Low for negative Signal)
 *  Interrupt supported : PD0~PD3 : INT0~INT3
 * 3. FNDs
 *  PE0 ~ PE7 to A ~ G and Development
 *  PB0 to PB3 : Select FND Left to Right, High for On
 *  Segment view 
 *          B        * G is incide
 *         ___
 *     A  |___|  C
 *     F  |___|  D
 *          
 *          E
 *  Common Annode type(off signal will turn on each segments)
 *  High speed swap programming is needed
 *      Cannot turn control each fnd, should use one by one at lease 30Hz
 *      Sophisticated programming skills are needed : 
 *          You ALSO need to control the intensity of light
 *  recommend using FND Fonts
 *  Can turn on/off with SW2(DIP switch)
 * 4. IR
 *  one receiver and one transmitter
 *  PB4 at TX, PB5 at RX
 *  No Inverting Signals
 *      High signal will turn on TX emitter
 *      High signal will be received when high signal is detected
 *  Can turn on/off with SW4(DIP switch)
 * 5. DC Motor
 *  H bridge with transistor
 *  PA0 for pin 1 of Motor, PA1 for pin 2 of Motor / one is CW, one is CCW(?)
 *  You should connect Motor+ to pin 1 of M1(Squre Pin of Molex Connector)
 *  On-board comparators are included. 
 *      potention meter R11 can be used to calibrate saturation point
 *  Can turn on/off with SW3(DIP switch)
 *  (?) : Maybe?
 * 6. Step motor Driver
 *  SLA7062M circuits
 *  PA0 : RESET
 *      Set the translator to home state(not sleep mode)
 *      if RESET is high, MO goes low, all inputs are ignored
 *  PA1 : CW/CCW(1 for CW, 0 for CCW)
 *      Decide direction. take effect when clock rises(low to high CLOCK)
 *  PA2 : CLOCK
 *      low to high transition : motor 1 increment(depend on Step Mode)
 *      Step mode : 1/2, 1/4, 1/8, 1/16 (defined by M1, M2 pin)
 *      when Clock pulse is stopeed, hold mode(will make noises)
 *  PA3~4 : M2~M1 (reverse)
 *      Microstep selector pins
 *      from M1 to M2, 11 : Half, 10 : Quarter, 01 : Eighth, 00 : Sixteenth
 *      Take effect when clk rises(low to high CLOCK)
 *  PA5 : MO(Motor Output)
 *      Logic output indicator of the initial/home state of the translator at every 45 degree.
 *      At 45, 135, 225, 315 degree positions, MO is high
 *  PA6 : Sync
 *      high : synchronous operation
 *      low : asynchronous operation
 *  <MODES>
 *      1) Synchronous operation mode
 *          hold state make noises, because of asynchronous PWM operation
 *          not recommended for normal use(less motor torque)
 *              cause motor vibration due to staircase current
 *          (#)I guess, Let's use this when "hold state" while clock stopped
 *      2) Sleep mode
 *          use REF pin with Pot R18. REF high -> sleep mode -> free state
 *          minimize power consumption
 *          Add delay at lease 100us before stablization
 *          
 * 7. Character LCD
 *  PC0 : RS
 *  PC1 : RW
 *  PC2 : E
 *  PC4 ~ PC7 : DB4 ~ DB7 (higher byte)
 *  Contrast control : Pot R26 / Backlight control : Pot R24
 *  On/Off : SW6 DIP
 * 8. Graphic LCD
 *  PC0 ~ PC7 : DB0 ~ DB7
 *  PF0 ~ 1 : /CS1 ~ /CS2
 *  PF2 : RESET
 *  PF3 : R/W
 *  PF4 : RS
 *  PF5 : E
 *  Contrast control : Pot R25 / Backlight control : Pot R23
 *  On/Off : SW6 DIP
 * 9. ADC Tester
 *  Simple connected to PF7
 *  Pot R22 will be work as Voltage Divided Bias
 * 10. RS232 Serial Communicator
 *  MAX232 communicator
 *  External J3
 *  PE0 for /R1OUT, PE1 for T1IN
 *  PE0 : Input, PE1 : Output
 */
#ifndef DDRA
#include <avr/io.h>
#endif
#ifndef F_CPU
#include <util/delay.h>
#endif

#ifdef STEP
#define STP_RST 1   //PORTA0 
#define STP_CW  2   //PORTA1 
#define STP_CLK 4   //PORTA2 
#define STP_M2  8   //PORTA3 
#define STP_M1  16   //PORTA4 
#define STP_MOUT 32  //PORTA5 
#define STP_SYNC 64  //PORTA6 
//#define PORTA7 
#define HALF 1
#define QUARTER 2
#define EIGHTH 3
#define SIXTEENTH 4

void step_init(int mode){
    DDRA |= STP_RST|STP_CW|STP_CLK|STP_M2|STP_M1|STP_SYNC;
    DDRA &= ~STP_MOUT;
	//PORTA |= 
	PORTA &= ~( STP_RST|STP_CW|STP_CLK|STP_SYNC );
	
	//debug //still failed...
	PORTA &= ~0xff;
	_delay_ms(1000);
	PORTA |= 0xff;
	_delay_ms(1000);
	PORTA &= ~0xff;
	_delay_ms(1000);
	PORTA |= 0xff;
	_delay_ms(1000);
	
    switch(mode)
    {
        case HALF:
            //Set M1-M2 to 1-1
            PORTA |= STP_M1|STP_M2;
			break;
        case QUARTER:
            //Set M1-M2 to 1-0
            PORTA |= STP_M1;
            PORTA &= ~STP_M2;
			break;
        case EIGHTH:
            //Set M1-M2 to 0-1
            PORTA &= ~STP_M1;
            PORTA |= STP_M2;
			break;
        case SIXTEENTH:
            //Set M1-M2 to 0-0
            PORTA &= ~(STP_M1&STP_M2);
			break;
    }
}

//void step_single(int pulses, const int duty_ms){
void step_single(int pulses){
	while(pulses >=0){
		//PORTA |= STP_CLK;
		//debug
		PORTA |= 0xff;
		//_delay_ms(duty_ms);
		_delay_ms(100);
		//PORTA &= ~STP_CLK;
		//_delay_ms(duty_ms);
		PORTA &= 0x00;
		_delay_ms(100);
		--pulses;
	}
}
#endif