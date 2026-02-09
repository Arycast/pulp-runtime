/**
    * this file is implementation of Arduino Time API
    * https://docs.arduino.cc/language-reference/en/functions/time/
*/

#ifndef arduino_time_h
#define arduino_time_h 

/**
    * method delay
    * pauses the program for the amount of time (in milliseconds)
    * @param ms The number of milliseconds to pause
    * https://docs.arduino.cc/language-reference/en/functions/time/delay/
*/
void delay(unsigned long ms);

/**
    * method delayMicroseconds
    * pauses the program for the amount of time (in microseconds)
    * @param us The number of microseconds to pause
    * https://docs.arduino.cc/language-reference/en/functions/time/delayMicroseconds/
*/
void delayMicroseconds(unsigned int us);

/**
    * method millis
    * method to get the number of milliseconds passed since the board began running the current program
    * @return The number of milliseconds passed since the program started
    * https://docs.arduino.cc/language-reference/en/functions/time/millis/
*/
unsigned long millis(void);

/**
    * method micros
    * method to get the number of microseconds passed since the board began running the current program
    * @return The number of microseconds passed since the program started
    * https://docs.arduino.cc/language-reference/en/functions/time/micros/
*/
unsigned long micros(void);

#endif