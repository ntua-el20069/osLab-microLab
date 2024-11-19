#include<util/delay.h>
#include "avr/io.h"
#include "DS1820_api.h"


uint8_t one_wire_reset(){
   
     DDRD|=0x10;         //PD4 output
    PORTD&= (uint8_t) ~(1U<<4);   //PD4=0
    
    _delay_ms(0.48);   //480usec

    DDRD|=0x00;       //PD4 input
    PORTD&= (uint8_t) ~(1U<<4);   //disable pull-up
    
    _delay_ms(0.1);  //wait 100 usec for connected devices to transmit the presence pulse
            
    uint8_t output;
            
    output=PIND;  
    
    
    _delay_ms(0.38);
    
    if((output & (1<<PD4))== 0x00 )  return 1; //PD4 =0 connected device is detected
    
    else
    {return 0;  } //connected device isn't detected
       
    
  }


uint8_t one_wire_receive_bit(){

    DDRD|=0x10;   //PD4 as output
    PORTD=~(1<<PD4);
    _delay_ms(0.002);
    
    
    DDRD&= (uint8_t) ~(1U<<4);     //PD4 input
    PORTD&= (uint8_t) ~(1U<<4);   //disable pull-up
    
    _delay_ms(0.01);
    
    uint8_t output=PIND;
    
    if((output & (1<<PD4))==0x00)  {
    
    _delay_ms(0.049);
    return 0;  //0 is read
    }}

void one_wire_transmit_bit(uint8_t bit){
 DDRD=0x10;   //PD4 output
    
    PORTD=~(1<<PD4);
    
    _delay_ms(0.002);
    
    if(bit == 0x00 ) PORTD=~(1<<PD4);
    else if(bit == 0x01) PORTD=(1<<PD4);
    
    _delay_ms(0.058);  //wait 58 usec for connected device to sample line 
   
    DDRD=0x00;   //PD4 input
    PORTD=~(1<<PD4); //PD4 off to disable pull-up
    
    _delay_ms(0.001);  //recovery time
    
   }

uint8_t one_wire_receive_byte(){

 uint8_t received_data=0x00;
    
    uint8_t aux;
    
    
    for(int i=0; i<8; i++){
    
       aux=one_wire_receive_bit();
       aux<<i;
       received_data|=aux;
}
}
void one_wire_transmit_byte(uint8_t data){

    uint8_t aux;
    for(int i=0; i<8; i++){
    
        aux=data & (1<<i);
        aux=aux>>i ; 
        one_wire_transmit_bit(aux);
        
         }

}
