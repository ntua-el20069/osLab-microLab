.include "m328PBdef.inc"
    
.equ freq=16          ;microcontroller operating frequency
.equ del=5            ; delay ( msec)    
.equ DEL_NU=freq*del  ; DEL_NU *1000 cycles required
    
.org 0x00
rjmp intreset
    
.org 0x02
rjmp ISRO
 
intreset:
; Interrupt of rising edge of INT0 PIN    
ldi r24,(1 << ISC01)|(1 << ISC00) 
sts EICRA,r24
    
;Enable the INT0 (PD2)
ldi r24, (1<<INT0)
out EIMSK,r24
    
sei                ; the Global Interrupt Flag is set
rjmp main
    
 main:
 rjmp main   
    
ISRO:
    
    ; Debouncing
    push r23
    push r24
    ldi r23,(1<<INTF0) 
    out EIFR,r23      ;clear INTF0
    rcall delay_ms
    
    in r23,EIFR
    andi r23,1
    pop r24
    pop r23
    brne ISRO
    ;
    
   push r22 
   push r23
   push r24
   in r24, SREG
   push r24
   
   ser r24
   out DDRC,r24 ; PORTC as output
   
   clr r24
   out DDRB,r24  ;PORT B as input
   
   in r22, PINB 
   
   ldi r24,4    ;counter
   clr r23
   clr r25
   
   loop:
    lsr r22      ;shift right, LSB is stored in C flag
    adc r23,r25  ;r23=r23+0+C
    dec r24
    brne loop
   
   out PORTC, r23
   pop r24
   out SREG,r24
   pop r24
   pop r23
   pop r22
   
   reti  ; the Global Interrupt Flag is set
   
   delay_ms:
    ldi r24,LOW(DEL_NU)    
    ; total delay next 4 instruction group=1+(249*4-1)=996 cycles
    ldi r23,249           ; 1 cycle
    
loop_inn:
    dec r23               ; 1 cycle
    nop                   ; 1 cycle
    brne loop_inn         ; 1 or 2 cycles
    
    dec  r24              ; 1 cycle
    nop                   ; 1 cycle
    brne delay_mS         ; 1 or 2 cycles
    
    ret                   ; 4 cycles
    