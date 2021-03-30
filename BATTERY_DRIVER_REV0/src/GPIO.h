/*
 * GPIO.h
 *
 *  Created on: 31.03.2021
 *      Author: david
 */

#ifndef GPIO_H_
#define GPIO_H_

#define CTRL_DC_DC 		GPIO_PIN_9
#define GREEN_LED 		GPIO_PIN_13
#define RED_LED 		GPIO_PIN_6
#define RED_ALARM		GPIO_PIN_7
#define BUZZER_ALARM	GPIO_PIN_12
#define PMOS_LOGIC 		GPIO_PIN_1
#define PMOS_DCDC 		GPIO_PIN_5
#define PMOS_STEP 		GPIO_PIN_2

void GPIO_INIT(void);


#endif /* GPIO_H_ */
