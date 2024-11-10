#define F_CPU 16000000UL
#include<util/delay.h>
#include "avr/io.h"
#include "twi_api.h"
#include "keyboard_api.h"

#define LED_PB0_ON 0x01       
#define LED_PB1_ON 0x02       
#define LED_PB2_ON 0x04       
#define LED_PB3_ON 0x08     


extern uint16_t pressed_keys = 0; 

void success(){
    PORTB = 0xFF;
    _delay_ms(3000);
    PORTB = 0x00;
    PORTB = 0x15;      // in order to show that no other number can be pressed
    _delay_ms(2000);
    PORTB = 0x00;      // until it gets 0x00
}

void fail(){
    for(uint8_t i=0; i<5; i++){
        PORTB = 0xFF;
        _delay_ms(500);
        PORTB = 0x00;
        _delay_ms(500);
    }
}

int main(void) {
 twi_init();
 
 DDRB = 0xFF;   // PORTB as output for leds PB0 - PB5
 PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); // IO1_4 -> IO1_7 as input pins, IO1_0 -> IO1_3 as output ports
 
 char first = ' ', second = ' ';    // for digits we wait (51 our number)
 char key_pressed;
 
 while(1){
     
     while(!scan_keypad_rising_edge()) ; // wait until a change occured in keypad
     
     key_pressed = keypad_to_ascii();
     
     if(key_pressed!=' ') {
         first = second;    // shift second digit to first
         second = key_pressed;
         
         if(first==' ') {PORTB = 0x01; continue;} // only 1 number pressed
         
         if(first=='5' && second=='1') success();
         else fail();
        
         // independently of success or failure discard the pressed keys
         first = ' ';
         second = ' ';
     }
      
 }
 
}
