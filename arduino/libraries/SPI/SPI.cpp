#include "pulp.h"
#include "SPI.h"

static spim_t *spim; //defined in pulp.h, spi section 
static spim_conf_t conf; //defined in pulp.h, spi section

SPIClass SPI;

uint32_t SPIClass::_interruptMask = 0;

SPIClass::current_type SPIClass::current = {1000000, SPI_MSBFIRST, SPI_MODE0};

void SPIClass::begin(void) {
    spim_conf_init(&conf);
    conf.id = 0;
    conf.cs = 0;
    conf.max_baudrate = 1000000;
    conf.polarity = 0;
    conf.phase = 0;

    spim = spim_open(&conf);
}

void SPIClass::beginTransaction(SPISettings s) {
    
    if(_interruptMask != 0) {
        uint32_t curr_intenVal = gpio_inten_get(ARCHI_GPIO_ADDR);
        uint32_t new_intenVal = curr_intenVal & ~(_interruptMask);

        gpio_inten_set(ARCHI_GPIO_ADDR, new_intenVal);
    }
    
    current.clock = s.clock;
    current.bitOrder = s.bitOrder;
    current.dataMode = s.dataMode;

    if(!spim) return;

    spim->bitOrder = s.bitOrder;

    spim->max_baudrate = s.clock;
    int div = spi_get_div(spim->max_baudrate);
    spim->div = div;

    spim->polarity = (s.dataMode & 0x02)? 1 : (s.dataMode & 0x03)? 1 : 0;
    spim->phase = (s.dataMode & 0x01)? 1 : (s.dataMode & 0x03)? 1 : 0;

    spim->cfg = SPI_CMD_CFG(div, spim->polarity, spim->phase);
}

void SPIClass::endTransaction(void) {
    if(_interruptMask != 0) {
        uint32_t current = gpio_inten_get(ARCHI_GPIO_ADDR);
        uint32_t restore_intenVal = current | _interruptMask;

        gpio_inten_set(ARCHI_GPIO_ADDR, restore_intenVal);
    }
}

void SPIClass::end(void) {
    if(spim) {
        spim_close(spim);
        spim = nullptr;
    }
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
    if(spim) {
        current.bitOrder = bitOrder;
        spim->bitOrder = bitOrder;
    }
}

void SPIClass::setClockDivider(uint8_t divider) {
    if(spim) {
        current.clock = divider;
        spim->max_baudrate = divider;
        int div = spi_get_div(spim->max_baudrate);
        spim->div = div;
        spim->cfg = SPI_CMD_CFG(div, spim->polarity, spim->phase);
    }
}

void SPIClass::setDataMode(uint8_t dataMode) {
    if(spim) {
        current.dataMode = dataMode;
        spim->polarity = (dataMode & 0x02)? 1 : (dataMode & 0x03)? 1 : 0;
        spim->phase = (dataMode & 0x01)? 1 : (dataMode & 0x03)? 1 : 0;
        int div = spi_get_div(spim->max_baudrate);
        spim->cfg = SPI_CMD_CFG(div, spim->polarity, spim->phase);
    }
}

byte SPIClass::transfer(byte val) {
    if (!spim) return -1;

    uint8_t tx_data = val;
    uint8_t rx_data;

    spim_transfer(spim, &tx_data, &rx_data, 8, SPIM_CS_KEEP);
    return rx_data;
}

uint16_t SPIClass::transfer16(uint16_t *val16) {
    return -1;
}

void SPIClass::transfer(byte *buffer, size_t size) {
    if (!spim) return;
    spim_transfer(spim, buffer, buffer, size*8, SPIM_CS_KEEP);
}

void SPIClass::usingInterrupt(int interruptNumber) {
    if(interruptNumber < 32) {
        _interruptMask |= (1 << interruptNumber);
    }
}