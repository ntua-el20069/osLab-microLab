.include "m328PBdef.inc"
.def index=r22
.equ freq=16
.equ DELAY=500  ;delay (msec)
.equ DELAY1=10
.equ TOTAL=freq*DELAY  
.equ TOTAL1=freq*DELAY1 
 .dseg                     ;RAM MEMORY
 DC_VALUE : .byte 1 
   
 
.cseg                      ;FLASH MEMORY 
table: .dw 0x0005, 0x001a, 0x002e ,0x0043, 0x0057, 0x006c ,0x0080, 0x0094, 0x00a9, 0x00bd ,0x00d2, 0x00e6 ,0x00fb ; duty cycle values

 
reset:
    
  ldi r31, HIGH(table*2) ; initialize z to table address. 
  ldi r30, LOW(table*2)  ; x2 for byte access
  adiw r30,12             ; z has the address of 0x80 
  ldi index,6            ;index for table
  lpm r29 ,z             ; load the context of z address to r24 
  sts DC_VALUE,r29       ;50% duty cycle 0x80
  clr r25
  sts OCR1AH,r25
  sts OCR1AL, r29         ;OCR1A initialized to 0x80
  ldi r24,(1<<WGM10)|(1<<COM1A1)  
  sts TCCR1A,r24           ;Fast PWM not inverse output PB1
  
  ldi r24,(1<<CS12) |(1<<WGM12) 
  sts TCCR1B,r24         ;Frequency CLK/256=625000Hz

  ser r24
  out DDRB,r24   ;PB as output
  out DDRC,r24 
  out PORTC,index
  clr r24
  out PORTB,r24 ;PORTB off
  out DDRD,r24  ;PORTD as input
  
    
  start:
  ldi r24,LOW(TOTAL)
  ldi r25,HIGH(TOTAL)
  rcall delay_thousand_cycles      ;delay 0,5sec
  
  input:
  in r19, PIND
  mov r20,r19
  andi r19,(1<<3)|(1<<4)
  cpi r19, (1<<3)|(1<<4)                       
  breq input                       
  check:
  ldi r24,LOW(TOTAL1)
  ldi r25,HIGH(TOTAL1)
  rcall delay_thousand_cycles 
  in r19, PIND
  andi r19,(1<<3)|(1<<4)
  cpi r19,(1<<3)|(1<<4)
  brne check
  
  
  cpi r20,0b11101111               ;if equal ,PD4 pushed
  breq decrease
  
  cpi r20,0b11110111               ;if equal PD3 pushed
  breq increase
  
  rjmp start
  
  increase:
   cpi index,12
   breq start                    ;limit of 98%
   inc index                    ;index of next element
   adiw r30,2                     ; store z the address of next element
   lpm r29,z     
   ; load next element in r21
   out PORTC,r29
   sts DC_VALUE,r29            ;store next element in DC_VALUE
   sts OCR1AL,r29             ; store next element in OCRIAL 
   rjmp start
   
   decrease:
   cpi index,0
   breq start                   ;limit of 2%
   dec index                      ;index of previous element  
   sbiw r30,2                     ;store z the address of previous element
   lpm r29,z  ;load previous element in r21
   out PORTC,r29
   sts DC_VALUE,r29              ;store next element in DC_VALUE
   sts OCR1AL,r29               ;store previous element in OCRIAL
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
