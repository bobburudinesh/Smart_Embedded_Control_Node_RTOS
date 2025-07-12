/*
 * tickless_rtc.c
 *
 *  Created on: Jul 10, 2025
 *      Author: Dinesh Bobburu
 */


#include "tickless_rtc.h"

extern void SystemClock_Config(void);

RTC_HandleTypeDef	hrtc;

void vPortSupressTicksAndSleep(TickType_t expected_idle_time) {
	if(expected_idle_time < configEXPECTED_IDLE_TIME_BEFORE_SLEEP) {
		return;
	}
	uint32_t sleep_time_ms = expected_idle_time * portTICK_PERIOD_MS;
	uint32_t rtc_wakeup_counter = sleep_time_ms / 30;	//LSI ~32KHz
	if(rtc_wakeup_counter > 0xFFFF) {
		rtc_wakeup_counter = 0xFFFF;
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	HAL_SuspendTick();

	// Clear wakeup flags
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	//set RTC Wakeup timer
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, rtc_wakeup_counter, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

	//Enter STOP Mode
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	//wakeup here
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	SystemClock_Config();
	HAL_ResumeTick();
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void LPM_RTC_Init(void) {
	RCC_OscInitTypeDef rcc_osc_Init = {0};
	RCC_PeriphCLKInitTypeDef rcc_peri_clk_Init = {0};
	rcc_osc_Init.OscillatorType = RCC_OSCILLATORTYPE_LSI;
	rcc_osc_Init.LSIState = RCC_LSI_ON;
	rcc_osc_Init.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig(&rcc_osc_Init);

	rcc_peri_clk_Init.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	rcc_peri_clk_Init.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	HAL_RCCEx_PeriphCLKConfig(&rcc_peri_clk_Init);

	__HAL_RCC_RTC_ENABLE();

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	HAL_RTC_Init(&hrtc);

}
