
#include <iom128.h>
#include <intrinsics.h>

unsigned char segment_decoder[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98};
unsigned char num_count = 0, num_count1 = 1 , num_count2 = 0, num_count3 = 3;

#pragma vector=TIMER1_OVF_vect
__interrupt void TIMER1_OVF_intterupt(void) //그냥 num_count를 0부터 99까지 셈 
{
num_count++;
if(num_count>9)
num_count=0;

}
void display(void)  
{
  PORTB= (PORTB & 0xf0) | 0x08; 
  num_count2 = num_count+1;
  PORTE=segment_decoder[num_count2];
  __delay_cycles(1000);
  
  
  PORTB= (PORTB & 0xf0) | 0x04;
  num_count2 = num_count+2;
  PORTE =segment_decoder[num_count2];
  __delay_cycles(1000);
 

  
  PORTB= (PORTB & 0xf0) | 0x02;
  num_count2 = num_count+3;
  PORTE=segment_decoder[num_count2];
  __delay_cycles(1000);
  

  
  PORTB= (PORTB & 0xf0) | 0x01;
   num_count2 = num_count+4;
  PORTE =segment_decoder[num_count3] ; 
  __delay_cycles(1000);
  

}

void main(void)
{
DDRB=0x0f;PORTB=0x0f; 
DDRE=0xff;PORTE=0x00;  
DDRD=0x01;PORTD=0x01;  

 TCCR1A=0x03;               //노말모드, 프리스케일러 /32
TCCR1B=0x1d;               //CTC 모드 토글 OC0 match /128
TCNT1H=0x00;TCNT1L=0x00;   //시작점은 H,L둘다 0
OCR1AH=0x03;OCR1AL=0x0c;   //끝나는 점은 OCR1AH=0011(2), OCR1AL=1100(2)
TIMSK=0x04;                //레지스터 2번만 씀----OVERFLOW INTERRUPT enable
PORTE=segment_decoder[0];
__enable_interrupt();

while(1)
{
 
  display();

}
}
