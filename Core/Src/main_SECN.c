/*
 * main_SECN.c
 *
 *  Created on: May 23, 2025
 *      Author: Dinesh Bobburu
 */


#include "main_SECN.h"
#include "led_task.h"
#include "button_task.h"
#include "app_resources.h"


// Enable the Cycle count
#define DWT_CTRL	(*(volatile uint32_t*)0xE0001000)

void SystemClock_Config(void);

extern  void SEGGER_UART_init(uint32_t);

uint32_t captureValues[2] = {0};
uint8_t capturedComplete = 0;

uint32_t capture_difference =0;
double timer2_cnt_freq=0;
double timer2_cnt_res=0;
double user_signal_time_period =0;
double user_signal_freq=0;


int main(void) {

	HAL_Init();
	SystemClock_Config();
	debug_uart_init();
	DWT_CTRL |= (1<<0);
	SEGGER_UART_init(1050000);
	SEGGER_SYSVIEW_Conf();
	app_resources_init();
	led_task_init();
	button_task_init();
	sensor_task_init();
	//timer_OC_Init();
	//HAL_TIM_OC_Start_IT(&hlptim, TIM_CHANNEL_2);
	vTaskStartScheduler();



}
//void SystemClock_Config(void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//  /** Configure the main internal regulator output voltage
//  */
//  __HAL_RCC_PWR_CLK_ENABLE();
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//
//  /** Initializes the RCC Oscillators according to the specified parameters
//  * in the RCC_OscInitTypeDef structure.
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLM = 8;
//  RCC_OscInitStruct.PLL.PLLN = 64;
//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//  RCC_OscInitStruct.PLL.PLLQ = 7;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//	  print_error_uart();
//  }
//
//  /** Initializes the CPU, AHB and APB buses clocks
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//	  print_error_uart();
//  }
//}
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

// This function increments global tick for every 1ms based on Timer 6 for HAL peripherals.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
//  if(htim->Instance == TIM7) {
//  		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
//  }


}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
//	  if(htim->Instance == TIM2) {
//
//			  if(!captureValues[0]) {
//				  captureValues[0] = __HAL_TIM_GET_COMPARE(&hlptim, TIM_CHANNEL_2);
//			  } else if(!captureValues[1] && (captureValues[0] != 0)) {
//				  captureValues[1] = __HAL_TIM_GET_COMPARE(&hlptim, TIM_CHANNEL_2);
//				  capturedComplete = 1;
//				  get_frequency_from_IC();
//			  }
//	  }
//}


void button_irq_handler(void) {
	BaseType_t	higher_priority_task_woken = pdFALSE;
	print_debug_msg("Button_Pressed \n");
	xSemaphoreGiveFromISR(xbutton_Sema, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);

}

//void get_frequency_from_IC(void) {
//	if(captureValues[0] <= captureValues[1]) {
//		capture_difference = captureValues[1] - captureValues[0];
//	} else {
//		capture_difference = (0xFFFFFFFF - captureValues[0]) + captureValues[1];
//	}
//
//	timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2 )/(hlptim.Init.Prescaler + 1);
//	timer2_cnt_res = 1/timer2_cnt_freq;
//	user_signal_time_period = capture_difference * timer2_cnt_res;
//	user_signal_freq = 1/user_signal_time_period;
//	print_debug_msg("Frequency of the signal applied = %.2f Hz\r\n",user_signal_freq);
//	//sprintf(usr_msg,"Frequency of the signal applied = %.2f Hz\r\n",user_signal_freq );
//
//	memset(captureValues, 0, sizeof(captureValues));
//	capturedComplete = 0;
//}

uint32_t get_PCK1(void) {
	if(((RCC->CFGR >> 10) & 0x7) > 1) {
		return HAL_RCC_GetPCLK1Freq()*2;
	}
	return HAL_RCC_GetPCLK1Freq();
}




