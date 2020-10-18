//#include <iom128.h>
//#include <intrinsics.h> // 헤더파일
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

int i, j, k; // 반복문에 쓰일 변수 선언
char GLCD_data[128][8]; //좌표별 정보 저장 변수 선언

/*
#pragma vector=TIMER1_OVF_vect
__interrupt void TIMER1_OVF_intterupt(void)
{
if(num_count> 0xFE)
num_count=0;
}
*/




void port_init(void)
{

DDRF = 0xff; // 포트 초기화 (Control PORT)
DDRC = 0xff; // 포트 초기화 (Data PORT)

}


void GLCD_handler(char rs, char cs, char data) // 데이터시트에 준하여 만든 GLCD용 기본 제어함수
{

__delay_cycles(1); // 연속된 신호 입력에 따른 오류 방지용 delay
PORTF = 0x00;   // Control Port가 새로운 입력을 받을 수 있는 상태로 만든다.
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

 

//GLCD의 Display를 켜고 좌표의 시작위치를 0으로 설정한다
void GLCD_init(void)
{

GLCD_handler(~0x01, 0x18, 0x3f); // display on
GLCD_handler(~0x01, 0x18, 0xc0); // start line set

}

 

//각 좌표에 저장된 데이터와 화면을 모두 지운다.
void GLCD_Clear(void)
{

int i,j;
for (j = 0 ; j < 64 ; j ++ ) // address를 0부터 63까지 반복
{

for (i = 0 ; i < 8 ; i ++ ) // page를 0부터 7까지 반복
{

GLCD_data[j][i]=0x00; // 0부터 63까지 모든 좌표의 데이터정보를 0x00으로 초기화
GLCD_data[64+j][i]=0x00;   //64부터 127까지 모든 좌표의 데이터정보를 0x00으로 초기화
GLCD_handler(~0x01, 0x18, 0xb8|i); // page set
GLCD_handler(~0x01,0x18, 0x40|j); // address set
GLCD_handler(0x01, 0x18, 0x00); // data write

}

}

}

 

void GLCD_pixel(int col, int row, char color)
{

int page, xadd, cs, yadd; 
 
if(col >= 0 && col < 128 && row >= 0 && row <64) //표현 가능한 좌표만 실행
{

cs = col < 64 ? 0x08 : 0x10; // 가로좌표 64를 기준으로 출력 화면 선택
page = row/8; // Page는 row좌표를 8로 나눈 값
xadd  = row%8; // xadd는 해당 페이지에서 변경될 도트픽셀의 세로 위치
yadd  = col%64; // yadd는 해당 화면에서 변경될 도트픽셀의 가로 위치
 
//기존 정보에서 하나의 픽셀만을 변경한다. color에 따라 ON OFF 결정
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
  float temp = 0; //임시 변수 : led함수 내에서만 사용하기 위함
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
    cnt = (temp + cnt)/2.0; //평균 필터
    bpm = 5000/cnt; //역수 취하고 보정값 곱해줌.
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

port_init(void);
GLCD_init(void);
GLCD_Clear(void);//화면 전체를 지운다.


GLCD_pixel(0,1,1); // 좌표(0,1)에 점을 추가한다.


DDRA= 0XFF-0x04;
PORTC=0xFF; //CLCD 동작 PORT 설정
DDRF &= 0xFE;
DDRB=0x0f;PORTB=0x0f;  //segment 4개 다 출력, 4개 다 high상태
DDRE=0xff;PORTE=0x00;  //PE 다 출력, 처음은 다 LOW
ADMUX= 0x27;
ADCSR = 0xA7;
ADCSR |= 0x40;

/*int i, j; // 반복문에 쓰일 변수 선언
DDRA = 0xff; // 포트 초기화 (Control PORT)

DDRF = 0xff; // 포트 초기화 (Data PORT)

GLCD_handler(~0x01, 0x08, 0x3f); // display on;
GLCD_handler(~0x01, 0x08, 0xc0); // start line set;

for (i = 0 ; i < 8 ; i ++ ) // page를 0부터 7까지 반복
{

for (j = 0 ; j < 64 ; j ++ ) // address를 0부터 63까지 반복
{

GLCD_handler(~0x01, 0x08, 0xb8|i); // page set;
GLCD_handler(~0x01, 0x08, 0x40|j); // address set;
GLCD_handler(0x01, 0x08, 0xff); // data write;
}
*/


  


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
    __delay_cycles(1000);
    __enable_interrupt();
  }
  display();  
     
  led();
  
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



