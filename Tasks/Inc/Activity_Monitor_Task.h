/*
 * Activity_Monitor_Task.h
 *
 *  Created on: Jul 11, 2025
 *      Author: Dinesh Bobburu
 */

#ifndef INC_ACTIVITY_MONITOR_TASK_H_
#define INC_ACTIVITY_MONITOR_TASK_H_

#include "main_SECN.h"
#include "app_resources.h"


extern IWDG_HandleTypeDef hiwdg;

void IWDGT_Init(void);

void activity_Monitor_Task_Init(void);

void activity_Monitor_Task(void *pParameters);

#endif /* INC_ACTIVITY_MONITOR_TASK_H_ */
