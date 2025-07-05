# Lab Exercise 5: I2C & PCA9555 I/O Expansion

## Development Environment
- **Microcontroller:** ATmega328PB  
- **Board:** ntuAboard_G1  
- **Protocol:** TWI/I2C  
- **Expander:** PCA9555 (0x40)  

## Key Peripherals
1. **TWI/I2C Interface**  
   - SCL (PC5), SDA (PC4)  
   - 7-bit addressing  
   - 10kΩ pull-ups (J12/J13)  

2. **PCA9555 I/O Expander**  
   - 16-bit I/O (Port0/Port1)  
   - Configurable input/output  

## Task 5.1: Logical Functions via I2C (C)  
- Implement:  
  - `F0 = (A'BC + B'D)'`  
  - `F1 = (A+B+C) * (B*D')`  
- **Inputs:** PB0-PB3 (A-D)  
- **Outputs:** PCA9555 IO0_0/IO0_1 → LEDs PD2/PD3  

## Task 5.2: Keypad-LED Control (C)  
- **PCA9555 Setup:**  
  - Port0.0-0.3: Output → LEDs PD4-PD7  
  - Port1.4-1.7: Input ← Keypad  
- **Mapping:**  
  - '*' → PD4  
  - '0' → PD5  
  - '#' → PD6  
  - 'D' → PD7  

## Task 5.3: LCD via PCA9555 (C)  
- Connect LCD to PCA9555 Port1 (J19)  
- Display first name and surname  
- Adapt existing LCD routines for I2C  

## Hardware Setup  
1. Enable I2C pull-ups (J12/J13)  
2. Connect PCA9555 to:  
   - LEDs (J18 for Tasks 5.1/5.2)  
   - LCD (J19 for Task 5.3)  

## Verification  
- Check LED responses to PB inputs (5.1)  
- Test keypad button mappings (5.2)  
- Validate LCD display (5.3)  