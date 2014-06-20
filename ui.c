/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * Generic Library Includes
 */

#include "button.h"

/*
 * Local Application Includes
 */

#include "app.h"

/*
 * Defines and Typedefs
 */

#define BUTTON_DEBOUNCE_MS (100)
#define BUTTON_DEBOUNCE_COUNT (BUTTON_DEBOUNCE_MS / BUTTON_SCAN_PERIOD_MS)

#define IDLE_MS_COUNT (2000)

/*
 * Private Function Prototypes
 */
 
void halfBtnChange(BTN_STATE_ENUM state);
void thirdBtnChange(BTN_STATE_ENUM state);
void doubleBtnChange(BTN_STATE_ENUM state);
void trebleBtnChange(BTN_STATE_ENUM state);
void encBtnChange(BTN_STATE_ENUM state);

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

static SELECTEDLINE s_selectedLine = FREQ;
static uint8_t s_selectedDigit = 0;

static uint8_t s_scanPeriodMs;

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
	
	return success;
}
 
void UI_Tick(BTN_STATE_ENUM half, BTN_STATE_ENUM third, BTN_STATE_ENUM dbl, BTN_STATE_ENUM treble, BTN_STATE_ENUM enc)
{
	BTN_Update(&halfButton, half);
	BTN_Update(&thirdButton, third);
	BTN_Update(&doubleButton, dbl);
	BTN_Update(&trebleButton, treble);
	BTN_Update(&encButton, enc);
}

int8_t UI_SelectedDigit(void) { return s_selectedDigit; }
SELECTEDLINE UI_SelectedLine(void) { return s_selectedLine; }
bool UI_EncoderButtonIsPressed(void) { return encButton.current_state == BTN_STATE_ACTIVE; } 

void UI_HandleEncoderChange(int16_t encoderChange)
{
	(void)encoderChange;
	//TODO: Change digit selection based on encoder movement
}

/*
 * Private Functions
 */

void halfBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_HalfFreq();
	}
}

void thirdBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_ThirdFreq();
	}
}

void doubleBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_DoubleFreq();
	}
}

void trebleBtnChange(BTN_STATE_ENUM state)
{
	if (state == BTN_STATE_ACTIVE)
	{
		APP_TrebleFreq();
	}
}

void encBtnChange(BTN_STATE_ENUM state) { (void)state; }