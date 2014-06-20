/*
 * app.c
 *
 *  Application file for stroboscope
 *
 *  Created on: 20 June 2014
 *      Author: james
 *
 *		Target: ATMEGA328P
 */

/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/*
 * Utility Includes
 */

#include "util_macros.h"

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

/*
 * Device Includes
 */

// None

/*
 * Generic Library Includes
 */

//None

/*
 * AVR Library Includes
 */

//None

/*
 * Local Application Includes
 */

#include "strobe.h"

/*
 * Private Defines and Datatypes
 */

 //None
 
/*
 * Function Prototypes
 */

static void setupTimer(void);
static void setupIO(void);

static void applicationTick(void);

/*
 * Private Variables
 */

static TMR8_TICK_CONFIG appTick;
static TMR8_TICK_CONFIG heartbeatTick;

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	setupTimer();

	setupIO();

	/* All processing interrupt based from here*/

	sei();

	while (true)
	{
		if (TMR8_Tick_TestAndClear(&appTick))
		{
			applicationTick();
		}

		if (TMR8_Tick_TestAndClear(&heartbeatTick))
		{
			s_BlinkState = !s_BlinkState;
		}
	}

	return 0;
}

/*
 * Private Functions
 */
static void setupIO(void)
{
}

static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(2, 0);
}

static void applicationTick(void)
{
}
