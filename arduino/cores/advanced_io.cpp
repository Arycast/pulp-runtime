/* only to provide polymorphysm */

#include "advanced_io.h"

unsigned long pulseIn(int pin, int value) {
    return __pulseIn_c(pin, value);
}

unsigned long pulseIn(int pin, int value, unsigned long timeout) {
    return __pulseIn_c_with_timeout(pin, value, timeout);
}

unsigned long pulseInLong(int pin, int value) {
    return __pulseInLong_c(pin, value);
}

unsigned long pulseInLong(int pin, int value, unsigned long timeout) {
    return __pulseInLong_c_with_timeout(pin, value, timeout);
}
