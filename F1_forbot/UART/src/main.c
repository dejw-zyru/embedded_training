/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <string.h>
#include "stm32f1xx.h"

#define z4

#ifdef z1

UART_HandleTypeDef uart;

void send_string(char* s){
	HAL_UART_Transmit(&uart, (uint8_t*)s, strlen(s), 1000);
}

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_AF_PP; //wyjsice TX jako funkcja alternatywna push-pull
	gpio.Pin = GPIO_PIN_2;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT; //wejsice RX jako funkcja alternatywna
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio);

	//przygotawanie modulu USART2

	uart.Instance = USART2;
	uart.Init.BaudRate = 115200; //predkosc 115200
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	while (1){
		send_string("Hello world!\r\n");
		HAL_Delay(100);
	}
}

#endif

#ifdef z2	//odczytywanie znakow

UART_HandleTypeDef uart;

void send_string(char* s){
	HAL_UART_Transmit(&uart, (uint8_t*)s, strlen(s), 1000);
}

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio;		//obiekt gpio bedacy konfiguracja portow GPIO
	gpio.Mode = GPIO_MODE_AF_PP; //wyjsice TX jako funkcja alternatywna push-pull
	gpio.Pin = GPIO_PIN_2;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);



	gpio.Mode = GPIO_MODE_AF_INPUT; //wejsice RX jako funkcja alternatywna
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio);

	//przygotawanie modulu USART2

	uart.Instance = USART2;
	uart.Init.BaudRate = 115200; //predkosc 115200
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	while (1){

		if (__HAL_UART_GET_FLAG(&uart, UART_FLAG_RXNE) == SET){

			uint8_t value;
			HAL_UART_Receive(&uart, &value, 1, 100);

			switch(value){

			case 'a':
				send_string("Odebrano: a\r\n");
				break;
			case 'b':
				send_string("Odebrano: b\r\n");
				break;
			case 'c':
				send_string("Odebrano: c\r\n");
				break;
			default:
				send_string("Niepoprawny znak: c\r\n");
				break;
			}

		}
	}
}


#endif

#ifdef z3 // wlacza i wylacz diode L2 na nucleo



UART_HandleTypeDef uart;

void send_string(char* s){
	HAL_UART_Transmit(&uart, (uint8_t*)s, strlen(s), 1000);
}

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio;		//obiekt gpio bedacy konfiguracja portow GPIO
	gpio.Mode = GPIO_MODE_AF_PP; //wyjsice TX jako funkcja alternatywna push-pull
	gpio.Pin = GPIO_PIN_2;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Pin = GPIO_PIN_5; 				// konfigurowany pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 	// jako wyjscie
	gpio.Pull = GPIO_NOPULL;			// rezystory podciagajace sa wylaczone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcza niskie czestotliwosci przelaczania
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT; //wejsice RX jako funkcja alternatywna
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio);

	//przygotawanie modulu USART2

	uart.Instance = USART2;
	uart.Init.BaudRate = 115200; //predkosc 115200
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	while (1){

		if (__HAL_UART_GET_FLAG(&uart, UART_FLAG_RXNE) == SET){

			uint8_t value;
			HAL_UART_Receive(&uart, &value, 1, 100);

			switch(value){

			case 'a':
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				break;
			case 'b':
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				break;
			}

		}
	}
}


#endif


#ifdef z4 // wlacza i wylacz diode L2 na nucleo

UART_HandleTypeDef uart;

void send_string(char* s){
	HAL_UART_Transmit(&uart, (uint8_t*)s, strlen(s), 1000);
}

/*void receive_string(char* s){
	HAL_UART_Receive(&uart, (uint8_t*)s, strlen(s), 1000);
}*/

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef gpio;		//obiekt gpio bedacy konfiguracja portow GPIO
	gpio.Mode = GPIO_MODE_AF_PP; //wyjsice TX jako funkcja alternatywna push-pull
	gpio.Pin = GPIO_PIN_2;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Pin = GPIO_PIN_5; 				// konfigurowany pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 	// jako wyjscie
	gpio.Pull = GPIO_NOPULL;			// rezystory podciagajace sa wylaczone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcza niskie czestotliwosci przelaczania
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT; //wejsice RX jako funkcja alternatywna
	gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio);

	//przygotawanie modulu USART2

	uart.Instance = USART2;
	uart.Init.BaudRate = 115200; //predkosc 115200
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&uart);

	while (1){

		if (__HAL_UART_GET_FLAG(&uart, UART_FLAG_RXNE) == SET){

			uint8_t value[3];
			for (int i = 0;i < 3; i++){
				HAL_UART_Receive(&uart, &value[i], 1, 100);
			}

			//send_string(value);

		}
	}
}


#endif
