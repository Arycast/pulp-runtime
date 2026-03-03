#include "pulp.h"
#include "variables.h"
#include "digital_io.h"

int digitalRead(int pin){
	/* switch pad function from default to GPIO mode */
    setPadmuxToGPIO(digitalPinToPad(pin));

	/* read the current state of all GPIO pins from hardware register*/
	uint32_t padin = hal_gpio_get_value();

	/* mask the register value to isolate the specific bit for this pin*/
	int val = padin & (1 << pin);

	/* return standard Arduino HIGH (1) or LOW (0) based on bit state*/
	return val ? HIGH : LOW;
}

void digitalWrite(int pin, int value){
	setPadmuxToGPIO(digitalPinToPad(pin));

	/* apply the logic level to the hardware output register*/
	if (value == LOW) {
		hal_gpio_set_pin_value(pin, 0);
	} else {
		hal_gpio_set_pin_value(pin, 1);
	}
}

void pinMode(int pin, uint8_t mode){
    uint32_t pad = digitalPinToPad(pin);
	if (pad == NOT_A_PIN) return;

	/* connect physical pad to GPIO controller*/
	setPadmuxToGPIO(pad);

	/* enable the specific GPIO pin*/
	hal_gpio_en_set(1<<pin);

	/* set data direction: 1 for OUTPUT, 0 for INPUT*/
	if (mode == OUTPUT) {
		hal_gpio_set_dir(1<<pin, 1);
	} else {
		hal_gpio_set_dir(1<<pin, 0);
	}
}
