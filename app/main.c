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
	char 	key,
			chr;

	char *power_str = "P=";

	numkey_init();
	lcd_init();
	uart_init();
	lcd_clear();

	/* Enable global interrupts */
	sei();

	lcd_write_str(power_str);

	while(1) 
	{
		/* Added to be able to clear the screen */
		key = numkey_read();
		delay_ms(10);
		lcd_set_cursor_pos(0, 4);

		/* Clear screen */
		if (key == 'C')
		{
			lcd_clear();
			lcd_write_str(power_str);
		}

		/* Get two characters from the UART */
		for(int i = 0; i < 3; ++i)
		{
			while(chr = uart_get_char(true) == 'W')
			{
				// do nothing
			}

			chr = uart_get_char(false);

			lcd_write(CHR, chr);
		}

		lcd_write_str(" E-3W");
		delay_ms(20);
	}
}





