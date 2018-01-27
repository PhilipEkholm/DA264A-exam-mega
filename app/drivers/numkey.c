/*
 * numkey.c
 *
 * This file contains a driver for reading the keypad on the
 * "iBridge keypad shield". The driver is adapted for a shield mounted
 * on a Arduino Mega board (ATmega2560).
 *
 * Key mapping:
 *					-----------------
 *					| 1 | 2 | 3 | A |
 *					-----------------
 *					| 4 | 5 | 6 | B |
 *					-----------------
 *					| 7 | 8 | 9 | C |
 *					-----------------
 *					| * | 0 | # | D |
 *					-----------------
 *
 * Created by Mathias Beckius, 26 June 2015, for the course DA346A at
 * Malmo University.
 */
#include <inttypes.h>

#include <avr/io.h>

#include "numkey.h"
#include "../common.h"
#include "delay.h"

/*
 * Key mapping table for the numeric keyboard.
 * The positions are defined a 4-bit address. The 4 MSB's represents the
 * keyboard's columns, the 4 LSB's represents the keyboards rows:
 * Example:
 *		col 0, row 0 (binary 0000, decimal 0) is mapped to '1',
 *		col 1, row 0 (binary 0100, decimal 4) is mapped to '2'
 * The address corresponds to a position in the mapping table.
 */
static const char key_map[16] =	{'1', '4', '7', '*',
								 '2', '5', '8', '0',
								 '3', '6', '9', '#',
								 'A', 'B', 'C', 'D'};

/*
 * Initialization of the numeric keyboard.
 * This function configures the ports and pins to be used by the keyboard.
 */
void numkey_init(void)
{
	// Configure I/O
	SET_BIT(DDRG, 5);		// Column 0 (output)
	SET_BIT(DDRE, 3);		// Column 1 (output)
	SET_BIT(DDRH, 3);		// Column 2 (output)
	SET_BIT(DDRH, 4);		// Column 3 (output)
	CLR_BIT(DDRF, 5);		// Row 0 (input)
	CLR_BIT(DDRF, 4);		// Row 1 (input)
	CLR_BIT(DDRE, 4);		// Row 2 (input)
	CLR_BIT(DDRE, 5);		// Row 3 (input)
}

/*
 * Read keyboard.
 * If a key is pressed, then the corresponding character
 * is returned:
 *	If '1' is pressed, then the character '1' is returned.
 * Otherwise a NO_KEY character is returned.
 * Important! Due to MUX propagation delay and button bounce, it's necessary
 * to generate a delay (after setting column and row) before reading the pin.
 *
 * return:
 *	If a key is pressed, then a character that represents the key is returned.
 *	Otherwise, a NO_KEY character is returned!
 */
char numkey_read(void)
{
	SET_BIT(PORTG, 5);			// Set column 0
	CLR_BIT(PORTE, 3);			// Clear column 1
	CLR_BIT(PORTH, 3);			// Clear column 2
	CLR_BIT(PORTH, 4);			// Clear column 3
	delay_ms(1);
	if (PINF & 0x20) {			// Row 0?
		return key_map[0];
	} else if (PINF & 0x10) {	// Row 1?
		return key_map[1];
	} else if (PINE & 0x10) {	// Row 2?
		return key_map[2];
	} else if (PINE & 0x20) {	// Row 3?
		return key_map[3];
	}

	CLR_BIT(PORTG, 5);			// Clear column 0
	SET_BIT(PORTE, 3);			// Set column 1
	CLR_BIT(PORTH, 3);			// Clear column 2
	CLR_BIT(PORTH, 4);			// Clear column 3
	delay_ms(1);
	if (PINF & 0x20) {			// Row 0?
		return key_map[4];
	} else if (PINF & 0x10) {	// Row 1?
		return key_map[5];
	} else if (PINE & 0x10) {	// Row 2?
		return key_map[6];
	} else if (PINE & 0x20) {	// Row 3?
		return key_map[7];
	}

	CLR_BIT(PORTG, 5);			// Clear column 0
	CLR_BIT(PORTE, 3);			// Clear column 1
	SET_BIT(PORTH, 3);			// Set column 2
	CLR_BIT(PORTH, 4);			// Clear column 3
	delay_ms(1);

	if (PINF & 0x20)
		return key_map[8];
	else if (PINF & 0x10)
		return key_map[9];
	else if (PINE & 0x10)
		return key_map[10];
	else if (PINE & 0x20)
		return key_map[11];

	CLR_BIT(PORTG, 5);			// Clear column 0
	CLR_BIT(PORTE, 3);			// Clear column 1
	CLR_BIT(PORTH, 3);			// Clear column 2
	SET_BIT(PORTH, 4);			// Set column 3
	delay_ms(1);

	if (PINF & 0x20)
		return key_map[12];
	else if (PINF & 0x10)
		return key_map[13];
	else if (PINE & 0x10)
		return key_map[14];
	else if (PINE & 0x20)
		return key_map[15];

	// no key was pressed!
	return NO_KEY;
}
















