/*
 * app_resources.h
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_APP_RESOURCES_H_
#define INC_APP_RESOURCES_H_

#include "main_SECN.h"



#define BIT_0	( 1 << 0 )
#define BIT_1	( 1 << 1 )


typedef enum {
	idle_Task = 0,
	Led_Green_Task,
	Led_Orange_Task,
	Button_Task
}Tasks_t;

typedef enum {
	SYS_STATE_INIT = 0,
	SYS_STATE_RUNNING,
	SYS_STATE_ERROR,
	SYS_STATE_RESET
} system_state_t;



void app_resources_init(void);

extern SemaphoreHandle_t	xbutton_Sema;

extern QueueHandle_t	xSensor_Queue;

extern TaskHandle_t next_task_to_delete;

extern EventGroupHandle_t	xSensor_Button_Event_Group;

void vHeartbeat_Timer_Callback( TimerHandle_t xTimer );
void vSensor_Timer_Callback( TimerHandle_t xTimer );




extern Tasks_t current_task;



#endif /* INC_APP_RESOURCES_H_ */
