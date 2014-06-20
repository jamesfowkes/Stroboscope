/*
 * Standard Library Includes
 */
#include <stdint.h>

/*
 * Utility Library Includes
 */

#include "util_macros.h"


/*
 * Application Includes
 */

#include "strobe.h"

/*
 * Defines and Typedefs
 */
#define MAX_FREQ 5000U
#define MIN_FREQ 5U

/*
 * Local Variables
 */
 
// Start at minimum frequency, 50% duty
STROBESETTINGS s_settings = {MIN_FREQ, 50U};

/*
 * Private Function Declarations
 */

static const STROBESETTINGS * setNewFreq(uint16_t freq);

/*
 * Public Function Definitions
 */

 /* GetDuty, GetFrequency
 :Accessor functions for duty and frequency
*/
uint8_t GetDuty(void) { return s_settings.duty; }
uint16_t GetFrequency(void) { return s_settings.frequency; }

/* HalfFrequency, ThirdFrequency, DoubleFrequency, TrebleFrequency, SetFrequency, AlterFrequency
 :All these functions pass a frequency to the private set function and return a pointer to the private data.
*/

const STROBESETTINGS * HalfFrequency(void) { return setNewFreq( div_round_pos(s_settings.frequency, 2) ); }
const STROBESETTINGS * ThirdFrequency(void) { return setNewFreq( div_round_pos(s_settings.frequency, 3) ); }
const STROBESETTINGS * DoubleFrequency(void) { return setNewFreq(s_settings.frequency * 2); }
const STROBESETTINGS * TrebleFrequency(void) { return setNewFreq(s_settings.frequency * 3); }
const STROBESETTINGS * SetFrequency(uint16_t new) { return setNewFreq(new); }
const STROBESETTINGS * AlterFrequency(int16_t change) { return setNewFreq(s_settings.frequency + change); }

/* SetDuty
 :If in range, immediately sets a new strobe duty cycle
*/
const STROBESETTINGS * SetDuty(uint8_t duty)
{
	if ((duty <= 100) && (duty > 0)) { s_settings.duty = duty; }
	return &s_settings;
}

/*
 * Private Function Definitions
 */

/* setNewFreq
 :If in range, immediately sets a new strobe frequency
*/
static const STROBESETTINGS * setNewFreq(uint16_t freq)
{
	if ((freq <= MAX_FREQ) && (freq >= MIN_FREQ))
	{
		s_settings.frequency = freq;
	}
	return (const STROBESETTINGS *)&s_settings;
}
