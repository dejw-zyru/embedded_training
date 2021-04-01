/*
 * TIM2_Handler.c
 *
 *  Created on: 01.04.2021
 *      Author: david
 */

#include "stm32f1xx.h"
#include "TIM2_HANDLER.h"

TIM_HandleTypeDef tim2;

void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&tim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOB, GREEN_LED); // toggle green led
	HAL_GPIO_TogglePin(GPIOC, RED_LED);	// red led*/

	HAL_GPIO_WritePin(GPIOB, BUZZER_ALARM, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, RED_ALARM, GPIO_PIN_SET);

}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
	switch (htim->Channel){
	case HAL_TIM_ACTIVE_CHANNEL_1:
		HAL_GPIO_WritePin(GPIOB, BUZZER_ALARM, GPIO_PIN_RESET);
		break;
	case HAL_TIM_ACTIVE_CHANNEL_2:
		HAL_GPIO_WritePin(GPIOC, RED_ALARM, GPIO_PIN_RESET);
		break;
	default:
		break;
	}

}
