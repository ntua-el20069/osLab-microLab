# Lab Exercise 8: IoT Patient Monitoring System

## Development Environment
- **Microcontroller:** ATmega328PB  
- **Board:** ntuAboard_G1  
- **Sensors:** DS18B20, POT0  
- **Wireless:** ESP8266 WiFi  
- **Protocol:** UART (9600 baud)  

## Key Components
1. **UART Communication**  
   - 8 data bits, 1 stop bit, no parity  
   - Commands end with `\n`  
   - ESP8266 control via `ESP:` prefix  

2. **IoT Architecture**  
   - Node (ntuAboard) → Gateway → Cloud  
   - JSON payload transmission  

3. **Medical Sensors**  
   - DS18B20: Patient temperature (+offset to ~36°C)  
   - POT0: Simulated venous pressure (0-20 cm H₂O)  

## Task 8.1: WiFi Connection
- Send commands:  
  ```c
  ESP:connect
  ESP:url:"http://192.168.1.250:5000/data"