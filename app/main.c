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

	lcd_write_str("HELLO WORLD");
	char last_key;
	char key;

	uint8_t counter = 0;
	char string[25];

	while(1) {
		/*key = numkey_read();

		if (key != NO_KEY && key != last_key){
			if(key == '#')
				lcd_clear();
			else
				lcd_write(CHR, key);
		}

		last_key = key;*/

		delay_ms(40);

		sprintf(string, "Hello world: %d\r\n", counter);

		uart_write_str(string);
		counter++;
	}
}





