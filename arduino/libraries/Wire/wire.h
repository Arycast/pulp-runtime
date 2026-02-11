#ifndef wire_h
#define wire_h

/*This library inherits from stream functions.
so, send() and receive() been replaced with read() and write()
This library implementation use a 32 byte buffer*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "String.hpp"

#include "stream.h" //-> parent library for wire

#define BUFFER_LENGTH 32

#define WIRE_HAS_TIMEOUT // macro if setWireTimeout(), getWireTimeoutFlag(), and clearWireTimeout() are available

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
    void begin(void);
    void begin(uint8_t address);
    void end(void);
    int requestFrom(int address, int quantity);
    int requestFrom(int address, int quantity, bool stop);
    void beginTransmission(int address);
    int endTransmission(void);
    size_t write(uint8_t value) override;
    size_t write(const char *str);
    size_t write(const String &myString);
    size_t write(const uint8_t *data, size_t length) override;
    int available(void) override;
    void flush(void) override;
    int peek(void) override;
    int read(void) override; // inherit from stream utility class
    void setClock(uint32_t frequency);
    void onReceive(void (*function)(int));
    void onRequest(void(*function)(int));
    void setWireTimeout(uint32_t timeout, bool reset_on_timeout);
    void setWireTimeout(void); //using default timeout
    void clearWireTimeoutFlag(void);
    bool getWireTimeoutFlag(void);
};

extern TwoWire Wire;

#endif