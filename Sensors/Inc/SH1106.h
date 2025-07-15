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

#define SH1106_COMMAND_DISPLAY_ON				0xA4
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

typedef enum {
	SH1106_STATUS_ERROR = 0,
	SH1106_STATUS_OK
} SH1106_STATUS;

typedef struct {
 I2C_HandleTypeDef	i2c;
} SH1106_I2C_Handle;

SH1106_STATUS SH1106_I2C_INIT(void);

SH1106_STATUS SH1106_Send_Command_Single_Byte(uint8_t command);
SH1106_STATUS SH1106_Send_Command_Double_Byte(uint8_t command1, uint8_t command2);
SH1106_STATUS SH1106_Send_Display_Data(uint8_t *data, uint8_t len);





#endif /* INC_SH1106_H_ */
