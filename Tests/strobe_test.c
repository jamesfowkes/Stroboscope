#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "unity.h"

#include "strobe.h"

void setUp(void) {};
void tearDown(void) {};

void testInit(void)
{
	TEST_ASSERT_EQUAL(5, GetFrequency());
	TEST_ASSERT_EQUAL(50, GetDuty());
}

void testWithinLimitsPasses(void)
{
	SetFrequency(2500);
	TEST_ASSERT_EQUAL(2500, GetFrequency());
	SetFrequency(5);
	TEST_ASSERT_EQUAL(5, GetFrequency());
	SetFrequency(5000);
	TEST_ASSERT_EQUAL(5000, GetFrequency());
}

void testSetAboveMaxFreqFails(void)
{
	uint16_t before = GetFrequency();
	SetFrequency(5001);
	TEST_ASSERT_EQUAL(before, GetFrequency());
}

void testSetBelowMinFreqFails(void)
{
	uint16_t before = GetFrequency();
	SetFrequency(4);
	TEST_ASSERT_EQUAL(before, GetFrequency());
}

void testHalfFrequencyDivisionIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	SetFrequency(5000);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(2500, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(1250, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(625, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(313, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(157, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(79, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(40, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(20, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(10, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(5, rtn->frequency);
	rtn = HalfFrequency();
	TEST_ASSERT_EQUAL(5, rtn->frequency);
}

void testThirdFrequencyDivisionIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	SetFrequency(5000);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(1667, rtn->frequency);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(556, rtn->frequency);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(185, rtn->frequency);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(62, rtn->frequency);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(21, rtn->frequency);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(7, rtn->frequency);
	rtn = ThirdFrequency();
	TEST_ASSERT_EQUAL(7, rtn->frequency);
}

void testDoubleFrequencyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	SetFrequency(5);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(10, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(20, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(40, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(80, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(160, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(320, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(640, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(1280, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(2560, rtn->frequency);
	rtn = DoubleFrequency();
	TEST_ASSERT_EQUAL(2560, rtn->frequency);
}

void testTrebleFrequencyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	SetFrequency(5);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(15, rtn->frequency);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(45, rtn->frequency);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(135, rtn->frequency);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(405, rtn->frequency);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(1215, rtn->frequency);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(3645, rtn->frequency);
	rtn = TrebleFrequency();
	TEST_ASSERT_EQUAL(3645, rtn->frequency);
}

void testAlterFrequencyIsCorrect(void)
{
	const STROBESETTINGS * rtn;
	SetFrequency(5);
	rtn = AlterFrequency(100);
	TEST_ASSERT_EQUAL(105, rtn->frequency);
	rtn = AlterFrequency(1000);
	TEST_ASSERT_EQUAL(1105, rtn->frequency);
	rtn = AlterFrequency(3895);
	TEST_ASSERT_EQUAL(5000, rtn->frequency);
	rtn = AlterFrequency(1);
	TEST_ASSERT_EQUAL(5000, rtn->frequency);
	
	rtn = AlterFrequency(-100);
	TEST_ASSERT_EQUAL(4900, rtn->frequency);
	rtn = AlterFrequency(-1000);
	TEST_ASSERT_EQUAL(3900, rtn->frequency);
	rtn = AlterFrequency(-3895);
	TEST_ASSERT_EQUAL(5, rtn->frequency);
	rtn = AlterFrequency(-1);
	TEST_ASSERT_EQUAL(5, rtn->frequency);
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
