.include "m328PBdef.inc"
.equ freq=16
.equ DELAY=500  ;delay (msec)
.equ TOTAL=freq*DELAY  
 
 .dseg                     ;RAM MEMORY
 DC_VALUE : .byte 1 
   
    
table: .db 0x05, 0x1a, 0x2e , 0x43, 0x57, 0x6c , 0x80, 0x94, 0xa9, 0xbd ,0xd2, 0xe6 ,0xfb 
  ; duty cycle values
 
.cseg                      ;FLASH MEMORY 
.org 0x00
 rjmp reset
 
reset:
    
  ldi Zh, HIGH(table) 
  ldi Zl, LOW(table)
  ldi r22,6                ;index for table
  ldi r24 ,0x80    
  sts DC_VALUE,r24         ;50% duty cycle
  ldi r24,(1<<WGM10)|(1<<COM1A1)  
  sts TCCR1A,r24           ;Fast PWM not inverse output PB1
  
  ldi r24,(1<<CS12) |(0<<CS11) |(0<<CS10) 
  sts TCCR1B,r24         ;Frequency CLK/256=625000Hz

 
  ldi r24,2
  out DDRB,r24   ;PB1 as output
  
  clr r24
  out PORTB,r24 ;PORTB off
  out DDRD,r24  ;PORTD as input
  
  sts OCR1AH,r24
  
  ldi r24, 126
  sts OCR1AL, r24
  
  start:
  ldi r24,LOW(TOTAL)
  ldi r25,HIGH(TOTAL)
  rcall delay_thousand_cycles      ;delay 0,5sec
  
  in r24, PORTD
  
  cpi r24,0b11101111               ;if equal ,PD4 pushed
  breq decrease
  
  cpi r24,0b11110111               ;if equal PD3 pushed
  breq increase
  
  rjmp start
  
  increase:
   cpi r22,12
   breq start                   ;limit of 98%
   inc r22                      ;index of next element
   adiw z,1                     ; store z the address of next element
   ld r21,z                    ; load next element in r21
   sts DC_VALUE,r21            ;store next element in DC_VALUE
   sts OCR1AL,r21               ; store next element in OCRIAL 
   rjmp start
   
   decrease:
   cpi r22,0
   breq start                   ;limit of 2%
   dec r22                      ;index of previous element  
   sbiw z,1                     ;store z the address of previous element
   ld r21,z                     ;load previous element in r21
   sts DC_VALUE,r21            ;store next element in DC_VALUE
   sts OCR1AL,r21               ;store previous element in OCRIAL
   rjmp start
  
  
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