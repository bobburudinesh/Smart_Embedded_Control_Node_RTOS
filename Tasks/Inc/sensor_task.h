/*
 * sensor_task.h
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_SENSOR_TASK_H_
#define INC_SENSOR_TASK_H_

void sensor_task_init(void);

void v_sensor_task(void *pvParamerter);
void v_buzzer_task(void *pvParamerter);

void vSensor_Timer_Callback( TimerHandle_t xTimer );

TaskHandle_t get_sensor_task_handle(void);
TaskHandle_t get_buzzer_task_handle(void);

#endif /* INC_SENSOR_TASK_H_ */
