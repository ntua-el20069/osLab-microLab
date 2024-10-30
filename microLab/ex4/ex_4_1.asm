.include "m328PBdef.inc"
    
.equ FOSC_MHZ = 16	    ; microcontroller operating frequency (MHz)
.equ DEL_mS = 100	    ; delay in mS (valid number from 1 to 4095)
.equ DEL_NU = FOSC_MHZ * DEL_mS	; delay_thousand_cycles routine: (1000*DEL_NU+6) cycles

.equ PD0=0
.equ PD1=1
.equ PD2=2
.equ PD3=3
.equ PD4=4
.equ PD5=5
.equ PD6=6
.equ PD7=7
    
.def temp = r16
.def ADC_L = r17
.def ADC_H = r18
.def test = r19 
.def high_digit = r28
.def middle_digit = r29
.def low_digit = r30
    
.org 0x0
rjmp reset
.org 0x2A
rjmp ADC_SR
    

ADC_SR:
    push test
    push r24
    push r25
    push temp
    in temp, SREG
    push temp
    
    
    clr ADC_H
    lds ADC_H, ADCH	; I load only high 8 bits 
			; (8bit result) <= (10bit result) < (8bit result) + 4
			; (8bit voltage result) <= (10bit voltage result) < (8bit voltage result) + 0.02
    ldi temp, 125
    mul ADC_H, temp
    mov r24, r0
    mov r25, r1
    
    ldi temp, 6
    loop:
	lsr r25 ; Divide r25:r24 by two (lsr gives bit 0 to carry)
	ror r24 ; r25:r24 is an unsigned two-byte integer (bit 7 is obtained by carry)
	dec temp
	cpi temp, 0
	brne loop
    
    nop
	
    clr high_digit
    clr middle_digit
    clr low_digit
    
    subtract_hundred:
    cpi r25, 0
    breq subtract_hundred_2nd	; branch if high bit (of r25) is 0 (unless you cannot compare with cpi r24, 100)
    ldi temp, 99    ; for debugging Register Trace
    sbiw r24, 50    ; because SBIW Rd+1:Rd,K	 
    sbiw r24, 50    ; d ? {24,26,28,30}, 0 ? K ? 63
    inc high_digit
    rjmp subtract_hundred
    
    subtract_hundred_2nd:
    cpi r24, 100
    brcs subtract_decade    ; branch if < 100
    ldi temp, 199   ; for debugging Register Trace
    sbiw r24, 50    ; because SBIW Rd+1:Rd,K	 
    sbiw r24, 50    ; d ? {24,26,28,30}, 0 ? K ? 63
    inc high_digit
    rjmp subtract_hundred_2nd
    
    subtract_decade:
    cpi r24, 10
    brcs monades_left	; branch if < 10
    ldi temp, 203   ; for debugging Register Trace
    sbiw r24, 10
    inc middle_digit
    rjmp subtract_decade
    
    monades_left:
    mov low_digit, r24
    
    ldi temp, 75    ; a number to see in Register Trace 
    
    ;; display 
    out PORTB, high_digit
    
    mov temp, high_digit
    rcall number_to_lcd_data  ; writes the byte for data to the screen in r24
    ;ldi r24, 0b00110000	     ;high_digit
    call lcd_data
    
    ldi r24, 0b00101110	; this is for the decimal dot (.)
    call lcd_data
    
    mov temp, middle_digit
    rcall number_to_lcd_data  ; writes the byte for data to the screen in r24
    call lcd_data
    
    mov temp, low_digit
    rcall number_to_lcd_data  ; writes the byte for data to the screen in r24
    call lcd_data
    
    ldi r24, low(800)	    ; CHANGED from 1000 to 800 ms
    ldi r25, high(800)
    rcall wait_msec ; delay 
    
    
    
    nop
    
    pop temp
    out SREG, temp
    pop temp
    pop r25
    pop r24
    pop test
    
    reti
    
reset:
; init stack pointer
    ldi temp, LOW(RAMEND)
    out SPL, temp
    ldi temp, HIGH(RAMEND)
    out SPH, temp
    
    ; init PORTs as  input output
    ser temp
    out DDRB, temp  ; port B as output
    ser r24
    out DDRD, r24 ; set PORTD as output
    clr r24
    rcall lcd_init	; LCD initialization
    ldi r24, low(100)
    ldi r25, high(100) ; delay 100 mS
    rcall wait_msec
    
; jobs to enable ADC and ADC interrupt
    ldi temp, 0b01100001    ; REFSn[7:6] = 01 -> Vref => AVcc with external capacitor at AREF pin
			    ; ADLAR[5] = 1 -> left adjusted output
    sts ADMUX, temp	    ; MUX[3:0] = 0001 for channel ADC1
    
    ldi temp, 0b10001111    ; ADEN[7] = 1 -> enable ADC
			    ; ADSC[6] = 0 -> no conversion
			    ; ADIF[4] (is set when a conversion has completed)
			    ; ADIE[3] = 1 -> enable ADC interrupt	    
    sts ADCSRA, temp	    ; ADPS[2:0] = 111 -> Division factor 128 so fADC = 16MHz/128 = 125kHz
    
    sei		    ; set the global interrupt flag

    
main:
    rcall lcd_clear_display
    ldi r24, low(3)		    ; CHANGED 1000 to 3
    ldi r25, high(3)
    rcall wait_msec ; delay 1 Sec
    
    lds temp, ADCSRA
    ori temp, (1 << ADSC)	// start conversion
    sts ADCSRA, temp            // ADCS will stay high as long as the conversion is in progress, and will be
                                // cleared by hardware when the conversion is completed
				
    ldi r24, LOW(DEL_NU)
    ldi r25, HIGH(DEL_NU)
    rcall delay_thousand_cycles
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

lcd_data:
    sbi PORTD ,PD2 ; LCD_RS=1(PD2=1), Data
    rcall write_2_nibbles ; send data
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec
    ret
    
    
lcd_command:
    cbi PORTD ,PD2 ; LCD_RS=0(PD2=0), Instruction
    rcall write_2_nibbles ; send Instruction
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec
    ret
    
    
lcd_clear_display:
    ldi r24 ,0x01 ; clear display command
    rcall lcd_command
    ldi r24 ,low(5) ;
    ldi r25 ,high(5) ; Wait 5 mSec
    rcall wait_msec ;
    ret
    
    
lcd_init:
ldi r24 ,low(200) ;
ldi r25 ,high(200) ; Wait 200 mSec
rcall wait_msec ;
ldi r24 ,0x30 ; command to switch to 8 bit mode
out PORTD ,r24 ;
sbi PORTD ,PD3 ; Enable Pulse
nop
nop
cbi PORTD ,PD3
ldi r24 ,250 ;
ldi r25 ,0 ; Wait 250uSec
rcall wait_usec ;
ldi r24 ,0x30 ; command to switch to 8 bit mode
out PORTD ,r24 ;
sbi PORTD ,PD3 ; Enable Pulse
nop
nop
cbi PORTD ,PD3
ldi r24 ,250 ;
ldi r25 ,0 ; Wait 250uSec
rcall wait_usec ;
ldi r24 ,0x30 ; command to switch to 8 bit mode
out PORTD ,r24 ;
sbi PORTD ,PD3 ; Enable Pulse
nop
nop
cbi PORTD ,PD3
ldi r24 ,250 ;
ldi r25 ,0 ; Wait 250uSec
rcall wait_usec
ldi r24 ,0x20 ; command to switch to 4 bit mode
out PORTD ,r24
sbi PORTD ,PD3 ; Enable Pulse
nop
nop
cbi PORTD ,PD3
ldi r24 ,250 ;
ldi r25 ,0 ; Wait 250uSec
rcall wait_usec
ldi r24 ,0x28 ; 5x8 dots, 2 lines
rcall lcd_command
ldi r24 ,0x0c ; dislay on, cursor off
rcall lcd_command
rcall lcd_clear_display
ldi r24 ,0x06 ; Increase address, no display shift
rcall lcd_command ;
ret
    

write_2_nibbles:
    push r24 ; save r24(LCD_Data)
    in r25 ,PIND ; read PIND
    andi r25 ,0x0f ;
    andi r24 ,0xf0 ; r24[3:0] Holds previus PORTD[3:0]
    add r24 ,r25 ; r24[7:4] <-- LCD_Data_High_Byte
    out PORTD ,r24 ;
    sbi PORTD ,PD3 ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    pop r24 ; Recover r24(LCD_Data)
    swap r24 ;
    andi r24 ,0xf0 ; r24[3:0] Holds previus PORTD[3:0]
    add r24 ,r25 ; r24[7:4] <-- LCD_Data_Low_Byte
    out PORTD ,r24
    sbi PORTD ,PD3 ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ret
    
    
wait_msec:
    push r24 ; 2 cycles
    push r25 ; 2 cycles
    ldi r24 , low(999) ; 1 cycle
    ldi r25 , high(999) ; 1 cycle
    rcall wait_usec ; 998.375 usec
    pop r25 ; 2 cycles
    pop r24 ; 2 cycles
    nop ; 1 cycle
    nop ; 1 cycle
    sbiw r24 , 1 ; 2 cycles
    brne wait_msec ; 1 or 2 cycles
    ret ; 4 cycles
    
wait_usec:
    sbiw r24 ,1 ; 2 cycles (2/16 usec)
    call delay_8cycles ; 4+8=12 cycles
    brne wait_usec ; 1 or 2 cycles
    ret

delay_8cycles:
    nop
    nop
    nop
    nop
    ret
    
    
number_to_lcd_data:
    mov r24, temp
    ori r24, 0b00110000
    ret






