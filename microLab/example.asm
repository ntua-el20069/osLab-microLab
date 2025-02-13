.include "m328PBdef.inc"    ;ATmega328PB definitions
    
.equ FOSC_MHz=16    ;MHz
.equ DEL_mS=1000	;mS
.equ F1=FOSC_MHz*DEL_mS
    
; Init Stack Pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24
    
; Init PORTC as output
    ser r26
    out DDRC, r26   ; DIFF from Lab code
    
    ldi r24, low(F1)	;
    ldi r25, high(F1)	; set delay
    
loop1:
    ser r26
    out DDRC, r26
    rcall delay_outer	; mS
    clr r26
    out PORTC, r26
    rcall delay_outer	; mS
    rjmp loop1
    
; this routine is used to produce a delay 993 cycles
delay_inner:
    ldi r23, 247    ; 1 cycle
loop3:
    dec r23	; 1 cycle
    nop		; 1 cycle
    brne loop3	; 1 or 2 cycles
    nop		; 1 cycle
    ret		; 1 cycle
    
;
 delay_outer:
    push r24	;
    push r25	;
    
loop4:
    rcall delay_inner	;
    sbiw r24, 1		;
    brne loop4		   ;
    
    pop r25		;
    pop r24		;
    ret			;
