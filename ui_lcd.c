/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>

/*
 * AVR Library Includes
 */

#include "lib_io.h"

/*
 * Device Library Includes
 */

#include "lcd/lcd.h"

/*
 * Format Library Includes
 */

#include "format.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/*
 * Application Includes
 */

#include "ui.h"
#include "ui_lcd.h"

/*
 * Defines and Typedefs
 */

#define RPM_VALUE_LENGTH (5) // RPM takes five digits
#define RPM_UNIT_LENGTH (3) // Length of "rpm" string
#define RPM_VALUE_START_CHAR (15 - RPM_UNIT_LENGTH - RPM_VALUE_LENGTH + 1)

#define FREQ_VALUE_LENGTH (6) // Frequency takes 5 digits with a decimal point
#define FREQ_UNIT_LENGTH (2) // Length of "Hz" string
#define FREQ_VALUE_START_CHAR (15 - FREQ_UNIT_LENGTH - FREQ_VALUE_LENGTH + 1)

#define DUTY_VALUE_LENGTH (3)
#define DUTY_UNIT_LENGTH (1) // Length of "%" string
#define DUTY_VALUE_START_CHAR (15 - DUTY_UNIT_LENGTH - DUTY_VALUE_LENGTH + 1)

/*
 * Local Variables
 */
 
static char rpmLine[]  = "RPM:         rpm";
static char freqLine[] = "Freq:         Hz";
static char dutyLine[] = "Duty:          %";

static char * lines[] = {rpmLine, freqLine, dutyLine};

static LCD_PORT lcdDataPorts;
static LCD_PORT lcdDirectionPorts;
static LCD_PIN lcdPins;
static LCD_CONFIG lcdConfig;

/*
 * Private Function Declarations
 */

static void clearValue(SELECTEDLINE line, uint8_t start, uint8_t len);
static void updateDisplay(SELECTEDLINE line, uint8_t start, uint8_t length);

/*
 * Public Function Definitions
 */

/* UI_LCD_Init
 :Sets up LCD
*/
void UI_LCD_Init(void)
{
	lcdDataPorts.port0.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port1.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port2.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port3.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port0.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port1.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port2.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port3.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDirectionPorts.port0.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.port1.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.port2.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.port3.w = IO_GetDirectionPortDirect(IO_PORTD);

	lcdDirectionPorts.rsPort = IO_GetDirectionPortDirect(IO_PORTB);
	lcdDirectionPorts.rwPort = IO_GetDirectionPortDirect(IO_PORTB);
	lcdDirectionPorts.enPort = IO_GetDirectionPortDirect(IO_PORTB);
	lcdDataPorts.rsPort = IO_GetWritePortDirect(IO_PORTB);
	lcdDataPorts.rwPort = IO_GetWritePortDirect(IO_PORTB);
	lcdDataPorts.enPort = IO_GetWritePortDirect(IO_PORTB);

	lcdPins.pin0 = 2;
	lcdPins.pin1 = 3;
	lcdPins.pin2 = 4;
	lcdPins.pin3 = 5;
	lcdPins.rsPin = 0;
	lcdPins.rwPin = 2;
	lcdPins.enPin = 3;
	
	lcdConfig.type = LCD_CONTROLLER_HD44780;
	lcdConfig.lines = 2;
	lcdConfig.dispLength = 16;
	lcdConfig.internalLength = 0x40;
	lcdConfig.line1Start = 0x00;
	lcdConfig.line2Start = 0x40;
	lcdConfig.line3Start = 0x14;
	lcdConfig.line4Start = 0x54;
	lcdConfig.wrapLines = false;

	lcd_init(LCD_DISP_ON_CURSOR_BLINK, &lcdDataPorts, &lcdDirectionPorts, &lcdPins, &lcdConfig);
	
	lcd_gotoxy(0,0);
	lcd_puts(lines[0]);
	lcd_gotoxy(0,1);
	lcd_puts(lines[1]);
}

/* UI_LCD_Init
 : Overwrites LCD display with new lines
*/
void UI_LCD_SetTopLine(SELECTEDLINE line)
{
	lcd_gotoxy(0,0);
	lcd_puts(lines[line]);
	
	lcd_gotoxy(0,1);
	incrementwithrollover(line, DUTY);
	lcd_puts(lines[line]);

	UI_LCD_UpdateCursor();
}

void UI_LCD_SetRPM(uint16_t rpm)
{
	// Get a pointer into the RPM string at the right point
	char * chars = &rpmLine[RPM_VALUE_START_CHAR];
	
	clearValue(RPM, RPM_VALUE_START_CHAR, RPM_VALUE_LENGTH);

	// In order to right align, temporarily null-terminate the string (wiping out 'r' char)
	rpmLine[RPM_VALUE_START_CHAR + RPM_VALUE_LENGTH] = '\0';
	
	// Then do the conversion, right align and restore the 'r'
	(void)U16ToString(rpm, chars);
	RightAlign(chars);
	rpmLine[RPM_VALUE_START_CHAR + RPM_VALUE_LENGTH] = 'r';
	
	updateDisplay(RPM, RPM_VALUE_START_CHAR, RPM_VALUE_LENGTH);
}

void UI_LCD_SetFrequency(uint16_t freq)
{
	// Get a pointer into the freq string at the right point
	char * chars = &freqLine[FREQ_VALUE_START_CHAR];
	
	clearValue(FREQ, FREQ_VALUE_START_CHAR, FREQ_VALUE_LENGTH);

	// In order to right align, temporarily null-terminate the string (wiping out 'H' char)
	freqLine[FREQ_VALUE_START_CHAR + FREQ_VALUE_LENGTH] = '\0';
	
	// Then do the conversion, right align and restore the 'H'
	(void)U16ToString(freq, chars);
	RightAlign(chars);
	freqLine[FREQ_VALUE_START_CHAR + FREQ_VALUE_LENGTH] = 'H';
	
	// "Divide by 100" by shifting top three MSDs to left and inserting decimal point
	chars[0] = chars[1];
	chars[1] = chars[2];
	chars[2] = chars[3];
	chars[3] = '.';
	
	updateDisplay(FREQ, FREQ_VALUE_START_CHAR, FREQ_VALUE_LENGTH);
}

void UI_LCD_SetDuty(uint8_t duty)
{
	// Get a pointer into the duty string at the right point
	char * chars = &dutyLine[DUTY_VALUE_START_CHAR];
	
	clearValue(DUTY, DUTY_VALUE_START_CHAR, DUTY_VALUE_LENGTH);

	// In order to right align, temporarily null-terminate the string (wiping out '%' char)
	dutyLine[DUTY_VALUE_START_CHAR + DUTY_VALUE_LENGTH] = '\0';
	
	// Then do the conversion, right align and restore the '%'
	(void)U8ToString(duty, chars);
	RightAlign(chars);
	dutyLine[DUTY_VALUE_START_CHAR + DUTY_VALUE_LENGTH] = '%';
	
	updateDisplay(DUTY, DUTY_VALUE_START_CHAR, DUTY_VALUE_LENGTH);
}

void UI_LCD_UpdateCursor(void)
{
	int8_t selectedDigit = UI_SelectedDigit();
	switch(UI_SelectedLine())
	{
	case RPM:
		selectedDigit = RPM_VALUE_START_CHAR + RPM_VALUE_LENGTH - selectedDigit - 1;
		break;
	case FREQ:
		if (selectedDigit == 2) {selectedDigit = 3;} // Skip over D.P.
		selectedDigit = FREQ_VALUE_START_CHAR + FREQ_VALUE_LENGTH - selectedDigit - 1;
		break;
	case DUTY:
		selectedDigit = DUTY_VALUE_START_CHAR + DUTY_VALUE_LENGTH - selectedDigit - 1;
		break;
	}
	lcd_gotoxy(selectedDigit, 0);
}	

/*
 * Private Function Definitions
 */

static void clearValue(SELECTEDLINE line, uint8_t start, uint8_t len)
{
	for(uint8_t i = start; i < start + len; ++i)
	{
		lines[line][i] = ' ';
	}
}

static void updateDisplay(SELECTEDLINE line, uint8_t start, uint8_t length)
{
	SELECTEDLINE topLine = UI_SelectedLine();
	
	bool updateNow = false;
	updateNow |= (topLine == RPM) && ((line == RPM) || (line == FREQ));
	updateNow |= (topLine == FREQ) && ((line == FREQ) || (line == DUTY));
	updateNow |= (topLine == DUTY) && ((line == DUTY) || (line == RPM));
	
	if (updateNow)
	{
		lcd_gotoxy(start, topLine == line ? 0 : 1); 
		lcd_putsn(&lines[line][start], length);
		UI_LCD_UpdateCursor();
	}
}
