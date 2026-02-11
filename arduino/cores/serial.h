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
    size_t write(uint8_t data) override;
    size_t write(const uint8_t *data, size_t length) override;

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
};

/**
    * Pre-instantiated object for global access to the Serial hardware
    */
extern Serials Serial;

#endif