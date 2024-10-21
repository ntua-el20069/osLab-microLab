.include "m328PBdef.inc"
.def index=r22
.equ freq=16
.equ DELAY=500  ;delay (msec)
.equ TOTAL=freq*DELAY  
 
 .dseg                     ;RAM MEMORY
 DC_VALUE : .byte 1 
   
 
.cseg                      ;FLASH MEMORY 
table: .dw 0x05, 0x1a, 0x2e ,0x43, 0x57, 0x6c ,0x80, 0x94, 0xa9, 0xbd ,0xd2, 0xe6 ,0xfb ; duty cycle values

 
reset:
    
  ldi r31, HIGH(table*2) ; initialize z to table address. 
  ldi r30, LOW(table*2)  ; x2 for byte access
  adiw r30,6             ; z has the address of 0x80 
  ldi index,6            ;index for table
  lpm r24 ,z             ; load the context of z address to r24 
  sts DC_VALUE,r24       ;50% duty cycle 0x80
  clr r25
  sts OCR1AH,r25
  sts OCR1AL, r24         ;OCR1A initialized to 0x80
  ldi r24,(1<<WGM10)|(1<<COM1A1)  
  sts TCCR1A,r24           ;Fast PWM not inverse output PB1
  
  ldi r24,(1<<CS12) |(0<<CS11) |(0<<CS10) 
  sts TCCR1B,r24         ;Frequency CLK/256=625000Hz

  ser r24
  out DDRB,r24   ;PB as output
  
  clr r24
  out PORTB,r24 ;PORTB off
  out DDRD,r24  ;PORTD as input
  
    
  start:
  ldi r24,LOW(TOTAL)
  ldi r25,HIGH(TOTAL)
  rcall delay_thousand_cycles      ;delay 0,5sec
  
  in r20, PIND
  
  cpi r20,0b11101111               ;if equal ,PD4 pushed
  breq decrease
  
  cpi r20,0b11110111               ;if equal PD3 pushed
  breq increase
  
  rjmp start
  
  increase:
   cpi index,12
   breq start                    ;limit of 98%
   inc index                    ;index of next element
   adiw r30,1                     ; store z the address of next element
   lpm r21,z                    ; load next element in r21
   sts DC_VALUE,r21            ;store next element in DC_VALUE
   sts OCR1AL,r21               ; store next element in OCRIAL 
   rjmp start
   
   decrease:
   cpi index,0
   breq start                   ;limit of 2%
   dec index                      ;index of previous element  
   sbiw r30,1                     ;store z the address of previous element
   lpm r21,z                     ;load previous element in r21
   sts DC_VALUE,r21              ;store next element in DC_VALUE
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
