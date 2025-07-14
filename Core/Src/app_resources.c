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



TimerHandle_t	heartbeat_timer;

EventGroupHandle_t	xSensor_Button_Event_Group;

QueueHandle_t	xSensor_Queue_Ultrasonic;




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

	heartbeat_timer = xTimerCreate("Heartbeat_Timer", pdMS_TO_TICKS(100), pdTRUE, (void*)0, vHeartbeat_Timer_Callback);

	xTimerStartResult = xTimerStart(heartbeat_timer, 10);
	configASSERT_RTOS(xTimerStartResult == pdPASS, "Failed to start Heartbeat Timer\n");

	xSensor_Button_Event_Group = xEventGroupCreate();
	if(!xSensor_Button_Event_Group) {
		print_error_uart();
	}


}


void vHeartbeat_Timer_Callback( TimerHandle_t xTimer ) {
	//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
}

