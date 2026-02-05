#include <stdint.h>

#include "pulp.h"

#include "variables.h"

#include "advanced_io.h"
#include "digital_io.h"

static inline int digitalReadPulse (int pin) {
    return (hal_gpio_get_value() >> pin) & 0x01;
}

void noTone(uint8_t pin) {

}

unsigned long __pulseIn_c(int pin, int value) {
    return (unsigned long) (-1);
}

unsigned long __pulseIn_c_with_timeout(int pin, int value, unsigned long timeout) {
    return (unsigned long) (-1);
}

unsigned long __pulseInLong_c(int pin, int value) {
    return __pulseInLong_c_with_timeout(pin, value, 180000000UL);
}

unsigned long __pulseInLong_c_with_timeout(int pin, int value, unsigned long timeout) {

    timeout = (timeout > 180000000UL) ? 180000000UL : timeout;

    unsigned long startMicros = pos_tick_get_counter_us();

    while (digitalReadPulse(pin) == value) {
        if (pos_tick_get_counter_us() - startMicros > timeout) return 0;
    }

    while (digitalReadPulse(pin) != value) {
        if (pos_tick_get_counter_us() - startMicros > timeout) return 0;
    }

    unsigned long startPulse = pos_tick_get_counter_us();

    while (digitalReadPulse(pin) == value) {
        if (pos_tick_get_counter_us() - startMicros > timeout) return 0;
    }

    return pos_tick_get_counter_us() - startPulse;
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t val = 0;
    int i;

    for (i=0; i<8; i++) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST) {
            val |= digitalRead(dataPin) << i;
        } else {
            val |= digitalRead(dataPin) << (7-i);
        }
        digitalWrite(clockPin, LOW);
    }
    return val;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value) {
    int i;
    for (i=0; i<8; i++) {
        if(bitOrder == LSBFIRST) {
            digitalWrite(dataPin, !!(value & (1 << i)));
        } else {
            digitalWrite(dataPin, !!(value & (1 << (7 - i))));
        }
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

void tone(uint8_t pin, unsigned int frequency, unsigned long duration) {

}


