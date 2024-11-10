#define F_CPU 16000000UL
#include<util/delay.h>

#include "twi_api.h"

#define FIRST_ROW  0b11111110      // IO1_0 should be 0 for exploration of 1st row
#define SECOND_ROW 0b11111101
#define THIRD_ROW  0b11111011
#define FOURTH_ROW 0b11110111    

#define FIRST_MASK 0x10      // mask to see the corresponging bit for 1st column
#define SECOND_MASK 0x20      // mask to see the corresponging bit for "0"
#define THIRD_MASK 0x40   // mask to see the corresponging bit for "#"
#define FOURTH_MASK  0x80        // mask to see the corresponging bit for "D"

uint16_t pressed_keys;  // definition,declaration of pressed_keys variable


char keypad[4][4] = {
    {'*', '0', '#', 'D'},
    {'7', '8', '9', 'C'},
    {'4', '5', '6', 'B'},
    {'1', '2', '3', 'A'}
};

// scan_row returns a number in which 
// each bit is 1 only if the corresponding key is pressed
// (bit0 for first column, bit1 for second...)
uint8_t scan_row(){

 uint8_t keyboard_input;
 uint8_t pressed = 0;
 keyboard_input = PCA9555_0_read(REG_INPUT_1);
 
 // if the button of this line and 1st column is pressed, then OR with 0000 0001
 if((keyboard_input & FIRST_MASK) == 0x00)      // pressed means 0 
     pressed |= 0x01;

 if((keyboard_input & SECOND_MASK) == 0x00)
     pressed |= 0x02;

 if((keyboard_input & THIRD_MASK) == 0x00)
     pressed |= 0x04;

 if((keyboard_input & FOURTH_MASK) == 0x00)
     pressed |= 0x08;

 return pressed;
}

uint16_t scan_keypad(){
    
    uint16_t state = 0;
    // I make 0 the bit of the row I want to explore (4 rows, times)
    PCA9555_0_write(REG_OUTPUT_1, FIRST_ROW);   
    state |= scan_row();        // scan first row for pushed buttons
    PCA9555_0_write(REG_OUTPUT_1, SECOND_ROW);
    state |= (scan_row() << 4) ;
    PCA9555_0_write(REG_OUTPUT_1, THIRD_ROW);
    state |= (scan_row() << 8);
    PCA9555_0_write(REG_OUTPUT_1, FOURTH_ROW);
    state |= (scan_row() << 12);
    
    // state is now like
    // [4th row][3rd row][2nd row][1st row]
    // columns inside
    // [Columns 4th 3rd 2nd 1st][..][..][..]
    // [A321][B654][C987][D#0*]
    // 1 means pressed
    return state;
}

uint16_t scan_keypad_rising_edge(){
    
    uint16_t modified_bits;
    uint16_t pressed_keys_tempo = scan_keypad();    // initial read of keypad
    _delay_ms(10); // delay to avoid weird phenomena
    
    // XOR (^) with scan_keypad gives 1 in bits that differ
    // NOT (~) makes these bits 0 in order to filter the
    // pressed_keys_tempo var with bitwise AND (bits that have changed in 10ms get 0)
    pressed_keys_tempo &= ~(pressed_keys_tempo ^ scan_keypad())  ;
    
    // 0 for all bits that are same with previous keypad scan (pressed_keys)
    // 1 for keys that have changed
    // ignore all bits that changed in 10 ms with the & ... (make them 0) 
    modified_bits =  (pressed_keys_tempo ^ pressed_keys) & (~(pressed_keys_tempo ^ scan_keypad()) );
    
    pressed_keys = pressed_keys_tempo;
    return modified_bits;
}

char keypad_to_ascii(){
    // we assume one key is pressed each time
    // this loop can pass from right to left these bits
    // [A321][B654][C987][D#0*]
    uint16_t temp = pressed_keys;
    for(uint8_t i=0; i<4; i++){ //  rows
        for(uint8_t j=0; j<4; j++){ // columns 
            if(temp%2 == 1) return keypad[i][j];    // if pressed return char
            temp = (temp >> 1);  // shift right to take 
                        // next more significant bit as LSB 
        }
    }
    return ' ';
}

