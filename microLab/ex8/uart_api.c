#include "avr/io.h"
#include "string.h"


/*  Routine: usart_init
    Description:
    This routine initializes the
    usart as shown below.
    ------- INITIALIZATIONS -------
    Baud rate: 9600 (Fck= 8MH)
    Asynchronous mode
    Transmitter on
    Reciever on
    Communication parameters: 8 Data ,1 Stop, no Parity
    --------------------------------
    parameters: ubrr to control the BAUD.
    return value: None.*/
void usart_init(unsigned int ubrr){
    UCSR0A=0;
    UCSR0B=(1<<RXEN0)|(1<<TXEN0);
    UBRR0H=(unsigned char)(ubrr>>8);
    UBRR0L=(unsigned char)ubrr;
    UCSR0C=(3 << UCSZ00);
    return;
}

/*  Routine: usart_transmit
    Description:
    This routine sends a byte of data
    using usart.
    parameters:
    data: the byte to be transmitted
    return value: None. */
void usart_transmit(uint8_t data){
    while(!(UCSR0A&(1<<UDRE0)));
    UDR0=data;
}

void usart_send_message(char *message){
	for(int i=0; i<strlen(message); i++)
		usart_transmit(message[i]);
	usart_transmit('\n');
}


/*  Routine: usart_receive
    Description:
    This routine receives a byte of data
    from usart.
    parameters: None.
    return value: the received byte */
uint8_t usart_receive(){
    while(!(UCSR0A&(1<<RXC0)));
    return UDR0;
}

void usart_receive_message(char *buffer, int buffer_size){

	// clear buffer
	for(int i=0; i<buffer_size; i++) buffer[i] = '\0';
	
	// receive line
	uint8_t received_char;
	int i = 0;
	while((received_char = usart_receive()) != '\n')
		buffer[i++] = received_char;

}


int conversation(char *message, char *buffer, int buffer_size, char* expected_reply){
    // sends message and fills buffer with the received message
    // returns  1 if received message matches the expected_reply     else 0
    usart_send_message(message);         
    usart_receive_message(buffer, buffer_size);
    if(strcmp(buffer, expected_reply) == 0) return 1;
    return 0; // if the message does not match the expected reply
}
