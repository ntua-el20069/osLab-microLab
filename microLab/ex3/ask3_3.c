#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>

int table[13]={5 , 26, 46, 67, 87, 108 , 128, 148, 169, 189, 210, 230, 251};//values of duty cycle

int index=6;
int mode=-1;
int DC_VALUE=128;


void mode_1(){
    mode=1;
    int flag=PIND;
    
    if( (flag & (1<<PD2)  ) == 0)  //PD2 pushed
    {            index=safe_dec(index);
                   OCR1AL=table[index];
                   _delay_ms(500);
   }  
    
    else{
    
        if( (flag & (1<<PD1)  ) == 0) //PD1 pushed
        {
                  index=safe_incr(index);
                   OCR1AL=table[index];
                   _delay_ms(500);
        }
        
        else {      if( (flag & (1<<PD4)  ) == 0)  //PD4 pushed
    {            index=safe_dec(index);
                   OCR1AL=table[index];
                   _delay_ms(500);}  
    
    else{
    
        if( (flag & (1<<PD3)  ) == 0) //PD3 pushed
        {
                  index=safe_incr(index);
                   OCR1AL=table[index];
                   _delay_ms(500);
        }
            
       
    }
    
        }
    }
     return;
}
void mode_2(){
    
    mode=2;
    ADCSRA|=(1<<ADSC);  //set ADSC flag to start
    
    int flag=ADCSRA & (1<<ADSC);
    
    while(flag!=0){
    
        flag=ADCSRA &(1<<ADSC);
    }
    
     int result=ADCH;
     
     OCR1AL=result;   
     
     _delay_ms(500);
     return;
    }

void main(void) {
    
    TCCR1A=(1<<WGM10)|(1<<COM1A1); // fast PWM ,output PB1 
    TCCR1B=(1<<CS12)|(1<<WGM12); //frequency 62500Hz

    DDRB=0b00000010;  //PB1 output
    DDRD=0x00;        //PORTD input
    DDRC=0x00; //input
  
   ADMUX=0b01100000; //Vref=5V, ADC0, left adjust
  
   ADCSRA=0b10000111; //ADC enable, No Conversion, disable adc interrupt 125kHz
    
    OCR1AL=128;
    _delay_ms(10);
     
    
    while(1){
    int status= PIND;
   
    if(( status & (1<<PD6)) == 0 ) //PD6 is  pushed
      {mode_1();  _delay_ms(100);} 
    
    else 
    {     if(( status & (1<<PD7)) == 0)  //PD7 pushed
               {      mode_2();  _delay_ms(100);} 
    
   
         else{
         if(mode == 2) mode_2();
         
           else{ 
               
               if(mode == 1) mode_1();
               else{
                   
                     if( (status & (1<<PD3))== 0) //PD3 pushed
               {   index=safe_incr(index);
                   OCR1AL=table[index];
                   _delay_ms(100);
           
                                }
                   
                   else{
               if((status & (1<<PD4))== 0 ){ //PD4 pushed
                   index=safe_dec(index);
                   OCR1AL=table[index];
                   _delay_ms(100);
               
               
               }
           
               
               
               
               
               }
         }
         }
               }
    
   
    }
}
}
int safe_incr(int x){

    if(x==12) return 12;
    else return x+1;
}

int safe_dec(int x){

    if(x==0) return 0;
    else return x-1;
}
