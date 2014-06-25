#ifndef _UI_LCD_H_
#define _UI_LCD_H_

void UI_LCD_Init(void);

void UI_LCD_SetTopLine(SELECTEDLINE line);

void UI_LCD_SetRPM(uint16_t freq);
void UI_LCD_SetFrequency(uint16_t freq);
void UI_LCD_SetDuty(uint8_t duty);

#endif
