#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "pulp.h"
#include "serial.h"
#include "variables.h"
#include "pgmspace.h"

Serials Serial(0);

Serials::Serials(int uart_id) {
    _uart_id = uart_id;
    _opened = false;
}

int Serials::availableForWrite() {
    return -1;
}

void Serials::begin(unsigned long baud) {
    uart_open(_uart_id, baud);
    _opened = true;
}

void Serials::begin(unsigned long baud, int config) {

}

void Serials::end(void) {
    uart_close(_uart_id);
    _opened = false;
}

int Serials::read() {
    uint8_t byte;
    uart_read(_uart_id, &byte, 1);
    return byte;
}

size_t Serials::write(byte data) {
    uart_write(_uart_id, &data, 1);
    return 1;
}

size_t Serials::write(const byte *data, size_t length) {
    uart_write(_uart_id, data, length);
    return length;
}

int Serials::available(void) {
    int periph = ARCHI_UDMA_UART_ID(_uart_id);
    int ret = plp_uart_rx_busy(periph);
    if (ret > 0 ) {
        return ret;
    } else {
        return -1;
    }
}

int Serials::peek(void) {
    return -1;
}

void Serials::flush(void) {

}

size_t Print::print(char val){
    return 0; 
}

size_t Print::print(const char *val){
    return 0;
}

size_t Print::print(unsigned char val){
    return print((unsigned long) val, DEC);
}

size_t Print::print(int val){
    return print((long) val, DEC);
}

size_t Print::print(unsigned int val){
    return print((unsigned long) val, DEC);
}

#ifndef __FLASHSTRINGHELPER_IS_CHAR
/* Prints a string stored in Flash memory (F-string) instead of RAM */
size_t Print::print(const __FlashStringHelper *val){
    const char *p = (const char *)val;
    size_t n = 0;

    /* Loop until null terminator ('\0') is reached*/
    while (*p) {
        if(write(p++))n++;

        /* Stop if the output device is full or fails*/
        else break;
    }
    return n;
}
#endif /* ! defined(__FLASHSTRINGHELPER_IS_CHAR) */

/* missing implementation from above are const char */
size_t Print::print(char val, int format){
    return 0; 
}

size_t Print::print(unsigned char val, int format){
    return print((unsigned long) val, format);
}

size_t Print::print(int val, int format){
    return print((long) val, format);
}

size_t Print::print(unsigned int val, int format){
    return print((unsigned long) val, format);
}

size_t Print::print(long val, int format){
    return 0;    
}

size_t Print::print(unsigned long val, int format){
    return 0;
}

size_t Print::print(double val, int format){
    /* redirects double-precision values to the specialized float formatter*/
    return printFloat(val, (uint8_t) format);
}

size_t Print::println(void){
    return 0;
}

size_t Print::println(char val){
    return 0; 
}

size_t Print::println(const char *val){
    return 0;
}

size_t Print::println(unsigned char val){
    return println((unsigned long) val, DEC);
}

size_t Print::println(int val){
    return println((long) val, DEC);
}

size_t Print::println(unsigned int val){
    return println((unsigned long) val, DEC);
}

#ifndef __FLASHSTRINGHELPER_IS_CHAR
size_t Print::println(const __FlashStringHelper *val){
    return 0;
}
#endif

/* missing implementation from above are const char */
size_t Print::println(char val, int format){
    return 0; 
}

size_t Print::println(unsigned char val, int format){
    return println((unsigned long) val, format);
}

size_t Print::println(int val, int format){
    return println((long) val, format);
}

size_t Print::println(unsigned int val, int format){
    return println((unsigned long) val, format);
}

size_t Print::println(long val, int format){
    return 0;
}

size_t Print::println(unsigned long val, int format){
    return 0;
}

size_t Print::println(double val, int format){
    return 0;
}

// Private Methods /////////////////////////////////////////////////////////////

/* checks if a number is "Not-a-Number" (NaN) */
static inline bool _isnan(double x)
{
    /* verifying as NaN is the only value that does not equal itself*/
    return x != x;
}

/* checks if a number is infinity */
static inline bool _isinf(double x)
{
    /* verifying it is not NaN but produces NaN when subtracted from itself (inf - inf = NaN) */
    return !_isnan(x) && _isnan (x - x);
}

size_t Print::printFloat(double number, uint8_t digits) 
{ 
  size_t n = 0;
  
  if (_isnan(number)) return print("nan");
  if (_isinf(number)) return print("inf");
  if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return print ("ovf");  // constant determined empirically
  
  /* Handle negative numbers */ 
  if (number < 0.0)
  {
    //  n += print('-');
     number = -number;
  }

  /* Round correctly so that print(1.999, 2) prints as "2.00" */
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  /* Extract the integer part of the number and print it */ 
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  /* Print the decimal point, but only if there are digits beyond */
  if (digits > 0) {
    // n += print('.'); 
  }

  /* Extract digits from the remainder one at a time */
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += print(toPrint);
    remainder -= toPrint; 
  } 
  
  return n;
}

