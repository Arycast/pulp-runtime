/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

/* platform-dependent definition of board */

/* use shared define between boards,
* so only 1 header will be used between all boards */
#ifndef __BOARD_PLATFORM_BOARD_H__
#define __BOARD_PLATFORM_BOARD_H__

/* board macro should be defined */
#ifndef ARDUINO_SOFT_PULPISSIMO_NUSACORE
#error  macro ARDUINO_SOFT_PULPISSIMO_NUSACORE is not defined, \
	it seems that this file is included in compilation unit \
	that is being built without arduino or \
	misconfiguration in platform.txt or board.txt or \
	this file is included in wrong board configuration
#endif

/**
	* module port mapping
	*
	* PCB        FPGA     TOP LEVEL RTL
	* LED_EXT_0  AB14     led0_o
	* LED_EXT_1  AA13     led1_o
	* LED_EXT_2  AB13     led2_o
	* LED_EXT_3  AB4      led3_o
	* LED_EXT_4  M19      pad_pmod1_0
	* LED_EXT_5  B5       -
	* LED_EXT_6  A5       -
	* LED_EXT_7  B7       -
	*
	* SW_EXT_6   V13      btn0_i
	* SW_EXT_2   U14      btn1_i
	* SW_EXT_3   AA18     btn2_i
	* SW_EXT_7   U12      btn3_i
	* SW_EXT_4   W19      pad_pmod1_4
	* SW_EXT_0   V8       pad_pmod1_5
	* SW_EXT_5   R5       pad_pmod1_6
	* SW_EXT_1   N19      pad_pmod1_7
	*/

/* available LED definion for soft-pulpissimo NUSACORE */

/* LED_EXT_0 -> D48 in schematic, label "1" on PCB */
#define BOARD_LED1_PAD_MUX_INDEX         5
#define BOARD_LED1_PAD_MUX_VALUE         2
#define BOARD_LED1_GPIO_INDEX            5
#define LED1 BOARD_LED1_GPIO_INDEX

/* LED_EXT_1 -> D49 in schematic, label "2" on PCB */
#define BOARD_LED2_PAD_MUX_INDEX         9
#define BOARD_LED2_PAD_MUX_VALUE         2
#define BOARD_LED2_GPIO_INDEX            9
#define LED2 BOARD_LED2_GPIO_INDEX

/* LED_EXT_2 -> D50 in schematic, label "3" on PCB */
#define BOARD_LED3_PAD_MUX_INDEX        10
#define BOARD_LED3_PAD_MUX_VALUE         2
#define BOARD_LED3_GPIO_INDEX           10
#define LED3 BOARD_LED3_GPIO_INDEX

/* LED_EXT_3 -> D56 in schematic, label "4" on PCB */
#define BOARD_LED4_PAD_MUX_INDEX        11
#define BOARD_LED4_PAD_MUX_VALUE         2
#define BOARD_LED4_GPIO_INDEX           11
#define LED4 BOARD_LED4_GPIO_INDEX

/* LED_EXT_4 -> D57 in schematic, label "5" on PCB */
#define BOARD_LED5_PAD_MUX_INDEX        22
#define BOARD_LED5_PAD_MUX_VALUE         2
#define BOARD_LED5_GPIO_INDEX           22
#define LED5 BOARD_LED5_GPIO_INDEX

/* LED_EXT_5 -> D58 in schematic, label "6" on PCB */
/*#define BOARD_LED6_PAD_MUX_INDEX
#define BOARD_LED6_PAD_MUX_VALUE
#define BOARD_LED6_GPIO_INDEX
#define LED6 BOARD_LED6_GPIO_INDEX*/

/* LED_EXT_6 -> D59 in schematic, label "7" on PCB */
/*#define BOARD_LED7_PAD_MUX_INDEX
#define BOARD_LED7_PAD_MUX_VALUE
#define BOARD_LED7_GPIO_INDEX
#define LED7 BOARD_LED7_GPIO_INDEX*/

/* LED_EXT_7 -> D60 in schematic, label "8" on PCB */
/*#define BOARD_LED8_PAD_MUX_INDEX
#define BOARD_LED8_PAD_MUX_VALUE
#define BOARD_LED8_GPIO_INDEX
#define LED8 BOARD_LED8_GPIO_INDEX*/


/* definion for led BOARD_LED_BUILTIN */
#ifndef BOARD_LED_BUILTIN
#define BOARD_LED_BUILTIN   BOARD_LED1_GPIO_INDEX
#endif /* ! defined(BOARD_LED_BUILTIN) */

/* available BUTTON definion for soft-pulpissimo NUSACORE */

/* S16 -> SW_EXT_6 in schematic, label "PB1" on pcb */
#define BOARD_BTN1_PAD_MUX_INDEX		14
#define BOARD_BTN1_PAD_MUX_VALUE		 2
#define BOARD_BTN1_GPIO_INDEX			14
#define BTN1 BOARD_BTN1_GPIO_INDEX

/* S15 -> SW_EXT_2 in schematic, label "PB2" on pcb */
#define BOARD_BTN2_PAD_MUX_INDEX		15
#define BOARD_BTN2_PAD_MUX_VALUE		 2
#define BOARD_BTN2_GPIO_INDEX			15
#define BTN2 BOARD_BTN2_GPIO_INDEX

/* S17 -> SW_EXT_3 in schematic, label "PB3" on pcb */
#define BOARD_BTN3_PAD_MUX_INDEX		16
#define BOARD_BTN3_PAD_MUX_VALUE		 2
#define BOARD_BTN3_GPIO_INDEX			16
#define BTN3 BOARD_BTN3_GPIO_INDEX

/* S18 -> SW_EXT_7 in schematic, label "PB4" on pcb */
#define BOARD_BTN4_PAD_MUX_INDEX		17
#define BOARD_BTN4_PAD_MUX_VALUE		 2
#define BOARD_BTN4_GPIO_INDEX			17
#define BTN4 BOARD_BTN4_GPIO_INDEX

/* S12 -> SW_EXT_4 in schematic, label "PB5" on pcb */
#define BOARD_BTN5_PAD_MUX_INDEX		35
#define BOARD_BTN5_PAD_MUX_VALUE		 2
#define BOARD_BTN5_GPIO_INDEX			35
#define BTN5 BOARD_BTN5_GPIO_INDEX

/* S11 -> SW_EXT_0 in schematic, label "PB6" on pcb */
#define BOARD_BTN6_PAD_MUX_INDEX		36
#define BOARD_BTN6_PAD_MUX_VALUE		 2
#define BOARD_BTN6_GPIO_INDEX			36
#define BTN6 BOARD_BTN6_GPIO_INDEX

/* S14 -> SW_EXT_5 in schematic, label "PB7" on pcb */
#define BOARD_BTN7_PAD_MUX_INDEX		37
#define BOARD_BTN7_PAD_MUX_VALUE		 2
#define BOARD_BTN7_GPIO_INDEX			37
#define BTN7 BOARD_BTN7_GPIO_INDEX

/* S13 -> SW_EXT_1 in schematic, label "PB8" on pcb */
#define BOARD_BTN8_PAD_MUX_INDEX		38
#define BOARD_BTN8_PAD_MUX_VALUE		 2
#define BOARD_BTN8_GPIO_INDEX			38
#define BTN8 BOARD_BTN8_GPIO_INDEX

#ifdef __cplusplus
extern "C"
{
#endif

int platform_board_analog_read(size_t pin, uint32_t *value);
int platform_board_analog_write(size_t pin, uint32_t value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ! defined(__BOARD_PLATFORM_BOARD_H__) */
