#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "pulp.h"
#include "wire.h"

static i2c_t *i2c;
static i2c_dev_t dev;

/* Receive buffer state */
uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxIndex = 0;
uint8_t TwoWire::rxLength = 0;

/* Transmit buffer state */
uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txIndex = 0;
uint8_t TwoWire::txLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onReceive)(int);
void (*TwoWire::user_onRequest)(void);

TwoWire Wire;

void TwoWire::begin(void) {
    /* Sanitize the device structure with safe default values. */
    i2c_dev_init(&dev);

    /* Configure hardware parameters: ID, Chip Select/Address, and Speed (50kHz) */
    dev.id = 0;
    dev.cs = 0xD0;
    dev.max_baudrate = 50000;

    /* Connect to the physical I2C bus and wake it up. */
    i2c = i2c_open(&dev);

    /* Reset Receive (RX) buffer indices to ensure it starts empty*/
    rxIndex = 0;
    rxLength = 0;

    /* Reset Transmit (TX) buffer indices to prepare for new data transmission*/
    txIndex = 0;
    txLength = 0;
}

void TwoWire::begin(int address) {
    begin();
}

void TwoWire::end(void) {
    /* Check if the I2V handles is active before attempting to close. */
    if(i2c){

        /* Deactivate the hardware and realease system resources. */
        i2c_close(i2c);

        /* Clear handle to prevent accidental reuse or memory issues. */
        i2c = nullptr;
    }
}

void TwoWire::beginTransmission(int address) {
    /* Indicate that we are transmitting. */
    transmitting = 1;

    /* Set address of the targeted slave. */
    txAddress = address;

    /* Reset tx buffer iterator vars. */ 
    txIndex = 0;
    txLength = 0;

    /* Empty the buffer */
    for (size_t i = 0; i < BUFFER_LENGTH; i++) {
        txBuffer[i] = 0;
    }

    /* Set the target peripheral address for the current transaction. */
    i2c->cs = txAddress;
}

size_t TwoWire::write(byte value) {
    if(transmitting){
        /* MASTER MODE: Buffer a single byte for later transmission */
        if(txLength < BUFFER_LENGTH){
            /* Store the byte and advance the buffer pointers */
            txBuffer[txIndex] = value;
            ++txIndex;
        }
        txLength++;
    }else{
        /* PERIPHERAL MODE: Transmit the single byte immediately */

        /* Directly push a single byte through the uDMA command pipeline */
        if (i2c) i2c_write(i2c, &value, 1, true);
    }
    return 1;    
}

size_t TwoWire::write(const char *string) {
    return write((const byte *)string, strlen(string));
}

size_t TwoWire::write(const byte *data, size_t length) {
    if(transmitting){
        /* MASTER MODE: Iteratively buffer each byte in the transmit queue */        
        for(size_t i = 0; i < length; ++i){

            /* Sequentially add each byte from the array to the transmit queue */
            write(data[i]);
        }
    }else{
        /* PERIPHERAL MODE: Transmit the entire block immediately */
        if (i2c) i2c_write(i2c, (unsigned char*)data, length, true);
    }
    return length;
}

uint8_t TwoWire::endTransmission(void) {
    return endTransmission(true);
}

uint8_t TwoWire::endTransmission(bool stop) {
    int i2c_error = 0;
    bool overflow = false;
    
    /* Checking if the buffer overflow and limit the ongoing buffer length so the exiciding byte will be dropped */
    if (BUFFER_LENGTH < txLength){
        txLength = BUFFER_LENGTH;
        overflow = true;
    }

    /* True will send a stop message, releasing the bus after transmission. False will send a restart, keeping the connection active. */
    if (transmitting){

        /* Build and enqueue I2C command sequence to uDMA for transmission. */
        i2c_error = i2c_write(i2c, txBuffer, txLength, stop);

        /* Reset TX buffer indices to clear the buffer for the next transmission. */
        txIndex = 0;
        txLength = 0;
    }
    
    /* Reset transmission flag to indicate the bus is no longer busy. */
    transmitting = 0;

    /* Returns 1 to indicate data too long to fit in transmit buffer*/
    if (overflow) {
        return 1;}

    /* Returns 5 to indicate timeout */
    else if (i2c_error == 5) {
        return 5;}

    /* Return 0 to indicate success. */
    else if (i2c_error == 0) {
        return 0;}

    /* Returns 2 and 3 are not supported as the 2nd bit of uDMA I2C0 STATUS registers is disabled so the NACK flag cannot be checked*/

    /* Returns 4 to indicate other error */
    else {
        return 4;}
}

int TwoWire::requestFrom(int address, int quantity) {
    return requestFrom((int)address, (int)quantity, (bool)true);
}

int TwoWire::requestFrom(int address, int quantity, bool stop) {
    /* Ensure the hardware handle is intialized before proceeding. */
    if (i2c == nullptr) return 0;

    /* Enforce buffer safety by limiting the request size. */
    if (quantity > BUFFER_LENGTH) quantity = BUFFER_LENGTH;
    
    /* Set target peripheral address and execute the hardware-level read. */
    i2c->cs = address;
    int quantity_read = i2c_read(i2c, rxBuffer, quantity, !stop);
    
    /* Set read pointer to the beginning and update the count of available bytes. */
    rxIndex = 0;
    rxLength = quantity_read;

    return quantity_read;
}

int TwoWire::available(void) {
    /* Returns the number of bytes remaing to be read */
    return rxLength - rxIndex;
}

int TwoWire::read(void) {
    int value = -1;
  
    // get each successive byte on each call
    if(rxIndex < rxLength){
        value = rxBuffer[rxIndex];
        ++rxIndex;
    }

    return value;
}

void TwoWire::setClock(uint32_t clockfrequency) {
    i2c->max_baudrate = clockfrequency;
    i2c->div = i2c_get_div(i2c->max_baudrate);
}

void TwoWire::onReceive(void (*handler)(int)) {

}

void TwoWire::onRequest(void (*handler)(void)) {

}

void TwoWire::setWireTimeout(uint32_t timeout, bool reset_on_timeout) {
    i2c_settimeout(timeout, reset_on_timeout);
}

void TwoWire::clearWireTimeoutFlag(void) {
    i2c_managetimeoutflag(true);
}

bool TwoWire::getWireTimeoutFlag(void) {
    return i2c_managetimeoutflag(false);
}

void TwoWire::flush(void)
{
}

int TwoWire::peek(void)
{
    return 0;
}