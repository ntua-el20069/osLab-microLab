#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>

/*
 *   Simple program to read from A1 ADC input
 *   use POT2 for input
 */

int adc;
float Vadc;

float adc_to_voltage(int adc){
    float Vref = 5.0;   // 5V reference
    return Vref*adc/1024;   // 10 bits resolution -> 1024
}

int led_out(float Vadc){
    if     (Vadc <= 0.625) return 0x01;
    else if(Vadc <= 1.25) return 0x02;
    else if(Vadc <= 1.875) return 0x04;
    else if(Vadc <= 2.5) return 0x08;
    else if(Vadc <= 3.125) return 0x10;
    else if(Vadc <= 3.75) return 0x20;
    else if(Vadc <= 4.375) return 0x40;
    else /* (Vadc <= 5) */ return 0x80;
}

int main(){
    
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
        Vadc = adc_to_voltage(adc);     // convert adc to voltage
        PORTD = led_out(Vadc);          // show output in port D
        
        _delay_ms(100);  
    }
    
    
    
    return 0;
}



