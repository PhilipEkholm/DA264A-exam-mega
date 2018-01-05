/*
 * 	delay.c
 *
 *	Create specified busy-wait delays
 *
 * 	Created: 2016-11-01
 *  Author: Magnus Krampell, Philip Ekholm
 */

#include "delay.h"

#define QUARTER_1000 250

/* Let's try to use the assembler NOP-instructions instead, worked good thus far */
void delay_1_micros() {
	__asm("NOP");
	__asm("NOP");
	__asm("NOP");
}

void delay_micros(uint8_t us) {
	for (uint8_t i = 0; i < us; ++i) {
		delay_1_micros();
	}
}

void delay_ms(uint8_t ms) {
	for (uint8_t i = 0; i < ms; ++i) {
		delay_micros(QUARTER_1000);
		delay_micros(QUARTER_1000);
		delay_micros(QUARTER_1000);
		delay_micros(QUARTER_1000);
	}
}

void delay_s(uint8_t s) {
	for (int i = 0; i < s; ++i) {
		delay_ms(QUARTER_1000);
		delay_ms(QUARTER_1000);
		delay_ms(QUARTER_1000);
		delay_ms(QUARTER_1000);
	}
}






