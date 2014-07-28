/*
 * Standard Library Includes
 */
#include <stdint.h>

/*
 * Utility Library Includes
 */

#include "util_macros.h"
#include "util_units.h"

/*
 * Application Includes
 */

#include "strobe.h"

/*
 * Defines and Typedefs
 */

/*
 * Local Variables
 */
 

STROBESETTINGS s_settings;

static MILLIHZ s_maxFreq = 0U;
static uint16_t s_minRPM = 0U;

/*
 * Private Function Declarations
 */

static const STROBESETTINGS * setNewFreq(MILLIHZ freq);
static const STROBESETTINGS * setNewRpm(uint16_t rpm);

static MILLIHZ rpmToMilliHz(uint16_t rpm);
static uint16_t milliHzToRpm(MILLIHZ freq);

/*
 * Public Function Definitions
 */

/* Init
 :Set defaults
 */
const STROBESETTINGS * Strobe_Init(void)
{
	s_settings.frequency = rpmToMilliHz(DEFAULT_RPM);
	s_settings.rpm = DEFAULT_RPM;
	s_settings.duty = 50U;

	s_maxFreq = rpmToMilliHz(MAX_RPM);
	s_minRPM = milliHzToRpm(MIN_FREQ);

	return &s_settings;
}

/* GetDuty, GetFrequency, GetRPM
 :Accessor functions for current duty, frequency and RPM
*/
uint8_t GetDuty(void) { return s_settings.duty; }
MILLIHZ GetFrequency(void) { return s_settings.frequency; }
uint16_t GetRPM(void) { return s_settings.rpm; }

const STROBESETTINGS * GetSettings(void) { return &s_settings; }

/* HalfFrequency, ThirdFrequency, DoubleFrequency, TrebleFrequency, SetFrequency, AlterFrequency
 :All these functions pass a frequency to the private set function and return a pointer to the private data.
*/

const STROBESETTINGS * HalfFrequency(void) { return setNewFreq( div_round_pos(s_settings.frequency, 2) ); }
const STROBESETTINGS * ThirdFrequency(void) { return setNewFreq( div_round_pos(s_settings.frequency, 3) ); }
const STROBESETTINGS * DoubleFrequency(void) { return setNewFreq(s_settings.frequency * 2); }
const STROBESETTINGS * TrebleFrequency(void) { return setNewFreq(s_settings.frequency * 3); }

const STROBESETTINGS * SetFrequency(MILLIHZ new) { return setNewFreq(new); }
const STROBESETTINGS * AlterFrequency(int16_t change)
{
	MILLIHZ newFrequency = s_settings.frequency + change;
	newFrequency = min(newFrequency, s_maxFreq);
	newFrequency = max(newFrequency, MIN_FREQ);
	return setNewFreq(newFrequency);
}

const STROBESETTINGS * SetRPM(uint16_t new) { return setNewRpm(new); }
const STROBESETTINGS * AlterRPM(int16_t change) { return setNewRpm(s_settings.rpm + change); }

/* SetDuty
 :If in range, immediately sets a new strobe duty cycle
*/
const STROBESETTINGS * SetDuty(uint8_t duty)
{
	if ((duty <= 100) && (duty > 0))
	{
		s_settings.duty = duty;
	}
	else if (duty > 100)
	{
		s_settings.duty = 100;
	}
	else if (duty == 0)
	{
		s_settings.duty = 1;
	}
	return &s_settings;
}

/* AlterDuty
 :Changes the current strobe duty cycle by specified amount
*/
const STROBESETTINGS * AlterDuty(int16_t duty)
{
	int16_t newDuty = (int16_t)s_settings.duty + duty;
	newDuty = min(newDuty, 100);
	newDuty = max(newDuty, 1);
	return SetDuty((uint8_t)newDuty);
}

/*
 * Private Function Definitions
 */

/* setNewFreq
 :If in range, immediately sets a new strobe frequency
*/
static const STROBESETTINGS * setNewFreq(MILLIHZ freq)
{
	if ((freq <= s_maxFreq) && (freq >= MIN_FREQ))
	{
		s_settings.frequency = freq;
		s_settings.rpm = milliHzToRpm(freq);
	}
	return (const STROBESETTINGS *)&s_settings;
}

static const STROBESETTINGS * setNewRpm(uint16_t rpm)
{
	if ((rpm <= MAX_RPM) && (rpm >= s_minRPM))
	{
		s_settings.rpm = rpm;
		s_settings.frequency = rpmToMilliHz((MILLIHZ)rpm);
	}
	else if (rpm > MAX_RPM)
	{
		s_settings.rpm = MAX_RPM;
		s_settings.frequency = rpmToMilliHz((MILLIHZ)MAX_RPM);
	}
	else if (rpm < s_minRPM)
	{
		s_settings.rpm = s_minRPM;
		s_settings.frequency = rpmToMilliHz((MILLIHZ)s_minRPM);
	}

	return (const STROBESETTINGS *)&s_settings;
}

static MILLIHZ rpmToMilliHz(uint16_t rpm)
{
	MILLIHZ mHz = (MILLIHZ)(rpm);
	mHz *= 1000U;
	mHz= div_round_pos(mHz, 60U);
	return mHz;
}

static uint16_t milliHzToRpm(MILLIHZ freq)
{
	uint32_t rpm = freq * 60U;
	rpm = div_round_pos(rpm, 1000U);
	return (uint16_t)rpm;
}


