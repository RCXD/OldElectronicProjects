#include "avr/io.h"

unsigned char SEG[16] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
                        0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

int main(void)
{
    int n, loop;

    DDRA = 0xff;
    DDRB = 0xff;

    TCCR0 = 0b000001111;
    TCNT0 = 112;
    PORTB = 0xf0;

    while(1){
        for (loop=1; loop<=100; loop++){
            while(!(TIFR & 0b00000001));
            TIFR |= 0b00000001;
            TCNT = 112;
        }
        PORTA = SEG[n++];
        if n >= 10) n=0;
    }
}