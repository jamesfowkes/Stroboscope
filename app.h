#ifndef _APP_H_
#define _APP_H_

/*
 * Defines and Typedefs
 */

#define BUTTON_SCAN_PERIOD_MS (10)

enum selectedline
{
	FREQ,
	DUTY
};
typedef enum selectedline SELECTEDLINE;

bool UI_Init(uint8_t scanPeriodMs);
void UI_Tick(BTN_STATE_ENUM half, BTN_STATE_ENUM third, BTN_STATE_ENUM dbl, BTN_STATE_ENUM treble, BTN_STATE_ENUM enc);

SELECTEDLINE UI_SelectedLine(void);
int8_t UI_SelectedDigit(void);
bool UI_EncoderButtonIsPressed(void);

void UI_HandleEncoderChange(int16_t encoderChange);

void APP_HalfFreq();
void APP_ThirdFreq();
void APP_DoubleFreq();
void APP_TrebleFreq();

#endif
