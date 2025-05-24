/*
 * button_task.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"
#include "button_task.h"
#include "app_resources.h"
#include "led_task.h"



TaskHandle_t	button_task_handle;

static BaseType_t		status;

static void button_gpio_init(void) {
	GPIO_InitTypeDef	gpiox;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	gpiox.Pull = GPIO_NOPULL;
	gpiox.Speed = GPIO_SPEED_LOW;
	gpiox.Mode = GPIO_MODE_IT_RISING;
	gpiox.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &gpiox);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void button_task_init(void) {
	button_gpio_init();
	status = xTaskCreate(v_button_task_handler, "Button_Task", 100, NULL, 2, &button_task_handle);
	configASSERT_RTOS(status == pdPASS, "Button Task Init failed\n");
}



void v_button_task_handler(void *pvParameters) {
	while(1) {
		if (xSemaphoreTake(xbutton_Sema,portMAX_DELAY) == pdTRUE) {
			TaskHandle_t	led_orange_task = get_led_task_orange_handle();
			vTaskSuspend(led_orange_task);
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)) {
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			}
			vTaskDelay(pdMS_TO_TICKS(5000));
			vTaskResume(led_orange_task);
		}
	}
}

// return task handle of button_task
TaskHandle_t get_button_task_handle(void) {
	return button_task_handle;
}
