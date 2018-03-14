/*
 * lcd.h
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

#pragma once

#include <inttypes.h>

/*
 * To be used with lcd_write()
 */
enum lcd_register
{
	CMD,	// to send a Command to the LCD
	CHR		// to send a character to the LCD
};

void lcd_init(void);
void lcd_write(enum lcd_register, uint8_t);
void lcd_write_str(char *);
void lcd_clear(void);
void lcd_set_cursor_pos(uint8_t, uint8_t);
