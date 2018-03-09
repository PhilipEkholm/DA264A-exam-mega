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

#define TX_BUFFER_SIZE 1024
#define RX_BUFFER_SIZE 1024

static struct {
	char tx_buffer[TX_BUFFER_SIZE];
	char rx_buffer[RX_BUFFER_SIZE];
	uint8_t tx_read_pos;
	uint8_t rx_read_pos;
	uint8_t tx_write_pos;
	uint8_t rx_write_pos;
} me;

ISR(USART1_TX_vect) {
	if (me.tx_read_pos != me.tx_write_pos){
		UDR1 = me.tx_buffer[me.tx_read_pos];
		me.tx_read_pos++;

		if (me.tx_read_pos >= TX_BUFFER_SIZE)
			me.tx_read_pos = 0;
	}
}

ISR(USART1_RX_vect) {
	/* First check for parity error */
	if (UCSR1A & (1 << UPE1)){
		/* Return an E as in error if that's the case */
		me.rx_buffer[me.rx_write_pos] = UDR1;
		me.rx_buffer[me.rx_write_pos] = 'E';
	}
	else {
		me.rx_buffer[me.rx_write_pos] = UDR1;
	}

	me.rx_write_pos++;

	/* TODO: Eventually need to fix this if we write enough chars to "run over" */
	if (me.rx_write_pos >= RX_BUFFER_SIZE)
		me.rx_write_pos = 0;
}

static void uart_append_serial(char chr) {
	me.tx_buffer[me.tx_write_pos] = chr;
	me.tx_write_pos++;

	if (me.tx_write_pos >= TX_BUFFER_SIZE)
		me.tx_write_pos = 0;
}

/*
 * Get the oldest char found in the buffer,
 *
 * @param peek set to true if you only want to peek
 * otherwise false.
 */

char uart_get_char(bool peek) {
	char ret = '\0';

	if (me.rx_read_pos != me.rx_write_pos) {
		ret = me.rx_buffer[me.rx_read_pos];



		if (!peek)
			me.rx_read_pos++;

		if (me.rx_read_pos >= RX_BUFFER_SIZE)
			me.rx_read_pos = 0;
	}

	return ret;
}

void uart_get_str(char *p_str) {
	while(uart_get_char(true) != '\0') {
		*p_str = uart_get_char(false);
		p_str++;
	}

	*p_str = '\0';
}


/* Setup the UART */

void uart_init(void) {
	/*
	 * Set Baud rate by setting BRCC value, for some reason
	 * the high register only uses right nibble...
	 */
	UBRR1H = (BRC >> 8);
	UBRR1L = BRC;
	/* Enable both transmit and receive, as well as interrupts for both */
	UCSR1B = (1 << TXEN0)  | (1 << RXEN0) | (1 << TXCIE0) | (1 << RXCIE0);
	/* Set async-mode, 8-bit data, 1 stop-bit and no parity and enable even parity */
	UCSR1C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << UPM01) | (0 << UPM00);
	


	/* Set pos variables to 0 */
	me.tx_write_pos = 0;
	me.tx_read_pos = 0;
	me.rx_write_pos = 0;
	me.rx_read_pos = 0;
}

/* Transmit a string, do not send more than 500 characters per second */

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







