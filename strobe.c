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
 
// Start at default frequency, 50% duty
STROBESETTINGS s_settings = {DEFAULT_FREQ, DEFAULT_RPM, 50U};

/*
 * Private Function Declarations
 */

static const STROBESETTINGS * setNewFreq(MILLIHZ freq);

/*
 * Public Function Definitions
 */

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
const STROBESETTINGS * AlterFrequency(MILLIHZ change) { return setNewFreq(s_settings.frequency + change); }

const STROBESETTINGS * SetRPM(uint16_t new) { return setNewFreq(RPM_TO_MILLIHZ(new)); }
const STROBESETTINGS * AlterRPM(int16_t change) { return setNewFreq(RPM_TO_MILLIHZ(s_settings.rpm + change)); }

/* SetDuty
 :If in range, immediately sets a new strobe duty cycle
*/
const STROBESETTINGS * SetDuty(uint8_t duty)
{
	if ((duty <= 100) && (duty > 0)) { s_settings.duty = duty; }
	return &s_settings;
}

/* AlterDuty
 :Changes the current strobe duty cycle by specified amount
*/
const STROBESETTINGS * AlterDuty(int16_t duty)
{
	return SetDuty(s_settings.duty + duty);
}

/*
 * Private Function Definitions
 */

/* setNewFreq
 :If in range, immediately sets a new strobe frequency
*/
static const STROBESETTINGS * setNewFreq(MILLIHZ freq)
{
	if ((freq <= MAX_FREQ) && (freq >= MIN_FREQ))
	{
		s_settings.frequency = freq;
		s_settings.rpm = MILLIHZ_TO_RPM(freq);
	}
	return (const STROBESETTINGS *)&s_settings;
}
