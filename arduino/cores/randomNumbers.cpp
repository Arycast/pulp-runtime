/* only to provide polymorphysm */

#include "randomNumbers.h"

long random(long max) {
    return __random_c(max);
}

long random(long min, long max) {
    return __random_c_with_range(min, max);
}
