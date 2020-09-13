/* General Information
 * File info : Headers for MESL.BME.KKU Development Board for ATmega128A-AU
 * Board Version : 1.4
 * Date : 2020-09-13
 * Version : 0.0.1
 * Author : TPXDKSS
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
 *  Cannot Turn off
 * 2. Switch matrix
 *  
 * 3. FNDs
 *  PE0 ~ PE7 to A ~ G and Development
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
 *  PA1 : CW/CCW(1 for CW, 0 for CCW)
 *  PA2 : CLOCK
 *  PA3~5 : M2~M0 (reverse)
 *  PA6 : Sync
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
