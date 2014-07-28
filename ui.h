#ifndef _UI_H_
#define _UI_H_

/*
 * Defines and Typedefs
 */
 
enum selectedline
{
	RPM,
	FREQ,
	DUTY
};
typedef enum selectedline SELECTEDLINE;

enum buttons
{
	HALF,
	DOUBLE,
	THIRD,
	TREBLE,
	ENC
};
typedef enum buttons BUTTONS;

/*
 * Public Function Prototypes
 */

bool UI_Init(void);
void UI_Tick(void);

SELECTEDLINE UI_SelectedLine(void);
int8_t UI_SelectedDigit(void);
bool UI_EncoderButtonIsPressed(void);

void UI_HandleEncoderChange(int16_t encoderChange);
void UI_UpdateDisplay(uint16_t freq, uint16_t rpm, uint8_t duty);

#ifdef TEST_HARNESS
void UI_PressButton(BUTTONS button, bool set);
#endif

#endif
