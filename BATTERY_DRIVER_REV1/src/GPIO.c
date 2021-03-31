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

	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pin = GPIO_PIN_0;				// MEASURE 3.3V
	HAL_GPIO_Init(GPIOB, &gpio);

	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;				// IN0 -> 2.4V input voltage IN1 -> REF Voltage ->0.576
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Pin = RED_LED | PMOS_LOGIC | PMOS_DCDC | RED_ALARM; 			// red led output, SW_LOGIC_CTRL, SW_DR_DC_CTRL
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 	// jako wyjscie
	gpio.Pull = GPIO_NOPULL;			// rezystory podciagajace sa wylaczone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcza niskie czestotliwosci przelaczania
	HAL_GPIO_Init(GPIOC, &gpio); 		// inicjalizacja modulow GPIOA

	gpio.Mode = GPIO_MODE_AF_PP;	//alternatywna push-pull
	gpio.Pin = GPIO_PIN_2;			//TX
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;	//alternatywne wejscie
	gpio.Pin = GPIO_PIN_3;			//RX
	HAL_GPIO_Init(GPIOA, &gpio);
}

