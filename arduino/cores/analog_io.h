#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 

/**
 * function analogRead
 * Reads the value from a specified analog input pin.
 * 
 * https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/
 */
int analogRead(int pin);

/**
 * function analogReadResolution
 * analogReadResolution()is an extension of the Analog API that let you set the 
 * resolution (in bits) of the value returned by analogRead().
 *
 * https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReadResolution/
 */
void analogReadResolution(int bits);

/**
 * function analogReference
 * Configures the reference voltage used for analog input (i.e. the maximum voltage 
 * that can be sensed). 
 *
 * https://docs.arduino.cc/language-reference/en/functions/analog-io/analogReference 
 */
void analogReference(uint8_t type);

/**
 * function analogWrite
 * Writes an analog value (PWM wave) to a pin. Can be used to light a LED at 
 * varying brightness or drive a motor at various speeds. After a call to 
 * analogWrite(), the pin will generate a steady rectangular wave of the 
 * specified duty cycle until the next call to analogWrite() (or a call to 
 * digitalRead() or digitalWrite()) on the same pin.
 *
 * https://docs.arduino.cc/language-reference/en/functions/analog-io/analogWrite/
 */
void analogWrite(int pin, int value);

/**
 * function analogWriteResolution
 * analogWriteResolution() is an extension of the Analog API to set the resolution 
 * of the analogWrite() function.
 *
 * https://docs.arduino.cc/language-reference/en/functions/analog-io/analogWriteResolution/
 */
void analogWriteResolution(int bits);

#ifdef __cplusplus
}
#endif