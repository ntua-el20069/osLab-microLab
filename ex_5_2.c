// Example Code for using TWI & PCA9555 
// Exercise not implemented yet

#include<util/delay.h>
#include "twi_api.h"

int main(void) {
 twi_init();
 PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output

 while(1)
 {
 PCA9555_0_write(REG_OUTPUT_0, 0x00);
 _delay_ms(1000);
 PCA9555_0_write(REG_OUTPUT_0, 0xFF);
 _delay_ms(1000);
 }
}
