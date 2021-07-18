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
#include "GPIO.h"
#include "ADC_CONFIG.h"
#include "CLOCK_CONFIG.h"
#include "UART_CONFIG.h"
#include "TIM_CONFIG.h"
#include "BUTTON_SYSTEM_START.h"
#include "DMA_CONFIG.h"
#include "PRINTF.h"
#include "SPI_CONFIG.h"

#define	BUTTON_SYSTEM_START_OFF
#define	TRY_SPI_ON

uint16_t adcValue[ADC_CHANNELS];

SPI_HandleTypeDef spi;
uint8_t resistance;

//obsluge SPI
void mcp_write_reg(uint8_t addr)
{
	uint8_t tx_buf[] = { addr };

	HAL_GPIO_WritePin(GPIOA, RTD_CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi, tx_buf, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, RTD_CS, GPIO_PIN_SET);
}
uint8_t get_resistance(){
	uint8_t tx_buf[] = { 0x01, 0xff };
	uint8_t rx_buf[1];

	HAL_GPIO_WritePin(GPIOA, RTD_CS, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&spi, tx_buf, rx_buf, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, RTD_CS, GPIO_PIN_SET);

	return rx_buf[1];
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
	#ifdef TRY_SPI_ON
		SpiConfig();
		mcp_write_reg(0x80);
		mcp_write_reg(0xD2);
	#endif


	int i = 0;

	while(1){

		resistance=get_resistance();

		printf_("Hello world!%d\n\r",i);
		printf_("Resistance value is %d\n\r",resistance);
		printf_("\n\n\nSTM32 supply voltage is = %d convert: (%dmV)\n\r", adcValue[0], 2 * adcValue[0] * 3300 / 4096);
		printf_("Input voltage is = %d convert:(%.1fV)\n\r", adcValue[1], 10 * adcValue[1] * 3.3f / 4096.0f);
		printf_("Reference voltage is = %d convert:(%.1eV)\n\n\n\n\r", adcValue[2],  adcValue[2] * 3.3f / 4096.0f);

		HAL_Delay(2000);
		i++;

		if (i==1){
			HAL_GPIO_WritePin(GPIOB, PMOS_STEP, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, PMOS_LOGIC, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, PMOS_DCDC, GPIO_PIN_SET);
		}
	}
}









