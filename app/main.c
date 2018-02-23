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
	char key;
	char *comp_str = "HELU";

	numkey_init();
	lcd_init();
	uart_init();
	lcd_clear();

	/* Enable global interrupts */
	sei();

	lcd_write_str("MESSAGE: ", 0);

	while(1) {
		comp_str = "";
		key = numkey_read();
		delay_s(3);

		/* Clear screen */
		if (key == 'C'){
			lcd_clear();
			lcd_write_str("MESSAGE: ", 0);
		}

		uart_get_str(comp_str);

		lcd_write_str(comp_str, 1);
	}
}





