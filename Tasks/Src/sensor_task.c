/*
 * sensor_task.c
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */

#include "main_SECN.h"
#include "app_resources.h"
#include "sensor_task.h"

TIM_HandleTypeDef htim2;

TaskHandle_t	sensor_task_handle;

TaskHandle_t	buzzer_task_handle;

TimerHandle_t	sensor_timer;

BaseType_t	status;

extern uint8_t latest_distance;

void sensor_task_init(void) {
	BaseType_t xTimerStartResult;
	status = xTaskCreate(v_sensor_task, "Sensor_Task", TASK_STACK_SIZE, NULL, 2, &sensor_task_handle);
	configASSERT_RTOS(status == pdPASS, "Sensor Task Init Failed\n");

	status = xTaskCreate(v_buzzer_task, "Buzzer_Task", TASK_STACK_SIZE, NULL, 2, &buzzer_task_handle);
	configASSERT_RTOS(status == pdPASS, "Buzzer Task Init Failed\n");

	Timer2_Init();

	sensor_timer = xTimerCreate("Sensor_Timer", pdMS_TO_TICKS(100), pdTRUE, (void *)1, vSensor_Timer_Callback);
	xTimerStartResult = xTimerStart(sensor_timer, 10);
	configASSERT_RTOS(xTimerStartResult == pdPASS, "Failed to start Sensor Timer\n");
}

void v_sensor_task(void *pvParamerter) {
	uint8_t distance;
	while(1) {
//		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//		data = generate_sensor_data();
//		if(xQueueSend(xSensor_Queue,(void *)&data,pdMS_TO_TICKS(200)) != pdPASS) {
//			print_error_uart();
//		}
//		print_debug_msg("Data Sent ON Queue: %u\n", data);
		ulTaskGenericNotifyTake(0, pdTRUE, portMAX_DELAY);
		taskENTER_CRITICAL();
		distance = latest_distance;
		taskEXIT_CRITICAL();
		vTaskDelay(pdMS_TO_TICKS(20));
		// Call display
		print_debug_msg("Distance: %u cm\n", distance);
	}
}



TaskHandle_t get_sensor_task_handle(void) {
	return sensor_task_handle;
}

TaskHandle_t get_buzzer_task_handle(void) {
	return buzzer_task_handle;
}





void v_buzzer_task(void *pvParamerter) {
	uint8_t distance;
	while(1) {
		ulTaskGenericNotifyTake(0, pdTRUE, portMAX_DELAY);
		taskENTER_CRITICAL();
		distance = latest_distance;
		taskEXIT_CRITICAL();
		if(distance >= 30) {
			continue;
		} else if (distance >=20 && distance <30) {
			beep(250);
		} else if (distance >=10 && distance <20) {
			beep(125);
		} else if (distance >=0 && distance <10) {
			beep(50);
		}
	}
}



void beep(TickType_t delay) {
	for(uint8_t i = 0;i<5; i++) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS(delay));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		vTaskDelay(pdMS_TO_TICKS(delay));
	}

}

void Timer2_Init(void) {

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 1;
	htim2.Init.Period = 0xFFFFFFFF;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_IC_Init(&htim2) != HAL_OK) {
		print_error_uart();
	}
	TIM_IC_InitTypeDef tim2_IC_Config;
	tim2_IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2_IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	tim2_IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim2_IC_Config.ICFilter = 0;
	if(HAL_TIM_IC_ConfigChannel(&htim2, &tim2_IC_Config, TIM_CHANNEL_2) != HAL_OK) {
		print_error_uart();
	}
	if(HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2) != HAL_OK) {
		print_error_uart();
	}
}
void ultraSonic_Setup(void) {
	GPIO_InitTypeDef  gpio;
    __HAL_RCC_GPIOC_CLK_ENABLE();
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_LOW;
	gpio.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOC, &gpio);
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

void distance_Read(void) {
	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	//HAL_Delay(10);
	uint32_t current_TickCount = HAL_GetTick();
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	while((HAL_GetTick() - current_TickCount) < 10);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC2);
}

void vSensor_Timer_Callback( TimerHandle_t xTimer ) {

	distance_Read();

}

