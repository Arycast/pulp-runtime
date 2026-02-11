/**
 * The Print class is an abstract base class that provides a common interface for 
 * printing data to different output devices. It defines several methods that 
 * allow printing data in different formats. Some of the libraries that use the 
 * Print class are: Serial, LiquidCrystal, Ethernet, Wifi.
 *
 * this file is implementation of Print class of arduino
 * https://docs.arduino.cc/language-reference/en/functions/communication/print/
 */

#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

#include "variables.h"
#include "pgmspace.h"

/*#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN*/ // Prevent warnings if BIN is previously defined
/*#undef BIN
#endif
#define BIN 2*/

class Print {
    private:
        size_t printNumber(unsigned long n, uint8_t base);
        size_t printFloat(double number, uint8_t digits);

    public:
        static inline size_t _strlen(const char *s) {
            size_t n = 0;
            while (*s++) n++;
            return n;
        }

        /**
         * method write
         * The write method details in Print class are redirected to the
         * Wire.write page. 
         * 
         * https://docs.arduino.cc/language-reference/en/functions/communication/wire/write/
         */
        
        /* The Print class is an abstract base class. This is a pure virtual function, mearning every class that inherits from Print must provide its own implementation*/
        virtual size_t write(uint8_t value) = 0;

        /* The Print class is an abstract base class. Subclasses can customize this to handle large blocks of data faster.*/
        virtual size_t write(const uint8_t *data, size_t length);

        size_t write(const char *string){
            if (string == NULL) return 0;
            return write((const uint8_t *)string, _strlen(string));
        }
        size_t write(const char *data, size_t length) {
            return write((const uint8_t *)data, length);
        }

        /**
         * method print
         * The print method details in Print class are redirected to the 
         * Serial.print page. Prints data to the serial port as human-readable 
         * ASCII text. Allowed data types for val: any data type. 
         * 
         * https://docs.arduino.cc/language-reference/en/functions/communication/serial/print/
         * 
         */
        size_t print(char val);
        size_t print(const char *val);
        size_t print(unsigned char val);
        size_t print(int val);
        size_t print(unsigned int val);
#ifndef __FLASHSTRINGHELPER_IS_CHAR
        size_t print(const __FlashStringHelper *val);
#endif
        size_t print(unsigned char val, int format);
        size_t print(int val, int format);
        size_t print(unsigned int val, int format);
        size_t print(long val, int format = DEC);
        size_t print(unsigned long val, int format = DEC);
        size_t print(double val, int format = 2);

        /**
         * method println
         * The println method details in Print class are redirected to the 
         * Serial.println page. Prints data to the serial port as human-readable
         * ASCII text followed by a carriage return character (ASCII 13, or '\r') 
         * and a newline character (ASCII 10, or '\n'). This command takes the same
         * forms as Serial.print().
         *
         * https://docs.arduino.cc/language-reference/en/functions/communication/serial/println/
         */
        size_t println(unsigned char val);
        size_t println(int val);
        size_t println(unsigned int val);
        size_t println(unsigned char val, int format);
        size_t println(int val, int format);
        size_t println(unsigned int val, int format);
        size_t println(long val, int format = DEC);
        size_t println(unsigned long val, int format = DEC);
        size_t println(double val, int format = 2);
        size_t println(const __FlashStringHelper *val);
        size_t println(void);
};

#endif