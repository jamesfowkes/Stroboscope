#ifndef _STROBE_H_
#define _STROBE_H_

/*
 * Defines and Typedefs
 */

typedef uint32_t MILLIHERTZ;

#define MILLIHZ_TO_RPM(f) ((((f) * 60U) + 500U) / 1000U)
#define RPM_TO_MILLIHZ(rpm) ((((rpm) * 1000U) + 30U) / 60U)

// Top of range is defined by max rpm
#define MAX_RPM (10000U)
#define MAX_FREQ RPM_TO_MILLIHZ(MAX_RPM)

// Bottom of range is defined by min frequency
#define MIN_FREQ (2000U)
#define MIN_RPM MILLIHZ_TO_RPM(MIN_FREQ)

#define DEFAULT_RPM (1000U)
#define DEFAULT_FREQ  RPM_TO_MILLIHZ(DEFAULT_RPM)

struct strobesettings
{
	MILLIHERTZ frequency;
	uint16_t rpm;
	uint8_t duty;
};
typedef struct strobesettings STROBESETTINGS;

/*
 * Public Function Declarations
 */

uint8_t GetDuty(void);
MILLIHERTZ GetFrequency(void);
uint16_t GetRPM(void);

const STROBESETTINGS * HalfFrequency(void);
const STROBESETTINGS * ThirdFrequency(void);
const STROBESETTINGS * DoubleFrequency(void);
const STROBESETTINGS * TrebleFrequency(void);

const STROBESETTINGS * SetFrequency(MILLIHERTZ freq);
const STROBESETTINGS * AlterFrequency(MILLIHERTZ change);

const STROBESETTINGS * SetRPM(uint16_t rpm);
const STROBESETTINGS * AlterRPM(int16_t change);

const STROBESETTINGS * SetDuty(uint8_t duty);
const STROBESETTINGS * AlterDuty(int16_t change);

#endif
