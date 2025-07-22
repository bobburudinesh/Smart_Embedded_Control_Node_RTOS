/*
 * SH1106.c
 *
 *  Created on: Jul 14, 2025
 *      Author: Dinesh Bobburu
 */



#include "SH1106.h"



SH1106_I2C_Handle hsh1106;

uint8_t data_buffer[133];
static bool sh1106_vertical_scroll_enable = 0;
static uint8_t current_page_position = 0;
static uint8_t current_segment_position = 0;

static const uint8_t SSD1306_font[][5]=
{
    {0x00, 0x00, 0x00, 0x00, 0x00},   // space
    {0x00, 0x00, 0x2f, 0x00, 0x00},   // !
    {0x00, 0x07, 0x00, 0x07, 0x00},   // "
    {0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
    {0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
    {0x23, 0x13, 0x08, 0x64, 0x62},   // %
    {0x36, 0x49, 0x55, 0x22, 0x50},   // &
    {0x00, 0x05, 0x03, 0x00, 0x00},   // '
    {0x00, 0x1c, 0x22, 0x41, 0x00},   // (
    {0x00, 0x41, 0x22, 0x1c, 0x00},   // )
    {0x14, 0x08, 0x3E, 0x08, 0x14},   // *
    {0x08, 0x08, 0x3E, 0x08, 0x08},   // +
    {0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
    {0x08, 0x08, 0x08, 0x08, 0x08},   // -
    {0x00, 0x60, 0x60, 0x00, 0x00},   // .
    {0x20, 0x10, 0x08, 0x04, 0x02},   // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
    {0x42, 0x61, 0x51, 0x49, 0x46},   // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
    {0x27, 0x45, 0x45, 0x45, 0x39},   // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
    {0x01, 0x71, 0x09, 0x05, 0x03},   // 7
    {0x36, 0x49, 0x49, 0x49, 0x36},   // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
    {0x00, 0x36, 0x36, 0x00, 0x00},   // :
    {0x00, 0x56, 0x36, 0x00, 0x00},   // ;
    {0x08, 0x14, 0x22, 0x41, 0x00},   // <
    {0x14, 0x14, 0x14, 0x14, 0x14},   // =
    {0x00, 0x41, 0x22, 0x14, 0x08},   // >
    {0x02, 0x01, 0x51, 0x09, 0x06},   // ?
    {0x32, 0x49, 0x59, 0x51, 0x3E},   // @
    {0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
    {0x7F, 0x49, 0x49, 0x49, 0x36},   // B
    {0x3E, 0x41, 0x41, 0x41, 0x22},   // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
    {0x7F, 0x49, 0x49, 0x49, 0x41},   // E
    {0x7F, 0x09, 0x09, 0x09, 0x01},   // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
    {0x00, 0x41, 0x7F, 0x41, 0x00},   // I
    {0x20, 0x40, 0x41, 0x3F, 0x01},   // J
    {0x7F, 0x08, 0x14, 0x22, 0x41},   // K
    {0x7F, 0x40, 0x40, 0x40, 0x40},   // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
    {0x7F, 0x09, 0x09, 0x09, 0x06},   // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},   // R
    {0x46, 0x49, 0x49, 0x49, 0x31},   // S
    {0x01, 0x01, 0x7F, 0x01, 0x01},   // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
    {0x63, 0x14, 0x08, 0x14, 0x63},   // X
    {0x07, 0x08, 0x70, 0x08, 0x07},   // Y
    {0x61, 0x51, 0x49, 0x45, 0x43},   // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00},   // [
    {0x55, 0xAA, 0x55, 0xAA, 0x55},   // Backslash (Checker pattern)
    {0x00, 0x41, 0x41, 0x7F, 0x00},   // ]
    {0x04, 0x02, 0x01, 0x02, 0x04},   // ^
    {0x40, 0x40, 0x40, 0x40, 0x40},   // _
    {0x00, 0x03, 0x05, 0x00, 0x00},   // `
    {0x20, 0x54, 0x54, 0x54, 0x78},   // a
    {0x7F, 0x48, 0x44, 0x44, 0x38},   // b
    {0x38, 0x44, 0x44, 0x44, 0x20},   // c
    {0x38, 0x44, 0x44, 0x48, 0x7F},   // d
    {0x38, 0x54, 0x54, 0x54, 0x18},   // e
    {0x08, 0x7E, 0x09, 0x01, 0x02},   // f
    {0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g
    {0x7F, 0x08, 0x04, 0x04, 0x78},   // h
    {0x00, 0x44, 0x7D, 0x40, 0x00},   // i
    {0x40, 0x80, 0x84, 0x7D, 0x00},   // j
    {0x7F, 0x10, 0x28, 0x44, 0x00},   // k
    {0x00, 0x41, 0x7F, 0x40, 0x00},   // l
    {0x7C, 0x04, 0x18, 0x04, 0x78},   // m
    {0x7C, 0x08, 0x04, 0x04, 0x78},   // n
    {0x38, 0x44, 0x44, 0x44, 0x38},   // o
    {0xFC, 0x24, 0x24, 0x24, 0x18},   // p
    {0x18, 0x24, 0x24, 0x18, 0xFC},   // q
    {0x7C, 0x08, 0x04, 0x04, 0x08},   // r
    {0x48, 0x54, 0x54, 0x54, 0x20},   // s
    {0x04, 0x3F, 0x44, 0x40, 0x20},   // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},   // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},   // w
    {0x44, 0x28, 0x10, 0x28, 0x44},   // x
    {0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y
    {0x44, 0x64, 0x54, 0x4C, 0x44},   // z
    {0x00, 0x10, 0x7C, 0x82, 0x00},   // {
    {0x00, 0x00, 0xFF, 0x00, 0x00},   // |
    {0x00, 0x82, 0x7C, 0x10, 0x00},   // }
    {0x00, 0x06, 0x09, 0x09, 0x06}    // ~ (Degrees)
};

SH1106_STATUS SH1106_I2C_INIT(I2C_HandleTypeDef	i2c, bool vertical_scroll_enable) {
	hsh1106.i2c = i2c;
	sh1106_vertical_scroll_enable = vertical_scroll_enable;
	uint32_t	current_Ticks;
	SH1106_Send_Command_Single_Byte(0xAE);			// Display OFF
	SH1106_Send_Command_Double_Byte(0xAD, 0x8B);	// DC-DC Control mode, DC-DC ON.
	SH1106_Send_Command_Single_Byte(0x32);

	SH1106_Send_Command_Double_Byte(0xD5, 0x80);	// Clock
	SH1106_Send_Command_Double_Byte(0xD3, 0x00);	// Display offset mode
	SH1106_Send_Command_Double_Byte(0xA8, 0x3F); // Set Multiplex Ratio, enable 64 COM Lines.

	SH1106_Send_Command_Single_Byte(0x40);	// Set Display Start Line
	SH1106_Send_Command_Single_Byte(0xA0);  // Set Segment Re-map

	SH1106_Send_Command_Single_Byte(0xC8); 	// COM Scan Direction
	SH1106_Send_Command_Double_Byte(0xDA, 0x12);	// COM PIN Config

	SH1106_Send_Command_Double_Byte(0xDB, 0x35);	// VCOM setting

	SH1106_Send_Command_Double_Byte(0xD9, 0x1F);	//Pre-Charge

	SH1106_Send_Command_Double_Byte(0x81, 0xFF);	// Contrast Control Mode Set.

	SH1106_Send_Command_Single_Byte(0xA6);	// Set Normal Display

	SH1106_Send_Command_Single_Byte(0xA4);	// Resume RAM


	SH1106_Send_Command_Single_Byte(0xB0);	// Set Page 0
	current_Ticks = HAL_GetTick();
	while((HAL_GetTick() - current_Ticks) < 100);


	SH1106_Send_Command_Single_Byte(0xAF);	// Turn ON Display
	current_Ticks = HAL_GetTick();
	while((HAL_GetTick() - current_Ticks) < 100);
	return SH1106_STATUS_OK;

}

SH1106_STATUS SH1106_Send_Command_Single_Byte(uint8_t command) {
	uint8_t cmd_buffer[2];
	cmd_buffer[0] = SH1106_CONTROL_LAST_COMMAND;	// 0x00
	cmd_buffer[1] = command;

	if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, &cmd_buffer, 2, HAL_MAX_DELAY) != HAL_OK) {
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;
}
SH1106_STATUS SH1106_Send_Command_Double_Byte(uint8_t command1, uint8_t command2) {
	uint8_t cmd_buffer[4] = {0x80, 0x00, 0x00, 0x00};
	cmd_buffer[0] = SH1106_CONTROL_NOT_LAST_COMMAND;	// 0x80
	cmd_buffer[1] = command1;
	cmd_buffer[2] = SH1106_CONTROL_LAST_COMMAND;	//0x00
	cmd_buffer[3] = command2;
	if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, &cmd_buffer, 4, HAL_MAX_DELAY) != HAL_OK) {
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;
}
SH1106_STATUS SH1106_Send_Display_Data(uint8_t *data, uint8_t dataLength) {
	uint8_t data_buf[SH1106_WRITE_BUFFER_SIZE];
	data_buf[0] = 0x40;
	uint8_t remaining_length = dataLength;
	while(remaining_length > 0) {
		uint8_t chunckSize = 0;
		if((current_segment_position + remaining_length) > SH1106_MAX_SEGMENTS) {
			// Current page cannot accommodate the data
			chunckSize = SH1106_MAX_SEGMENTS - current_segment_position;

		} else {
			chunckSize = remaining_length;

		}
		memcpy(&data_buf[1], data, chunckSize); // load the chunck to be transmitted to data_buf from 1st index as 0th index will be having 0x40 control byte
		if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, data_buf, chunckSize + 1, HAL_MAX_DELAY) != HAL_OK) {
			return SH1106_STATUS_ERROR;
		}
		current_segment_position += chunckSize;
		remaining_length -= chunckSize;
		data += chunckSize;
		if(current_segment_position >= SH1106_MAX_SEGMENTS && (remaining_length > 0)) {
			if(SH1106_Go_To_Next_Line() != SH1106_STATUS_OK) {
				return SH1106_STATUS_ERROR;
			}
		}
	}
	return SH1106_STATUS_OK;
}

void SH1106_SCROLL_PAGE(uint8_t noOfPages) {
	// this function scrolls from current state noOfPages ranges from 0-7; SH1106_NO_OF_LINES_PER_PAGE = 8,
	uint8_t safe_page = noOfPages % SH1106_MAX_PAGES;
	SH1106_Send_Command_Single_Byte(0x40 | ((safe_page * SH1106_NO_OF_LINES_PER_PAGE) & 0x3F));
}

SH1106_STATUS SH1106_Go_To_Page(uint8_t pageNumber) {
	uint8_t safe_page = pageNumber % SH1106_MAX_PAGES;
	if(pageNumber >= SH1106_MAX_PAGES && sh1106_vertical_scroll_enable) {
		// scroll if page number goes >=8 only when enabled
		SH1106_SCROLL_PAGE(safe_page);	// scroll by safe_page number of lines after wrap around vertically down, SH1106_MAX_PAGES = 8
	} else if (pageNumber >= SH1106_MAX_PAGES && !sh1106_vertical_scroll_enable) {

		//Clear the display and display current info in line 0, segment 0;
		SH1106_Clear_Display();
	}
	if(SH1106_Send_Command_Single_Byte(0xB0 | (safe_page & 0x07))!= SH1106_STATUS_OK) {
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;
}

SH1106_STATUS SH1106_Set_Segment(uint8_t segNumber) {
	if(segNumber > 131) {
		segNumber = 131;
	}
	if(SH1106_Send_Command_Single_Byte(0x00 | (segNumber & 0x0F))!= SH1106_STATUS_OK) { // lower nibble
		return SH1106_STATUS_ERROR;
	}
	if(SH1106_Send_Command_Single_Byte(0x10 | ((segNumber >> 4) & 0x0F))!= SH1106_STATUS_OK) { // upper nibble
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;
}

SH1106_STATUS SH1106_Set_Cursor(uint8_t x, uint8_t y){
	current_page_position = (x != current_page_position) ? x : current_page_position;
	current_segment_position = (y != current_segment_position) ? y : current_segment_position;
	if(SH1106_Go_To_Page(x) != SH1106_STATUS_OK) { // Goto page x
		return SH1106_STATUS_ERROR;
	}
	if(SH1106_Set_Segment(y) != SH1106_STATUS_OK) { // And then in that Page go to segment y
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;
}

SH1106_STATUS	SH1106_Go_To_Next_Line(void) {

	if(sh1106_vertical_scroll_enable) {
		current_page_position++;
	} else {
		current_page_position = current_page_position % SH1106_MAX_PAGES;
	}

	if(SH1106_Set_Cursor(current_page_position, current_segment_position)!= SH1106_STATUS_OK) {
		return SH1106_STATUS_ERROR;
	}
	current_segment_position = 0;
	return SH1106_STATUS_OK;
}


SH1106_STATUS SH1106_Clear_Display(void) {
	uint8_t data_buf[SH1106_WRITE_BUFFER_SIZE] = {0x00};
	data_buf[0] = 0x40;
	memset(&data_buf[1], 0x00, SH1106_MAX_SEGMENTS);
	for(uint8_t i = 0; i< SH1106_MAX_PAGES; i++) {
		if(SH1106_Set_Cursor(i,0) != SH1106_STATUS_OK) {
			return SH1106_STATUS_ERROR;
		}
		if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, data_buf, 132, HAL_MAX_DELAY) != HAL_OK) {
			return SH1106_STATUS_ERROR;
		}

	}
	current_page_position = 0;
	current_segment_position = 0;
	if(SH1106_Set_Cursor(current_page_position,current_segment_position) != SH1106_STATUS_OK) {
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;
}



