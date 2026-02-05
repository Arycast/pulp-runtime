/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __AARDUINO_H__
#define __AARDUINO_H__

/**
	* basic type and constants
	*/
#include "variables.h"

/* string class */
#ifdef __clpusplus
#include "String.hpp"
#endif


/**
	* structure (declaration of loop and setup)
	*/
#include "structure.h"

/**
	* pulpissimo port of avr-libc's pgmspace
	*/
#include "pgmspace.h"


/**
	* API
	*/

/* Digital I/O */
#include "digital_io.h"

/* Math */
#include "arduino_math.h"

/* Bits and Bytes */
#include "bitBytes.h"

/* Analog I/O */
#include "analog_io.h"

/* Trigonometry */
#include "trigonometry.h"

/* External Interrupts */
#include "external_interrupts.h"

/* Advanced I/O */
#include "advanced_io.h"

/* Characters */
#include "characters.h"

/* Interrupts */
#include "arduino_interrupts.h"

/* Time */
#include "time.h"

/* Random Numbers */
#include "randomNumbers.h"

/* Communication */
#include "SPI.h"         /* SPI (libraries/SPI) */
#include "Print.h"       /* Print */
#include "serial.h"      /* Serial */
#include "stream.h"      /* Stream */
#include "wire.h"        /* Wire (libraries/Wire) */

/* USB */
/*#include "Keyboard.h"*/    /* Keyboard (libraries/Keyboard) */
/*#include "Mouse.h"*/       /* Mouse (libraries/Mouse) */

/* Wi-Fi */
#include "IPAddress.h"     /* IPAddress */
#include "wifi_network.h"  /* WiFi Network */
#include "wifi_client.h"   /* WiFiClient */
#include "wifi_server.h"   /* WiFiServer */
#include "WiFiUDP.h"       /* WiFiUDP (libraries/WiFiUDP) */

#endif /* ! defined(__AARDUINO_H__) */
