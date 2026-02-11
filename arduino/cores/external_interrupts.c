#include "variables.h"

#include "external_interrupts.h"

void attachInterrupt(int interrupt, void(*userFunc)(void), int mode){
    
    switch (mode) {
        case LOW:   
        /**
         * to trigger the interrupt whenever the pin is low,
         */
            break;

        case CHANGE:
        /**
         * to trigger the interrupt whenever the pin changes value
         */
            break;

        case RISING:
        /**
         * to trigger when the pin goes from low to high
         */

            break;

        case FALLING:
        /**
         * for when the pin goes from high to low 
         */
            break;
            
        default:
            return;
    }

}

void detachInterrupt(int interrupt){
    /* the function returns nothing.*/
}

int digitalPinToInterrupt(int pin){
    if (1){ /* dummy */
        /* If the pin is available for interrupt, it will return the given pin */
        return pin;
    }
    else{
        /* if the pin is not available for interrupt, it will return -1 */
        return -1;
    }
}