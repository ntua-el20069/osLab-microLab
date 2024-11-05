#include<avr/io.h>
#include<avr/interrupt.h>
#include "avr/io.h"
#include<util/delay.h>
#include "twi_api.h"


int write_2_nibbles(int a){

    uint8_t input=PCA9555_0_read(REG_INPUT_1) ;  
    uint8_t first_nibble=a;
    uint8_t second_nibble;
    
    input&=0x0f;   //we have to keep PD[3:0] because PD2->RS and PD3->Enable
   
    first_nibble&=0xf0;
    
    first_nibble=input + first_nibble;
    
    
    PCA9555_0_write(REG_OUTPUT_1,first_nibble) ; 
    
    first_nibble|=(1<<3); //enable pulse
    
    PCA9555_0_write(REG_OUTPUT_1,first_nibble) ; 
    
        __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
      
      first_nibble&=~(1<<3);  //clear PD3
      PCA9555_0_write(REG_OUTPUT_1,first_nibble) ; 
      
    second_nibble=a;
    second_nibble=(second_nibble << 4) | (second_nibble >> 4); //swaps the nibbles
    second_nibble&=0xf0;
    second_nibble=input +second_nibble;
    
  
    PCA9555_0_write(REG_OUTPUT_1,second_nibble) ; 
    
    second_nibble|=(1<<3);  //enable pulse
    
     PCA9555_0_write(REG_OUTPUT_1,second_nibble) ;
    
    __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");


      second_nibble&=~(1<<3); //clear pulse
      
      PCA9555_0_write(REG_OUTPUT_1,second_nibble) ;
    return 1;
}
int lcd_command(int a){
    uint8_t res=PCA9555_0_read(REG_INPUT_1);
    
     res &=~(1<<2);         //Clear PD2 so that command transfer            
    
     
     PCA9555_0_write(REG_OUTPUT_1,res);
     
     write_2_nibbles(a);
    
     _delay_ms(0.25);
    return 1;
}
int lcd_clear_display(){

    lcd_command(0x01);
    _delay_ms(5);    
    return 1;
}


void lcd_data(int data){
    
    //Read IO1
    uint8_t res=PCA9555_0_read(REG_INPUT_1);
 
    res|=(1<<2);   //set the bit IO1_2 for data
    
    write_2_nibbles(data);
      
     _delay_ms(0.25);  //1usec=0.001msec
}


void lcd_init(){
    uint8_t value;
    
    //count 1
    _delay_ms(200);
     
     value=0x30;       //command to switch to 8 bit mode
      
     PCA9555_0_write(REG_OUTPUT_1,value);  //maybe i have to delete it
     
     value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_1,value);

      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed
  
      
         value &= ~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_1,value);
      
         _delay_ms(0.25);
      
    //count 2
    _delay_ms(200);
     
     value=0x30;       //command to switch to 8 bit mode
      
     PCA9555_0_write(REG_OUTPUT_1,value);  //maybe i have to delete it
     
     value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_1,value);

      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed
  
      
         value &= ~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_1,value);
      
         _delay_ms(0.25);
      
      
         
         //count 3
    _delay_ms(200);
     
     value=0x30;       //command to switch to 8 bit mode
      
     PCA9555_0_write(REG_OUTPUT_1,value);  //maybe i have to delete it
     
     value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_1,value);

      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed
  
      
         value &= ~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_1,value);
      
         _delay_ms(0.25);
      
         
         
      
       value=0x20;       //command to switch to 8 bit mode
      
      PCA9555_0_write(REG_OUTPUT_1,value); //maybe i have to delete this command
       value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_1,value);
            __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
       value&=~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_1,value);
      
      _delay_ms(0.25);
      
      
      lcd_command(0x28); //5x8 dots ,2 lines
      

      lcd_command(0x0c); //display on ,cursor off
       
       lcd_clear_display();
       
       lcd_command(0x06);
       return ;
}



