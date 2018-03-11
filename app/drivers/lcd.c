/*
 * lcd.c
 *
 * This file contains a driver for communicating with a Philips PCD8544
 * LCD controller (48x84 pixels), 6 rows and 14 columns with 8x6 pixel
 * characters. This is the same display that was mounted in a
 * Nokia 5110 cell phone.
 * This driver is written for a LCD mounted on a "iBridge keypad shield",
 * which is mounted on a Arduino Mega board (ATmega2560).
 * SPI communication with the LCD is implemented by bit-banging the protocol.
 *
 * A character is 6 pixels wide, but only 5 pixels forms the character. The
 * last pixel is used as padding between characters.
 *
 * Created by Mathias Beckius, 26 June 2015, for the course DA346A at
 * Malmo University.
 */

#include <avr/io.h>
#include "lcd.h"
#include "char_table.h"
#include "../common.h"
#include "delay.h"

static void write_byte(uint8_t);

/*
 * Write a byte of data to the LCD, starting with the most significant bit.
 */
static void write_byte(uint8_t data)
{
	uint8_t i;
	// Set CS pin LOW
	CLR_BIT(PORTB, 6);
	// Write byte
	for (i = 0; i < 8; i++) {
		// Write MSB
		if (data & 0x80) {		// bit is 1?
			SET_BIT(PORTH, 6);	// write 1
		} else {				// bit is 0?
			CLR_BIT(PORTH, 6);	// write 0
		}
		// shift out data to LCD
		SET_BIT(PORTH, 5);
		delay_1_micros();
		CLR_BIT(PORTH, 5);
		// shift to next bit
		data = data << 1;
	}
	// Set CS pin HIGH
	SET_BIT(PORTB, 6);
}

/*
 * Initialization of the LCD:
 *	- configuration of ports and pins
 *	- configuration of LCD communication
 */
void lcd_init(void)
{
	// Configure I/O
	SET_BIT(DDRH, 5);		// Clock pin (output)
	SET_BIT(DDRH, 6);		// Data pin (output)
	SET_BIT(DDRB, 4);		// Data/command pin (output)
	SET_BIT(DDRB, 5);		// Reset pin (output)
	SET_BIT(DDRB, 6);		// CS/SS pin (output)

	// Initialize LCD
	delay_ms(20);			// Let Vcc stabilize after power-up!
	SET_BIT(PORTB, 5);		// Set Reset pin HIGH
	delay_ms(10);			// Wait a little...
	SET_BIT(PORTB, 6);		// Set CS pin HIGH (normal level)
	lcd_write(CMD, 0x21);	// Function set (H=1)
	lcd_write(CMD, 0x13);	// Set bias mode 1:48
	lcd_write(CMD, 0xC5);	// Set Vop (contrast)
	lcd_write(CMD, 0x04);	// Set temperature coefficient
	lcd_write(CMD, 0x20);	// Function set (H=0)
	lcd_write(CMD, 0x0C);	// Set display control (normal mode)
}

/*
 * Clears the LCD and moves the cursor to position row 0, column 0.
 */
void lcd_clear(void)
{
	for (uint8_t row = 0; row < 6; ++row){
		for (uint8_t col = 0; col < 14; ++col) {
			lcd_set_cursor_pos(row, col);
			lcd_write(CHR, 0x20);
		}
	}
}

/*
 * Set position of the cursor.
 *
 * parameter:
 *	row: 0-5  (row 1-6)
 *	col: 0-13 (col 1-14)
 */
void lcd_set_cursor_pos(uint8_t row, uint8_t col)
{
	row = 0x40 | row;						// set row
	lcd_write(CMD, row);
	col = 0x80 | (col * CHR_TOTAL_WIDTH);	// set column
	lcd_write(CMD, col);
}

/*
 * Writes data or command to the LCD.
 *
 * parameter:
 *	lcd_reg: register to communicate with (Command or Data register)
 *	data: 8-bit command or character
 */
void lcd_write(enum lcd_register lcd_reg, uint8_t data)
{
	uint8_t character;
	uint8_t i;

	if (lcd_reg == CMD) {
		// Prepare to send a command
		CLR_BIT(PORTB, 4);
		// Write command
		write_byte(data);
	} else {
		// Prepare to send a character
		SET_BIT(PORTB, 4);
		// write segment 1st to 5th segment of character
		character = data - ASCII_OFFSET;
		for (i = 0; i < CHR_WIDTH; i++) {
			write_byte(lcd_char_table[character][i]);
		}
		// write last segment of character (padding)
		write_byte(0x00);
	}
}

/*
 *  Write a string of character to the LCD.
 *
 *  parameter:
 *	p_str: pointer to the string's first character
 */
void lcd_write_str(char *p_str, uint8_t row)
{
	uint8_t pos = 0;

	//lcd_set_cursor_pos(row, 0);

	while (*(p_str) != '\0'){
		lcd_write(CHR, *p_str);
		lcd_set_cursor_pos(row, ++pos);

		p_str++;
	}
}



