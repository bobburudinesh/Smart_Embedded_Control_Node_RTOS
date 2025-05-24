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
static uint8_t generate_sensor_data(void);

#endif /* INC_SENSOR_TASK_H_ */
