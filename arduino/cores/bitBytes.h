/**
    * this file is implementation of Arduino bitBytes API
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/
*/

#ifndef bitBytes_h
#define bitBytes_h

/**
    * macro to create a bitmask with a single bit set at a specific position
    * set the bit state on the 'n' position
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/bit/
*/
#define bit(n) (1 << (n))

/**
    * macro to clear a bit of numeric variable at a spesific position (set it to 0)
    * clear the bit state on the 'bit' position of the 'value' variable.
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/bitClear/
*/
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

/**
    * macro to read a bit of a variable on a spesific position
    * read the bit state on the 'bit' position of the 'value' variable
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/bitRead/
*/
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

/**
    * macro to set a bit of a numeric variable at a specific position (set it to 1)
    * set the bit state on the 'bit' position of the 'value' variable
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/bitSet/
*/
#define bitSet(value, bit) ((value) |= (1UL << (bit)))

/**
    * macro to write to a bit of a variable
    * freely modify the bit state on the 'n' position of the 'value' variable
    * if bit = 1, use bitset macro on 'n' position of the 'value' variable (set bit state to 1)
    * if bit = 0, use bitclear macro on 'n' position of the 'value' variable (set bit state to 0)
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/bitWrite/
*/
#define bitWrite(value, n, bit) ((bit)? bitSet(value, n) : bitClear(value, n))

/**
    * macro to extract the high 8 bits (MSB) from 16 bit-integer
    * right shift "w" variable by 8 and perform a bitwise AND with 0xFF to isolate the byte
    * returns MSB (bits 8 - 15)
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/highByte/
*/
#define highByte(w) ((uint8_t) (((w) >> 8) & 0xFF))

/**
    * macro to extract the low 8 bits (LSB) from 16 bit-integer
    * perform a bitwise AND with 0xFF to isolate the byte
    * returns LSB (bits 0 - 7)
    * https://docs.arduino.cc/language-reference/en/functions/bits-and-bytes/lowByte/
*/
#define lowByte(w) ((uint8_t) ((w) & 0xFF))

#endif