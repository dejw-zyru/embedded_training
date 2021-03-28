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

#define CTRL_DC_DC 		GPIO_PIN_9
#define GREEN_LED 		GPIO_PIN_13
#define RED_LED 		GPIO_PIN_6
#define PMOS_LOGIC 		GPIO_PIN_1
#define PMOS_DCDC 		GPIO_PIN_5
#define PMOS_STEP 		GPIO_PIN_2
#define ADC_CHANNELS	3

#define	BUTTON_SYSTEM_START_OFF

uint16_t adc_value[ADC_CHANNELS];

DMA_HandleTypeDef dma;

UART_HandleTypeDef uart;
ADC_HandleTypeDef adc;
TIM_HandleTypeDef tim2;
GPIO_InitTypeDef gpio;
			
void GPIO_INIT(void);
void UART_INIT(void);
void ADC_INIT(void);
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
}

int main(void)
{

	SystemCoreClock = 8000000; //taktowanie8MHz
	HAL_Init();
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

	//start przetwornicy
	#ifdef BUTTON_SYSTEM_START_ON
		system_start();
	#endif
	GPIO_INIT();
	UART_INIT();
	ADC_INIT();

	tim2.Instance = TIM2;
	tim2.Init.Period = 5000 - 1;
	tim2.Init.Prescaler = 8000 - 1;
	tim2.Init.ClockDivision = 0;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.RepetitionCounter = 0;
	tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim2);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start_IT(&tim2);

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

	HAL_ADC_Start_DMA(&adc, (uint32_t*)adc_value, ADC_CHANNELS);

	int i = 0;

	while(1){
		/*HAL_GPIO_TogglePin(GPIOB, GREEN_LED); // toggle green led
		//HAL_Delay(2000);
		HAL_GPIO_TogglePin(GPIOC, RED_LED);	// red led*/
		printf("Hello world!%d\n",i);


		printf("\n\n\nSTM32 supply voltage is = %d convert: (%.1eV)\r\n", adc_value[0], 2 * adc_value[0] * 3.3f / 4096.0f);


		printf("Input voltage is = %d convert:(%.1eV)\r\n", adc_value[1], 10 * adc_value[1] * 3.3f / 4096.0f);


		printf("Reference voltage is = %d convert:(%.1eV)\r\n\n\n\n", adc_value[2],  adc_value[2] * 3.3f / 4096.0f);
		HAL_Delay(2000);
		i++;



		HAL_GPIO_WritePin(GPIOB, PMOS_STEP, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, PMOS_LOGIC, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, PMOS_DCDC, GPIO_PIN_SET);
		}


}
#ifdef BUTTON_SYSTEM_START_ON
	void system_start(void){
		//start prztwornicy
		gpio.Pin = CTRL_DC_DC;
		gpio.Mode = GPIO_MODE_OUTPUT_PP;
		gpio.Pull = GPIO_NOPULL;
		gpio.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &gpio);
		HAL_GPIO_WritePin(GPIOC, CTRL_DC_DC, GPIO_PIN_SET);
	}
#endif


void GPIO_INIT(void){
	 				//obiekt gpio bedacy konfiguracja portow GPIO
	gpio.Pin = GREEN_LED | PMOS_STEP; 			// green led output, SW_STEP_DR_CTRL
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


	gpio.Pin = RED_LED | PMOS_LOGIC | PMOS_DCDC; 			// red led output, SW_LOGIC_CTRL, SW_DR_DC_CTRL
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

void UART_INIT(void){
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

void ADC_INIT(void){
	//konfiguraca petli PLL
	// przetwornik nie moze pracowac powyzej 14MHz
	RCC_PeriphCLKInitTypeDef adc_clk;
	adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV2;//najmniejszy dzielnik 2
	HAL_RCCEx_PeriphCLKConfig(&adc_clk);

	//usatwienie zmiennej + parametry pracy
	adc.Instance = ADC1;
	adc.Init.ContinuousConvMode = ENABLE;			//chcemy caly czas odczytywac pomiarow
	adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc.Init.ScanConvMode = ADC_SCAN_ENABLE;		//wiele kanalow odczytujemy
	adc.Init.NbrOfConversion = ADC_CHANNELS;		//liczba kana³ow
	adc.Init.DiscontinuousConvMode = DISABLE;
	adc.Init.NbrOfDiscConversion = 1;
	HAL_ADC_Init(&adc);							//wlaczenie przetwornika



	// konfigorowanie kana³ow przetwornika
	ADC_ChannelConfTypeDef adc_ch;
	adc_ch.Channel = ADC_CHANNEL_8;				//STM32 supply voltage
	adc_ch.Rank = ADC_REGULAR_RANK_1;			// pole Rank ustala kolejnosc wykonywania pomairow
	adc_ch.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	adc_ch.Channel = ADC_CHANNEL_0;				//Input voltage is
	adc_ch.Rank = ADC_REGULAR_RANK_2;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	adc_ch.Channel = ADC_CHANNEL_1;				//Input voltage is
	adc_ch.Rank = ADC_REGULAR_RANK_3;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	//start kalibracji
	HAL_ADCEx_Calibration_Start(&adc);			//autokalibracaj przetwornika
}


