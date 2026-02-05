#include "randomNumbers.h"

/* stop define random to __random_c, so we can use stdlib's random */
#undef random

/* use stdlib random function */
#include <stdlib.h>

long __random_c(long max) {
    if (max == 0) {
        return 0;
    }
    return random() % max;
}

long __random_c_with_range(long min, long max) {
    if (min >= max) {
        return min;
    }
    long diff = max - min;
    return __random_c(diff) + min;
}

void randomSeed(unsigned long seed) {
    srandom(seed);
}
