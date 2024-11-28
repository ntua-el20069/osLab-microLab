#include<stdint.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include "avr/io.h"
#include "DS1820_api.h"

uint8_t one_wire_reset(){
   
    DDRD =  1<<4;         //PD4 output
    PORTD =  ~(1<<4);   //PD4=0
    
    _delay_ms(0.48);   //480usec

    DDRD =  ~(1<<4);        //PD4 input
    PORTD =  ~(1<<4);   //disable pull-up
    
    _delay_ms(0.1);  //wait 100 usec for connected devices to transmit the presence pulse
            
    uint8_t output;
            
    output=PIND;  
    
    
    _delay_ms(0.38);
    
    if((output & (1<<PD4))== 0x00 )  return 1; //PD4 =0 connected device is detected
    
    return 0;   //connected device isn't detected
       
    
  }


uint8_t one_wire_receive_bit(){

    DDRD=0x10;   //PD4 as output
    PORTD=~(1<<PD4);
    _delay_ms(0.002);
    
    
    DDRD=0x00;       //PD4 input
    PORTD=~(1<<PD4);   //disable pull-up
    
    _delay_ms(0.01);
    
    uint8_t output=PIND;
    
    if((output & (1<<PD4))==0x00)  {
    
    _delay_ms(0.049);
    return 0;  //0 is read
    }

    else{
    _delay_ms(0.049);
    return 1; //1 is read
    }    

}

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
       aux = aux<<i;
       received_data|=aux;
      
     }

     return received_data;
}

void one_wire_transmit_byte(uint8_t data){

    uint8_t aux;
    for(int i=0; i<8; i++){
    
        aux=data & (1<<i);
        aux=aux>>i ; 
        one_wire_transmit_bit(aux);
        
         }

}

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


        info |= second;    
        
        info = info << 8;   //[second][0x00]
        
        info|=first; //[second][first]
    
    return info;
}

float convert(uint16_t res, int status){ // status 0 for DS1820, status 1 for DS18B20
	int result = (int16_t)res;
	float temp = -55.0 + (result + 110.0)*0.5;      // DS1820
	float temp_b = -55.0 + (result + 880.0)*0.0625; // DS18B20
    if(status==0) return temp;
    return temp_b;  
}

