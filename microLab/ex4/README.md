# Lab Exercise 4: LCD Interface & ADC Applications

## Development Environment
- **Microcontroller:** ATmega328PB
- **Board:** ntuAboard_G1
- **IDE:** MPLAB X

## Key Peripherals
1. **2x16 LCD Display**
   - 4-bit interface mode
   - Requires custom delay routines
   - Connected via PORTD with voltage level shifting

2. **10-bit ADC**
   - 8 input channels
   - VREF = 5V
   - Conversion formula: `VIN = (ADC/1024)*5`

## Task 4.1: ADC Voltage Display (Assembly)
- Read POT2 voltage via ADC1 every 1s
- Calculate voltage in ADC ISR
- Display on LCD with 2 decimal places
- Example: "2.45V"

## Task 4.2: Polled ADC Display (C)
- Reimplement Task 4.1 in C
- Use polling (ADSC bit) instead of interrupts
- Same LCD output format

## Task 4.3: CO Monitoring System (C)
- Read POT3 (simulated CO sensor) every 100ms
- LED indicators (PB0-PB5) show gas level
- LCD alerts:
  - `>70ppm`: "GAS DETECTED" + blinking LEDs
  - `≤70ppm`: "CLEAR" + steady LEDs

## Hardware Setup
1. Connect POT2 to ADC1 (Task 4.1/4.2)
2. Connect POT3 to ADC2 (Task 4.3)
3. Enable LCD via PORTD jumpers
4. Disable PORTC LEDs when using ADC

## Verification
- Check ADC-LCD voltage matching
- Test CO alarm thresholds
- Verify display refresh rates