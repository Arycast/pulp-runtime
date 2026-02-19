/**
    * this file is implementation of Arduino Serial API
    * https://docs.arduino.cc/language-reference/en/functions/communication/serial/
*/

#ifndef serial_h
#define serial_h

#include <stddef.h> /* provides standard definitions, like size_t*/
#include <stdbool.h> /* provides boolean type support */
#include "stream.h" /* base class for character and binary based streams */

/**
    * class Serials
    * inherits from Stream class
    */
class Serials : public Stream {
protected :
    bool _opened; /* Tracks if the serial is intialized */
    int _uart_id; /* UART id */
    size_t printFloat(double number, uint8_t digits) override;
    size_t printNumber(unsigned long val, int format) override;
    
public :
    /**
        * Public class method
        */

    /**
        * Serials constructor to specify which UART to use
        */
    Serials(int uart_id = 0);

    /**
        * method availableForWrite
        * Get the number of bytes available for writing in the serial buffer without blocking the write operation
        * @return The number of bytes available to write
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/availableForWrite/
        */
    int availableForWrite(void);

    /**
        * method begin
        * sets the data rate in bits per second (baud) for serial data transmission
        * @param baud in bits per seconds (baud)
        * @param config sets data, parity, and stop bits
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/
        */
    void begin(unsigned long baud = 115200);
    void begin(unsigned long baud, int config);

    /**
        * method end
        * disables serial communication
        * allowing the Rx dan Tx pins to be used for general input/output
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/end/
        */
    void end(void);

    /**
        * method read
        * reads incoming serial data and consumes it from the serial buffer
        * override read method in Stream class, because serial has its own implementation
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/read/
        */
    int read(void) override;

    /**
        * method write
        * writes binary data to the serial
        * the data is sent as a byte
        * override write method in Stream class, because serial has its own implementation
        * @param data The byte to be transmitted
        * @return The number of bytes written
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/write/
        */
    size_t write(byte data) override;
    size_t write(const char *string) override;
    size_t write(const byte *data, size_t length) override;

    /**
        * method available
        * get the number of bytes available for reading from serial
        * refers to data that has already been received and is currently stored in the serial receive buffer
        * override available method in Stream class, because serial has its own implementation
        * @return The number of bytes available to read
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/available/
        */
    int available(void) override;

    /**
        * method peek
        * returns the next byte of incoming serial data without removing it from the internal serial buffer
        * override peek method in Stream class, because serial has its own implementation
        * @return The first byte of incoming serial data available (or -1 if no data is available)
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/peek/
        */
    int peek(void) override;

    /**
        * method flush
        * waits for the transmission of outgoing serial data to complete
        * override flush method in Stream class, because serial has its own implementation
        * https://docs.arduino.cc/language-reference/en/functions/communication/serial/flush/
        */
    void flush(void) override;

    /**
         * method print
         * The print method details in Print class are redirected to the 
         * Serial.print page. Prints data to the serial port as human-readable 
         * ASCII text. Allowed data types for val: any data type. 
         * 
         * https://docs.arduino.cc/language-reference/en/functions/communication/serial/print/
         */
    size_t print(char val) override;
    size_t print(const char *val) override;
    // size_t print(unsigned char val) override;
    size_t print(int val) override;
    size_t print(unsigned int val) override;
#ifndef __FLASHSTRINGHELPER_IS_CHAR
    size_t print(const __FlashStringHelper *val) override;
#endif
    size_t print(char val, int format) override;
    size_t print(unsigned char val, int format) override;
    size_t print(int val, int format) override;
    size_t print(unsigned int val, int format) override;
    size_t print(long val, int format = DEC) override;
    size_t print(unsigned long val, int format = DEC) override;
    size_t print(double val, int format = 2) override;

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
    size_t println(void) override;
    size_t println(char val) override;
    size_t println(const char *val) override;
    size_t println(unsigned char val) override;
    size_t println(int val) override;
    size_t println(unsigned int val) override;
#ifndef __FLASHSTRINGHELPER_IS_CHAR
    size_t println(const __FlashStringHelper *val) override;
#endif
    size_t println(char val, int format = DEC) override;
    size_t println(unsigned char val, int format) override;
    size_t println(int val, int format) override;
    size_t println(unsigned int val, int format) override;
    size_t println(long val, int format = DEC) override;
    size_t println(unsigned long val, int format = DEC) override;
    size_t println(double val, int format = 2) override;
    };

/**
    * Pre-instantiated object for global access to the Serial hardware
    */
extern Serials Serial;

#endif