/*
 * GPIO.c
 *
 *  Created on: 31.03.2021
 *      Author: david
 */


#include "GPIO.h"
#include "stm32f1xx.h"
GPIO_InitTypeDef gpio;

void GPIO_INIT(void){
	gpio.Pin = GREEN_LED | PMOS_STEP | BUZZER_ALARM; 			// green led output, SW_STEP_DR_CTRL
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 	// jako wyjscie
	gpio.Pull = GPIO_NOPULL;			// rezystory podciagajace sa wylaczone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcza niskie czestotliwosci przelaczania
	HAL_GPIO_Init(GPIOB, &gpio); 		// inicjalizacja modulow GPIOB



	gpio.Pin = RED_LED | PMOS_LOGIC | PMOS_DCDC | RED_ALARM; 			// red led output, SW_LOGIC_CTRL, SW_DR_DC_CTRL
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 	// jako wyjscie
	gpio.Pull = GPIO_NOPULL;			// rezystory podciagajace sa wylaczone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcza niskie czestotliwosci przelaczania
	HAL_GPIO_Init(GPIOC, &gpio); 		// inicjalizacja modulow GPIOA


}

