/*!
 * @file        utils.h
 * @brief       Various utility functions and macros.
 * @author      Cesar Parent
 * @copyright   2016 Cesar Parent
 */
#pragma once
#include <avr/io.h>
#include <util/delay.h>

// MARK: Binary I/O utility

/*!
 * @brief       Creates a bitmask.
 * @param       bit         The bit to set in the mask.
 * @return      A bit mask with `bit` set.
 */
#define bitmask(bit) (1 << (bit))

/*!
 * @brief       Sets the bit at a given index of a variable.
 * @param       port        The variable or I/O port to modify.
 * @param       bit         The index of the bit to set.
 */
#define sbi(port, bit) (port |= bitmask(bit))

/*!
 * @brief       Clears the bit at a given index of a variable.
 * @param       port        The variable or I/O port to modify.
 * @param       bit         The index of the bit to clear.
 */
#define cbi(port, bit) (port &= ~bitmask(bit))

/*!
 * @brief       Check whether a given bit of a variable is set
 * @param       port        The variable or I/O port to check.
 * @param       bit         The index of the bit to check.
 * @return      Whether the `bit` is set in `port`
 */
#define check(port, bit) ((((port) & bitmask(bit)) >> bit) != 0)

#define SETMODE_IN(port, pin) (cbi(DDR##port, pin))
#define SETMODE_OUT(port, pin) (sbi(DDR##port, pin))
#define SETMODE_INUP(port, pin) do {                    \
cbi(DDR##port, pin)                                     \
sbi(PORT##port, pin)                                    \
} while(0)
    
// MARK: Utility timing functions

#define delay(time) _delay_ms(time)
#define delayUS(time) _delay_us(time)