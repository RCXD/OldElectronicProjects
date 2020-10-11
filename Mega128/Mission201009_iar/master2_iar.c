#include <iom128.h>
#include <intrinsics.h> // 헤더파일
#define ENABLE (PORTC |= 0x04)
#define DISABLE (PORTC &= ~0x04)
unsigned char segment_decoder[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98};
unsigned char d,num_count=0,num_count_1=0,num_count_2=0;
unsigned char t_sharing=0;
unsigned char a, b, c, d,e, result;
#pragma vector=TIMER1_OVF_vect
__interrupt void TIMER1_OVF_intterupt(void)
{

if(num_count> 0xFE)
num_count=0;
}

void instruction_out(unsigned char b)
{
   PORTC = b & 0xF0;   // OUT upper 4bits
   ENABLE;   // LCD Enable
   DISABLE;   // LCD Disable
      
   PORTC = (b<<4) & 0xF0;   // OUT lower 4bits
   ENABLE;   // LCD Enable
   DISABLE;   // LCD Disable

   __delay_cycles(2000);   // 2ms delay
}

void char_out(unsigned char b)
{
   PORTC = (b & 0xF0) | 0x01;    // OUT upper 4bits
   ENABLE;   // LCD Enable
   DISABLE;   // LCD Disable
      
   PORTC = ((b<<4) & 0xF0) | 0x01;    // OUT upper 4bits
   ENABLE;   // LCD Enable
   DISABLE;   // LCD Disable

   __delay_cycles(2000);   // 2ms delay
}

void string_out(unsigned char b, unsigned char *str)
{
      unsigned int i = 0;
      
      instruction_out(b);
      
      do{
            char_out(str[i]);
      }while(str[++i] != '\0');
}

void init_CLCD(void)
{
   DDRC = 0xFF;
   PORTC = 0xFF;
   __delay_cycles(2000);   // 2ms delay
        
   __delay_cycles(1000);   // 1ms delay   

   instruction_out(0x28);   // LCD Function Set(16x2 Line, 4Bit, 5x8 DOT)
   __delay_cycles(2000);   // 2ms delay

   instruction_out(0x28);   // LCD Function Set(16x2 Line, 4Bit, 5x8 DOT)
   __delay_cycles(2000);   // 2ms delay

   instruction_out(0x0C);   // LCD Display ON, Cursor OFF, Blink OFF
   instruction_out(0x06);   // LCD Entry Mode Set
   instruction_out(0x02);  // Return home
   instruction_out(0x01);   // LCD Clear
}

void display(void)  
{ 
   
  
  PORTB= (PORTB & 0xf0) | 0x02;
  PORTE = segment_decoder[e/100];
  __delay_cycles(1000);
  PORTE = 0xff;
  
  
  PORTB= (PORTB & 0xf0) | 0x04;
  PORTE = segment_decoder[e/10];
  __delay_cycles(1000);
  PORTE = 0xff;
  
  
  PORTB= (PORTB & 0xf0) | 0x08; 
  PORTE = segment_decoder[e];
  __delay_cycles(1000);
  PORTE = 0xff;
  __delay_cycles(1000);
}

void led(void)
{
  
 
    e=e+1;
  }

void main(void)
{

DDRA=0X7F;
PORTA=0X7F;
PORTC=0xFF; //CLCD 동작 PORT 설정
DDRF &= 0xFE;
DDRB=0x0f;PORTB=0x0f;  //segment 4개 다 출력, 4개 다 high상태
DDRE=0xff;PORTE=0x00;  //PE 다 출력, 처음은 다 LOW
ADMUX= 0x27;
ADCSR = 0xA7;
ADCSR |= 0x40;

 init_CLCD();  // 초기화
  
  instruction_out(0x02);   // LCD Clear
  


while(1) 
{
  
__delay_cycles(8000);
while((ADCSR & 0x10)==0);
ADCSR |= 0x10;
result=ADCH+40;// 만약 0점조절을 하려면 오차를 여기에 빼줌

if((PIND&0x10)==0x10){
    __disable_interrupt();
    if((TIMSK&0x04)==0){
      TIMSK=0x04;
    }else{
      TIMSK=0x00;
    }
    __delay_cycles(100000);
    __enable_interrupt();
  }
  display();  

 if ((PINA | 0X7F)==1 ) ;
  {
    
    PORTA |= 0X01;
  led();
  }
a=result/1000;
b=(result-1000*a)/100;
c=(result-1000*a-100*b)/10;
d=(result-1000*a-100*b-10*c);


instruction_out (0x80);  
char_out('0'+a);
instruction_out (0x81);  
char_out('0'+b);
instruction_out (0x82);  
char_out('0'+c);
instruction_out (0x83);  
char_out('0'+d);

}

}