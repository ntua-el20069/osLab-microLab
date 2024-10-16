.include "m328PBdef.inc"
    
.equ freq=16          ;microcontroller operating frequency
.equ del=5            ; delay ( msec)    
.equ DEL_NU=freq*del  ; DEL_NU *1000 cycles required
    
.org 0x00
rjmp reset
    
.org 0x02
rjmp ISRO
 
reset:
    
ser r24
out DDRC,r24 ; PORTC as output

clr r24
out PORTC,r24 ; Initialize PORTC as 0x00    
    
 ldi r24, LOW(RAMEND)
 out SPL,r24

ldi r24, HIGH(RAMEND)
 out SPH,r24

 ; Interrupt of rising edge of INT0 PIN    
ldi r24,(1 << ISC01)|(1 << ISC00) 
sts EICRA,r24
    
;Enable the INT0 (PD2)
ldi r24, (1<<INT0)
out EIMSK,r24
    
sei                ; the Global Interrupt Flag is set

    
 main:
 rjmp main   
    
ISRO:
    
   
   push r22 
   push r23
   push r24
   in r24, SREG
   push r24
   
 
   clr r24
   out DDRB,r24  ;PORT B as input
   
   in r22, PINB 
   
   ldi r24,5    ;counter
   clr r23
 
   
   loop:
    dec r24
    breq output
    lsr r22      ;shift right, LSB is stored in C flag
    brcc increase ;branch if C=0 BUTTON PUSHED
    rjmp loop
    
    increase:
    inc r23
    rjmp loop
    
    output:
   out PORTC, r23
   pop r24
   out SREG,r24
   pop r24
   pop r23
   pop r22
   
   reti  ; the Global Interrupt Flag is set
   
  
