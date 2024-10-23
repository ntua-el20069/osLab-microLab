#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>

/*
 *   Simple program to read from A1 ADC input
 *   use POT2 for input
 */

int adc;

int table[13]={5 , 26, 46, 67, 87, 108 , 128, 148, 169, 189, 210, 230, 251};//values of duty cycle

int index=6;


                        // PB5/PB4 will be used to increment/decrement the PWM duty cycle
int last_pinb5 = 1;     // not pushed PB5
int last_pinb4 = 1;     // not pushed PB4
int now_pinb5; 
int now_pinb4; 
int pinb;

int led_out(int adc){       // mapping of ADC to PORT output
    if     (adc <= 200) return 0x01;
    else if(adc <= 400) return 0x02;
    else if(adc <= 600) return 0x04;
    else if(adc <= 800) return 0x08;
    else /* (adc <= 5) */ return 0x10;
}

int safe_incr(int x){

    if(x==12) return 12;
    else return x+1;
}

int safe_dec(int x){

    if(x==0) return 0;
    else return x-1;
}

void yesterday_check_pwm(){
    pinb = PINB;
    now_pinb5 = (pinb & (1 << PB5));
    now_pinb4 = (pinb & (1 << PB4));
    
    /*if(now_pinb4 == 0) {
        PORTD |= 0x02;
        _delay_ms(500);
    }*/
    
    if( /* (last_pinb5 == 1) && */ (now_pinb5 == 0)) {    // last time not pushed and now pushed
        safe_incr(index);                                    // increment duty cycle index
        PORTD |= 0x02;  // for debugging
        _delay_ms(500);
    }
    
    /*if( (last_pinb4 == 1) && (now_pinb4 == 0)) {
        safe_dec(index);
        PORTD |= 0x01;  // for debugging
        _delay_ms(500);
    } */
    
    OCR1AL = table[index];      // modify duty cycle
    
    last_pinb5 = now_pinb5;
    last_pinb4 = now_pinb4;

}

void check_pwm(){

    
    if(( PINB & (1 << PB5)) == 0) {    // last time not pushed and now pushed
        while(( PINB & (1 << PB5)) == 0) _delay_ms(10);
        safe_incr(index);                                    // increment duty cycle index
        PORTD |= (1 << PD7);  // for debugging
        _delay_ms(50);
    }
    
    if(( PINB & (1 << PB4)) == 0) {    // last time not pushed and now pushed
        while(( PINB & (1 << PB4)) == 0) _delay_ms(10);
        safe_dec(index);                                    // increment duty cycle index
        PORTD |= (1 << PD6);  // for debugging
        _delay_ms(50);
    }
    
    OCR1AL = table[index];      // modify duty cycle
    

}


int main(){
    
    DDRB = 0x00;        // PORT B as input for duty cycle switch 
    DDRC = 0x00;        // PORT C as input (required for ADC)
    DDRD = 0xFF;        // PORT D as output
    
    TCCR1A=(1<<WGM10)|(1<<COM1A1); // 8-bit phase correct PWM ,output PB1 
    TCCR1B=(1<<CS12)|(0<<CS11)|(0<<CS10); //frequency 62500Hz
    
    OCR1AL = table[index];  // initialize
    
    
    ADMUX = 0b01000001;     // REFSn[7:6] = 01 -> Vref => AVcc with external capacitor at AREF pin 
                            // ADLAR[5] = 0 -> right adjusted output
                            //  MUX[3:0] = 0001 for channel ADC1
    
    ADCSRA = 0b10000111;    // ADEN[7] = 1 -> enable ADC
                            // ADSC[6] = 0 -> no conversion
                            // ADIE[3] = 0 -> disable ADC interrupt
                            // ADPS[2:0] = 111 -> Division factor 128 so fADC = 16MHz/128 = 125kHz
    
    
    
    while (1){
               
        ADCSRA |= (1 << ADSC);  // start conversion
                                // ADCS will stay high as long as the conversion is in progress, and will be
                                // cleared by hardware when the conversion is completed
        while( (ADCSRA & (1 << ADSC)) == 1 ) ;   // wait until ADSC gets 0
        adc = ADCL + ADCH*256;   // read from adc (right adjusted) (read LOW before HIGH)

        PORTD = led_out(adc);          // show output in port D
        //PORTD = 0x00;
        
        check_pwm();        // check the buttons for duty cycle modifications
        
        _delay_ms(100);  
    }
    
    
    
    return 0;
}



