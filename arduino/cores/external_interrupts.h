/**
 * function attachInterrupts
 * Interrupts help make things happen automatically in microcontroller programs 
 * and can help solve timing problems. Good tasks for using an interrupt may 
 * include reading a rotary encoder, or monitoring user input. If you wanted to 
 * ensure that a program always caught the pulses from a rotary encoder, so that 
 * it never misses a pulse, it would make it very tricky to write a program to do 
 * anything else, because the program would need to constantly poll the sensor 
 * lines for the encoder, to catch pulses when they occurred. Other sensors have 
 * a similar interface dynamic, such as trying to read a sound sensor that 
 * detects a click, or an infrared slot sensor (photo-interrupter) that detects 
 * a coin drop. In all these situations, using an interrupt can free the 
 * microcontroller to do other work while still capturing the input.
 * 
 * https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
 */
void attachInterrupt(int interrupt, void(*userFunc)(void), int mode);

/**
 * function detachInterrupt
 * This function turns off a given interrupt that was attached previously.
 *
 * https://docs.arduino.cc/language-reference/en/functions/external-interrupts/detachInterrupt/
 */
void detachInterrupt(int interrupt);

/**
 * function digitalPinToInterrupt
 * The digitalPinToInterrupt() function takes a pin as an argument, and returns
 * the same pin if it can be used as an interrupt.
 * 
 * https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/
 */
int digitalPinToInterrupt(int pin);