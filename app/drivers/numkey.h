/*
 * numkey.h
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

#pragma once

// a character that represents that no key is pressed!
#define NO_KEY '\0'

void numkey_init(void);
char numkey_read(void);
