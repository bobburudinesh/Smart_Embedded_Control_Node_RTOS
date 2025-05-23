/*
 * it_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#include "main.h"

extern UART_HandleTypeDef	huart3;

void USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart3);
}

