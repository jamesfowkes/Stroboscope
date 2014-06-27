/*
 * Standard Library Includes
 */
 
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>

/*
 * Utility Library Includes
 */

#include "util_macros.h"
#include "util_units.h"

/*
 * AVR Library Includes
 */
 
#include "lib_io.h"
#include "lib_tmr8_tick.h"
#include "lib_tmr16_pwm.h"
#include "lib_encoder.h"

/* 
 * Test Harness Library Includes
 */

#include "test_harness.h"

/*
 * Local Application Includes
 */

#include "app_test_harness.h"

/*
 * Local Test Functions
 */

DECLARE_TEST_FUNCS(IncreaseEncoderByOneWithSwitchOpen);

/*
 * Local Test Variables
 */
 
static TMR16_PWM_DEBUG pwmDebug;

DECLARE_TEST_GROUP(ApplicationTests)
{
	TEST_STRUCT(IncreaseEncoderByOneWithSwitchOpen)
};
END_TEST_GROUP(ApplicationTests);

/*
**
*/

DECLARE_TEST_FUNC(IncreaseEncoderByOneWithSwitchOpen)
{

}

DECLARE_RESULT_FUNC(IncreaseEncoderByOneWithSwitchOpen)
{
	return false;
}

void DO_TEST_HARNESS_SETUP(void)
{
	setbuf(stdout, NULL);
}

void DO_TEST_HARNESS_RUNNING(void)
{
	TMR8_Tick_Kick(50);
}

void DO_TEST_HARNESS_TICK(void)
{
	if (!TestHarnessRunNext(&ApplicationTests))
	{
		printf("All tests ran.\n");
		exit(0);
	}
}

TMR16_PWM_DEBUG * TEST_HARNESS_TMR16_DEBUG_PTR(void)
{
	return &pwmDebug;
}
