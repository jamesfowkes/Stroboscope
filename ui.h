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

/*
 * Public Function Prototypes
 */

bool UI_Init(uint8_t scanPeriodMs);
void UI_Tick(void);

SELECTEDLINE UI_SelectedLine(void);
int8_t UI_SelectedDigit(void);
bool UI_EncoderButtonIsPressed(void);

void UI_HandleEncoderChange(int16_t encoderChange);
void UI_UpdateDisplay(uint16_t freq, uint16_t rpm, uint8_t duty);

#endif
