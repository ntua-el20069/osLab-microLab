#define F_CPU 16000000UL // F_CPU should be defined before include delay.h
#include <util/delay.h>

#include "avr/io.h"
#include <string.h>
#include <stdio.h>

#include "DS1820_api.h"     // our header files
#include "adc_api.h"        
#include "twi_api.h"
#include "lcd_api.h"
#include "keyboard_api.h"
#include "uart_api.h"

int main(){

    twi_init();

    DDRC = 0xFF;   // PORTC as output for leds PC0 - PC5
    DDRB = 0xFF;
    PORTC = 0x00; // portC off
    PORTB = 0x00;
    PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); // IO1_4 -> IO1_7 as input pins, IO1_0 -> IO1_3 as output ports


    char key_pressed;
    uint8_t ascii_code;
    uint8_t sum = 0;
    int count = 0;
    while(1){
        sum = 0;
        while(!scan_keypad_rising_edge()) ; // wait until a change occured in keypad
        
        key_pressed = keypad_to_ascii();
        if(key_pressed == ' ') continue;
        
        ascii_code = key_pressed;
        
        for(count = 0; count<8; count++){
            if ( (ascii_code & 0x01) == 0 ) sum += 1;
            ascii_code = ascii_code >> 1;
        }
        PORTB = ((sum << 1) & 0x10) | ((sum << 1) & 0x08) | ((sum << 1) & 0x04) | ((sum << 1) & 0x02);
        
        if(key_pressed == '2') {
            PORTC = 0x02;
            _delay_ms(2000);
            PORTC = 0x00;
        }

    }
    
    
    return 0;
}
