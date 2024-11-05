#include "lcd_api.h"
#include "twi_api.h"
#include "avr/io.h"
#include<util/delay.h>

void type_filothei(){
     lcd_data('F');
     lcd_data('I');
     lcd_data('L');
     lcd_data('O');
     lcd_data('T');
     lcd_data('H');
     lcd_data('E');
     lcd_data('I');
     
     for(int i=0; i<9; i++)
     {lcd_data(0b00100000);} //break
     
     lcd_data('L');
     lcd_data('I');
     lcd_data('N');
     lcd_data('A');
     lcd_data('R');
     lcd_data('D');
     lcd_data('A');
     lcd_data('T');
     lcd_data('O');
     lcd_data('U');
    

}


void type_nikos(){
     lcd_data('P');
     lcd_data('A');
     lcd_data('P');
     lcd_data('A');
     lcd_data('K');
     lcd_data('O');
     lcd_data('N');
     lcd_data('S');
     lcd_data('T');
     lcd_data('A');
     lcd_data('N');
     lcd_data('T');
     lcd_data('O');
     lcd_data('P');
     lcd_data('O');
     lcd_data('U');
     lcd_data('L');
     lcd_data('O');
     lcd_data('S');
   
    lcd_data(0b00100000); //break
     
     lcd_data('N');
     lcd_data('I');
     lcd_data('K');
     lcd_data('O');
     lcd_data('S');
    

}
void main(void) {
    lcd_init();

   while(1){    
    lcd_clear_display();
    
    type_filothei();
    
    _delay_ms(5000);
    
    lcd_clear_display();
    
    type_nikos();
    
    _delay_ms(5000);
    
   
   }

    
    
}
