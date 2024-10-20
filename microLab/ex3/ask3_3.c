#define F_CPU 16000000UL
#include "avr/io.h"
#include <util/delay.h>

int table[13]={5 , 26, 46, 67, 87, 108 , 128, 148, 169, 189, 210, 230, 251};//values of duty cycle

int index=6;

int DC_VALUE=128;



void mode_0(){
    int stat=PIND && ( (1<<PD3) || (1<<PD4) );
    
    if( stat == 24 || stat ==0 ) return;  //PD3 not pushed and PD4 not pushed or both pushed
    
    else {
    
        if(stat == 8 ) //PD4 pushed
        {  index=safe_dec(index);
        
            //DC_VALUE=table[index];
            
            OCR1AL=table[index];
        
                 }
        
        else { //
            index=safe_incr(index);
            
            //DC_VALUE=table[index];
            
            OCR1AL=table[index];
        }
    
    }
    return;
}

void mode_1(){

    int flag=PIND && ((1<<PD2) | (1<<PD1));
    
    if(flag == 0 | flag == 6 ) //PD2 and PD1 both pushed or both not pushed
    {mode_0(); return;}  //check if PD3 or PD4 are pressed
    
    else{
    
        if(flag == 2) //PD2 pushed
        {
            DC_VALUE=safe_dec(DC_VALUE);
            OCR1AL=DC_VALUE;
            _delay_ms(500);
        }
        
        else //PD1 pushed
            
        {DC_VALUE=safe_dc_incr(DC_VALUE);
            OCR1AL=DC_VALUE;
          _delay_ms(500);
        }
    }
    return;
}
void mode_2(){
    ADCSRA|=(1<<ADSC);  //set ADSC flag to start
    
    int flag=ADCSRA && (1<<ADSC);
    
    while(flag!=0){
    
        flag=ADCSRA && (1<<ADSC);
    }
    
     int result=ADCH;
     
     OCR1AL=result;   
     
     _delay_ms(500);
     return;
    }

void main(void) {
    int PD6status, PD7status;
    TCCR1A=(1<<WGM10)|(1<<COM1A1); // fast PWM ,output PB1 
    TCCR1B=(1<<CS12)|(0<<CS11)|(0<<CS10); //frequency 62500Hz

    DDRB=0b00000010;  //PB1 output
    DDRD=0x00;        //PORTD input
    DDRC=0x00; //input
  
   ADMUX=0b01100000; //Vref=5V, ADC0, left adjust
  
   ADCSRA=0b10000111; //ADC enable, No Conversion, disable adc interrupt 125kHz
    
    OCR1AL=128;
    _delay_ms(10);
     
    
    while(1){
    int status= PIND && ((1<<PD6) | (1<<PD7));
   
    if(status == 0 || status == 192 ) //PD6 and PD7 both pushed or both not pushed
    {mode_0();  _delay_ms(100);} 
    
    else 
    {     if(status == 64 )  //PD7 pushed
    {      
        PD6status=1;  
        
            while(PD6status!=0) //break when PD6 pushed  
            {  mode_2();
            
               _delay_ms(100);
            
               PD6status=PIND && (1<<PD6);
                       }
    
               }
    
    else { //PD6 pushed
    
    PD7status=1;  
        
            while(PD7status!=0) //break when PD7 pushed  
            {  mode_1();
            
               _delay_ms(100);
            
               PD7status=PIND && (1<<PD7);
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
int safe_dc_incr(int x){

    if(x==255) return 255;
    else return x+1;
}
