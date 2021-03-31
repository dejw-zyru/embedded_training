/*
 * CLOCK_ENABLE.c
 *
 *  Created on: 31.03.2021
 *      Author: david
 */

#include "stm32f1xx.h"
#include "CLOCK_ENABLE.h"

void ClockEnable(void){
	//enable ports
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	//enable usart
	__HAL_RCC_USART2_CLK_ENABLE();
	//enable ADC1
	__HAL_RCC_ADC1_CLK_ENABLE();
	//clock tim2
	__HAL_RCC_TIM2_CLK_ENABLE();
	//DMA clock enable
	__HAL_RCC_DMA1_CLK_ENABLE();
}

