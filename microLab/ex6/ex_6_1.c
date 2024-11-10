#define F_CPU 16000000UL
#include<util/delay.h>
#include "twi_api.h"
#include "keyboard_api.h"

#define LED_PB0_ON 0x01       
#define LED_PB1_ON 0x02       
#define LED_PB2_ON 0x04       
#define LED_PB3_ON 0x08     


extern uint16_t pressed_keys = 0; 

int main(void) {
 twi_init();
 
 DDRB = 0xFF;   // PORTB as output for leds PB0 - PB3
 PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); // IO1_4 -> IO1_7 as input pins, IO1_0 -> IO1_3 as output ports
 
 while(1){
     
     while(!scan_keypad_rising_edge()) ; // wait until a change occured in keypad
     
     if(keypad_to_ascii()=='A') PORTB = LED_PB0_ON;
     else if(keypad_to_ascii()=='8') PORTB = LED_PB1_ON;
     else if(keypad_to_ascii()=='6') PORTB = LED_PB2_ON;
     else if(keypad_to_ascii()=='*') PORTB = LED_PB3_ON;
     else PORTB = 0x00;
 }
 
}
