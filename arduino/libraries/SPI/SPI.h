/**
    * this file is implementation of Arduino SPI API
    * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/
*/

#ifndef SPI_h
#define SPI_h

#include <stdint.h> /* provides standard fixed-width integer */
#include "variables.h" /* provides MSBFIRST and LSBFIRST definition */


/* #define MSBFIRST 1
#define LSBFIRST 0 */

/**
    * define SPI Mode variant
    * CPHA = Clock Phase, CPOL = Clock Polarity
    * Mode 0 -> CPHA = 0, CPOL = 0
    * Mode 1 -> CPHA = 1, CPOL = 0
    * Mode 2 -> CPHA = 0, CPOL = 1
    * Mode 3 -> CPHA = 1, CPOL = 1
    */
#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

/**
    * SPISettings class
    * stores the clock frequency, bit order (MSB/LSB), and SPI data mode 
    */
class SPISettings {
public:
    uint32_t clock;     /* max baudrate */
    uint8_t bitOrder;   /* bit order : MSB/LSB */
    uint8_t dataMode;   /* SPI Mode : Mode 0, Mode 1, Mode 2, or Mode 3 */

    /**
        * parameterized constructor 
        */
    SPISettings(uint32_t clk, uint8_t bitO, uint8_t mode) : clock(clk), bitOrder(bitO), dataMode(mode) {}

    /**
        * default constructor
        */
    SPISettings() : clock(1000000), bitOrder(MSBFIRST), dataMode(SPI_MODE0) {}
};

/**
    * SPIClass class
    */
class SPIClass {
public:
    /**
        * public class method
        */

    /**
        * method begin
        * initializes the SPI bus
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/begin/
        */
    static void begin(void);

    /** 
        * method beginTransaction
        * initializes the SPI bus using spesific settings (from the constructor)
        * note that calling begin method is required before calling this method
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/beginTransaction/
        */
    static void beginTransaction(SPISettings settings);

    /**
        * method endTransaction
        * stop using the SPI bus
        * called after de-asserting the chip select 
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/endTransaction/
        */
    static void endTransaction(void);

    /**
        * method end
        * disables the SPI bus
        https://docs.arduino.cc/language-reference/en/functions/communication/SPI/end/
        */
    static void end(void);

    /**
        * method setBitOrder
        * sets the order of the bits shifted out of and into the SPI bus
        * should not be use in new projects
        * use SPISettings with beginTransaction method to configure SPI parameters
        * @param bitOrder LSBFIRST or MSBFIRST
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/setBitOrder/
        */
    static void setBitOrder(uint8_t bitOrder);

    /**
        * method setClockDivider
        * sets the SPI clock divider relative to the system clock
        * should not be use in new projects
        * use SPISettings with beginTransaction method to configure SPI parameters
        * @param divider
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/setClockDivider/
        */
    static void setClockDivider(uint8_t divider);

    /**
        * method setDataMode
        * sets the SPI data mode
        * should not be use in new projects
        * use SPISettings with beginTransaction method to configure SPI parameters
        * @param dataMode SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/setDataMode/
        */
    static void setDataMode(uint8_t dataMode);

    /**
        * method transfer and transfer16
        * based on simultaneous send and receive
        * the received data is stored in the buffer
        * @param val The byte to send out over the bus
        * @param val16 The two bytes variable to send out over the bus
        * @param buffer The array of data to be transferred
        * @param size The number of data to be transferred
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/transfer/ 
        */
    static void transfer(void *val);
    static void transfer16(void *val16);
    static void transfer(void *buffer, size_t size);

    /**
        * method usingInterrupt
        * registers the interrupt number or name with the SPI library
        * allows beginTransactions method to prevent usage conflicts
        * Note that the interrupt specified in the call to usingInterrupt() will be disabled on a call to beginTransaction() 
        * and re-enabled in endTransaction()
        * @param interruptNumber The associated interrupt number
        * https://docs.arduino.cc/language-reference/en/functions/communication/SPI/usingInterrupt/
        */
    static void usingInterrupt(int interruptNumber);

private:
    /**
        * struct current_type
        * for tracking active SPI Settings
        */
    struct current_type {
        uint32_t clock;
        uint8_t bitOrder;
        uint8_t dataMode;
    } static current;
};

/**
    * Pre-instantiated object for global access to the SPI hardware
    */
extern SPIClass SPI;

#endif