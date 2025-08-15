/*
 * sensor_task.c
 *
 *  Created on: May 24, 2025
 *      Author: Dinesh Bobburu
 */


#include "uart_async.h"
#include "sensor_task.h"

uart_async_t sensor_uart_async;
#define MAX_MESSAGE_AGE_MS		150
void sensor_task(void *args) {

	uart_async_init(&sensor_uart_async, &huart1_sensor);
	uart_async_enable_irqs(&sensor_uart_async);
	TickType_t currentTime = xTaskGetTickCount();
	TickType_t next_sample_time = currentTime + pdMS_TO_TICKS(100);	// 100 ms
	uint8_t line[256]; // scraping line for emptying space in ring buf
	uint8_t last_stable_line[256] = {0}; // last stable line
	uint32_t last_stable_line_tc = 0;	// last stable line time stamp
	uint32_t last_sent_ts = 0;			// time stamp of last sent line
	bool have_last = false;



	while(1) {
		TickType_t now = xTaskGetTickCount();
		TickType_t timeout = (next_sample_time > now) ? next_sample_time - now : 0;
		xSemaphoreTake(sensor_uart_async.uart_semaHandle, timeout);
		bool is_CheckSum_OK = false;
		// Drain everything in current ring.
		while(uart_async_getline(&sensor_uart_async, line, (uint16_t)sizeof(line), &is_CheckSum_OK)) {
			if(is_CheckSum_OK) {
				strncpy(last_stable_line, line, sizeof(last_stable_line)-1);
				last_stable_line_tc = xTaskGetTickCount();
				have_last = true;

			} else {
				// bad checksum ignore
			}
		}
		now = xTaskGetTickCount();
		if(now >= next_sample_time) {
			// post it to queue TODO: this will be handled
			uint32_t publish_ts = now;
			bool new_since_last = have_last & (last_stable_line_tc > last_sent_ts);
			uint32_t age = new_since_last ? (publish_ts - last_sent_ts) : 0xFFFFFFFFU;
			sensor_data_t staged_line = {0};
			staged_line.msg_type = MSG_SENSOR_SAMPLE;
			staged_line.age_ms = age;
			if(new_since_last && age <= MAX_MESSAGE_AGE_MS) {
				strncpy(staged_line.line, (char*)last_stable_line, (sizeof(staged_line.line))-1);
			    staged_line.line[sizeof(staged_line.line)-1] = '\0';
			} else {
				// LOG error packet
				staged_line.line[0] = '\0';
			}
			if(xQueueSend(qSensorToLogger, &staged_line, 0) != pdTRUE) {
				//TODO: Handle queue error
			}
			next_sample_time += pdMS_TO_TICKS(100);
		}
	}
}





















// TODO: ****************** LEGACY*********************8

/*
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
*/
