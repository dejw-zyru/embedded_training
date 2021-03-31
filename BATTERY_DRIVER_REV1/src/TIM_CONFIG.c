/*
 * TIM_CONFIG.c
 *
 *  Created on: 31.03.2021
 *      Author: david
 */

#include "stm32f1xx.h"
#include "TIM_CONFIG.h"

TIM_HandleTypeDef tim2;

void TimConfig(void){
	tim2.Instance = TIM2;
	tim2.Init.Period = 5000 - 1;
	tim2.Init.Prescaler = 8000 - 1;
	tim2.Init.ClockDivision = 0;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;	//zlicza w gore
	tim2.Init.RepetitionCounter = 0;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&tim2);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start_IT(&tim2);

	/*set compare channel*/
	__HAL_TIM_SET_COMPARE(&tim2, TIM_CHANNEL_1,500);
	__HAL_TIM_SET_COMPARE(&tim2, TIM_CHANNEL_2,900);
	__HAL_TIM_ENABLE_IT(&tim2, TIM_IT_CC1 | TIM_IT_CC2);
}

