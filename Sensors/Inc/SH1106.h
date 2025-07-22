/*
 * SH1106.h
 *
 *  Created on: Jul 14, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_SH1106_H_
#define INC_SH1106_H_

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stm32f4xx_hal.h"
//#include "i2c.h"

#define SH1106_ADDRESS							0x3C
#define SH1106_ADDRESS_READ						((0x3C << 1) | 0x01)
#define SH1106_ADDRESS_WRITE					((0x3C << 1) | 0x00)



#define SH1106_CONTROL_LAST_COMMAND				0x00	// This is to Indicate last Control Byte
#define SH1106_CONTROL_NOT_LAST_COMMAND			0x80	// This is to indicate the Command Byte continue.
#define SH1106_COMMAND_WRITE_DISPLAY_DATA		0x40	// This is to indicate that the next following byte/bytes are data.


#define SH1106_COMMAND_PUMP_VOLTAGE				0x30

#define SH1106_COMMAND_DISPLAY_LINE_BASE		0x40

#define SH1106_COMMAND_CONTRAST_MODE_SET		0x81
#define SH1106_COMMAND_CONTRAST_REGISTER		0x00

#define SH1106_COMMAND_DISPLAY_OFF				0xA4
#define SH1106_COMMAND_DISPLAY_ON				0xA5

#define SH1106_COMMAND_DISPLAY_NORMAL			0xA6
#define SH1106_COMMAND_DISPLAY_REVERSE			0xA7

#define SH1106_COMMAND_SET_PAGE_ADDRESS_BASE	0xB0
#define SH1106_COMMAND_SET_PAGE_ADDRESS_MIN 	0xB0
#define SH1106_COMMAND_SET_PAGE_ADDRESS_MAX 	0xB7


#define SH1106_COMMAND_SCAN_DIRECTION			0xC0

#define SH1106_COMMAND_READ_MODIFY_WRITE		0xE0

#define SH1106_COMMAND_END						0xEE

#define SH1106_COMMAND_NOP						0xE3


#define SH1106_NO_OF_LINES_PER_PAGE				8
#define SH1106_NO_OF_SEGMENTS_PER_CHARACTER		5
#define SH1106_NO_OF_SEGMENTS_PER_LETTER_GAP    1


#define SH1106_MAX_LINES						64
#define SH1106_MAX_PAGES						8
#define SH1106_MAX_SEGMENTS						132
#define SH1106_MAX_CHARACTERS_IN_PAGE			((uint8_t)(SH1106_MAX_SEGMENTS/(SH1106_NO_OF_SEGMENTS_PER_CHARACTER + SH1106_NO_OF_SEGMENTS_PER_LETTER_GAP)))
#define SH1106_WRITE_BUFFER_SIZE				(SH1106_MAX_SEGMENTS + 1)





typedef enum {
	SH1106_STATUS_ERROR = 0,
	SH1106_STATUS_OK
} SH1106_STATUS;

typedef struct {
 I2C_HandleTypeDef	i2c;
} SH1106_I2C_Handle;

SH1106_STATUS SH1106_I2C_INIT(I2C_HandleTypeDef	i2c, bool vertical_scroll_enable);

SH1106_STATUS SH1106_Send_Command_Single_Byte(uint8_t command);
SH1106_STATUS SH1106_Send_Command_Double_Byte(uint8_t command1, uint8_t command2);
SH1106_STATUS SH1106_Send_Display_Data(uint8_t *data, uint8_t len);


SH1106_STATUS SH1106_Clear_Display(void);
SH1106_STATUS	SH1106_Go_To_Next_Line(void);
SH1106_STATUS SH1106_Set_Cursor(uint8_t x, uint8_t y);

#endif /* INC_SH1106_H_ */
