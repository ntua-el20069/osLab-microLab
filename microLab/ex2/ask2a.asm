.include "m328PBdef.inc"
    
.equ freq=16               ; Microcontroller operating frequency
.equ m_sec=2000            ; Delay in ms
.equ DEL_NU=freq*m_sec     ;DEL_NU*1000 cycles required    

ldi r24,LOW(RAMEND)
out SPL,r24

ldi r24,HIGH(RAMEND)    
out SPH,r24
 
ser r24
out DDRC,r24               ; PORTC as output    

ldi r24,LOW(DEL_NU);
ldi r25,HIGH(DEL_NU);   
    
main:
  clr r22

  loop:
  out PORTC,r22
  push r24
  push r25
  rcall delay_ms
  pop r25
  pop r24
  inc r22  
  cpi r22,32  
  brne loop
  rjmp main
  
delay_ms:
    
    ; total delay next 4 instruction group=1+(249*4-1)=996 cycles
    ldi r23,249           ; 1 cycle
    
loop_inn:
    dec r23               ; 1 cycle
    nop                   ; 1 cycle
    brne loop_inn         ; 1 or 2 cycles
    
    sbiw  r24,1           ; 2 cycles
    brne delay_mS         ; 1 or 2 cycles
    
    ret                   ; 4 cycles
    


