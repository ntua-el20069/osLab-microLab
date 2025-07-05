# Lab Exercise 3: Timers, PWM & ADC on ATmega328PB

## Development Environment
- **Microcontroller:** ATmega328PB  
- **Board:** ntuAboard_G1  
- **IDE:** MPLAB X  

## Key Peripherals
1. **Timer1 (TCNT1):** 16-bit timer with PWM generation
2. **PWM Outputs:** OC1A (PB1), OC1B (PB2)  
3. **ADC:** 10-bit resolution, 8 channels (PC0-PC5, PE2-PE3)  

## Task 3.1: PWM Brightness Control (Assembly)
- Generate **62500 Hz PWM** on PB1  
- Buttons control duty cycle:  
  - **PD3:** +8% (max 98%)  
  - **PD4:** -8% (min 2%)  
- Pre-calculated OCR1A values in lookup table  

## Task 3.2: PWM+ADC with Averaging (C)
- PWM on PB1 (as Task 3.1) with alternate buttons  
- Read filtered PWM via ADC every **100ms**  
- **16-sample average** (bit-shift division)  
- Light PORTD LEDs based on ADC ranges:  
  - PD0 (0-200), PD1 (200-400), ..., PD4 (800+)  

## Task 3.3: Dual-Mode PWM Control (C)
- **Mode 1:** Button-controlled brightness  
  - PD1: Increase  
  - PD2: Decrease  
- **Mode 2:** POT1-controlled brightness  
- **Mode switch:** PD6 (Mode 1), PD7 (Mode 2)  

## Hardware Setup
1. Connect POT1 to ADC0  
2. Bridge J5 for PWM filter  
3. Disable PORTC LEDs when using ADC  

## Verification
- Measure PWM frequency on PB1  
- Check ADC values match POT positions  
- Verify mode switching functionality  