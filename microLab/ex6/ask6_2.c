#define F_CPU 16000000UL
#include<avr/io.h>
#include "keyboard_api.h"
#include "lcd_api.h"
#include "twi_api.h"
int main(){
   
    twi_init();
    PCA9555_0_write(6 ,0x00); //IO0 as output for LCD
    
    PCA9555_0_write(7,0xF0) ; //IO1[7:4]->inputs IO1[3:0]->outputs

    lcd_init();
    lcd_clear_display();
    
    while(1){
    
    
        while(!scan_keypad_rising_edge());
    
        lcd_clear_display();
        lcd_data( keypad_to_ascii());
    
    }


}

