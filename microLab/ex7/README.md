# Lab Exercise 7: DS1820 Temperature Sensor

## Development Environment
- **Microcontroller:** ATmega328PB  
- **Sensor:** DS1820/DS18B20  
- **Protocol:** 1-Wire  
- **Interface:** PD4 (J14)  

## Key Concepts
1. **1-Wire Protocol**  
   - Single-wire communication  
   - Master-slave architecture  
   - Requires 4.7kΩ pull-up  

2. **Temperature Sensor**  
   - Range: -55°C to +125°C  
   - Accuracy: ±0.5°C  
   - 9-bit resolution (750ms conversion)  

## Task 7.1: Temperature Reading (C)  
Implement 1-Wire routines:  
- `one_wire_reset()` - Device detection  
- `one_wire_receive/transmit_bit()` - Bit operations  
- `one_wire_receive/transmit_byte()` - Byte transfers  
- **Measurement sequence:**  
  1. Reset + presence detect  
  2. Skip ROM (0xCC)  
  3. Start conversion (0x44)  
  4. Read temperature (0xBE)  
- Returns:  
  - Temperature in r25:r24 (2's complement)  
  - 0x8000 if no device  

## Task 7.2: LCD Temperature Display (C)  
- Call Task 7.1 routine  
- Convert raw data to °C:  
  - DS1820: LSB = 0.5°C  
  - DS18B20: LSB = 0.0625°C  
- Display format:  
  - Signed 3-digit (e.g., "+025", "-012")  
  - "NO Device" if sensor absent  

## Hardware Setup  
1. Connect DS1820 to PD4 (J14)  
2. Enable sensor power (SW1-5)  
3. Verify correct orientation  

## Verification  
- Test with known temperatures  
- Check negative values  
- Verify "NO Device" detection  