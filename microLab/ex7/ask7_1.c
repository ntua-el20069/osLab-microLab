#define F_CPU 16000000UL
#include<util/delay.h>
#include "avr/io.h"
#include "DS1820_api.h"


uint16_t read_temperature(){

    uint16_t info=0x0000;
    uint8_t first, second;
    
    if(one_wire_reset() == 0)              //initialization and check for devices 
    {   PORTB=0x00;
        return 0x8000;  //no device
    
                }
    
    one_wire_transmit_byte(0xCC);  // off multiple device selection
     
    one_wire_transmit_byte(0x44);  //start  measurement
    
    while(!one_wire_receive_bit()) ; //wait for the measurement to finish
    
    if(one_wire_reset() == 0){
      
        PORTB=0x00;
     return 0x8000;   
    }
    
    one_wire_transmit_byte(0xCC);  // off multiple device selection

    one_wire_transmit_byte(0xBE);  // read 16-bits measurement
    
    first=one_wire_receive_byte();
    
    second= one_wire_receive_byte(); 

      PORTB=second;
    
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
                    

        info|=first;
        
        info<<8;     //[first][0x00]
        
        info|=second; //[first][second]
        
        return info;
}

/*void convert_to_temp(uint16_t measure){
    
    uint8_t val= measure & 0xFF;
    
    uint8_t div ,dec1,dec2, dec3;
     
    if((measure & (15<<1)) == 0x00)  //MSB=0 => positive
    {   div=val/2;
        
    
        if(val % 2 == 0){dec1=0; dec2=0; dec3=0;}
       
        else {dec1=5; dec2=0; dec3=0;}
        
    }

    else{  //negative
    
        val=val-1;
        
        uint8_t res = ~val;  //1-complement
        
        div=res/2;
        
    
        if(res % 2 == 0){dec1=0; dec2=0; dec3=0;}
       
        else {dec1=5; dec2=0; dec3=0;}
    
    }
    
}

*/
int main(){

    DDRB=0xFF; //output for check
    
    while(1){
    
        
       read_temperature();
       
       _delay_ms(1000);
        
}    




}