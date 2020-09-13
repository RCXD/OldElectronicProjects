# Fuse bits
## How to change clock speed (ATmel Studio 7.0.2397)
1. Open ATmel Studio 7.0
2. Connect your debugger or programmer to your Dev B/D
3. Tools > Device Programming
4. Enter ***Tool***, ***Device***, ***Interface*** and press ***Apply***
5. in Fuse menu, let's modify LOW.SUT_CKSEL. There are:
   1. External Clock; Start-up time ......
      1. Using External Clock, not internal, Xtal, or Resonator
      2. **WARNING**: You should input function(pulse) into pin 24(XTAL1 pin) of Mega128
      3. If you accidently stuck here, **see below**
   2. Internal RC Oscilator; ......
      1. Using built-in Oscilator in ATmega
      2. Quite accurate if you don't need realtime stuff
   3. External Xtal/Resonator Low, Med, High Freq, ......
      1. Utilzing Xtal at pin 24, 25
      2. I'm not sure how much this is accurate
      3. I believe that this is better option at least.
      4. Also you need to define F_CPU along with the form below
        ~~~c
        #define F_CPU 1000000UL
        #define F_CPU 2000000UL
        #define F_CPU 4000000UL
        #define F_CPU 8000000UL
        #define F_CPU 16000000UL
        //After include what you need
        #include <avr/io.h>
        #include <util/delay.h>
        ~~~


## If you stuck at external clock recovery
1. [Reference](https://beta.ivc.no/wiki/index.php/ATmega128_external_clock_recovery)
   1. Additional Arduino Function generator
    ~~~Arduino
    void setup(){
        pinMode(13, OUTPUT); //Built-in led to see working well
        pinMode(2, OUTPUT);  //pin for function generator
    }

    void loop(){
        int i = 5
        while(i-- >= 0){     //Initiating B/D state
            digitalWrite(13, HIGH); //Blink 5 times
            delay(500);
            digitalWrite(13, LOW);
            delay(500);
        }
        while{              //Function generator state
            digitalWrite(2, HIGH);
            digitalWrite(2, LOW); //No delay, as fast as possible
        }
    }
    ~~~