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
#include "ui.h"
#include "strobe.h"

/*
 * Local Test Functions
 */

DECLARE_TEST_FUNCS(ApplicationDefaultsAreSet);
DECLARE_TEST_FUNCS(IncreaseEncoderByOneWithSwitchOpen);
DECLARE_TEST_FUNCS(IncreaseEncoderByTenWithSwitchOpen);
DECLARE_TEST_FUNCS(IncreaseEncoderByOneWithSwitchClosed);
DECLARE_TEST_FUNCS(IncreaseEncoderByTenWithSwitchClosed);
DECLARE_TEST_FUNCS(DecreaseEncoderByFourWithSwitchClosed);
DECLARE_TEST_FUNCS(DecreaseEncoderBySevenWithSwitchOpen);
DECLARE_TEST_FUNCS(PressHalfButton);
DECLARE_TEST_FUNCS(PressDoubleButton);
DECLARE_TEST_FUNCS(PressThirdButton);
DECLARE_TEST_FUNCS(PressTrebleButton);

/*
 * Local Test Variables
 */
 
static TMR16_PWM_DEBUG pwmDebug;

DECLARE_TEST_GROUP(ApplicationTests)
{
	TEST_STRUCT(ApplicationDefaultsAreSet),
	TEST_STRUCT(IncreaseEncoderByOneWithSwitchOpen),
	TEST_STRUCT(IncreaseEncoderByTenWithSwitchOpen),
	TEST_STRUCT(IncreaseEncoderByOneWithSwitchClosed),
	TEST_STRUCT(IncreaseEncoderByTenWithSwitchClosed),
	TEST_STRUCT(DecreaseEncoderByFourWithSwitchClosed),
	TEST_STRUCT(DecreaseEncoderBySevenWithSwitchOpen),
	TEST_STRUCT(PressHalfButton),
	TEST_STRUCT(PressDoubleButton),
	TEST_STRUCT(PressThirdButton),
	TEST_STRUCT(PressTrebleButton)
};
END_TEST_GROUP(ApplicationTests);

/*
**
*/

DECLARE_TEST_FUNC(ApplicationDefaultsAreSet)
{
	UI_PressButton(HALF, false);
	UI_PressButton(DOUBLE, false);
	UI_PressButton(THIRD, false);
	UI_PressButton(TREBLE, false);
	UI_PressButton(ENC, false);
}

DECLARE_RESULT_FUNC(ApplicationDefaultsAreSet)
{
	TEST_ASSERT_EQUAL(UI_SelectedLine(), RPM );
	TEST_ASSERT_EQUAL(UI_SelectedDigit(), 0 );
	TEST_ASSERT_EQUAL(GetDuty(), 50);
	TEST_ASSERT_EQUAL(GetFrequency(), 16667 );
	TEST_ASSERT_EQUAL(GetRPM(), 1000 );
}

/* TEST 1 */
DECLARE_TEST_FUNC(IncreaseEncoderByOneWithSwitchOpen) { ENC_SetMovement(1); }
DECLARE_RESULT_FUNC(IncreaseEncoderByOneWithSwitchOpen)
{
	TEST_ASSERT_EQUAL(UI_SelectedLine(), RPM );
	TEST_ASSERT_EQUAL(UI_SelectedDigit(), 0 );
	TEST_ASSERT_EQUAL(GetRPM(), 1001 );
}

/* TEST 2 */
DECLARE_TEST_FUNC(IncreaseEncoderByTenWithSwitchOpen) {	ENC_SetMovement(10); }
DECLARE_RESULT_FUNC(IncreaseEncoderByTenWithSwitchOpen) { TEST_ASSERT_EQUAL(GetRPM(), 1011 ); }

/* TEST 3 */
DECLARE_TEST_FUNC(IncreaseEncoderByOneWithSwitchClosed)
{
	UI_PressButton(ENC, true);
	ENC_SetMovement(1);
}

DECLARE_RESULT_FUNC(IncreaseEncoderByOneWithSwitchClosed)
{
	TEST_ASSERT_EQUAL(GetRPM(), 1011 ); // The RPM should not have changed
	TEST_ASSERT_EQUAL(UI_SelectedDigit(), 1 );
}

/* TEST 4 */
DECLARE_TEST_FUNC(IncreaseEncoderByTenWithSwitchClosed)
{
	ENC_SetMovement(10);
}

DECLARE_RESULT_FUNC(IncreaseEncoderByTenWithSwitchClosed)
{
	TEST_ASSERT_EQUAL(UI_SelectedLine(), DUTY );
	TEST_ASSERT_EQUAL(UI_SelectedDigit(), 1 );
}

/* TEST 5 */
DECLARE_TEST_FUNC(DecreaseEncoderByFourWithSwitchClosed)
{
	ENC_SetMovement(-4);
}

DECLARE_RESULT_FUNC(DecreaseEncoderByFourWithSwitchClosed)
{
	TEST_ASSERT_EQUAL(UI_SelectedLine(), FREQ );
	TEST_ASSERT_EQUAL(UI_SelectedDigit(), 2 );
}

/* TEST 6 */
DECLARE_TEST_FUNC(DecreaseEncoderBySevenWithSwitchOpen)
{
	UI_PressButton(ENC, false);
	ENC_SetMovement(-7);
}

DECLARE_RESULT_FUNC(DecreaseEncoderBySevenWithSwitchOpen)
{
	TEST_ASSERT_EQUAL(UI_SelectedLine(), FREQ );
	TEST_ASSERT_EQUAL(UI_SelectedDigit(), 2 );
	// 1011 RPM is 16.85Hz, which is 16850 mHz.
	// The trailing zero is hidden by the display, so index '2'
	// represents the '6' digit.
	// After a change of -7 encoder ticks on the '6' digit,
	// new value should be 9850 (change of -7 * 100 * 10 = 7000mHz)
	TEST_ASSERT_EQUAL(GetFrequency(), 9850 );
}

/* Tests 8, 9, 10, 11 */

DECLARE_TEST_FUNC(PressHalfButton) { UI_PressButton(ENC, false); UI_PressButton(HALF, true);}
DECLARE_TEST_FUNC(PressDoubleButton)  { UI_PressButton(HALF, false); UI_PressButton(DOUBLE, true);}
DECLARE_TEST_FUNC(PressThirdButton)  { UI_PressButton(DOUBLE, false); UI_PressButton(THIRD, true);}
DECLARE_TEST_FUNC(PressTrebleButton)  { UI_PressButton(THIRD, false); UI_PressButton(TREBLE, true);}

DECLARE_RESULT_FUNC(PressHalfButton) { TEST_ASSERT_EQUAL( GetFrequency(), 4925 ); }
DECLARE_RESULT_FUNC(PressDoubleButton) { TEST_ASSERT_EQUAL( GetFrequency(), 9850 ); }
DECLARE_RESULT_FUNC(PressThirdButton) { TEST_ASSERT_EQUAL( GetFrequency(), 3283 ); }
DECLARE_RESULT_FUNC(PressTrebleButton) { TEST_ASSERT_EQUAL( GetFrequency(), 9849 ); }
	
void DO_TEST_HARNESS_SETUP(void)
{
	setbuf(stdout, NULL);
}

void DO_TEST_HARNESS_RUNNING(void)
{
	TMR8_Tick_Kick(5);
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
