/*
 * timer_Low_Power_Mode.c
 *
 *  Created on: Jun 4, 2025
 *      Author: Dinesh Bobburu
 */

#include "main_SECN.h"

TIM_HandleTypeDef	hlptim;
TIM_HandleTypeDef	htim7;
RTC_HandleTypeDef	hrtc;

void timer_init_Basic(void) {

	htim7.Instance = TIM7;
	htim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 41;
	htim7.Init.Prescaler = 999;
	if(HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		print_debug_msg("Error Initializing Basic Tim 7 timer\n");
	}
}


void timer_init_LPM(void) {
	TIM_IC_InitTypeDef tim2_ic;

	hlptim.Instance = TIM2;
	hlptim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hlptim.Init.CounterMode = TIM_COUNTERMODE_UP;
	hlptim.Init.Period = 0xFFFFFFFF;
	hlptim.Init.Prescaler = 1;
	if(HAL_TIM_IC_Init(&hlptim) != HAL_OK) {
		print_debug_msg("Error Initializing timer 2\n");
	}
	tim2_ic.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2_ic.ICPrescaler = TIM_ICPSC_DIV1;
	tim2_ic.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim2_ic.ICFilter = 0;

	if(HAL_TIM_IC_ConfigChannel(&hlptim, &tim2_ic, TIM_CHANNEL_2) != HAL_OK) {
		print_debug_msg("Error Initializing Tim 2 IC\n");
	}


}

void timer_OC_Init(void) {
	TIM_OC_InitTypeDef	htimoc;
	hlptim.Instance = TIM2;
	hlptim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hlptim.Init.Prescaler = 1;
	hlptim.Init.Period = 0xFFFFFFFF;
	hlptim.Init.CounterMode = TIM_COUNTERMODE_UP;

	if(HAL_TIM_OC_Init(&hlptim)!= HAL_OK){
		print_debug_msg("Error Initializing timer 2\n");
	}
	htimoc.OCMode = TIM_OCMODE_TOGGLE;
	htimoc.Pulse = 42000;
	htimoc.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	if(HAL_TIM_OC_ConfigChannel(&hlptim, &htimoc, TIM_CHANNEL_2) != HAL_OK) {
		print_debug_msg("Error Initializing Tim 2 IC\n");
	}

}


void timer_PWM_Init(void) {
	TIM_OC_InitTypeDef	htimoc;
	hlptim.Instance = TIM2;
	hlptim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hlptim.Init.Prescaler = 41;
	hlptim.Init.Period = 999;
	hlptim.Init.CounterMode = TIM_COUNTERMODE_UP;

	if(HAL_TIM_PWM_Init(&hlptim)!= HAL_OK){
		print_debug_msg("Error Initializing timer 2\n");
	}

	htimoc.OCMode = TIM_OCMODE_PWM1;
	htimoc.Pulse = 0.75 * hlptim.Init.Period;
	htimoc.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	if(HAL_TIM_PWM_ConfigChannel(&hlptim, &htimoc, TIM_CHANNEL_2) != HAL_OK) {
		print_debug_msg("Error Initializing TIM 2 PWM\n");
	}

}

void rtc_init(void) {

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xF9;
	if(HAL_RTC_Init(&hrtc) != HAL_OK){
		print_debug_msg("Error Initializing RTC\n");
	}
}

void rtc_date_time_config(void) {
	RTC_TimeTypeDef	rtc_time;
	RTC_DateTypeDef rtc_date;

	rtc_time.TimeFormat = RTC_HOURFORMAT12_AM;
	rtc_time.Hours = 9;
	rtc_time.Minutes = 16;
	rtc_time.Seconds = 00;
	if(HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN) != HAL_OK) {
		print_debug_msg("Error Initializing RTC Date\n");
	}
	rtc_date.Date = 8;
	rtc_date.Month = RTC_MONTH_JUNE;
	rtc_date.Year = 25;
	rtc_date.WeekDay = RTC_WEEKDAY_SUNDAY;

	if(HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN) != HAL_OK) {
		print_debug_msg("Error Initializing RTC Date and Time\n");
	}

}
