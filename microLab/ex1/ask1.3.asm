.include "m328PBdef.inc"
    
ser r22
out DDRD,r22  ;PORTD output
    
ldi r22,1    
out PORTD,r22 ;PORTD initialized to 1

    
main:
set       ; T flag=1 move left
ldi r23,8    
    
loop1:    
rcall wait_x_msec
dec r23
breq move_right    
lsl r22
out PORTD,r22    
rjmp loop1
   
move_right:
  clt  ; T flag=0 move right
  ldi r23,8
  
  loop2:
   rcall wait_x_msec
   dec r23
   breq main
   lsr r22
   out PORTD,r22
   rjmp loop2
       

   wait_x_msec:
   ldi r24,0xE8
   ldi r25,0x03 ;r25:r24 1000 dec=0x03E8 hex
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
