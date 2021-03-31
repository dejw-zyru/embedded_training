/*
 * ADC_CONFIG.c
 *
 *  Created on: 31.03.2021
 *      Author: david
 */

#include "stm32f1xx.h"
#include "ADC_CONFIG.h"

#define ADC_CHANNELS	3

ADC_HandleTypeDef adc;

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

	// konfigurowanie kana³ow przetwornika
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
