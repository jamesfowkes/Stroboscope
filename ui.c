/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>

/*
 * Device Library Includes
 */
 
#include "lcd/lcd.h"

/*
 * Generic Library Includes
 */

#include "button.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_tmr8_tick.h"
#include "lib_io.h"
#include "lib_encoder.h"

/*
 * Local Application Includes
 */

#include "app.h"
#include "ui.h"
#include "ui_lcd.h"

/*
 * Defines and Typedefs
 */

#define BUTTON_DEBOUNCE_MS (100)
#define BUTTON_DEBOUNCE_COUNT (BUTTON_DEBOUNCE_MS / BUTTON_SCAN_PERIOD_MS)

#define IDLE_MS_COUNT (2000)

// TODO: replace with actual ports/pins
#define HALF_BUTTON_PORT IO_PORTB
#define THIRD_BUTTON_PORT IO_PORTB
#define DOUBLE_BUTTON_PORT IO_PORTB
#define TREBLE_BUTTON_PORT IO_PORTB
#define ENC_BUTTON_PORT IO_PORTB

#define HALF_BUTTON_PIN 0
#define THIRD_BUTTON_PIN 1
#define DOUBLE_BUTTON_PIN 2
#define TREBLE_BUTTON_PIN 3
#define ENC_BUTTON_PIN 4

/*
 * Private Function Prototypes
 */
 
static void nextLine(void);
static void prevLine(void);

static void halfBtnChange(BTN_STATE_ENUM state);
static void thirdBtnChange(BTN_STATE_ENUM state);
static void doubleBtnChange(BTN_STATE_ENUM state);
static void trebleBtnChange(BTN_STATE_ENUM state);
static void encBtnChange(BTN_STATE_ENUM state);

/*
 * Local Variables
 */
 
static BTN halfButton = 
{
	.current_state = BTN_STATE_INACTIVE,
	.change_state_callback = halfBtnChange,
	.repeat_callback = NULL,
	.max_repeat_count = 0,
	.max_debounce_count = BUTTON_DEBOUNCE_COUNT
};

static BTN thirdButton = 
{
	.current_state = BTN_STATE_INACTIVE,
	.change_state_callback = thirdBtnChange,
	.repeat_callback = NULL,
	.max_repeat_count = 0,
	.max_debounce_count = BUTTON_DEBOUNCE_COUNT
};

static BTN doubleButton = 
{
	.current_state = BTN_STATE_INACTIVE,
	.change_state_callback = doubleBtnChange,
	.repeat_callback = NULL,
	.max_repeat_count = 0,
	.max_debounce_count = BUTTON_DEBOUNCE_COUNT
};

static BTN trebleButton = 
{
	.current_state = BTN_STATE_INACTIVE,
	.change_state_callback = trebleBtnChange,
	.repeat_callback = NULL,
	.max_repeat_count = 0,
	.max_debounce_count = BUTTON_DEBOUNCE_COUNT
};

static BTN encButton = 
{
	.current_state = BTN_STATE_INACTIVE,
	.change_state_callback = encBtnChange,
	.repeat_callback = NULL,
	.max_repeat_count = 0,
	.max_debounce_count = BUTTON_DEBOUNCE_COUNT
};

static SELECTEDLINE s_topLine = FREQ;
static int8_t s_selectedDigit = 0;

static uint8_t s_scanPeriodMs;

static uint8_t s_maxDigits[] = {3, 3, 2}; // RPM, Freq, Duty

static TMR8_TICK_CONFIG uiTick;

/*
 * Public Functions
 */
 
bool UI_Init(uint8_t scanPeriodMs)
{
	bool success = true;
	
	s_scanPeriodMs = scanPeriodMs;
	
	success &= BTN_InitHandler(&halfButton);
	success &= BTN_InitHandler(&thirdButton);
	success &= BTN_InitHandler(&doubleButton);
	success &= BTN_InitHandler(&trebleButton);
	success &= BTN_InitHandler(&encButton);
	
	uiTick.reload = scanPeriodMs;
	uiTick.active = true;
	TMR8_Tick_AddTimerConfig(&uiTick);
	
	// TODO: Change to actual encoder inputs
	ENC_Setup(IO_PORTB, 0, 1);
	
	UI_LCD_Init();
	
	return success;
}
 
void UI_Tick(void)
{
	if (TMR8_Tick_TestAndClear(&uiTick))
	{
		BTN_STATE_ENUM half = IO_ReadPin(HALF_BUTTON_PORT, HALF_BUTTON_PIN) ? BTN_STATE_INACTIVE : BTN_STATE_ACTIVE;
		BTN_STATE_ENUM third = IO_ReadPin(THIRD_BUTTON_PORT, THIRD_BUTTON_PIN) ? BTN_STATE_INACTIVE : BTN_STATE_ACTIVE;
		BTN_STATE_ENUM dbl = IO_ReadPin(DOUBLE_BUTTON_PORT, DOUBLE_BUTTON_PIN) ? BTN_STATE_INACTIVE : BTN_STATE_ACTIVE;
		BTN_STATE_ENUM treble = IO_ReadPin(TREBLE_BUTTON_PORT, TREBLE_BUTTON_PIN) ? BTN_STATE_INACTIVE : BTN_STATE_ACTIVE;
		BTN_STATE_ENUM enc = IO_ReadPin(ENC_BUTTON_PORT, ENC_BUTTON_PIN) ? BTN_STATE_INACTIVE : BTN_STATE_ACTIVE;
				
		BTN_Update(&halfButton, half);
		BTN_Update(&thirdButton, third);
		BTN_Update(&doubleButton, dbl);
		BTN_Update(&trebleButton, treble);
		BTN_Update(&encButton, enc);
	}
}


int8_t UI_SelectedDigit(void) { return s_selectedDigit; }
SELECTEDLINE UI_SelectedLine(void) { return s_topLine; }
bool UI_EncoderButtonIsPressed(void) { return encButton.current_state == BTN_STATE_ACTIVE; } 

void UI_HandleEncoderChange(int16_t encoderChange)
{
	s_selectedDigit += encoderChange;
	
	// Wrap the selected digit inside limits for the current edit line
	if (s_selectedDigit > s_maxDigits[s_topLine]) { nextLine(); }
	if (s_selectedDigit < 0) { prevLine(); }
}

void UI_UpdateDisplay(uint16_t freq, uint16_t rpm, uint8_t duty)
{
	UI_LCD_SetFrequency(freq);
	UI_LCD_SetDuty(duty);
	UI_LCD_SetRPM(rpm);
}

/*
 * Private Functions
 */

static void nextLine(void)
{
	incrementwithrollover(s_topLine, DUTY);
	s_selectedDigit = 0;
	UI_LCD_SetTopLine(s_topLine);
}

static void prevLine(void)
{
	decrementwithrollover(s_topLine, DUTY);
	s_selectedDigit = s_maxDigits[s_topLine];
	UI_LCD_SetTopLine(s_topLine);
}

static void halfBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_HalfFreq();
	}
}

static void thirdBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_ThirdFreq();
	}
}

static void doubleBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_DoubleFreq();
	}
}

static void trebleBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_TrebleFreq();
	}
}

static void encBtnChange(BTN_STATE_ENUM state)
{
	(void)state; // Let the button library keep the button state
}
