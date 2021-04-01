/*
 * ADC_CONFIG.h
 *
 *  Created on: 31.03.2021
 *      Author: david
 */

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#define ADC_CHANNELS						3
#define MEASURE_OUTPUT_DCDC_CONV_VOLTAGE 	GPIO_PIN_0
#define MEASURE_INPUT_SYSTEM_VOLTAGE 		GPIO_PIN_0
#define	MEASURE_REFERENCE_VOLTAGE 			GPIO_PIN_1

void AdcConfig(void);

#endif /* ADC_CONFIG_H_ */
