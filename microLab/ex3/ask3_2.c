#define F_CPU 16000000UL
#define BUFFER_SIZE 16
#include "avr/io.h"
#include <util/delay.h>
#include <stdint.h>  // For uint16_t and uint32_t types


/*
 *   Simple program to read from A1 ADC input
 *   use POT2 for input
 */

int adc;

int table[13]={5 , 26, 46, 67, 87, 108 , 128, 148, 169, 189, 210, 230, 251};//values of duty cycle

int index=6;

// Circular buffer and rolling sum
int buffer[BUFFER_SIZE] = {0};  // Stores last 16 values
int sum = 0;                    // Sum of the last N values
int LRU = 0;                   // Points to the next position in the buffer
int count = 0;                   // Keeps track of the number of values inserted



int led_out(int adc){
    if     (adc <= 200) return 0x01;
    else if(adc <= 400) return 0x02;
    else if(adc <= 600) return 0x04;
    else if(adc <= 800) return 0x08;
    else /* (adc <= 1024) */ return 0x10;
}

int safe_incr(int x){

    if(x==12) return 12;
    else return x+1;
}

int safe_dec(int x){

    if(x==0) return 0;
    else return x-1;
}

void check_pwm(){

    
    
    if(( PINB & (1 << PB5)) == 0) {    // if PINB5 pushed
        while(( PINB & (1 << PB5)) == 0) _delay_ms(10);  // wait until released
        index = safe_incr(index);                        // increment index for duty cycle
        PORTD |= (1 << PD7);  // for debugging
        _delay_ms(50);
    }
    
    if(( PINB & (1 << PB4)) == 0) {    
        while(( PINB & (1 << PB4)) == 0) _delay_ms(10);
        index = safe_dec(index);                                    
        PORTD |= (1 << PD6);  // for debugging
        _delay_ms(50);
    }
    
    //PORTD = (PORTD & 0b00111111);      // clear PD[7:6] (debug bits)
    
    OCR1AL = table[index];      // modify duty cycle

}

int compute_avg(int new_value) {
    // Subtract the value being replaced from the sum only if the buffer is full
    if (count == BUFFER_SIZE) {
        sum -= buffer[LRU];
    }

    // Insert the new value into the buffer
    buffer[LRU] = new_value;

    // Add the new value to the sum
    sum += new_value;

    // Move to the next LRU in the circular buffer
    LRU = (LRU + 1) % BUFFER_SIZE;

    // Increase count until it reaches the buffer size (16)
    if (count < BUFFER_SIZE) {
        count++;
    }

    // Return the average (sum / count), using a shift right by 4 only if count == 16
    if (count == BUFFER_SIZE) {
        return (sum >> 4);  // Equivalent to sum / 16
    } else {
        return (sum / count);  // For the initial case when count < 16
    }
}


int main(){
    
    DDRB = (1 << PB1);   // PORT B as input except PB1 output
    DDRC = 0x00;        // PORT C as input (required for ADC)
    DDRD = 0xFF;        // PORT D as output
    
    
    TCCR1A=(1<<WGM10)|(1<<COM1A1); // 8-bit phase correct PWM ,output PB1 
    TCCR1B= (1<<WGM12) | (1<<CS12); //frequency 62500Hz
    
    _delay_ms(100);
    OCR1AL = table[index];  // initialize
    _delay_ms(100);
    
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
        adc =  ADCL + ADCH*256  ;   // read from adc (right adjusted)
                                    // we should firstly read Low and then High
                                    // else it does not give expected output

            
        PORTD = led_out(compute_avg(adc));          // show output in port D
        
        check_pwm();
        
        _delay_ms(100);  
    }
    
    
    
    return 0;
}

