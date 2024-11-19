#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>
#include "twi_api.h"
#include "lcd_api.h"
#include "DS1820_api.h"

uint16_t read_temperature(){

    uint16_t info=0x0000;
    uint8_t first, second;
    
    if(one_wire_reset() == 0)              //initialization and check for devices 
    {   PORTB=0x02;
        return 0x8000;  //no device
    
                }
    
    one_wire_transmit_byte(0xCC);  // off multiple device selection
     
    one_wire_transmit_byte(0x44);  //start  measurement
    
    while(!one_wire_receive_bit()) ; //wait for the measurement to finish
    
    if(one_wire_reset() == 0){
      
        PORTB=0x01;
     return 0x8000;   
    }
    
    one_wire_transmit_byte(0xCC);  // off multiple device selection

    one_wire_transmit_byte(0xBE);  // read 16-bits measurement
    
    first=one_wire_receive_byte();
    
    second= one_wire_receive_byte(); 

      PORTB=second;
    /*
        asm volatile(
        "mov r24, %0"    // Move the value of 'second' into register R24
        :                // No output operands
        : "r" (second)      // Input operand: 'second' stored in a general-purpose register
        : "r24"          // Clobber list: R24 is modified
    );
    
        
        asm volatile(
        "mov r25, %0"    // Move the value of 'second' into register R25
        :                // No output operands
        : "r" (first)      // Input operand: 'second' stored in a general-purpose register
        : "r25"          // Clobber list: R25 is modified
    );
      */

        info |= second;    
        
        info = info << 8;   //[second][0x00]
        
        info|=first; //[second][first]
        
    
    return info;
}


uint16_t get_temperature(void) {
	if (!one_wire_reset()) return 0x8000L;
	one_wire_transmit_byte(0xCC);
	one_wire_transmit_byte(0x44);
	while (!one_wire_receive_bit());
	if (!one_wire_reset()) return 0x8000L;
	one_wire_transmit_byte(0xCC);
	one_wire_transmit_byte(0xBE);
	
	uint16_t ret;
	ret = one_wire_receive_byte(); //LSB byte
	ret |= one_wire_receive_byte() << 8; //MSB byte

	return ret;
}

void no_device(){
     lcd_data('N');
     lcd_data('O');

     lcd_data(0b00100000); //break
     
     lcd_data('D');
     lcd_data('e');
     lcd_data('v');
     lcd_data('i');
     lcd_data('c');
     lcd_data('e');
}

float convert(uint16_t res, int status){ // status 0 for DS1820, status 1 for DS18B20
	int result = (int16_t)res;
	float temp = -55.0 + (result + 110.0)*0.5;      // DS1820
	float temp_b = -55.0 + (result + 880.0)*0.0625; // DS18B20
    if(status==0) return temp;
    return temp_b;  
}


void display_temperature(uint16_t response){
    float x = convert(response, 1); // status 1 for DS18B20
    
    // + or - in lcd and get absolute value in x
    if(x < 0) {lcd_data('-') ; x = -x;}
    else lcd_data('+');
    
    int a,b,c,d,e,f,g; // number format: abc.defg

    // x integer and decimal part
    int x_i = (int)x;
    int x_d = (int)(10000*(x - x_i));

    a = x_i/100;            // hundreds;
    b = (x_i%100)/10;       // decades
    c = x_i%10;             // units
    d = x_d/1000;           // first decimal 
    e = (x_d%1000)/100;     // second
    f = (x_d%100)/10;       // third
    g = x_d%10;


    if(a==0 && b==0) ;
    else {
            if(a!=0) lcd_data(a | 0b00110000);
            lcd_data(b | 0b00110000);
    }
    lcd_data(c | 0b00110000);
    lcd_data('.');
    lcd_data(d | 0b00110000);
    lcd_data(e | 0b00110000);
    lcd_data(f | 0b00110000);
    lcd_data(g | 0b00110000);

    // dot // lcd_data(0b00101110);           // code for dot '.'
}

int main(){

// ... init ... twi, lcd, temp sensor
// while true
    // clear display
    // take temp
    // if not 0x80 convert and display
    // else no device
    // delay
    
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0 ,0x00); //IO0 as output for LCD
    
    lcd_init();
    lcd_clear_display();

    DDRB=0xFF; //output for check
    
    uint16_t answer;
    
    while(1){
       lcd_clear_display();
        
       answer = read_temperature();
       if(answer == 0x8000) no_device(); 
       else display_temperature(answer);
       
       _delay_ms(1000); 
    }    

}
