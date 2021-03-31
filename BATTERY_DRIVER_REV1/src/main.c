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
#include "GPIO.h"
#include "ADC_CONFIG.h"
#include "UART_CONFIG.h"
#include "CLOCK_ENABLE.h"
#include "TIM_CONFIG.h"


#define	BUTTON_SYSTEM_START_OFF

uint16_t adcValue[ADC_CHANNELS];

DMA_HandleTypeDef dma;

UART_HandleTypeDef uart;
ADC_HandleTypeDef adc;
TIM_HandleTypeDef tim2;
GPIO_InitTypeDef gpio;
			

#ifdef BUTTON_SYSTEM_START_ON
	void system_start(void);
#endif

void send_char(char c)
{
	HAL_UART_Transmit(&uart, (uint8_t*)&c, 1, 1000);
}

int __io_putchar(int ch)
{
	if (ch == '\n')
		send_char('\r');
	send_char(ch);
	return ch;
}

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

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();
	ClockEnable();


	//start przetwornicy
	#ifdef BUTTON_SYSTEM_START_ON
		system_start();
	#endif

	GPIO_INIT();
	UART_INIT();
	ADC_INIT();

	TimConfig();


	dma.Instance = DMA1_Channel1;				//kana³ pierwszy DMA
	dma.Init.Direction = DMA_PERIPH_TO_MEMORY;	//kopiowanie z ukladu peryferyjnego do pamieci
	dma.Init.PeriphInc = DMA_PINC_DISABLE;		//adres rejestru przetwornika staly??
	dma.Init.MemInc = DMA_MINC_ENABLE;			//zapisywanie wynikow w kolejnych zmiennych
	dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;	//16 bitowe zmienne do odczytywania 12 bitowego wyniku
	dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	dma.Init.Mode = DMA_CIRCULAR;				//odczyt ma nastepowac ciagle
	dma.Init.Priority = DMA_PRIORITY_HIGH;		//ustawienie wyskiego priorytetu
	HAL_DMA_Init(&dma);							//inicjalizacja DMA
	__HAL_LINKDMA(&adc, DMA_Handle, dma);		//makro powiazujace kanal DMA z modulem ADC
	HAL_ADC_Start_DMA(&adc, (uint32_t*)adcValue, ADC_CHANNELS);

	int i = 0;

	while(1){

		printf("Hello world!%d\n",i);


		printf("\n\n\nSTM32 supply voltage is = %d convert: (%.1eV)\r\n", adcValue[0], 2 * adcValue[0] * 3.3f / 4096.0f);


		printf("Input voltage is = %d convert:(%.1eV)\r\n", adcValue[1], 10 * adcValue[1] * 3.3f / 4096.0f);


		printf("Reference voltage is = %d convert:(%.1eV)\r\n\n\n\n", adcValue[2],  adcValue[2] * 3.3f / 4096.0f);
		HAL_Delay(2000);
		i++;


		if (i==1){
			HAL_GPIO_WritePin(GPIOB, PMOS_STEP, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, PMOS_LOGIC, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, PMOS_DCDC, GPIO_PIN_SET);

		}

	}


}

//start prztwornicy - zasilania calego systemu

#ifdef BUTTON_SYSTEM_START_ON
	void system_start(void){
		gpio.Pin = CTRL_DC_DC;
		gpio.Mode = GPIO_MODE_OUTPUT_PP;
		gpio.Pull = GPIO_NOPULL;
		gpio.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &gpio);
		HAL_GPIO_WritePin(GPIOC, CTRL_DC_DC, GPIO_PIN_SET);
	}
#endif






