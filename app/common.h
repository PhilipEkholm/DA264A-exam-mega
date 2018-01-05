/*
 * common.h
 *
 * Common macros throughout the program
 *
 * Author:	Philip Ekholm, Aron Polner
 *
 * Date:	2017-10-28
 */

#pragma once

/*
 * This macro is used to set a specific bit in a register.
 */
#define SET_BIT(reg, pos)	(reg |= (1 << pos))

/*
 * This macro is used to clear a specific bit in a register.
 */
#define CLR_BIT(reg, pos)   (reg &= ~(1 << pos))

/*
 * This macro is used to modify several bits of a register.
 * Example:
 *	Bit 7-4 of PORTD should be set to 1010, while the
 *	rest of the register should NOT be modified! Usage:
 *		SET_BIT_LEVELS(PORTD, 0b00001111, 0b10100000);
 * The bit mask is used to clear the bits that should be modified. The bit
 * mask also protects the bits that shouldn't be modified.
 */
#define SET_BIT_LEVELS(reg, bit_mask, bit_data) \
	(reg) = (((reg) & (bit_mask)) | ((bit_data) & ~(bit_mask)))
