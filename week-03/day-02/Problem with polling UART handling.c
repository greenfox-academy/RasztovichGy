/*
 * Problem with polling UART handling.c
 *
 * Created: 2017.05.16. 14:27:12
 * Author : Gyula
 */ 

#include <avr/io.h>			// This header contains the definitions for the io registers
#include <stdint.h>
#include <stdio.h>

#define F_CPU	16000000	// This definition tells to _delay_ms() that the CPU runs at 16MHz
#include <util/delay.h>		// This header contains the _delay_ms() function

void UART_Init() {
	// Write this function
	// See the datasheet on page 246 for hints and table 25-9.

	// At first set the baud rate to 9600
	// The CPU clock frequency is 16MHz
	uint16_t ubbr = 103;
	UBRR0H = ubbr >> 8;
	UBRR0L = ubbr;

	// Set the following frame format: 8N1
	UCSR0C |= 0b011;

	// Enable TX
	UCSR0B |= 1 << TXEN0;

	// Enable RX
	UCSR0B |= 1 << RXEN0;
}

void UART_SendCharacter(char character) {
	// Write this function, which can send a character through UART will polling method
	// See page 247 of the datasheet for hints, be aware that the code in the datasheet 
	// has a problem :)

	// Wait for empty buffer
	while ( !( UCSR0A & (1<<UDRE0)) );

	// Put data to buffer
	UDR0 = character;
}

char UART_GetCharacter() {
	// Write this function, which waits for a character and returns when one is received
	// See page 249 of the datasheet for hints, be aware that the code in the datasheet 
	// has a problem :)

	// Wait for data received
	while ( !(UCSR0A & (1<<RXC0)) );

	// Put data to buffer
	return UDR0;
}

void ProcessorDoesSomethingElse(){
	// This function simulates a time-consuming task
	_delay_ms(500);
}

int main(void) {
	char buffer[255];

	//Don't forget to call the init function :)
	UART_Init();

	// Setting up STDIO input and output buffer
	// You don't have to understand this!
	//----- START OF STDIO IO BUFFER SETUP
	FILE UART_output = FDEV_SETUP_STREAM(UART_SendCharacter, NULL, _FDEV_SETUP_WRITE);
	stdout = &UART_output;
	FILE UART_input = FDEV_SETUP_STREAM(NULL, UART_GetCharacter, _FDEV_SETUP_READ);
	stdin = &UART_input;
	//----- END OF STDIO IO BUFFER SETUP

	// Try printf
	printf("Startup...\r\n");

	// Loop that runs forever
	while (1) {
		// With gets and puts create a loopback, use the buffer variable!
		gets(buffer);
		ProcessorDoesSomethingElse();
		puts(buffer);
		ProcessorDoesSomethingElse();
	}
}


		/* Note: Don't forget set in Realterm Baud rate to 9600, 
		and Port according to device manager; after settings hit "Change"!
		At send tab, check +CR +LF buttons before sending text! */

