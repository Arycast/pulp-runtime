#include <stdint.h>

#include "variables.h"
#include "external_interrupts.h"
#include "pulp.h"
#include "digital_io.h"

#ifdef __cplusplus
extern "C" {
#endif

/* External assembly wrapper (protects CPU registers)*/
extern void global_gpio_handler(void);

/* Array to store user-defined ISR callbacks*/ 
void (*gpio_callbacks[32])(void) = {0};

/* Flag to ensure the hardware IRQ line is registered only once*/ 
static int is_handler_registered = 0;

/* Main C dispacther called by the assembly wrapper*/
void global_gpio_handler_c() {

    /* Read the interrupt status register to get all pending GPIO interrupts*/
    uint32_t status = INTSTATUS_REG;

    /* Identifies which pin triggered the interrupt and executes the corresponding callback*/
    while (status != 0) {
        volatile uint8_t pin = __builtin_ctz(status);
        if (pin < 32 && gpio_callbacks[pin] != 0) {
            gpio_callbacks[pin](); 
        }
        
        /* Clear the processed pins's bit to evaluate the next pending interrupt*/
        status &= ~(1UL << pin);
    }

    /* Hardware safeguards: Dummy read to clear mechanical bouncing*/
    volatile uint32_t dummy_clear = INTSTATUS_REG; 
    (void)dummy_clear;

    /* Hardware safeguards: Clear pending flag at the main Interrupr Controller (ITC) level*/
    ITC_INT_CLEAR_REG = (1UL << 15);
}

void attachInterrupt(uint8_t pin, void (*isr)(void), uint8_t mode) {
        if (pin >= 32) return;

        /* Set the pad multiplexer to route the physical pin to the GPIO peripheral*/
        setPadmuxToGPIO(digitalPinToPad(pin));

        /* Register the dispacther to the SoC's Event Controller once*/
        if (is_handler_registered == 0) {
            rt_irq_set_handler(15, &global_gpio_handler); 
            hal_itc_enable_set(hal_itc_enable_value_get() | (1 << 15));
            is_handler_registered = 1;
        }    

        /* Store the user callback in the array*/ 
        gpio_callbacks[pin] = isr;

        /* Configure the GPIO pin direction as INPUT*/ 
        PADDIR_REG &= ~(1 << pin);

        /* Interrupt modes mapping
        PULPissimo is stricly edge-triggered so LOW (level-triggered) is not supported */
        uint32_t inttype_val = 0;
        if (mode == LOW) {
            INTEN_REG &= ~(1 << pin);
            gpio_callbacks[pin] = 0;
            return;
        } else if (mode == RISING) {
            inttype_val = 0x01; 
        } else if (mode == FALLING) {
            inttype_val = 0x00;
        } else if (mode == CHANGE) {
            inttype_val = 0x02;
        } else {
            INTEN_REG &= ~(1 << pin);
            gpio_callbacks[pin] = 0;
            return;
        }

        /* Apply the 2-bit trigger mode configuration to the correct INTTYPE register*/ 
        if (pin < 16) {
            uint8_t shift = pin * 2;
            INTTYPE_0_15 &= ~(0x3 << shift);            
            INTTYPE_0_15 |= (inttype_val << shift);     
        } else {
            uint8_t shift = (pin - 16) * 2;
            INTTYPE_16_31 &= ~(0x3 << shift);
            INTTYPE_16_31 |= (inttype_val << shift);
        }

        /* Enable the interrupt capability for this specific pin*/ 
        INTEN_REG |= (1 << pin);
}

#ifdef __cplusplus
}
#endif

void detachInterrupt(int interrupt){
    /* Boundary check for valid GPIO pins*/
    if (interrupt < 0 || interrupt >= 32) {
        return;
    }

    /* Mask the interrupt for the specific pin */
    INTEN_REG &= ~(1 << interrupt);

    /* Clear the register ISR callback */
    gpio_callbacks[interrupt] = 0;

    /* Reset the trigger type configuration */
    if (interrupt < 16) {
        uint8_t shift = interrupt * 2;
        INTTYPE_0_15 &= ~(0x3 << shift);
    } else {
        uint8_t shift = (interrupt - 16) * 2;
        INTTYPE_16_31 &= ~(0x3 << shift);
    }

    /* Disable main GPIO IRQ handler if no pins are active */
    if (INTEN_REG == 0) {
        uint32_t current_itc_mask = hal_itc_enable_value_get();
        hal_itc_enable_set(current_itc_mask & ~(1UL << 15));
        
        is_handler_registered = 0;
    }
}

int digitalPinToInterrupt(int pin){
    /* Maximum number of digital pins mapped for the PYNQ-Z1 board implementation is 32*/
    if (pin >= 0 && pin < 32){
        return pin;
    }
    else{
        return -1;
    }
}