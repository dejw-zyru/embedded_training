/*
 * DMA_CONFIG.c
 *
 *  Created on: 01.04.2021
 *      Author: david
 */

#include "stm32f1xx.h"
#include "DMA_CONFIG.h"

DMA_HandleTypeDef dma;
ADC_HandleTypeDef adc;

uint16_t adcValue[ADC_CHANNELS];

void DmaConfig(void){
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

}

