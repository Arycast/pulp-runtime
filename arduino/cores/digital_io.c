#include "pulp.h"
#include "variables.h"
#include "digital_io.h"

/* Validates pin range and returns the mapped physical pad index*/
static inline uint32_t digitalPinToPad(uint8_t pin){

	/* Ensure the pin index doesn't exceed the mapping table's size*/
    if (pin >= (sizeof(digital_pin_to_pad) / sizeof(digital_pin_to_pad[0])))
        return NOT_A_PIN;
    return digital_pin_to_pad[pin];
}

static inline void setPadmuxToGPIO(uint32_t pad_index) {
    uint32_t reg_addr;

	/* Calculate bit position: each pad uses 2 bits, so we shift by (index % 16) * 2 */
    uint32_t shift = (pad_index % 16) * 2;

	/* Select the appropriate register: PADMUX_0 for pads 0-15, PADMUX_1 for 16-31 */
    if (pad_index < 16) {
		reg_addr = PADMUX_0;
	}	else {
			reg_addr = PADMUX_1;
		}
    
	/* Read the current register value from hardware*/
    uint32_t reg_val = pulp_read32(reg_addr);

    /*reg_val &= ~(0b11 << shift);
    reg_val |= (0b01 << shift);*/
    reg_val &= ~(0x3 << shift);
    reg_val |=  (0x1 << shift);

	/* Write the modified configuration back to the hardware register*/
    pulp_write32(reg_addr, reg_val);
}

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
