/*
 * led_task.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"
#include "led_task.h"
#include "app_resources.h"

static void GPIO_Init_Led_Task(void);

TaskHandle_t		led_task_green_handle;
TaskHandle_t		led_task_orange_handle;

static BaseType_t status;

// This creates LED_TASKS for Green and Orange LED's
void led_task_init(void) {
	GPIO_Init_Led_Task();
	status = xTaskCreate(v_led_task_green, "LED_GREEN", TASK_STACK_SIZE, NULL, 1, &led_task_green_handle);
	configASSERT_RTOS(status == pdPASS, "Led Green Task Create");

	status = xTaskCreate(v_led_task_orange, "LED_ORANGE", TASK_STACK_SIZE, NULL, 1, &led_task_orange_handle);
	configASSERT_RTOS(status == pdPASS, "Led Green Task Create");
}

//This function is task handler for Green LED
void v_led_task_green(void *pvParameters) {
	uint8_t	data = 0;
	while(1) {
		if(xQueueReceive(xSensor_Queue, (void*)&data, portMAX_DELAY)) {
			data = data % 2;
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, data);
		}

	}
}

//This function is task handler for Orange LED
void v_led_task_orange(void *pvParameters)	 {
	EventBits_t	event_bits;
	while(1) {
		event_bits = xEventGroupWaitBits(xSensor_Button_Event_Group, (BIT_1 | BIT_0), pdTRUE, pdFALSE, 0);
		if(event_bits) {
			print_debug_msg("Event Bits: %lu\n", (uint32_t)event_bits);
		}


		if(event_bits & BIT_0) {
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		}
		if((event_bits & (BIT_1 | BIT_0)) == (BIT_1 | BIT_0)) {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
			vTaskDelay(pdMS_TO_TICKS(100));
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		}

//		if((event_bits & (BIT_1 | BIT_0)) == (BIT_1 | BIT_0)) {
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
//			vTaskDelay(pdMS_TO_TICKS(100));
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
//		}

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
	gpiox.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOD, &gpiox);

	gpiox.Mode = GPIO_MODE_IT_RISING_FALLING;
	gpiox.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &gpiox);

}

// returns task handle of led_task_green
TaskHandle_t get_led_task_green_handle(void) {
	return led_task_green_handle;
}

// returns task handle of led_task_orange
TaskHandle_t get_led_task_orange_handle(void) {
	return led_task_orange_handle;
}
