/*
 * it_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

#include "main_SECN.h"
#include "app_resources.h"
#include "uart_async.h"

extern UART_HandleTypeDef	huart3;

//extern TIM_HandleTypeDef	htim2;


extern TIM_HandleTypeDef htim6;
//extern void button_irq_handler(void);
extern uart_async_t sensor_uart_async;
void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);
}

void USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart3);
}


void USART1_IRQHandler(void) {
	BaseType_t highPriorityTaskWoken = pdFALSE;
	if(__HAL_UART_GET_FLAG(&huart1_sensor, UART_FLAG_RXNE)) {
			uart_async_isr_byte(&sensor_uart_async, &highPriorityTaskWoken);
	}
	portYIELD_FROM_ISR(highPriorityTaskWoken);
}


void USART6_IRQHandler(void) {
	HAL_UART_IRQHandler(&huart6_modem);
}

void EXTI0_IRQHandler(void) {
	//button_irq_handler();
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

//void TIM2_IRQHandler(void) {
//	HAL_TIM_IRQHandler(&htim2);
//}


