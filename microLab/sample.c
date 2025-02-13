
#define F_CPU 16000000UL    // ?
#include <util/delay.h>
#include <avr/io.h>

#define DEL 500U

int main(void)
{
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
 
    PORTB = 0xFF;
    while (1)
    {
        PORTC = 0xFF;
        PORTD = 0x00;
        _delay_ms(DEL);
        PORTC = 0x00;
        PORTD = 0xFF;
        _delay_ms(DEL);
    }

}
