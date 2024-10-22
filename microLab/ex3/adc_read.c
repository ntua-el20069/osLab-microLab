#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>

/*
 *   Simple program to read from A1 ADC input
 *   use POT2 for input
 */

int adc;

int led_out(int adc){
    if     (adc <= 200) return 0x01;
    else if(adc <= 400) return 0x02;
    else if(adc <= 600) return 0x04;
    else if(adc <= 800) return 0x08;
    else /* (adc <= 5) */ return 0x10;
}


int main(){
    
    DDRC = 0x00;        // PORT C as input (required for ADC)
    DDRD = 0xFF;        // PORT D as output
    
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
        while( (ADCSRA && (1 << ADSC)) == 1 ) ;   // wait until ADSC gets 0
        adc = ADCH*256 + ADCL;   // read from adc (right adjusted)

        PORTD = led_out(adc);          // show output in port D
        
        _delay_ms(100);  
    }
    
    
    
    return 0;
}



