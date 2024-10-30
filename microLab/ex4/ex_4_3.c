#define F_CPU 16000000UL
#define CO_limit    205
#include "avr/io.h"
#include <util/delay.h>


int led_out(int adc){
    if     (adc <= 170) return 0x01;
    else if(adc <= 340) return 0x02;
    else if(adc <= 510) return 0x04;
    else if(adc <= 680) return 0x08;
    else if(adc <= 850) return 0x10;
    else /* (adc <= 1024) */ return 0x20;
}

void lcd_data(int data){
    PORTD|=(1<<PD2);
   
    int status=write_2_nibbles(data);
      
      _delay_ms(0.25);  //1usec=0.001msec
}
void lcd_init(){

     _delay_ms(200);
     
      PORTD=0x30;       //command to switch to 8 bit mode
      PORTD|=(1<<PD3);  //Enable pulse
       
      //_delay_ms(0.000125); 2 nop -> 0.125 usec
      
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
  //__asm__ tells the compiler to insert an assembly instruction.
//_volatile__ prevents the compiler from optimizing away this statement, ensuring that the NOP operation is indeed executed

      PORTD &=0b11110111;  // Clear PD3
      
      _delay_ms(0.25);
      
       PORTD=0x30;       //command to switch to 8 bit mode
      PORTD|=(1<<PD3);  //Enable pulse
          __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
     
      PORTD &=0b11110111;  // Clear PD3
      
      _delay_ms(0.25);
      
         PORTD=0x30;       //command to switch to 8 bit mode
      PORTD|=(1<<PD3);  //Enable pulse
          __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
     
      PORTD &=0b11110111;  // Clear PD3
      
      _delay_ms(0.25);
      
      
       PORTD=0x20;       //command to switch to 4 bit mode
      PORTD|=(1<<PD3);  //Enable pulse
            __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
      PORTD &=0b11110111;  // Clear PD3
      
      _delay_ms(0.25);
      
      int status;
      
      status=lcd_command(0x28); //5x8 dots ,2 lines
      

       status=lcd_command(0x0c); //display on ,cursor off
       
       lcd_clear_display();
       
       lcd_command(0x06);
       return ;
}

int lcd_command(int a){
    PORTD &=0b11111011 ;  //PD2 0 ->instruction
     int status;
     
     status=write_2_nibbles(a);
    
     _delay_ms(0.25);
    return 1;
}

int write_2_nibbles(int a){

    int input=PIND ;  
    int first_nibble=a;
    int second_nibble;
    
    input&=0x0f;   //we have to keep PD[3:0] because PD2->RS and PD3->Enable
   
    first_nibble&=0xf0;
    
    first_nibble=input + first_nibble;
    
    PORTD=first_nibble;
    
    PORTD|=(1<<PD3); //enable pulse
    
        __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
    PORTD &=0b11110111;  // Clear PD3
    
    second_nibble=a;
    second_nibble=(second_nibble << 4) | (second_nibble >> 4); //swaps the nibbles
    second_nibble&=0xf0;
    second_nibble=input +second_nibble;
    
    PORTD=second_nibble;
    
    PORTD|=(1<<PD3); //enable pulse
    __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      
    PORTD &=0b11110111;  // Clear PD3

    return 1;
}
void clear_show(){

    lcd_data('C');
    lcd_data('L');
    lcd_data('E');
    lcd_data('A');
    lcd_data('R');
    
    return;
}

void gas_warning(){
    
    lcd_clear_display();
    _delay_ms(5);
    lcd_data('G');
    lcd_data('A');
    lcd_data('S');
    lcd_data(0b00100000);  // this is for break ' '
    lcd_data('D');
    lcd_data('E');
    lcd_data('T');
    lcd_data('E');
    lcd_data('C');
    lcd_data('T');
    lcd_data('E');
    lcd_data('D');
    
    return;
}
int lcd_clear_display(){

    int status=lcd_command(0x01);
    _delay_ms(5);    
    return 1;
}
void main(void) {
    uint16_t adc;
    uint8_t div_res, first_decimal,second_decimal, level, warning_flag;
    int order=0;

       ADMUX = 0b01000010;     // REFSn[7:6] = 01 -> Vref => AVcc with external capacitor at AREF pin 
                            // ADLAR[5] = 0 -> right adjusted output
                            //  MUX[3:0] = 0010 for channel ADC2
    
      ADCSRA = 0b10000111;    // ADEN[7] = 1 -> enable ADC
                            // ADSC[6] = 0 -> no conversion
                            // ADIE[3] = 0 -> disable ADC interrupt
                            // ADPS[2:0] = 111 -> Division factor 128 so fADC = 16MHz/128 = 125kHz
    
      DDRD=0xFF;         //PORTD output
      
      DDRB = 0xFF;      // PORTB output
      lcd_init();
      
      warning_flag = 0;
      
      while(1){
      
    ADCSRA|=(1<<ADSC);  //set ADSC flag to start
    
    int flag =ADCSRA & (1<<ADSC);
    
    while(flag!=0){
    
        flag=ADCSRA & (1<<ADSC);
    }  
    
      adc =  ADCL + ADCH*256  ;   // read from adc (right adjusted)
                                    // we should firstly read Low and then High
                                    // else it does not give expected output
      level = led_out(adc);
      
      if(warning_flag){
          if(adc > CO_limit){
              // PORTB handling   on/off
              
              if(order == 1 )
              {PORTB=0x00;
              order=0;
              }
              
              else{
                  PORTB=level;
                  order=1;
              }
              
          }
          else{
              lcd_clear_display();
              _delay_ms(5);
              clear_show();
              _delay_ms(500);       // CAUTION this should change
              lcd_clear_display();
              warning_flag = 0;
          }
      }
      else{ // less CO gas before than limit
        if(adc > CO_limit){
            gas_warning();
            warning_flag=1;
            }
            PORTB = level;
            order=1;
        }

       _delay_ms(100);

      
}
}
