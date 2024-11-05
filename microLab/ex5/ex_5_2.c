#define F_CPU 16000000UL
#include<util/delay.h>
#include "twi_api.h"

#define LED_PD4_ON 0x01       // connected to IO0_0
#define LED_PD5_ON 0x02       // connected to IO0_1
#define LED_PD6_ON 0x04       // connected to IO0_2
#define LED_PD7_ON 0x08       // connected to IO0_3

#define STAR_MASK 0x10      // mask to see the corresponging bit for "*"
#define ZERO_MASK 0x20      // mask to see the corresponging bit for "0"
#define HASHTAG_MASK 0x40   // mask to see the corresponging bit for "#"
#define D_MASK  0x80        // mask to see the corresponging bit for "D"


int main(void) {
 twi_init();
 PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output (I need IO0_0 -> IO0_3 as output ports) 
 PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); // IO1_4 -> IO1_7 as input pins, IO1_0 as output port
 
 PCA9555_0_write(REG_OUTPUT_1, 0x00); // logic 0 written to IO1_0 in order to compare with keyboard inputs
 
 uint8_t keyboard_input;
 
 while(1)
 {

 keyboard_input = PCA9555_0_read(REG_INPUT_1);
 
 // if "*" seen, then LED PD4 on
 if((keyboard_input & STAR_MASK) == 0x00)      // pressed means 0 (connected to IO1_0)
    PCA9555_0_write(REG_OUTPUT_0, LED_PD4_ON);

 
 // if "0" seen, then LED PD5 on
 else if((keyboard_input & ZERO_MASK) == 0x00)
    PCA9555_0_write(REG_OUTPUT_0, LED_PD5_ON);

 
 // if "#" seen, then LED PD6 on
 else if((keyboard_input & HASHTAG_MASK) == 0x00)
    PCA9555_0_write(REG_OUTPUT_0, LED_PD6_ON);

 
 // if "D" seen, then LED PD7 on
 else if((keyboard_input & D_MASK) == 0x00)
    PCA9555_0_write(REG_OUTPUT_0, LED_PD7_ON);

 // else keep all leds off
 else PCA9555_0_write(REG_OUTPUT_0, 0x00);

 }
}
