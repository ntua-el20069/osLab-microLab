#define F_CPU 16000000UL
#define TEMP_INCREMENT 15
#include <util/delay.h>
#include "avr/io.h"
#include <string.h>
#include <stdio.h>
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
char payload[200];


char* check(){

    if(pressure < 4 || pressure > 12){
    
        return "CHECK PRESSURE";
           }

    if(temperature < 34 || temperature > 37){
    
        return "CHECK TEMP";
        
    }
    
    
    return "OK";
  
}
void configure(char * stat){
  
sprintf(payload, "ESP:payload:[{\"name\":\"temperature\",\"value\":\"%.3f\"},{\"name\":\"pressure\",\"value\":\"%.3f\"},{\"name\":\"team\",\"value\":\"51\"},{\"name\":\"status\",\"value\":\"%s\"}]", temperature,pressure,stat);
   ;

    
 

}



int main(){
 
 char *status;

 int one_pressed;
    
        one_pressed=0;
        
 
    // initializations
    DDRB = 0xFF; // port B output for check
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0,0x00); //output
     PCA9555_0_write(REG_CONFIGURATION_1, 0xF0); // IO1_4 -> IO1_7 as input pins, IO1_0 -> IO1_3 as output ports
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
        //lcd_message("Temp: ");
        therm_answer = read_temperature();
        if(therm_answer == 0x8000) {
            lcd_message("NO device");
            temperature = -1.0;
        }
        else {
            temperature = convert(therm_answer, 1); // status 1 for DS18B20
            temperature += TEMP_INCREMENT; // make temperature around 37 (as human temperature)
            //display_float(temperature);
        }
        
        //_delay_ms(2000);
        
        // take pressure 0-20 from potensiometer
        //lcd_message("Press: ");
        adc = adc_read();
        pressure = adc * 20.0 / 1024;
        //display_float(pressure);
        
        //_delay_ms(2000);
        
        
        //here starts the code i added
        PORTB=0b11110000;
        lcd_message("insert");
        _delay_ms(2000);
        if(one_pressed == 0){   // '1' isn't pressed
         
            for(int i=0; i<10; i++){
        
            if(scan_keypad_rising_edge()!= 0) {  //a key was pressed
            
                if(keypad_to_ascii() == '1'){  //check if '1' is pressed
                    one_pressed=1;
                    status="NURSE CALL";
                    break;
                
                }
                                 }
            
            _delay_ms(100);
            }
            
            if(one_pressed == 0 ) status="OK";  // '1' isn't pressed
            
            else {  //one_pressed
                
                
                 for(int i=0; i<10; i++){
        
            if(scan_keypad_rising_edge()!= 0) {  //a key was pressed
            
                if(keypad_to_ascii() == '#'){  //check if '#' is pressed
                    
                    status=check();
                    one_pressed=0;
                    break;
                
                }
                                 }
            
            _delay_ms(100);
       }
        
           }
          
       }
        
      else{        //one is pressed
                   //let's see if # is pushed
                      for(int i=0; i<10; i++){
        
            if(scan_keypad_rising_edge()!= 0) {  //a key was pressed
            
                if(keypad_to_ascii() == '#')
                {status=check();
                    one_pressed=0;
                    break;}
                
                }
                            _delay_ms(100);     }
            
          
        
        }
        
            
         
      
        lcd_clear_display();
        lcd_twice_float(temperature,pressure);
        lcd_message_new_line(status);
        _delay_ms(2000);
        
        configure(status);
        if ( conversation(payload, buffer, BUF_SIZE, "\"Success\"") ) lcd_message("PAYLOAD Success");
        else lcd_message("PAYLOAD Failure");
    
        _delay_ms(3000);    // wait for the message to be seen
        
        if ( conversation("ESP:transmit", buffer, BUF_SIZE, "\"200 OK\"") ) lcd_message("TRANSMIT Success");
        else lcd_message("TRANSMIT Failure");
    
         _delay_ms(3000);    // wait for the message to be seen
        
}
return 0;
}
