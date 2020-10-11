#include <iom128.h>
#include <stdio.h>

void delay(int n)
{

 int i, j;
 for (i=0; i<n; i++); for (j=0; j<10; j++);

}

 

void GLCD_handler(char rs, char cs, char data) // 데이터시트에 준하여 만든 GLCD용 기본 제어함수
{

delay(1); // 연속된 신호 입력에 따른 오류 방지용 delay 
PORTA = 0x00;   // Control Port가 새로운 입력을 받을 수 있는 상태로 만든다. 
PORTA  = PORTA & ~0x04; // E Fall
PORTA = PORTA & ~0x02; // R/W Fall
 
switch(rs) // RS choice
{
case  0x01: PORTA = PORTA | 0x01; break; // data mode
case ~0x01: PORTA = PORTA & ~0x01; break; // ins mode
}


PORTA = PORTA | cs; // CS choice
PORTA = PORTA | 0x04; // E Rise 
PORTF = data;   // Data Input
PORTA = PORTA & ~0x04; //  E Fall

}

 

void main(void)
{ 

int i, j; // 반복문에 쓰일 변수 선언
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

}

}