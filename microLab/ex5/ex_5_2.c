// main code reading input not implemented
// only output handling 

#include<util/delay.h>
#include "twi_api.h"
#define F_CPU 16000000UL

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
 
 while(1)
 {
 PCA9555_0_write(REG_OUTPUT_0, 0x00);
 _delay_ms(1000);
 PCA9555_0_write(REG_OUTPUT_0, 0xFF);
 _delay_ms(1000);
 
 // if "*" seen, then LED PD4 on
 PCA9555_0_write(REG_OUTPUT_0, LED_PD4_ON);
 
 _delay_ms(300);    // REMOVE
 
 // if "0" seen, then LED PD5 on
 PCA9555_0_write(REG_OUTPUT_0, LED_PD5_ON);
 
 _delay_ms(300);    // REMOVE
 
 // if "#" seen, then LED PD6 on
 PCA9555_0_write(REG_OUTPUT_0, LED_PD6_ON);
 
 _delay_ms(300);    // REMOVE
 
 // if "D" seen, then LED PD7 on
 PCA9555_0_write(REG_OUTPUT_0, LED_PD7_ON);
 
 _delay_ms(300);    // REMOVE
 }
}
