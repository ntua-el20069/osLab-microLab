.include "m328PBdef.inc" ;ATmega328P microcontroller definition

.equ PD0=0
.equ PD1=1
.equ PD2=2
.equ PD3=3
.equ PD4=4
.equ PD5=5
.equ PD6=6
.equ PD7=7

main:
    ldi r24, low(RAMEND)
    out SPL, r24
    ldi r24, high(RAMEND)
    out SPH, r24 ; init stack pointer
    ser r24
    out DDRD, r24 ; set PORTD as output
    clr r24
    rcall lcd_init
    ldi r24, low(100)
    ldi r25, high(100) ; delay 100 mS
    rcall wait_msec

main1:
    rcall lcd_clear_display
    ldi r24, low(1000)
    ldi r25, high(1000)
    rcall wait_msec ; delay 1 Sec
    ldi r24, 'A'
    call lcd_data
    ldi r24, low(1000)
    ldi r25, high(1000)
    rcall wait_msec ; delay 1 Sec
    jmp main1
 
    
    
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


