#ifndef _STROBE_H_
#define _STROBE_H_

/*
 * Defines and Typedefs
 */

// Top of range is defined by max rpm
#define MAX_RPM (10000U)

// Bottom of range is defined by min frequency
#define MIN_FREQ (MILLIHZ)(2000)

#define DEFAULT_RPM (1000U)

struct strobesettings
{
	MILLIHZ frequency;
	uint16_t rpm;
	uint8_t duty;
};
typedef struct strobesettings STROBESETTINGS;

/*
 * Public Function Declarations
 */

const STROBESETTINGS * Strobe_Init(void);

uint8_t GetDuty(void);
MILLIHZ GetFrequency(void);
uint16_t GetRPM(void);

const STROBESETTINGS * GetSettings(void);

const STROBESETTINGS * HalfFrequency(void);
const STROBESETTINGS * ThirdFrequency(void);
const STROBESETTINGS * DoubleFrequency(void);
const STROBESETTINGS * TrebleFrequency(void);

const STROBESETTINGS * SetFrequency(MILLIHZ freq);
const STROBESETTINGS * AlterFrequency(int16_t change);

const STROBESETTINGS * SetRPM(uint16_t rpm);
const STROBESETTINGS * AlterRPM(int16_t change);

const STROBESETTINGS * SetDuty(uint8_t duty);
const STROBESETTINGS * AlterDuty(int16_t change);

#endif
