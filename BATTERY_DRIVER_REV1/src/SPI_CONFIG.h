/*
 * SPI_CONFIG.h
 *
 *  Created on: 09.04.2021
 *      Author: david
 */

#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

#define RTD_MOSI 	GPIO_PIN_7
#define RTD_MISO 	GPIO_PIN_6
#define RTD_CLK 	GPIO_PIN_5
#define RTD_CS 		GPIO_PIN_4
#define RTD_DRDY 	GPIO_PIN_14

void SpiConfig(void);

#endif /* SPI_CONFIG_H_ */
