# Lab Exercise 6: 4x4 Keypad Interface

## Development Environment
- **Microcontroller:** ATmega328PB  
- **Interface:** I2C/TWI  
- **Expander:** PCA9555 (0x40)  
- **Keypad:** 4x4 matrix  

## Key Concepts
1. **Keypad Scanning**  
   - Rows `IO1[3:0]` as outputs  
   - Columns `IO1[7:4]` as inputs  
   - Sequential row activation with column reading  

2. **Debouncing**  
   - 10-20ms delay between scans  
   - Rising edge detection for key presses  

## Task 6.1: Keypad-LED Control (C)  
- Implement functions:  
  - `scan_row()` - Single row check  
  - `scan_keypad()` - Full keypad scan  
  - `scan_keypad_rising_edge()` - Debounced input  
  - `keypad_to_ascii()` - Key to ASCII conversion  
- LED mapping:  
  - 'A'→PB0, '8'→PB1, '6'→PB2, '*'→PB3  

## Task 6.2: LCD Key Display (C)  
- Display last pressed key's ASCII on LCD  
- Uses functions from Task 6.1  

## Task 6.3: Electronic Lock (C)  
- **Correct code:** PB0-PB5 ON for 3s  
- **Wrong code:** PB0-PB5 blink (500ms) for 5s  
- **Features:**  
  - 5s lockout after input  
  - Single-press registration  
  - Continuous operation  

## Hardware Setup  
1. Connect keypad to PCA9555 Port1  
2. Enable I2C pull-ups (J12/J13)  
3. Map LEDs to PORTB  

## Verification  
- Test single key detection (6.1)  
- Validate LCD display (6.2)  
- Check lock behavior (6.3)  