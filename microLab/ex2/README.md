# Lab Exercise 2: External Interrupts on ATmega328

## Development Environment
- **Microcontroller:** ATmega328PB
- **IDE:** MPLAB X

## Task 2.1: Interrupt Counter with Debouncing
Implement an INT1 (PD3) interrupt service routine in Assembly.

**Requirements:**
- Count INT1 triggers (0-63, then reset)
- Pause counting when PD5 is pressed (logic 0)
- Display count in binary on LEDs PC5-PC0
- Implement debouncing:
  - Clear INTF1 flag
  - Add 5ms delay before rechecking

## Task 2.2: Dynamic Counter with LED Control
### Part A: Scaled Counter
- Count from 0 to 31
- Display on LEDs PC4-PC0
- 2-second delay between increments

### Part B: INT0 ISR Modification
- When INT0 (PD2) triggers:
- Light PORTC LEDs equal to pressed PB3-PB0 buttons
- LSB-first lighting pattern

## Task 2.3: Lighting Automation (Assembly/C)
Control PB0 LED with INT1 (PD3):

**Functionality:**
- Turn on PB0 when PD3 pressed
- Auto-off after 5 seconds
- Pressing PD3 again:
  - Renews 5-second timer
  - Flashes PB5-PB0 (except PB0) for 0.5s

**Visual Feedback:**
- PB0 remains on during renewal
- Other PORTB LEDs blink briefly on renewal

## Verification
1. Simulate in MPLAB X
2. For Task 2.1:
   - Verify debouncing with rapid PD3 presses
   - Check pause/resume with PD5
3. For Task 2.3:
   - Test timer renewal functionality
   - Verify 5s timeout accuracy