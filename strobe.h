#ifndef _STROBE_H_
#define _STROBE_H_

/*
 * Defines and Typedefs
 */
 
struct strobesettings
{
	uint16_t frequency;
	uint8_t duty;
};
typedef struct strobesettings STROBESETTINGS;

/*
 * Public Function Declarations
 */

uint8_t GetDuty(void);
uint16_t GetFrequency(void);

const STROBESETTINGS * HalfFrequency(void);
const STROBESETTINGS * ThirdFrequency(void);
const STROBESETTINGS * DoubleFrequency(void);
const STROBESETTINGS * TrebleFrequency(void);

const STROBESETTINGS * SetFrequency(uint16_t freq);
const STROBESETTINGS * AlterFrequency(int16_t change);

const STROBESETTINGS * SetDuty(uint8_t duty);

#endif
