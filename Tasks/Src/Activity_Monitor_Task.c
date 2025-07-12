/*
 * Activity_Monitor_Task.c
 *
 *  Created on: Jul 11, 2025
 *      Author: Dinesh Bobburu
 */


#include "Activity_Monitor_Task.h"

IWDG_HandleTypeDef hiwdg;
TaskHandle_t activity_monitor_task_handle;

void IWDGT_Init(void) {
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
	hiwdg.Init.Prescaler = 1999;
	if(HAL_IWDG_Init(&hiwdg) != HAL_OK) {
		print_error_uart();
	}

}


void activity_Monitor_Task_Init(void) {
	BaseType_t status;
	status = xTaskCreate(&activity_Monitor_Task, "ACT_MON_TASK", TASK_STACK_SIZE, NULL, 2, &activity_monitor_task_handle);
	configASSERT_RTOS(status == pdPASS, "Failed to Init Activity Monitor Task\r\n");
}

void activity_Monitor_Task(void *pParameters) {
	system_state_t system_state = SYS_STATE_INIT;
	while(1) {
		switch(system_state){
		 case SYS_STATE_INIT:
			 print_debug_msg("System Init...\r\n");
			 system_state = SYS_STATE_RUNNING;
			 break;
		 case SYS_STATE_RUNNING:
			 HAL_IWDG_Refresh(&hiwdg);
			 print_debug_msg("System Monitering...\r\n");
			 vTaskDelay(pdMS_TO_TICKS(1000));
			 break;
		 case SYS_STATE_ERROR:
			 print_debug_msg("Error Occurred...\r\n");
			 for(int i = 0; i<10; i++) {
				 HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				 vTaskDelay(pdMS_TO_TICKS(100));

			 }
			 break;
		 case SYS_STATE_RESET:
			 NVIC_SystemReset();
			 break;
		}
	}
}
