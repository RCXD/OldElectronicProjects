#include <iom128.h>
#include <intrinsics.h> // �������
#define ENABLE (PORTC |= 0x04)
#define DISABLE (PORTC &= ~0x04)
unsigned char segment_decoder[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98};
unsigned char num_count=0;
unsigned char t_sharing=0;
unsigned char a, b, c, d,e,f, result;
int state = 0;
float bpm = 0;
float cnt=0;
int t1=0, t2=0;
unsigned int flag = 0;

int i, j, k; // �ݺ����� ���� ���� ����
char GLCD_data[128][8]; //��ǥ�� ���� ���� ���� ����


#pragma vector=TIMER1_OVF_vect
__interrupt void TIMER1_OVF_intterupt(void)
{
if(num_count> 0xFE)
num_count=0;
}




void port_init(void)
{

DDRF = 0xff; // ��Ʈ �ʱ�ȭ (Control PORT)
DDRC = 0xff; // ��Ʈ �ʱ�ȭ (Data PORT)

}


void GLCD_handler(char rs, char cs, char data) // �����ͽ�Ʈ�� ���Ͽ� ���� GLCD�� �⺻ �����Լ�
{

__delay_cycles(1); // ���ӵ� ��ȣ �Է¿� ���� ���� ������ delay
PORTF = 0x00;   // Control Port�� ���ο� �Է��� ���� �� �ִ� ���·� �����.
PORTF  = PORTF & ~0x04; // E Fall
PORTF = PORTF & ~0x02; // R/W Fall
 
switch(rs) // RS choice
{
case  0x01: PORTF = PORTF | 0x01; break; // data mode
case ~0x01: PORTF = PORTF & ~0x01; break; // ins mode
}

 

PORTF = PORTA | cs; // CS choice
PORTF = PORTA | 0x20; // E Rise
PORTC = data;   // Data Input
PORTF = PORTF & ~0x20; //  E Fall

}

 

//GLCD�� Display�� �Ѱ� ��ǥ�� ������ġ�� 0���� �����Ѵ�
void GLCD_init(void)
{

GLCD_handler(~0x01, 0x18, 0x3f); // display on
GLCD_handler(~0x01, 0x18, 0xc0); // start line set

}

 

//�� ��ǥ�� ����� �����Ϳ� ȭ���� ��� �����.
void GLCD_Clear(void)
{

int i,j;
for (j = 0 ; j < 64 ; j ++ ) // address�� 0���� 63���� �ݺ�
{

for (i = 0 ; i < 8 ; i ++ ) // page�� 0���� 7���� �ݺ�
{

GLCD_data[j][i]=0x00; // 0���� 63���� ��� ��ǥ�� ������������ 0x00���� �ʱ�ȭ
GLCD_data[64+j][i]=0x00;   //64���� 127���� ��� ��ǥ�� ������������ 0x00���� �ʱ�ȭ
GLCD_handler(~0x01, 0x18, 0xb8|i); // page set
GLCD_handler(~0x01,0x18, 0x40|j); // address set
GLCD_handler(0x01, 0x18, 0x00); // data write

}

}

}

 

void GLCD_pixel(int col, int row, char color)
{

int page, xadd, cs, yadd; 
 
if(col >= 0 && col < 128 && row >= 0 && row <64) //ǥ�� ������ ��ǥ�� ����
{

cs = col < 64 ? 0x08 : 0x10; // ������ǥ 64�� �������� ��� ȭ�� ����
page = row/8; // Page�� row��ǥ�� 8�� ���� ��
xadd  = row%8; // xadd�� �ش� ���������� ����� ��Ʈ�ȼ��� ���� ��ġ
yadd  = col%64; // yadd�� �ش� ȭ�鿡�� ����� ��Ʈ�ȼ��� ���� ��ġ
 
//���� �������� �ϳ��� �ȼ����� �����Ѵ�. color�� ���� ON OFF ����
switch(color)
{
case 1: GLCD_data[col][page] = GLCD_data[col][page] |  0x01<<xadd; break;
case 0: GLCD_data[col][page] = GLCD_data[col][page] & ~(0x01<<xadd); break;
} 

GLCD_handler(~0x01, cs, 0xb8|page); // page set
GLCD_handler(~0x01, cs, 0x40|yadd); // address set
GLCD_handler(0x01, cs, GLCD_data[col][page]); // data write

}

}




/*void instruction_out(unsigned char b)
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
*/

void display(void)  
{ 
   
  
  PORTB= (PORTB & 0xf0) | 0x02;
  PORTE = segment_decoder[(int)bpm%1000/100];
  __delay_cycles(1000);
  PORTE = 0xff;
  
  
  PORTB= (PORTB & 0xf0) | 0x04;
  PORTE = segment_decoder[(int)bpm%100/10];
  __delay_cycles(1000);
  PORTE = 0xff;
  
  
  PORTB= (PORTB & 0xf0) | 0x08; 
  PORTE = segment_decoder[(int)bpm%10];
  __delay_cycles(1000);
  PORTE = 0xff;
  __delay_cycles(1000);
}


void led(void)
{ 
  float temp = 0; //�ӽ� ���� : led�Լ� �������� ����ϱ� ����
  if (state == 0)
  {
    if(flag) 
    {
      t2++;
    }
    if (PINA&0X04) 
    {
      state = 1;
    }
  }
  
  if (state == 1)
  {
    //e=e+1;
    flag = 1;
    temp = cnt;
    cnt = t1+t2;
    cnt = (temp + cnt)/2.0; //��� ����
    bpm = 3300/cnt; //���� ���ϰ� ������ ������.
    t1=0;
    t2=0;
    
    if (!(PINA & 0X04))
    {
      state = 0;
    }
    if (PINA&0x04)
    {
      state = 2;
    }
  }
   
   if (state ==2)
   {
     t1++;
     
     if(!(PINA & 0x04))
     {
       state = 0;
     }
   }

}
        






void main(void)
{




DDRA= 0XFF-0x04;
PORTC=0xFF; //CLCD ���� PORT ����
DDRF &= 0xFE;
DDRB=0x0f;PORTB=0x0f;  //segment 4�� �� ���, 4�� �� high����
DDRE=0xff;PORTE=0x00;  //PE �� ���, ó���� �� LOW
//ADMUX= 0x27;
//ADCSR = 0xA7;
//ADCSR |= 0x40;




  


  while(1) 
  {
    
    //__delay_cycles(8000);
    //while((ADCSR & 0x10)==0);
    //ADCSR |= 0x10;
    //result=ADCH+40;// ���� 0�������� �Ϸ��� ������ ���⿡ ����

    //if((PIND&0x10)==0x10){
    /*
      __disable_interrupt();
      if((TIMSK&0x04)==0){
        TIMSK=0x04;
      }else{
        TIMSK=0x00;
      }
      __delay_cycles(1000);
      __enable_interrupt();
    }
    */
    //}
    display();  
    led();

  //GLCD_handler(~0x01, 0x08, 0x3f); // display on;
  //GLCD_handler(~0x01, 0x08, 0xc0); // start line set;

    
  //a=result/1000;
  //b=(result-1000*a)/100;
  //c=(result-1000*a-100*b)/10;
  //d=(result-1000*a-100*b-10*c);


  //instruction_out (0x80);  
  //char_out('0'+a);
  //instruction_out (0x81);  
  //char_out('0'+b);
  //instruction_out (0x82);  
  //char_out('0'+c);
  //instruction_out (0x83);  
  //char_out('0'+d);


  }


}

