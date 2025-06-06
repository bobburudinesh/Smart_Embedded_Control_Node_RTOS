/*
 * app_resources.c
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */


#include "app_resources.h"
#include "sensor_task.h"

#define SENSOR_QUEUE_LENGTH 	1




SemaphoreHandle_t	xbutton_Sema;	// Semaphore to synchronize button ISR to button task

Tasks_t current_task;

TaskHandle_t next_task_to_delete;

QueueHandle_t	xSensor_Queue;

TimerHandle_t	sensor_timer;

TimerHandle_t	heartbeat_timer;

EventGroupHandle_t	xSensor_Button_Event_Group;



void app_resources_init(void) {
	BaseType_t xTimerStartResult;
	xbutton_Sema = xSemaphoreCreateBinary();
	if(!xbutton_Sema) {
		print_error_uart();
	}
	xSensor_Queue = xQueueCreate(SENSOR_QUEUE_LENGTH, sizeof(uint8_t));
	if(!xSensor_Queue) {
		print_error_uart();
	}
	heartbeat_timer = xTimerCreate("Heartbeat_Timer", pdMS_TO_TICKS(1000), pdTRUE, (void*)0, vHeartbeat_Timer_Callback);

	xTimerStartResult = xTimerStart(heartbeat_timer, 10);
	configASSERT_RTOS(xTimerStartResult == pdPASS, "Failed to start Heartbeat Timer\n");
	sensor_timer = xTimerCreate("Sensor_Timer", pdMS_TO_TICKS(2000), pdTRUE, (void *)1, vSensor_Timer_Callback);
	xTimerStartResult = xTimerStart(sensor_timer, 10);
	configASSERT_RTOS(xTimerStartResult == pdPASS, "Failed to start Sensor Timer\n");
	xSensor_Button_Event_Group = xEventGroupCreate();
	if(!xSensor_Button_Event_Group) {
		print_error_uart();
	}


}

void vSensor_Timer_Callback( TimerHandle_t xTimer ) {
	TaskHandle_t sensor_task=  get_sensor_task_handle();
	if(!sensor_task) {
		print_error_uart();
	}
	xTaskNotifyGive(sensor_task);
	xEventGroupSetBits(xSensor_Button_Event_Group, BIT_0);

}

void vHeartbeat_Timer_Callback( TimerHandle_t xTimer ) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
}

