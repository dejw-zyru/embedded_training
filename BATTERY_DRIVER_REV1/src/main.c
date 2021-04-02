/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

//#include <string.h>
#include "stm32f1xx.h"
#include "GPIO.h"
#include "ADC_CONFIG.h"
#include "CLOCK_CONFIG.h"
#include "UART_CONFIG.h"
#include "TIM_CONFIG.h"
#include "BUTTON_SYSTEM_START.h"
#include "DMA_CONFIG.h"

#define	BUTTON_SYSTEM_START_OFF

uint16_t adcValue[ADC_CHANNELS];

UART_HandleTypeDef uart;

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

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();
	ClockConfig();

	//start systemu dzieki zwarciu MOSFETA-N za pomoca przycisku
	#ifdef BUTTON_SYSTEM_START_ON
		SystemStart();
	#endif

	GpioConfig();
	UartConfig();
	AdcConfig();
	TimConfig();
	DmaConfig();

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









