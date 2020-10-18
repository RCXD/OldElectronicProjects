#include "avr/io.h"
#include "interrupt.h"

unsigned char SEG[16] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
                        0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};

int main(void)
{
    int n, loop;
    if (loop<100)
        loop++;
    else{
        loop=0;
        PORTA = SEG[n++];
        if (n>=10) n=0;
    }
    TNCT0 = 112;
}

int main(void)
{
    DDRA = 0xff;
    DDRB = 0xff;

    TCCR0 = 0b00000111;
    TCNT0 = 112;
    TIMSK = 0b00000001;
    SREG = 0x80;
    PORTB = 0xf0;
    PORTA = 0xff;

    while(1);
}