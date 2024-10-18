#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

int active = 0; // is active = true that mean PB0 is on for 5 seconds.
// only when light PB0 is already on, should we light up all lights for 0.5sec
int t = 10; // counter for timer (10 x 500ms = 5 seconds)
int flag=0;


ISR (INT1_vect) {
    sei(); // enable interrupts again
     if(active==1 && flag==1){ _delay_ms(500); return;}// all leds should light up for 500ms}
    if (active == 1) { //anameno to 1 LED
        t = 10; // renew time...
        
        PORTB = 0xFF;
        flag=1;
        _delay_ms(500);
        flag=0;
       
    }
   
    PORTB = 0x01 ; // only PB0 should be on...
   
    active = 1;
    while(t > 0) {
     _delay_ms(500); // delay 500ms
     t -= 1;
    }
    active = 0;
    t = 10;
    PORTB = 0x00; //turn of the lights
   }


int main(int argc, char** argv) {
    EICRA = (1 << ISC10) | (1 << ISC11);
    EIMSK = (1 << INT1);
    sei();
    
    DDRB = 0xFF; // set PORTB as output
    
    while (1) {
    ;
    }
     
    return (0);
}
