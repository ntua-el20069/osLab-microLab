#include<avr/io.h>
#include<avr/interrupt.h>
#include "avr/io.h"
#include "string.h"
#include<util/delay.h>
#include "twi_api.h"


int write_2_nibbles(int a){
    //PCA9555_0_write(REG_CONFIGURATION_0,0xFF); //input
    uint8_t input=PCA9555_0_read(REG_OUTPUT_0) ;  
    uint8_t first_nibble=a;
    uint8_t second_nibble;
    
    input&=0x0f;   //we have to keep PD[3:0] because PD2->RS and PD3->Enable
   
    first_nibble&=0xf0;
    
    first_nibble=input + first_nibble;
    
    //PCA9555_0_write(REG_CONFIGURATION_0,0x00); //output
    
    PCA9555_0_write(REG_OUTPUT_0,first_nibble) ; 
    
    first_nibble|=(1<<3); //enable pulse
    
    PCA9555_0_write(REG_OUTPUT_0,first_nibble) ; 
    
        __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
      
      first_nibble&=~(1<<3);  //clear PD3
      PCA9555_0_write(REG_OUTPUT_0,first_nibble) ; 
      
    second_nibble=a;
    second_nibble=(second_nibble << 4) | (second_nibble >> 4); //swaps the nibbles
    second_nibble&=0xf0;
    second_nibble=input +second_nibble;
    
  
    PCA9555_0_write(REG_OUTPUT_0,second_nibble) ; 
    
    second_nibble|=(1<<3);  //enable pulse
    
     PCA9555_0_write(REG_OUTPUT_0,second_nibble) ;
    
    __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");


      second_nibble&=~(1<<3); //clear pulse
      
      PCA9555_0_write(REG_OUTPUT_0,second_nibble) ;
    return 1;
}
int lcd_command(int a){
    
     //PCA9555_0_write(REG_CONFIGURATION_0,0xFF); //input
    uint8_t res=PCA9555_0_read(REG_OUTPUT_0);
    
     res &=~(1<<2);         //Clear PD2 so that command transfer            
    
     //PCA9555_0_write(REG_CONFIGURATION_0,0x00); //output
     PCA9555_0_write(REG_OUTPUT_0,res);
     
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
     //PCA9555_0_write(REG_CONFIGURATION_0,0xFF); //input
    //Read IO1
    uint8_t res=PCA9555_0_read(REG_OUTPUT_0);
 
    res|=(1<<2);   //set the bit IO1_2 for data
    
    //PCA9555_0_write(REG_CONFIGURATION_0,0x00); //output
    
    PCA9555_0_write(REG_OUTPUT_0,res); 
    
    
    write_2_nibbles(data);
      
     _delay_ms(0.25);  //1usec=0.001msec
}


void lcd_init(){
    uint8_t value;
    //PCA9555_0_write(REG_CONFIGURATION_0,0x00); //output
    //count 1
    _delay_ms(200);
     
     value=0x30;       //command to switch to 8 bit mode
      
     PCA9555_0_write(REG_OUTPUT_0,value);  //maybe i have to delete it
     
     value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_0,value);

      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed
  
      
         value &= ~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_0,value);
      
         _delay_ms(0.25);
      
    //count 2
    _delay_ms(200);
     
     value=0x30;       //command to switch to 8 bit mode
      
     PCA9555_0_write(REG_OUTPUT_0,value);  //maybe i have to delete it
     
     value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_0,value);

      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed
  
      
         value &= ~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_0,value);
      
         _delay_ms(0.25);
      
      
         
         //count 3
    _delay_ms(200);
     
     value=0x30;       //command to switch to 8 bit mode
      
     PCA9555_0_write(REG_OUTPUT_0,value);  //maybe i have to delete it
     
     value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_0,value);

      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed
  
      
         value &= ~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_0,value);
      
         _delay_ms(0.25);
      
         
         
      
       value=0x20;       //command to switch to 8 bit mode
      
      PCA9555_0_write(REG_OUTPUT_0,value); //maybe i have to delete this command
       value|=(1<<3);                 //Enable pulse

     PCA9555_0_write(REG_OUTPUT_0,value);
            __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
       value&=~(1 << 3);                  // Clear pulse
      
         PCA9555_0_write(REG_OUTPUT_0,value);
      
      _delay_ms(0.25);
      
      
      lcd_command(0x28); //5x8 dots ,2 lines
      

      lcd_command(0x0c); //display on ,cursor off
       
       lcd_clear_display();
       
       lcd_command(0x06);
       return ;
}

void display_float(float x){
    
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

void lcd_message(char *message){
    lcd_clear_display();    // clear screen
    for(int i=0; i<strlen(message); i++){    // send the message
        lcd_data(message[i]);
    }
}

void lcd_twice_float(float t, float p){


    lcd_data('T');
    lcd_data(':');
    
    // + or - in lcd and get absolute value in x
    if(t < 0) {lcd_data('-') ; t = -t;}
    else lcd_data('+');
    
    int a,b,c,d,e,f,g; // number format: abc.defg

    // x integer and decimal part
    int x_i = (int)t;
    int x_d = (int)(10000*(t - x_i));

    a = x_i/100;            // hundreds;
    b = (x_i%100)/10;       // decades
    c = x_i%10;             // units
    d = x_d/1000;           // first decimal 
    e = (x_d%1000)/100;     // second


    if(a==0 && b==0) ;
    else {
            if(a!=0) lcd_data(a | 0b00110000);
            lcd_data(b | 0b00110000);
    }
    
    lcd_data(c | 0b00110000);
    lcd_data('.');
    lcd_data(d | 0b00110000);
    lcd_data(e | 0b00110000);
    lcd_data(' ');

    lcd_data('P');
    lcd_data(':');

    
    if(p < 0) {lcd_data('-') ; p = -p;}
    else lcd_data('+');
    
    // x integer and decimal part
    x_i = (int)p;
     x_d = (int)(10000*(p - x_i));

    a = x_i/100;            // hundreds;
    b = (x_i%100)/10;       // decades
    c = x_i%10;             // units
    d = x_d/1000;           // first decimal 
    e = (x_d%1000)/100;     // second


    if(a==0 && b==0) ;
    else {
            if(a!=0) lcd_data(a | 0b00110000);
            lcd_data(b | 0b00110000);
    }
    
    lcd_data(c | 0b00110000);
    lcd_data('.');
    lcd_data(d | 0b00110000);
    lcd_data(e | 0b00110000);
       // dot // lcd_data(0b00101110);           // code for dot '.'


}
void lcd_message_new_line(char *message){
    lcd_command(0xc0);    // clear screen
    for(int i=0; i<strlen(message); i++){    // send the message
        lcd_data(message[i]);
    }
}
