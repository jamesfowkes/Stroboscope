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

#include "button.h"

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_tmr8_tick.h"
#include "lib_io.h"
#include "lib_encoder.h"

/*
 * Local Application Includes
 */

#include "app.h"
#include "strobe.h"

/*
 * Private Defines and Datatypes
 */

#define APP_TICK_MS 10
#define BLINK_TICK_MS 500
 
/*
 * Function Prototypes
 */

static void setupTimer(void);
static void setupIO(void);

static void applicationTick(void);

static void handleEncoderChange(int16_t change);

/*
 * Private Variables
 */

static TMR8_TICK_CONFIG appTick;
static TMR8_TICK_CONFIG heartbeatTick;

static bool s_bFrequencySettingChanged = false;

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	setupTimer();
	setupIO();
	
	UI_Init(APP_TICK_MS);

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
		}
	}

	return 0;
}

/*
 * Private Functions
 */
static void setupIO(void)
{
	// TODO: Change to actual encoder inputs
	ENC_Setup(IO_PORTB, 0, 1);
}

static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(2, 0);
	
	appTick.reload = APP_TICK_MS;
	appTick.active = true;
	TMR8_Tick_AddTimerConfig(&appTick);

	heartbeatTick.reload = BLINK_TICK_MS;
	heartbeatTick.active = true;
	TMR8_Tick_AddTimerConfig(&heartbeatTick);
}

static void applicationTick(void)
{
	int encoderChange = ENC_GetMovement();
	
	if (!UI_EncoderButtonIsPressed())
	{
		// Encoder is controlling frequency or duty
		handleEncoderChange(encoderChange);
	}
	else
	{
		UI_HandleEncoderChange(encoderChange);
	}
	
	if (s_bFrequencySettingChanged)
	{
		//TODO: Update Display (do both freq and duty, simpler code that way)
	}
}

static void handleEncoderChange(int16_t change)
{
	(void)change;
	switch (UI_SelectedLine())
	{
	case FREQ:
		//TODO: handle change
		break;
	case DUTY:
		//TODO: handle change
		break;
	}
}

void APP_HalfFreq() { HalfFrequency(); }
void APP_ThirdFreq() { ThirdFrequency(); }
void APP_DoubleFreq() { DoubleFrequency(); }
void APP_TrebleFreq() { TrebleFrequency(); }
