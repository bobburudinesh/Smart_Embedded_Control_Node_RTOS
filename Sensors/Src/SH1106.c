/*
 * SH1106.c
 *
 *  Created on: Jul 14, 2025
 *      Author: Dinesh Bobburu
 */



#include "SH1106.h"
#include "stm32f4xx_hal.h"


SH1106_I2C_Handle hsh1106;

uint8_t data_buffer[];


SH1106_STATUS SH1106_I2C_INIT(I2C_HandleTypeDef	i2c) {
	hsh1106.i2c = i2c;
	SH1106_Send_Command_Single_Byte(0xAE);			// Display OFF
	SH1106_Send_Command_Double_Byte(0xAD, 0x8B);	// DC-DC Control mode, DC-DC ON.
	SH1106_Send_Command_Single_Byte(0x32);

	SH1106_Send_Command_Double_Byte(0xD5, 0x80);	// Clock

	SH1106_Send_Command_Double_Byte(0xDB, 0x35);	// VCOM setting

	SH1106_Send_Command_Double_Byte(0xD9, 0xF1);	//Pre-Charge

	SH1106_Send_Command_Single_Byte(0x81);	// Contrast Control Mode Set.
	SH1106_Send_Command_Single_Byte(0x7F);	// Contrast Data Register Set.

	SH1106_Send_Command_Single_Byte(0x40);	// Set Display Start Line

	SH1106_Send_Command_Single_Byte(0xA0);  // Set Segment Re-map

	SH1106_Send_Command_Single_Byte(0xA6);	// Set Normal Display

	SH1106_Send_Command_Single_Byte(0xA4);	// Resume RAM

	SH1106_Send_Command_Double_Byte(0xA8, 0x3F); // Set Multiplex Ratio, enable 64 COM Lines.

	SH1106_Send_Command_Single_Byte(0xB0);	// Set Page 0

	SH1106_Send_Command_Single_Byte(0xC8); 	// COM Scan Direction

	SH1106_Send_Command_Double_Byte(0xD3, 0x00);	// Display offset mode

	SH1106_Send_Command_Double_Byte(0xDA, 0x12);	// COM PIN Config

	SH1106_Send_Command_Single_Byte(0xAF);	// Turn ON Display


}

SH1106_STATUS SH1106_Send_Command_Single_Byte(uint8_t command) {
	uint8_t cmd_buffer[2];
	cmd_buffer[0] = SH1106_CONTROL_LAST_COMMAND;	// 0x00
	cmd_buffer[1] = command;

	if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, &cmd, 2, HAL_MAX_DELAY) != HAL_OK) {
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
	if(HAL_I2C_Master_Transmit(&hsh1106.i2c, SH1106_ADDRESS_WRITE, data, len, HAL_MAX_DELAY) != HAL_OK) {
		return SH1106_STATUS_ERROR;
	}
	return SH1106_STATUS_OK;

}
