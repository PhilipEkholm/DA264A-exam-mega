/*
 *	Drivers for using the UART, configured for use both TX and RX
 *
 * 	After some testing, the UART should not be sending quicker than 40 ms.
 *
 * 	Created: 2017-01-04 14:42:40
 *  Author: Philip Ekholm, Aron Polner
 */
#include <stdint.h>

#include <avr/interrupt.h>

#include "uart.h"

#define F_CPU          16000000 /* 16MHz */

#define BAUD           9600
#define BRC            ((F_CPU/16/BAUD) - 1)

#define TX_BUFFER_SIZE 128

static struct {
	char serial_buffer[TX_BUFFER_SIZE];
	uint8_t serial_read_pos;
	uint8_t serial_write_pos;
} me;

ISR(USART0_TX_vect) {
	if (me.serial_read_pos != me.serial_write_pos){
		UDR0 = me.serial_buffer[me.serial_read_pos];
		me.serial_read_pos++;

		if (me.serial_read_pos >= TX_BUFFER_SIZE)
			me.serial_read_pos = 0;
	}
}

static void uart_append_serial(char chr) {
	me.serial_buffer[me.serial_write_pos] = chr;
	me.serial_write_pos++;

	if (me.serial_write_pos >= TX_BUFFER_SIZE)
		me.serial_write_pos = 0;
}

void uart_init(void) {
	/*
	 * Set Baud rate by setting BRCC value, for some reason
	 * the high register only uses right nibble...
	 */
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	/* Enable both transmit and receive, as well as interrupts for both */
	UCSR0B = (1 << TXEN0)  | (1 << RXEN0) | (1 << TXCIE0);
	/* Set async-mode, 8-bit data, 1 stop-bit and no parity */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	me.serial_write_pos = 0;
	me.serial_read_pos = 0;
}

/* Send over a string, do not send more than 500 characters per second */

void uart_write_str(char *str){
	while((*str) != '\0') {
		uart_append_serial(*str);
		str++;
	}

	uart_append_serial('\r');

	/*
	 * If we reach the end of earlier transmission, we need to reset the buffer
	 * in order for the interrupt to keep working.
	 */
	if (UCSR0A & (1 << UDRE0))
		UDR0 = 0;
}







