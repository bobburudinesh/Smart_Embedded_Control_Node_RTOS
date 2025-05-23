/*
 * debug_uart.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"

UART_HandleTypeDef	huart3;

void debug_uart_init(void) {
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	if(HAL_UART_Init(&huart3) != HAL_OK) {
		print_error_uart();
	}


}

void print_debug_msg(const char *message,...) {
	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, message);
	vsprintf(str, message,args);
	HAL_UART_Transmit(&huart3,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);

}
