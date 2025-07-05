# Lab Exercise 1: Microcontroller Programming

## Development Environment
- **Microcontroller:** ATmega328  
- **IDE:** MPLAB X  

## Task 1: Adjustable Time Delays
Implement an Assembly subroutine (`wait_x_msec`) for precise millisecond delays.  

**Requirements:**  
- Accepts delay value (1–65535) via register pair `r25:r24`  
- Must use `sbiw` instruction  
- Verify timing using MPLAB X Stopwatch tool  

## Task 2: Logical Function Calculator
Compute two Boolean functions across 6 loop iterations.  

**Functions:**  
1. `F0 = (A ∙ B' + B' ∙ D)'`  
2. `F1 = (A + C') ∙ (B + D')`  

**Execution:**  
- **Variables:** 8-bit `A`, `B`, `C`, `D`  
- **Initial values:** `A=0x51`, `B=0x41`, `C=0x21`, `D=0x01`  
- **Per-iteration increments:**  
  - `A += 0x01`  
  - `B += 0x02`  
  - `C += 0x03`  
  - `D += 0x04`  
- **Output:** Tabulate values for each iteration  

## Task 3: Wagon Motion Simulator
Simulate bidirectional wagon movement using PORTD.  

**Specifications:**  
- **Representation:** Single bit traverses PORTD (LSb→MSb→LSb)  
- **Timing:**  
  - 1 sec/position (using Task 1's delay)  
  - 2 sec pause at endpoints  
- **State tracking:**  
  - Direction stored in SREG's T flag  
  - Monitor PORTD via Run Time Watch  

## Verification  
1. Simulate tasks in MPLAB X  
2. For Task 1: Measure delays with Stopwatch  
3. For Task 3: Observe bit movement in PORTD  