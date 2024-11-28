#define F_CPU 16000000UL
#include <util/delay.h>
#include "avr/io.h"
#include <string.h>
#include "DS1820_api.h"

#include "adc_api.h"        // our header files
#include "twi_api.h"
#include "lcd_api.h"
#include "keyboard_api.h"
#include "uart_api.h"

#define UBRR 103    // for baud 9600
#define BUF_SIZE 40
char buffer[BUF_SIZE];  // buffer for receiving UART messages

uint16_t therm_answer;    // for temp answer from DS18B20
uint16_t adc;   // value from adc
float temperature;
float pressure;

int main(){
    
    // initializations
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0,0x00); //output
    lcd_init();
    adc_init();
    usart_init(UBRR);
    lcd_message("Program Start");
    // if needed restart
    //conversation("ESP:restart", buffer, BUF_SIZE, "\"Success\"");
    _delay_ms(2000);
    
    
    
    // maybe in a while loop to handle connection problems ?
    while(1){
        if ( conversation("ESP:connect", buffer, BUF_SIZE, "\"Success\"") ) {
            lcd_message("Connect Success");
            break;
        }
        else lcd_message("Connect Failure");
        // wait 20+ seconds for ESP to be able to reply again
        _delay_ms(10000);
        lcd_message("waiting...");
        _delay_ms(15000);
    }
    
    _delay_ms(3000);    // wait for the message to be seen
    
    if ( conversation("ESP:url:\"http://192.168.1.250:5000/data\"", buffer, BUF_SIZE, "\"Success\"") ) lcd_message("URL Success");
    else lcd_message("URL Failure");
    
    _delay_ms(3000);    // wait for the message to be seen
    
    
    
    
    // continuously reading data
    while(1){ 
        
        lcd_clear_display();
        
        // read temperature from DS18B20
        lcd_message("Temp: ");
        therm_answer = read_temperature();
        float temperature;
        if(therm_answer == 0x8000) {
            lcd_message("NO device");
            temperature = -1.0;
        }
        else {
            temperature = convert(therm_answer, 1); // status 1 for DS18B20
            temperature += 15.0; // make temperature around 37 (as human temperature)
            display_float(temperature);
        }
        
        _delay_ms(2000);
        
        // take pressure 0-20 from potensiometer
        lcd_message("Press: ");
        adc = adc_read();
        pressure = adc * 20.0 / 1024;
        display_float(pressure);
        
        _delay_ms(2000);
        
        
    }
    
    
    
    

    return 0;
}

