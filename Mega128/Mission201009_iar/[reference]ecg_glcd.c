///+++++++++++++++++++
//Chip type        : Atmega128
//Program type        : Application
//Clock frequency        : 16.000000 MHz
//Memory model        : Small
//External SRAM size    : 0
//Data Stack size        : 1024
//++++++++++++++++++++++++++++

#include <mega128.h>
#include <mega128_bits.h>
#include <stdio.h>
#include <delay.h>
#include <glcd.h>




signed long maf_temp[8]={0} , maf_result, ecg_data, rr_interval, delay, heart_rate;
signed long diff_filter[5]={0}, diff_data, lpf_filter[5]={0}, lpf_data;
unsigned char ecg_high, ecg_low, maf_high, maf_low, filter_high, filter_low;
unsigned long x_position =1;
unsigned long y_position =0;
unsigned long y_preposition=0; 
int flag,count_a=0;          
int flag_g=0;

void read_adc_data();
void write_adc_data();
void moving_average();
void ecg_filter();
void peak_detection();
void lcd_out_ecg(void);
void draw_line(void);



interrupt [TIM0_COMP] void timer0_comp_isr(void)      //타이머0 비교매치 인터럽트
{
    flag=1;
    PORTD=0xff;
}

#define ADC_VREF_TYPE 0x40     

unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);    
delay_us(10);
ADCSRA|=0x40; 
while ((ADCSRA & 0x10)==0); 
ADCSRA|=0x10;    
return ADCW;
}



void draw_line(void){
    int i, j;
    glcd_line(0,0,127,0);
    glcd_line(0,0,0,63);
    glcd_line(0,63,127,63);
    glcd_line(127,0,127,63);
}

void main(void)
{
GLCDINIT_t init;
DDRA = 0xff; // 포트 초기화 (Control PORT)
DDRC = 0xff; // 포트 초기화 (Data PORT)   
PORTA = 0x00;
PORTC = 0x00;
ASSR=0x00;    //0x00=0 , 0x0e=14;
TCCR0=0x0e;   //분주비 256   ctc mode
TCNT0=0x00;
OCR0=0x00;   

TIMSK=0x02;  //인터럽트 허용
ETIMSK=0x00;

//ADC initialization
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x84;

UCSR0A=0x00;       //usart 설정
UCSR0B=0x08;
UCSR0C=0x06;
UBRR0H=0x00;   //baud rate high
UBRR0L=0x08;   //baud rate low         baud rate 115200

glcd_init(&init); // glcd 초기화
glcd_clear();
draw_line();



#asm("sei")

while (1)
{
    if(flag==1){                        //인터럽트 발생시 마다 read adc -> MAF(->Filter -> peak detection)  -> write adc
                                        
        read_adc_data();
        moving_average();
        write_adc_data();
        if(flag_g == 4 ){     // 샘플링 5번 할때 한번씩 lcd 출력
            lcd_out_ecg();              
            flag_g = 0;
        }
        else{
            flag_g++;
        }
        PORTD=0x00;      

        OCR0=0x81;                    // 클럭 -> 16MHz/256/(1+128) =484.49Hz    sampling rate 480Hz에 가장 근사하게 만듬 
        flag=0;
        
}       
};
}

void read_adc_data(){
    ecg_data=read_adc(0);
    ecg_high=ecg_data>>8;
    ecg_low=ecg_data;
    
    if(ecg_low==255){ecg_low=254;}       //sync code와 중복 방지
}

void write_adc_data(){
    putchar(255);       //sync code  출력
    putchar(ecg_high);     // 결과값 출력
    putchar(ecg_low);
    putchar(maf_high);
    putchar(maf_low);
    putchar(filter_high);
    putchar(filter_low);
    putchar(heart_rate);
}

void moving_average(){
    int i;
    maf_result=0; 
    maf_temp[count_a]=ecg_data;         //maf_temp에 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2 .... 순으로 ecg데이터 입력
    for(i=0;i<8;i++){                   //ecg data 8개를 더함
        maf_result += maf_temp[i];
}
maf_result =maf_result/8;                //exg data 8개 평균
ecg_filter();                             //filter 시작
count_a +=1;                              //count_a 1증가
if(count_a>7){count_a=0;}                                   
maf_high=maf_result>>8;                        //통신을 위해서 8bit씩 분리
maf_low=maf_result;
if(maf_low==255){maf_low=254;}          //sync code와 중복 방지
}

void ecg_filter(){
    int i;
    diff_data=0;
    diff_filter[0]=maf_result;
    diff_data=diff_filter[0]-diff_filter[4]; //60hz notchfilter
    lpf_filter[0]=diff_data;
    lpf_data=lpf_filter[0]+4*lpf_filter[1]+6*lpf_filter[2]+4*lpf_filter[3]+lpf_filter[4];     //lowpass filter

     for(i=4;i>0;i--){
        diff_filter[i]=diff_filter[i-1];             //notchfilter를 위한 데이터 5개  저장 buffer 1비트씩 shift
    }
    for(i=4;i>0;i--){                         // lowpass filter를 위한 데이터 5개 저장 buffer 1비트씩 shift
        lpf_filter[i]=lpf_filter[i-1];
    }
    filter_high=lpf_data>>8;                          //통신을 위해서 8bit씩 분리
    filter_low=lpf_data;
    if(filter_low==255){filter_low=254;}  //sync code와 중복 방지

    peak_detection(); 
               //heart rate 검출 시작                                       
}

void peak_detection(){
    
    if(lpf_data>800 && delay > 80){         //delay 80 => 80/480Hz = 166ms, threshold 800 => 800*(5V/1024)  =3.9 V
        heart_rate=60*480/rr_interval;            //sampling rate 약 480hz       heart rate = 60*(480/간격)
        delay=0;
        rr_interval=0;
    }
    else{                               // delay 1증가
        delay++;                        // 간격 1증가
        rr_interval++;
    }
      
}

void lcd_out_ecg(void){
    int i;
   
    for(i=1; i<63; i++){  // 점 그리기 전에 해당 세로줄 지우기
        glcd_putpixel(x_position,i,0);
    }
    y_position = 62-(62*maf_result/1024);     // maf한 결과를 높이 62칸에 알맞은 칸 정하기
    glcd_line(x_position, y_preposition, x_position, y_position); //이전의 점과 현재점 사이 직선으로 연결 
    y_preposition = y_position;  //현재y점을 이전의 y점으로 함
    if(x_position == 126){   // 가로축 끝인 126 지점이 되면 다시 1번 칸으로 되돌림
        x_position = 1;
    }
    else{
        x_position++;
    }
}    