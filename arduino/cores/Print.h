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

class Print {
    private:
    protected:
        virtual size_t printFloat(double number, int digits) = 0;
        virtual size_t printNumber(unsigned long val, int format) = 0;

    public:
        /**
         * method write
         * The write method details in Print class are redirected to the
         * Wire.write page. 
         * 
         * https://docs.arduino.cc/language-reference/en/functions/communication/wire/write/
         */
        virtual size_t write(byte value) = 0;
        virtual size_t write(const char *string) = 0;
        virtual size_t write(const byte *data, size_t length) = 0;

        /**
         * method print
         * The print method details in Print class are redirected to the 
         * Serial.print page. Prints data to the serial port as human-readable 
         * ASCII text. Allowed data types for val: any data type. 
         * 
         * https://docs.arduino.cc/language-reference/en/functions/communication/serial/print/
         */
        virtual size_t print(const String &s) = 0;
        virtual size_t print(char val) = 0;
        virtual size_t print(const char *val) = 0;
        virtual size_t print(int val) = 0;
        virtual size_t print(unsigned int val) = 0;
        virtual size_t print(int val, int format) = 0;
        virtual size_t print(unsigned int val, int format) = 0;
        virtual size_t print(long val, int format = DEC) = 0;
        virtual size_t print(unsigned long val, int format = DEC) = 0;
        virtual size_t print(double val, int format = 2) = 0;

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
        virtual size_t println(void) = 0;
        virtual size_t println(const String &s) = 0;
        virtual size_t println(char val) = 0;
        virtual size_t println(const char *val) = 0;
        virtual size_t println(int val) = 0;
        virtual size_t println(unsigned int val) = 0;
        virtual size_t println(int val, int format) = 0;
        virtual size_t println(unsigned int val, int format) = 0;
        virtual size_t println(long val, int format = DEC) = 0;
        virtual size_t println(unsigned long val, int format = DEC) = 0;
        virtual size_t println(double val, int format = 2) = 0; 
};

#endif