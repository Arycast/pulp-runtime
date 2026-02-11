/**
 * The Wire library allows you to communicate with I2C devices. I2C is a very 
 * common protocol, primarily used for reading and sending data to external 
 * I2C components. This library inherits from the Stream functions.
 *
 * This file is implementation of Wire class of arduino.
 * https://docs.arduino.cc/language-reference/en/functions/communication/wire/
 */

#ifndef wire_h
#define wire_h

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "String.hpp"
#include "stream.h"

/* Size of the transmission and reception buffers */
#define BUFFER_LENGTH 32

/* The WIRE_HAS_TIMEOUT macro is only defined when Wire.setWireTimeout(), Wire.getWireTimeoutFlag(), and Wire.clearWireTimeoutFlag() are all available. */
#define WIRE_HAS_TIMEOUT

class TwoWire : public Stream {
private:
    static uint8_t rxBuffer[];
    static uint8_t rxIndex;
    static uint8_t rxLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txIndex;
    static uint8_t txLength;

    static uint8_t transmitting;
public:
    /**
     * method begin
     * This function initializes the Wire library and join the I2C bus as a controller 
     * or a peripheral. This function should normally be called only once.
     *
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/begin/
     */
    void begin(void);
    void begin(uint8_t address);

    /**
     * method end
     * Disable the Wire library, reversing the effect of Wire.begin(). To use the Wire 
     * library again after this, call Wire.begin() again.
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/end/
     */
    void end(void);

    /**
     * method requestFrom
     * This function is used by the controller device to request bytes from a 
     * peripheral device. The bytes may then be retrieved with the available()
     * and read() functions.
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/requestFrom/
     */
    int requestFrom(uint8_t address, uint8_t quantity);
    int requestFrom(uint8_t address, uint8_t quantity, bool stop);
    
    /**
     * method beginTransmission
     * This function begins a transmission to the I2C peripheral device with the given 
     * address. Subsequently, queue bytes for transmission with the write() function 
     * and transmit them by calling endTransmission().
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/beginTransmission/
     */
    void beginTransmission(uint8_t address);

    /**
     * method endTransmission
     * This function ends a transmission to a peripheral device that was begun by 
     * beginTransmission() and transmits the bytes that were queued by write().
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/endTransmission/
     */
    uint8_t endTransmission(void);
    uint8_t endTransmission(bool stop);

    /**
     * method write
     * This function writes data from a peripheral device in response to a request 
     * from a controller device, or queues bytes for transmission from a controller 
     * to peripheral device (in-between calls to beginTransmission() and 
     * endTransmission().
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/write/
     */
    size_t write(uint8_t value);
    size_t write(const char *str);
    size_t write(const String &myString);
    size_t write(const uint8_t *data, size_t length);

    /**
     * method available
     * This function returns the number of bytes available for retrieval with read().
     * available() inherits from the Stream utility class. 
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/available/
     */
    int available(void) override;
    
    /**
     * method read
     * This function reads a byte that was transmitted from a peripheral device to a 
     * controller device after a call to requestFrom() or was transmitted from a 
     * controller device to a peripheral device. read() inherits from the Stream 
     * utility class.
     *
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/read/
     */
    int read(void) override;
    
    /**
     * method setClock
     * This function modifies the clock frequency for I2C communication. 
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/setClock/
     */
    void setClock(uint32_t clockfrequency);

    /**
     * method onReceive
     * This function registers a function to be called when a peripheral device 
     * receives a transmission from a controller device.
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/onReceive/
     */
    void onReceive(void (*handler)(int));

    /**
     * method onRequest
     * This function registers a function to be called when a controller device
     * requests data from a peripheral device.
     *
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/onRequest/
     */
    void onRequest(void(*handler)(void));

    /**
     * method setWireTimeout
     * Sets the timeout for Wire transmissions in master mode. Note: these timeouts are
     * almost always an indication of an underlying problem, such as misbehaving 
     * devices, noise, insufficient shielding, or other electrical problems.
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/setWireTimeout/
     */
    void setWireTimeout(uint32_t timeout, bool reset_on_timeout);
    void setWireTimeout(void);

    /**
     * method clearWireTimeoutFlag
     * Clears the timeout flag. Timeouts might not be enabled by default. 
     *
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/clearWireTimeoutFlag/
     */
    void clearWireTimeoutFlag(void);
    
    /**
     * method getWireTimeoutFlag
     * Checks whether a timeout has occurred since the last time the flag was cleared.
     * 
     * https://docs.arduino.cc/language-reference/en/functions/communication/wire/getWireTimeoutFlag/
     */
    bool getWireTimeoutFlag(void);

    void flush(void) override;
    int peek(void) override;
};

extern TwoWire Wire;

#endif