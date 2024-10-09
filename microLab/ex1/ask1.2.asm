.include "m328PBdef.inc"

.def a = r16
.def b = r17
.def c = r18
.def d = r19
.def f0 = r20
.def f1 = r21
.def total = r22
.def counter = r23
.def temp = r24
.def temp1 = r25
.def temp2 = r26
    
.org 0x0000

ldi a, 0x51	; initial values
ldi b, 0x41
ldi c, 0x21
ldi d, 0x01
    
; for iterations
ldi counter, 0x0
ldi total, 0x6
    
ser temp	    ; temp = 0xFF
OUT DDRD, temp	    ; POTRD as output


LOOP:
    
    ;;; F0 Computation
    mov temp1, b
    com temp1		; find b' (complement of b)
    and temp1, a	; a and b'
    
    mov temp2, b
    com temp2
    and temp2, d	; d and b'
    
    or temp1, temp2	; a*b' + d*b'
    com temp1		; (a*b' + d*b')'
    mov f0, temp1	;
    nop
    
    ;;; F1 Computation
    mov temp1, c
    com temp1
    or temp1, a		; a + c'
    
    mov temp2, d
    com temp2
    or temp2, b		; b + d'
    
    and temp2, temp1	; (a + c')*(b + d')
    mov f1, temp2	
    nop			
    
    
    
    
    ; make them show in PORTS
    OUT PORTD, f0
    nop		    
    OUT PORTD, f1
    nop
    
    
    
    
    ; loop increments
    ldi temp, 0x1	
    add counter, temp	; counter increment
    
    ldi temp, 0x1	; registers increment...
    add a, temp		
    ldi temp, 0x2
    add b, temp
    ldi temp, 0x3
    add c, temp
    ldi temp, 0x4
    add d, temp
    
    cp counter, total
    brne LOOP
    
