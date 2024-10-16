.include "m328PBdef.inc"
    
.equ FOSC_MHZ = 16	    ; microcontroller operating frequency (MHz)
.equ DEL_mS = 500	    ; delay in mS (valid number from 1 to 4095)
.equ DEL_NU = FOSC_MHZ * DEL_mS	; delay_thousand_cycles routine: (1000*DEL_NU+6) cycles

.def reverse_time_counter = r21	; reverse_time_counter counts units of 500 ms
.def active = r22; 
.def temp = r20			
    
.org 0x0
rjmp reset
.org 0x2
rjmp reset
.org 0x4
rjmp ISR1
    
ISR1:	; Interrupt Service Routine for INT1
    ; TODO handle renew or other phenomena
    sei	// enable interrupts again
    ; handle led
    cpi active, 0
    breq next		    ; if active = 0 (not in interruption this time) 
    
    ; /// this part if renew
    ldi reverse_time_counter, 10    ; t = 10*0.5 = 5 sec
    ser temp			    ; 
    out PORTB, temp		    ; shine all leds portB
    ldi r24, LOW(DEL_NU)		    
    ldi r25, HIGH(DEL_NU)		    
    rcall delay_thousand_cycles		    ; wait 0.5 sec
    ; /// end of renew part
    
    next:
    ldi temp, 0x01  ; only PB0 on
    out PORTB, temp
    ldi active, 1
    
    loop:	; wait counter times * 0.5 sec =  (10 * 0.5 sec) = 5 sec
    ldi r24, LOW(DEL_NU)
    ldi r25, HIGH(DEL_NU)
    rcall delay_thousand_cycles		    ; wait 0.5 sec
    dec reverse_time_counter
    cpi reverse_time_counter, 0
    brne loop
    ; end of loop
    
    ldi reverse_time_counter, 10    ; t = 10*0.5 = 5 sec
    ldi active, 0		    ; 
    clr temp
    out PORTB, temp		    ; turn off the lights
    
    reti
    
reset:
; init stack pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24
    
; init PORTB as output
    ser temp
    out DDRB, temp

    ; some initializations
    ldi active, 0	; active = 0 (not in interruption)
    ldi reverse_time_counter, 10    ; (counter for interrupts) t = 10*0.5 = 5 sec
    clr temp		; PORTB off (initialize)
    out PORTB, temp	; show in port B

    
; jobs to enable interrupts
    ldi r24, (1 << ISC11) | (1 << ISC10)    ; INT1 rising edge
    sts EICRA, r24
    ldi r24, (1 << INT1)
    out EIMSK, r24
    
    sei
    
 main:
    rjmp main
    

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
    

