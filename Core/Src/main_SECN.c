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





int main(void) {

	HAL_Init();
	SystemClock_Config();
	debug_uart_init();
	app_resources_init();
	led_task_init();
	button_task_init();
	DWT_CTRL |= (1<<0);
	SEGGER_UART_init(200000);
	SEGGER_SYSVIEW_Conf();
	vTaskStartScheduler();



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
  RCC_OscInitStruct.PLL.PLLN = 64;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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




