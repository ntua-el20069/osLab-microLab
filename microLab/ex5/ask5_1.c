#define F_CPU 16000000UL
#include "avr/io.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "twi_api.h"


uint8_t compute_f0(uint8_t a, uint8_t b,uint8_t c,uint8_t d){
    uint8_t var1=(!a)&b&c;
    uint8_t var2=(!b)&d;
    
    return !(var1 | var2);

}

uint8_t compute_f1(uint8_t a1, uint8_t b1,uint8_t c1,uint8_t d1){

   uint8_t var1= a1|b1|c1;
    uint8_t var2=(!d1)&b1;
    var2=var2 & var1;
    return  var2;
}

void main(void) {
    int input;
    uint8_t a,b,c,d,f0,f1,res;
    DDRB=0x00;  //input
    DDRD=0xFF;  //output (?)
    
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0,0x00); //Set EXT_PORT0 as output -> IO0_0 and IO0_1 ouputs
    
    while(1){
    input=PINB;
    a=input & (1<<PB0) ;
    b=input & (1<<PB1);
    b=b>>1 ;     //shift 1 position to the right so that the value is stored in LSB
    c=input & (1<<PB2);
    c=c>>2;     //shift 2 positions to the right so that the value is stored in LSB
    d=input & (1<<PB3);
    d=d>>3;     //shift 2 positions to the right so that the value is stored in LSB
    
    f0=compute_f0(a,b,c,d);
    f1=compute_f1(a,b,c,d);
    
    f1=f1<<1;
    
    res=f0 |f1;
    
     PCA9555_0_write(REG_OUTPUT_0,res);
     
     _delay_ms(500);
    }
    
    
}

