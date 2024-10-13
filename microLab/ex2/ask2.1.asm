.include "m328PBdef.inc"
    
.equ FOSC_MHZ = 16	    ; microcontroller operating frequency (MHz)
.equ DEL_mS = 500	    ; delay in mS (valid number from 1 to 4095)
.equ DEL_NU = FOSC_MHZ * DEL_mS	; delay_thousand_cycles routine: (1000*DEL_NU+6) cycles

.def int_counter = r21
    
.org 0x0
rjmp reset
.org 0x2
rjmp reset
.org 0x4
rjmp ISR1
    
reset:
; init stack pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24
    
; init PORTB and PORTC as output, PORTD as input
    ser r26
    out DDRB, r26
    out DDRC, r26
    clr r26
    out DDRD, r26
    
    clr int_counter	// interrupts counter = 0
    
; jobs to enable interrupts
    ldi r24, (1 << ISC11) | (1 << ISC10)    ; INT1 rising edge
    sts EICRA, r24
    ldi r24, (1 << INT1)
    out EIMSK, r24
    
    sei

loop1:
    clr r26
loop2:
    out PORTB, r26
    ldi r24, low(DEL_NU)
    ldi r25, high(DEL_NU)   ; set delay
    rcall delay_thousand_cycles
    
    inc r26
    
    cpi r26, 16		; compare
    breq loop1
    rjmp loop2
    
    

    
; instruction service routine for INT1
ISR1:		    
    ldi r24, (0 << INTF1)   ; make EIFR for INT1 zero 
    out EIFR, r24	    
    
    ldi r24, low(FOSC_MHZ*5)
    ldi r25, high(FOSC_MHZ*5)   ; set delay of 5 ms
    
    rcall delay_thousand_cycles
    
    in r24, EIFR
    andi r24, (1 << INTF1)
    cpi r24, 0x0
    brne ISR1
    
    
    in r16, PORTD
    andi r16, 0x20	    ; and with 0b 0010 0000 to get PD5
    cpi r16, 0x0	    ; if PD5 = 0 (pushed) then 
    breq isr_end		    ; do not increment int_counter (got to isr_end)
    
    inc int_counter	    ; counter += 1
    andi int_counter, 0x3F  ; andi 0b 0011 1111  is like (mod 64)
    out PORTC, int_counter  ; TODO output to PORTC
    
    isr_end:
    reti
    
    
; delay of (1000*F1 + 6) cycles ; F1 saved on r24:r25
delay_thousand_cycles:
   ; total delay of 4 instruction grout = 1 + (249*4 - 1) = 996 cycles
    ldi r23, 249	    
  loop_inn:
    dec r23
    nop
    brne loop_inn
    
    sbiw r24, 1
    brne delay_thousand_cycles
    
    ret
