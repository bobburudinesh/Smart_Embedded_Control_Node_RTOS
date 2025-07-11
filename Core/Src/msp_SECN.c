/*
 * msp_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"



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


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {


	if(htim->Instance == TIM7) {
			__HAL_RCC_TIM7_CLK_ENABLE();
			HAL_NVIC_SetPriority(TIM7_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(TIM7_IRQn);
		}
}
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef	gpiox;
		gpiox.Mode = GPIO_MODE_AF_PP;
		gpiox.Alternate = GPIO_AF1_TIM2;
		gpiox.Pin = GPIO_PIN_1;
		HAL_GPIO_Init(GPIOA, &gpiox);
		HAL_NVIC_SetPriority(TIM2_IRQn, 14, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}


void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef	gpiox;
		gpiox.Mode = GPIO_MODE_AF_PP;
		gpiox.Alternate = GPIO_AF1_TIM2;
		gpiox.Pin = GPIO_PIN_1;
		HAL_GPIO_Init(GPIOA, &gpiox);
		HAL_NVIC_SetPriority(TIM2_IRQn, 14, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef	gpiox;
		gpiox.Mode = GPIO_MODE_AF_PP;
		gpiox.Alternate = GPIO_AF1_TIM2;
		gpiox.Pin = GPIO_PIN_1;
		HAL_GPIO_Init(GPIOA, &gpiox);
		HAL_NVIC_SetPriority(TIM2_IRQn, 14, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc) {
	RCC_OscInitTypeDef	oscInit;
	RCC_PeriphCLKInitTypeDef	rcc_rtc_peri_clk_init;
	oscInit.OscillatorType = RCC_OSCILLATORTYPE_LSI;
	oscInit.LSIState = RCC_LSI_ON;
	oscInit.PLL.PLLState = RCC_PLL_NONE;
	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK){
		print_debug_msg("Error Initializing RTC Oscillator \n");
	}
	rcc_rtc_peri_clk_init.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	rcc_rtc_peri_clk_init.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if(HAL_RCCEx_PeriphCLKConfig(&rcc_rtc_peri_clk_init) != HAL_OK){
		print_debug_msg("Error Initializing RTC Clock \n");
	}

	__HAL_RCC_RTC_ENABLE();
}
