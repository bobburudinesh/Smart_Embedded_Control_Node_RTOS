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
SH1106_STATUS SH1106_Send_Display_Data(uint8_t *data, uint8_t len) {
	//TODO: Work in progress
	uint8_t adjusted_length = 0;
	uint8_t remaining_length_after_adjustment = 0;
	if((current_segment_position + (len - 1)) >= SH1106_MAX_SEGMENTS) {
		adjusted_length = SH1106_MAX_SEGMENTS - current_segment_position;
		remaining_length_after_adjustment = (len - 1) - adjusted_length;
	} else {
		adjusted_length = len;
	}
	if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, data, adjusted_length, HAL_MAX_DELAY) != HAL_OK) {
		return SH1106_STATUS_ERROR;
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
	current_segment_position = 0;
	if(SH1106_Set_Cursor(current_page_position, current_segment_position)!= SH1106_STATUS_OK) {
		return SH1106_STATUS_ERROR;
	}

	return SH1106_STATUS_OK;
}


SH1106_STATUS SH1106_Clear_Display(void) {
	uint8_t data_buf[132] = {0x00};
	data_buf[0] = 0x40;
	memset(data_buf, 0x00, 131);
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



