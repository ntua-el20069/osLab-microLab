.include "m328PBdef.inc"

ser r16		; REMOVE ...
out DDRD, r16	; REMOVE ... (PORT D as output)
clr r22
    
start:
com r22		; CHANGE to clr r22
out DDRD, r22	; REMOVE
ldi r24,0
ldi r25,2
    
main:
 rcall wait_x_msec
 rjmp start		    ;CHANGE to rjmp finish
 
 ; total cycles r24:r25 *1000*16 
wait_x_msec:
  loop:
    rcall delay_inner ; 3 cycles + 15993 cycles =15996 cycles
    sbiw r24,1    ; 2 cycles
    brne loop   ; 1 or 2 cycles 
    ret   ;4 cycles
  
  delay_inner:    ;delay_inner: 2+ 3197*5 + 6=15993 cycles
    ldi r26,0x7D   ;LSB 1 cycle
    ldi r27,0x0C   ;MSB 1 cycle ;3197
   loop3:
    sbiw r26,1    ;2 cycles
    nop         ;1 cycle 
    brne loop3  ;1 cycle or 2 cycles
    nop         ;1 cycle
    nop         ;1 cycle
    nop         ;1 cycle
    ret         ;4 cycles

   finish:
    ser r22


