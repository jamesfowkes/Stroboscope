#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "unity.h"

#include "util_units.h"
#include "strobe.h"

void setUp(void) {};
void tearDown(void) {};

void testInit(void)
{
	TEST_ASSERT_EQUAL(DEFAULT_FREQ, GetFrequency());
	TEST_ASSERT_EQUAL(DEFAULT_RPM, GetRPM());
	TEST_ASSERT_EQUAL(50, GetDuty());
}

void testWithinLimitsPasses(void)
{
	SetFrequency(MAX_FREQ);
	TEST_ASSERT_EQUAL(MAX_FREQ, GetFrequency());
	SetFrequency(MIN_FREQ);
	TEST_ASSERT_EQUAL(MIN_FREQ, GetFrequency());
}

void testSetAboveMaxFreqFails(void)
{
	uint16_t before = GetFrequency();
	SetFrequency(MAX_FREQ+1);
	TEST_ASSERT_EQUAL(before, GetFrequency());
}

void testSetBelowMinFreqFails(void)
{
	uint16_t before = GetFrequency();
	SetFrequency(MIN_FREQ-1);
	TEST_ASSERT_EQUAL(before, GetFrequency());
}

void testHalfFrequencyDivisionIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	
	MILLIHZ expected = (MAX_FREQ+1)/2;
	SetFrequency(MAX_FREQ);
	
	while (expected > MIN_FREQ)
	{	
		rtn = HalfFrequency();
		TEST_ASSERT_EQUAL(expected, rtn->frequency);	
		expected = (expected + 1) / 2;
	}
}

void testThirdFrequencyDivisionIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	
	MILLIHZ expected = ((MAX_FREQ*2) + 3) / 6;
	SetFrequency(MAX_FREQ);
	
	while (expected > MIN_FREQ)
	{	
		rtn = ThirdFrequency();
		TEST_ASSERT_EQUAL(expected, rtn->frequency);	
		expected = ((expected * 2) + 3) / 6;
	}
}

void testDoubleFrequencyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	
	MILLIHZ expected = MIN_FREQ * 2;
	SetFrequency(MIN_FREQ);
	
	while (expected < MAX_FREQ)
	{	
		rtn = DoubleFrequency();
		TEST_ASSERT_EQUAL(expected, rtn->frequency);	
		expected *= 2;
	}
}

void testTrebleFrequencyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	
	MILLIHZ expected = MIN_FREQ * 3;
	SetFrequency(MIN_FREQ);
	
	while (expected < MAX_FREQ)
	{	
		rtn = TrebleFrequency();
		TEST_ASSERT_EQUAL(expected, rtn->frequency);	
		expected *= 3;
	}
}

void testAlterFrequencyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	SetFrequency(MIN_FREQ);
	MILLIHZ expected = MIN_FREQ;
	
	rtn = AlterFrequency(100);
	TEST_ASSERT_EQUAL(expected += 100, rtn->frequency);
	rtn = AlterFrequency(1000);
	TEST_ASSERT_EQUAL(expected += 1000, rtn->frequency);
	rtn = AlterFrequency(MAX_FREQ - expected);	
	TEST_ASSERT_EQUAL(expected = MAX_FREQ, rtn->frequency);
	rtn = AlterFrequency(1);
	TEST_ASSERT_EQUAL(MAX_FREQ, rtn->frequency);
	
	rtn = AlterFrequency(-100);
	TEST_ASSERT_EQUAL(expected -= 100, rtn->frequency);
	rtn = AlterFrequency(-1000);
	TEST_ASSERT_EQUAL(expected -= 1000, rtn->frequency);
	rtn = AlterFrequency(-expected + MIN_FREQ);
	TEST_ASSERT_EQUAL(expected = MIN_FREQ, rtn->frequency);
	rtn = AlterFrequency(-1);
	TEST_ASSERT_EQUAL(MIN_FREQ, rtn->frequency);
}

void testSetDutyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	rtn = SetDuty(1);
	TEST_ASSERT_EQUAL(1, rtn->duty);
	rtn = SetDuty(0);
	TEST_ASSERT_EQUAL(1, rtn->duty);
	rtn = SetDuty(100);
	TEST_ASSERT_EQUAL(100, rtn->duty);
	rtn = SetDuty(101);
	TEST_ASSERT_EQUAL(100, rtn->duty);
}

void testAlterDutyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	rtn = SetDuty(1);

	rtn = AlterDuty(0);
	TEST_ASSERT_EQUAL(1, rtn->duty);
	
	rtn = AlterDuty(-1);
	TEST_ASSERT_EQUAL(1, rtn->duty);
	
	rtn = AlterDuty(1);
	TEST_ASSERT_EQUAL(2, rtn->duty);
	
	rtn = AlterDuty(98);
	TEST_ASSERT_EQUAL(100, rtn->duty);
	
	rtn = AlterDuty(1);
	TEST_ASSERT_EQUAL(100, rtn->duty);
}

void testSetRPMIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	
	rtn = SetRPM(MIN_RPM);
	TEST_ASSERT_EQUAL(MIN_RPM, rtn->rpm);
	
	rtn = SetRPM(MIN_RPM-1);
	TEST_ASSERT_EQUAL(MIN_RPM, rtn->rpm);
	
	rtn = SetRPM(MAX_RPM);
	TEST_ASSERT_EQUAL(MAX_RPM, rtn->rpm);
	
	rtn = SetRPM(MAX_RPM+1);
	TEST_ASSERT_EQUAL(MAX_RPM, rtn->rpm);
}

void testAlterRPMIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	
	rtn = SetRPM(MIN_RPM);
	TEST_ASSERT_EQUAL(MIN_RPM, rtn->rpm);
	
	rtn = AlterRPM(1);
	TEST_ASSERT_EQUAL(MIN_RPM+1, rtn->rpm);
	
	rtn = AlterRPM(-1);
	TEST_ASSERT_EQUAL(MIN_RPM, rtn->rpm);
	
	rtn = AlterRPM(-1);
	TEST_ASSERT_EQUAL(MIN_RPM, rtn->rpm);
	
	rtn = AlterRPM(MAX_RPM - MIN_RPM);
	TEST_ASSERT_EQUAL(MAX_RPM, rtn->rpm);
	
	rtn = AlterRPM(1);
	TEST_ASSERT_EQUAL(MAX_RPM, rtn->rpm);
	
	rtn = AlterRPM(-1);
	TEST_ASSERT_EQUAL(MAX_RPM-1, rtn->rpm);
}

void testRPMFollowsFrequency(void)
{
	const STROBESETTINGS * rtn;
	
	rtn = SetFrequency(MIN_FREQ);
	TEST_ASSERT_EQUAL((MIN_FREQ*60)/1000, rtn->rpm);
	
	rtn = SetFrequency(5000);
	TEST_ASSERT_EQUAL((5000*60)/1000, rtn->rpm);
	
	rtn = SetFrequency(MAX_FREQ);
	TEST_ASSERT_EQUAL((MAX_FREQ*60)/1000, rtn->rpm);
}

void testFrequencyFollowsRPM(void)
{
	const STROBESETTINGS * rtn;
	
	rtn = SetRPM(MIN_RPM);
	TEST_ASSERT_EQUAL(((MIN_RPM * 1000) + 30)/60, rtn->frequency);
	
	rtn = SetRPM(7500UL);
	TEST_ASSERT_EQUAL(((7500UL * 1000) + 30)/60, rtn->frequency);
	
	rtn = SetRPM(MAX_RPM);
	TEST_ASSERT_EQUAL(((MAX_RPM * 1000) + 30)/60, rtn->frequency);
}