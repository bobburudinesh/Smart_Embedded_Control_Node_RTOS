/*
 * it_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#include "main.h"

extern UART_HandleTypeDef	huart3;

extern TIM_HandleTypeDef htim6;


void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);
}

void USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart3);
}

