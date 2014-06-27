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
#include <stdio.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

/*
 * Utility Library Includes
 */

#include "util_macros.h"
#include "util_units.h"

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
#include "lib_io.h"
#include "lib_tmr8_tick.h"
#include "lib_tmr16_pwm.h"
#include "lib_encoder.h"

/*
 * Local Application Includes
 */

#include "ui.h"
#include "strobe.h"
#include "app.h"
#include "app_test_harness.h"

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
static inline void SetStrobe(void);

/*
 * Private Variables
 */

static TMR8_TICK_CONFIG appTick;
static TMR8_TICK_CONFIG heartbeatTick;

static bool s_bSettingsChanged = false;
static STROBESETTINGS const * s_settings;

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	setupTimer();
	setupIO();
	
	UI_Init(APP_TICK_MS);

	/* All processing interrupt based from here*/

	DO_TEST_HARNESS_SETUP();
	
	sei();

	while (true)
	{
		DO_TEST_HARNESS_RUNNING();
		
		UI_Tick();
		
		if (TMR8_Tick_TestAndClear(&appTick))
		{
			applicationTick();
		}

		if (TMR8_Tick_TestAndClear(&heartbeatTick))
		{
			DO_TEST_HARNESS_TICK();
		}

	}

	return 0;
}

/* APP_HalfFreq, APP_ThirdFreq, APP_DoubleFreq, APP_TrebleFreq
 : Callbacks from the ui module for the half, third, double, treble buttons
 : Redirects straight to strobe module
*/
void APP_HalfFreq() { s_settings = HalfFrequency(); s_bSettingsChanged = true;}
void APP_ThirdFreq() { s_settings = ThirdFrequency(); s_bSettingsChanged = true;}
void APP_DoubleFreq() { s_settings = DoubleFrequency(); s_bSettingsChanged = true;}
void APP_TrebleFreq() { s_settings = TrebleFrequency(); s_bSettingsChanged = true;}

/*
 * Private Functions
 */
 
/* setupTimer, setupIO
 : Once-only setup functions
*/
static void setupIO(void)
{
	// TODO: Set output compare A to output 
	IO_SetMode(IO_PORTB, 5, IO_MODE_OUTPUT);
}

static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(3, 0);
	
	appTick.reload = APP_TICK_MS;
	appTick.active = true;
	TMR8_Tick_AddTimerConfig(&appTick);

	heartbeatTick.reload = BLINK_TICK_MS;
	heartbeatTick.active = true;
	TMR8_Tick_AddTimerConfig(&heartbeatTick);
	
	s_settings = GetSettings();
	SetStrobe();
}

/* applicationTick
 : Handles change of encoder by polling and sending to either 
 : UI (for display) or strobe (for parameter settings)
*/
static void applicationTick(void)
{
	int encoderChange = ENC_GetMovement();
	
	if (!UI_EncoderButtonIsPressed())
	{
		// Encoder is controlling rpm/frequency or duty
		handleEncoderChange(encoderChange);
	}
	else
	{
		// Encoder is moving cursor on display
		UI_HandleEncoderChange(encoderChange);
	}
	
	if (s_bSettingsChanged)
	{
		s_bSettingsChanged = false;
		UI_UpdateDisplay((s_settings->frequency + 5) / 10, s_settings->duty, s_settings->rpm);
		SetStrobe();
	}
}

/* handleEncoderChange
 : Converts encoder movement to change in strobe paramter value.
 : Passes that change in value to the strobe module.
 : The change of duty/frequency/rpm is the place value of the digit multipled by the encoder movement.
*/
static void handleEncoderChange(int16_t change)
{
	uint16_t multipliers[] = {1, 10, 100, 1000};
	
	change *= multipliers[ UI_SelectedDigit() ];
	
	switch (UI_SelectedLine())
	{
	case RPM:
		s_settings = AlterRPM(change); // Each encoder tick is change of 1RPM
		break;
	case FREQ:
		s_settings = AlterFrequency(change * 10); // Each encoder tick is change of 10mHz
		break;
	case DUTY:
		s_settings = AlterDuty(change); // Each encoder tick is change of 1%
		break;
	}
	
	s_bSettingsChanged = true;
}

static inline void SetStrobe(void)
{
	#ifndef TEST_HARNESS
	TMR16_PWM_Set(s_settings->frequency, s_settings->duty, TMR_OCCHAN_A);
	#else
	TMR16_PWM_Set(s_settings->frequency, s_settings->duty, TMR_OCCHAN_A, TEST_HARNESS_TMR16_DEBUG_PTR());
	#endif
}