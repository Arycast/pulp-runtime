#include "external_interrupts.h"

void attachInterrupt(int interrupt, void(*userFunc)(void), int mode){
    
    switch (mode) {
        case LOW:   
            break;
        case CHANGE:
            break;
        case RISING:
            break;
        case FALLING:
            break;
        default:
            return;
    }

}

void detachInterrupt(int interrupt){

}

int digitalPinToInterrupt(int pin){
    if (){
        return pin;
    }
    else{
        return -1;
    }
}