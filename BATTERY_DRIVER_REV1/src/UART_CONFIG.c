/*
 * UART.c
 *
 *  Created on: 31.03.2021
 *      Author: david
 */

#include "stm32f1xx.h"
#include "UART_CONFIG.h"

GPIO_InitTypeDef gpio;
UART_HandleTypeDef uart;

void UartConfig(void){

	/*konfigurcja pinow do USARTa 2*/

	gpio.Mode = GPIO_MODE_AF_PP;	//alternatywna push-pull
	gpio.Pin = UART_TX;			//TX
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;	//alternatywne wejscie
	gpio.Pin = UART_RX;			//RX

	HAL_GPIO_Init(GPIOA, &gpio);
	uart.Instance = USART2;
	uart.Init.BaudRate = 115200;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);
}
