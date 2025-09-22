/*
 * main_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */

/*
#include "main_SECN.h"
#include "led_task.h"
#include "button_task.h"
#include "app_resources.h"
#include "sensor_task.h"
#include "Activity_Monitor_Task.h"
*/
#include "main_SECN.h"
#include "app_resources.h"
#include "sensor_task.h"
#include "logger_task.h"
#include "crypto_task.h"
#include "modem_task.h"
#include "debug_uart.h"


extern TIM_HandleTypeDef htim2;


QueueHandle_t qSensorToLogger;
QueueHandle_t qLoggerToCrypto;
QueueHandle_t qCryptoToModem;

UART_HandleTypeDef huart1_sensor;
UART_HandleTypeDef huart6_modem;




// Enable the Cycle count
#define DWT_CTRL	(*(volatile uint32_t*)0xE0001000)

void SystemClock_Config(void);

#if DEBUG_ENABLE
extern  void SEGGER_UART_init(uint32_t);
#endif

TaskHandle_t	hSensor_task;


int main(void) {
	HAL_Init();

	SystemClock_Config();

	uart_debug_init();
	uart_sensor_init();
	uart_modem_init();

	qSensorToLogger = xQueueCreate(32, sizeof(sensor_data_t));
	qLoggerToCrypto = xQueueCreate(16, sizeof(logger_packet_t));
	qCryptoToModem = xQueueCreate(16, sizeof(tx_payload_t));

	if(xTaskCreate(sensor_task, "SENSOR", 512, NULL, 4, &hSensor_task) != pdPASS) LOGE("SENSOR task create failed");
	if(xTaskCreate(logger_task, "LOGGER", 512, NULL, 4, &hSensor_task) != pdPASS) LOGE("LOGGER task create failed");
	if(xTaskCreate(modem_task, "MODEM",   512, NULL, 5, &hSensor_task) != pdPASS) LOGE("MODEM task create failed");
	if(xTaskCreate(crypto_task, "CRYPTO", 512, NULL, 3, &hSensor_task) != pdPASS) LOGE("CRYPTO task create failed");


	xPortStartScheduler();
	LOGE("Scheduler returned");
	while(1){}


}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  print_error_uart();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
	  print_error_uart();
  }
}





void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
	while(1);
}

void vApplicationMallocFailedHook(void) {
	while(1);
}

void MemManage_Handler(void) {
	while(1);
}

void BusFault_Handler(void) {
	while(1);
}

void UsageFault_Handler(void) {
	while(1);
}


void uart_sensor_init(void) {
	huart1_sensor.Instance = USART1;
	huart1_sensor.Init.BaudRate = 9600;
	huart1_sensor.Init.StopBits = UART_STOPBITS_1;
	huart1_sensor.Init.WordLength = UART_WORDLENGTH_8B;
	huart1_sensor.Init.Parity = UART_PARITY_NONE;
	huart1_sensor.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart1_sensor) != HAL_OK) {
		//TODO: Handle error
		while(1);
	}

}


void uart_modem_init(void) {

	huart6_modem.Instance = USART6;
	huart6_modem.Init.BaudRate = 9600;
	huart6_modem.Init.StopBits = UART_STOPBITS_1;
	huart6_modem.Init.WordLength = UART_WORDLENGTH_8B;
	huart6_modem.Init.Parity = UART_PARITY_NONE;
	huart6_modem.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart6_modem) != HAL_OK) {
		//TODO: Handle error
		while(1);
	}

}

























//TODO: LEGACY CODE REMOVE

/*
uint32_t captureValues[2] = {0};
uint8_t capturedComplete = 0;

uint32_t capture_difference =0;
double timer2_cnt_freq=0;
double timer2_cnt_res=0;
double user_signal_time_period =0;
double user_signal_freq=0;

bool highCapture = true;
uint32_t risingedge_Value = 0;
uint32_t fallingedge_value = 0;
uint32_t pulse_Width_Counter_Difference = 0;
uint8_t latest_distance = 0;


int main(void) {

	HAL_Init();
	SystemClock_Config();
	debug_uart_init();
	DWT_CTRL |= (1<<0);
#if DEBUG_ENABLE
	SEGGER_UART_init(1050000);
	SEGGER_SYSVIEW_Conf();
#endif
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
	    print_debug_msg("System restarted from WDT reset!\n");
	    __HAL_RCC_CLEAR_RESET_FLAGS();
	}
	//IWDGT_Init();
	app_resources_init();
	//led_task_init();
	//button_task_init();
	sensor_task_init();
	//ultraSonic_Setup();
	//activity_Monitor_Task_Init();
	vTaskStartScheduler();

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  print_error_uart();
  }

  // Initializes the CPU, AHB and APB buses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
	  print_error_uart();
  }
}

// This function increments global tick for every 1ms based on Timer 6 for HAL peripherals.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }

}




void button_irq_handler(void) {
	BaseType_t	higher_priority_task_woken = pdFALSE;
	print_debug_msg("Button_Pressed \n");
	xSemaphoreGiveFromISR(xbutton_Sema, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);

}










void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	BaseType_t	higher_priority_task_woken = pdFALSE;
	if(htim->Instance == TIM2) {
		if(HAL_TIM_GetActiveChannel(&htim2) == HAL_TIM_ACTIVE_CHANNEL_2) {
			if(highCapture) {
				// Read Capture Compare Register Value when rising edge of pulse is received
				risingedge_Value = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
				highCapture = false;
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_2, TIM_ICPOLARITY_FALLING);
			} else {
				// Read Capture Compare Register Value when falling edge of pulse is received
				fallingedge_value = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
				__HAL_TIM_SET_COUNTER(&htim2, 0);
				if(fallingedge_value > risingedge_Value) {
					pulse_Width_Counter_Difference = fallingedge_value - risingedge_Value;
				} else {
					pulse_Width_Counter_Difference = (0xFFFF - risingedge_Value) + fallingedge_value;
				}
				// Assuming TIM2 clock is 42 MHz, speed of sound is 340 m/s or 34000 cm/s
				float time_in_seconds = pulse_Width_Counter_Difference / 42000000.0;
				latest_distance = (time_in_seconds * 34000) / 2;
				vTaskNotifyGiveIndexedFromISR(get_sensor_task_handle(),(UBaseType_t)0, &higher_priority_task_woken);
				portYIELD_FROM_ISR(higher_priority_task_woken);
				vTaskNotifyGiveIndexedFromISR(get_buzzer_task_handle(),(UBaseType_t)0, &higher_priority_task_woken);
				portYIELD_FROM_ISR(higher_priority_task_woken);
				highCapture = true;
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_2, TIM_ICPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC2);
			}
			}
		}
}




void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
	print_error_uart();
}

void vApplicationMallocFailedHook(void) {
	print_error_uart();
}

void MemManage_Handler(void) {
	while(1);
}

void BusFault_Handler(void) {
	while(1);
}

void UsageFault_Handler(void) {
	while(1);
}


*/
