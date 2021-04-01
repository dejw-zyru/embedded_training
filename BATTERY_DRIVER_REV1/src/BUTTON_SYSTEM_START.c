/*
 * BUTTON_SYSTEM_START.c
 *
 *  Created on: 01.04.2021
 *      Author: david
 */
#include "stm32f1xx.h"
#include "BUTTON_SYSTEM_START.h"

//start prztwornicy - zasilania calego systemu
GPIO_InitTypeDef gpio;

void SystemStart(void){
	gpio.Pin = CTRL_DC_DC;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &gpio);
	HAL_GPIO_WritePin(GPIOC, CTRL_DC_DC, GPIO_PIN_SET);
}
