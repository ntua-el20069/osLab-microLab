.include "m328PBdef.inc"
    
.equ FOSC_MHZ = 16	    ; microcontroller operating frequency (MHz)
.equ DEL_mS = 10	    ; delay in mS (valid number from 1 to 4095)
.equ DEL_NU = FOSC_MHZ * DEL_mS	; delay_thousand_cycles routine: (1000*DEL_NU+6) cycles

.def temp = r16
.def ADC_L = r17
.def ADC_H = r18
.def test = r19
.def high_digit = r28
.def middle_digit = r29
.def low_digit = r30
.org 0x0
rjmp reset
    
; In the convert routine
; the "test" 8-bit register is taken as input
; as the high 8 bits for an ADC for example
; and then converted to a voltage like 100*Vin (to be integer)
; so for example if ADC value is 512 (we know that Vin is 2.50 volts)
; the test register (high ADC) is 128 
; and the result in r25:r24 registers is approximately 250
; math :    Vin = (adc/1024)*Vref = (acd/2^10)*5
;	    Vin = (adc/2^2)* 5 / (2^8)
;		= (ADC 8 High bits) * 5 / (2^8)
;	 100Vin = (ADC 8 High bits) * 100 * 5 / (2^8)
;	 100Vin = (ADC 8 High bits) * 125 / (2^6)  
; so ADC_H multiplied by 125 and then shifted 6 times right (it is in r25:r24 though)

; then the result comes to high, middle, and low (decimal) digits (defined registers above)
; e.g. r25:r24 = 250 then high_digit = 2, middle_digit = 5, low_digit = 0
    
convert:
    push test
    push r24
    push r25
    push temp
    in temp, SREG
    push temp
    
    
    mov ADC_H, test	; I load only high 8 bits 
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
    sbiw r24, 50    ; d ∈ {24,26,28,30}, 0 ≤ K ≤ 63
    inc high_digit
    rjmp subtract_hundred
    
    subtract_hundred_2nd:
    cpi r24, 100
    brcs subtract_decade    ; branch if < 100
    ldi temp, 199   ; for debugging Register Trace
    sbiw r24, 50    ; because SBIW Rd+1:Rd,K	 
    sbiw r24, 50    ; d ∈ {24,26,28,30}, 0 ≤ K ≤ 63
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
    nop
    
    pop temp
    out SREG, temp
    pop temp
    pop r25
    pop r24
    pop test
    
    ret
    
reset:
; init stack pointer
    ldi temp, LOW(RAMEND)
    out SPL, temp
    ldi temp, HIGH(RAMEND)
    out SPH, temp
      
    
    
    ldi test, 0   ; for adc 0-3   (test: 8 high bits of ADC so 255 for 1023 adc)
    ldi temp, 50	  ; for adding and trying other numbers
main:
    
    rcall convert
    ;ldi r24, LOW(DEL_NU)
    ;ldi r25, HIGH(DEL_NU)
    ;rcall delay_thousand_cycles
    
    add test, temp  ; try another value
    nop
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





