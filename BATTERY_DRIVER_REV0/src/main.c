/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f1xx.h"
			

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef gpio; 				//obiekt gpio bedacy konfiguracja portow GPIO
	gpio.Pin = GPIO_PIN_6; 				// konfigurowany pin 5
	gpio.Mode = GPIO_MODE_OUTPUT_PP; 	// jako wyjscie
	gpio.Pull = GPIO_NOPULL;			// rezystory podciagajace sa wylaczone
	gpio.Speed = GPIO_SPEED_FREQ_LOW;	// wystarcza niskie czestotliwosci przelaczania
	HAL_GPIO_Init(GPIOC, &gpio); 		// inicjalizacja modulow GPIOA

	while(1){
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6); // zmiana stanu diody
		HAL_Delay(2000);

	}

}

