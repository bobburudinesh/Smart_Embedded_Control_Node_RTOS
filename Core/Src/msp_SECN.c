/*
 * msp_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main.h"


void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	vInitPrioGroupValue();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
// Low Level Initialization for UART Peripheral
	if(huart->Instance == USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	    __HAL_RCC_GPIOD_CLK_ENABLE();
		GPIO_InitTypeDef	gpiox;
		gpiox.Mode = GPIO_MODE_AF_PP;
		gpiox.Pull = GPIO_NOPULL;
		gpiox.Speed = GPIO_SPEED_FREQ_HIGH;
		gpiox.Alternate = GPIO_AF7_USART3;
		gpiox.Pin = GPIO_PIN_11;
		HAL_GPIO_Init(GPIOB, &gpiox);

		gpiox.Pin = GPIO_PIN_8;
		HAL_GPIO_Init(GPIOD, &gpiox);
		HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);

	} else if (huart->Instance == USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef	gpiox;
		gpiox.Mode = GPIO_MODE_AF_PP;
		gpiox.Pull = GPIO_NOPULL;
		gpiox.Speed = GPIO_SPEED_FREQ_HIGH;
		gpiox.Alternate = GPIO_AF7_USART3;
		gpiox.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &gpiox);
		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}
