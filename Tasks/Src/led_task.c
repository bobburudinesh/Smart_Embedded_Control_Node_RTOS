/*
 * led_task.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"
#include "led_task.h"

static void GPIO_Init_Led_Task(void);

static TaskHandle_t		led_task_green_handle;
static TaskHandle_t		led_task_orange_handle;

BaseType_t status;

// This creates LED_TASKS for Green and Orange LED's
void led_task_init(void) {
	GPIO_Init_Led_Task();
	status = xTaskCreate(v_led_task_green, "LED_GREEN", 100, NULL, 1, &led_task_green_handle);
	configASSERT_RTOS(status == pdPASS, "Led Green Task Create");

	status = xTaskCreate(v_led_task_orange, "LED_ORANGE", 100, NULL, 1, &led_task_orange_handle);
	configASSERT_RTOS(status == pdPASS, "Led Green Task Create");
}

//This function is task handler for Green LED
void v_led_task_green(void *pvParameters) {
	while(1) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

//This function is task handler for Orange LED
void v_led_task_orange(void *pvParameters)	 {
	while(1) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

// Function that Handles Generic GPIO Initializations
static void GPIO_Init_Led_Task(void) {

	GPIO_InitTypeDef	gpiox;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	gpiox.Mode = GPIO_MODE_OUTPUT_PP;
	gpiox.Pull = GPIO_NOPULL;
	gpiox.Speed = GPIO_SPEED_LOW;
	gpiox.Pin = GPIO_PIN_12 | GPIO_PIN_13;

	HAL_GPIO_Init(GPIOD, &gpiox);

	gpiox.Mode = GPIO_MODE_IT_RISING_FALLING;
	gpiox.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &gpiox);

}
