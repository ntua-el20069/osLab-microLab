#include "avr/io.h"

void adc_init(){
    ADMUX = 0b01000000;     // REFSn[7:6] = 01 -> Vref => AVcc with external capacitor at AREF pin 
                            // ADLAR[5] = 0 -> right adjusted output
                            //  MUX[3:0] = 0001 for channel ADC0 - POT1   
    ADCSRA = 0b10000111;    // ADEN[7] = 1 -> enable ADC
                            // ADSC[6] = 0 -> no conversion
                            // ADIE[3] = 0 -> disable ADC interrupt
                            // ADPS[2:0] = 111 -> Division factor 128 so fADC = 16MHz/128 = 125kHz
}



uint16_t adc_read(){
        ADCSRA |= (1 << ADSC);  // start conversion
                                // ADCS will stay high as long as the conversion is in progress, and will be
                                // cleared by hardware when the conversion is completed
        while( (ADCSRA & (1 << ADSC)) == 1 ) ;   // wait until ADSC gets 0
        uint16_t adc =  ADCL + ADCH*256  ;   // read from adc (right adjusted)
                                    // we should firstly read Low and then High
                                    // else it does not give expected output
        return adc; // or return compute_avg(adc) ?
}

