/*
 *	Drivers for keyboard and LCD-screen
 *
 * 	Will print out a key selected on the keyboard and print it to the LCD.
 * 	Use # for clearing screen
 *
 * 	Created: 2016-11-01 14:42:40
 *  Author: Philip Ekholm, Aron Polner
 */

#include <inttypes.h>
#include <stdio.h>

#include <avr/io.h>

#include "drivers/numkey.c"
#include "drivers/delay.c"
#include "drivers/lcd.c"
#include "drivers/uart.c"

int main(void)
{
	numkey_init();
	lcd_init();
	uart_init();
	lcd_clear();

	/* Enable global interrupts */
	sei();

	lcd_write_str("CURRENT BIT: ");
	char key;

	while(1) {
		char key = uart_get_char();

		if (key == '1'){
			lcd_write(CHR, '1');
			lcd_set_cursor_pos(0, 13);
		}
		else if(key == '0'){
			lcd_write(CHR, '0');
			lcd_set_cursor_pos(0, 13);
		}
	}
}





