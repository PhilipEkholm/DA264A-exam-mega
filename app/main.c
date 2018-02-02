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
	char key, comp_key;
	numkey_init();
	lcd_init();
	uart_init();
	lcd_clear();

	/* Enable global interrupts */
	sei();

	lcd_write_str("MESSAGE: ");
	lcd_set_cursor_pos(1, 0);

	while(1) {
		key = numkey_read();
		comp_key = uart_get_char(false);
		delay_ms(20);

		if (key == 'C'){
			lcd_clear();
			lcd_set_cursor_pos(0, 0);
			lcd_write_str("MESSAGE: ");
			lcd_set_cursor_pos(1, 0);
		}

		if (comp_key != '\0')
			lcd_write(CHR, comp_key);
	}
}





