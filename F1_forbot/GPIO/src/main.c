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

#define z4

#ifdef systick

int main(void)
{
 SystemCoreClock = 8000000; // taktowanie 8Mhz

 HAL_Init();

 __HAL_RCC_GPIOA_CLK_ENABLE();

 GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
 gpio.Pin = GPIO_PIN_5; // konfigurujemy pin 5
 gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
 gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
 gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
 HAL_GPIO_Init(GPIOA, &gpio); // inicjalizacja modu³u GPIOA

 while (1)
 {
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // zapalenie diody
 HAL_Delay(100);
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // zgaszenie diody
 HAL_Delay(100);
 }
}

#endif

#ifdef led_
uint32_t led = 0;
int main(void)
{
    SystemCoreClock = 8000000; // taktowanie 8Mhz

    HAL_Init();

    __HAL_RCC_GPIOC_CLK_ENABLE();

   GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
   gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOC, &gpio); // inicjalizacja modu³u GPIOC


   while (1) {
      HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
      HAL_Delay(150);
      HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
      if (++led >= 10) { // przejdz do nastepnej
         led = 0;
      }
   }
}
#endif


#ifdef interupt

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // zmieñ stan diody

}

int main(void)
{
	SystemCoreClock = 8000000; // taktowanie 8Mhz

 HAL_Init();

 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_GPIOC_CLK_ENABLE();

 GPIO_InitTypeDef gpio;
 gpio.Mode = GPIO_MODE_OUTPUT_PP;
 gpio.Pull = GPIO_NOPULL;
 gpio.Pin = GPIO_PIN_5;
 gpio.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(GPIOA, &gpio);

 gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
 gpio.Pull = GPIO_PULLUP;
 gpio.Pin = GPIO_PIN_13;
 HAL_GPIO_Init(GPIOC, &gpio);

 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

 while (1)
 {
 }
}

#endif

#ifdef z1

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin_13)
{
 /*if (temp == 0){
	 temp = 1;
 } else{
	 temp = 0;
	 led = 10;
 }*/
	//HAL_GPIO_WritePin(GPIOC, led, GPIO_PIN_SET);
	HAL_Delay(500);
	//HAL_GPIO_WritePin(GPIOC, led, GPIO_PIN_RESET);

}

int main(void)
{
    SystemCoreClock = 8000000; // taktowanie 8Mhz

    HAL_Init();

    __HAL_RCC_GPIOC_CLK_ENABLE();

   GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
   gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOC, &gpio); // inicjalizacja modu³u GPIOC

   GPIO_InitTypeDef gpio_it;
   gpio_it.Mode = GPIO_MODE_INPUT;
   gpio_it.Pull = GPIO_PULLUP;
   gpio_it.Pin = GPIO_PIN_13;
   HAL_GPIO_Init(GPIOC, &gpio_it);

   uint32_t direction = 1;
   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);


   uint32_t led = 0;
    while (1) {
    		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET){
    			if (direction == 0){
    				direction = 1;
    				led = 10;
    			} else {
    				direction = 0;
    				led = 0;
    			}
    		}
    		if (direction == 1){
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
    			HAL_Delay(150);
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
    			if (led-- == 0) { // przejdz do nastepnej
    				led = 10;
    			}
    		} else if (direction == 0){
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
    			HAL_Delay(150);
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
    			if (++led >= 10) { // przejdz do nastepnej
    			    led = 0;
    			}
    		}



   }
}

#endif

#ifdef z1_1

uint32_t led = 10;
uint32_t direction = 1;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin_13)
{

	    			if (direction == 0){
	    				direction = 1;
	    				//led = 10;
	    			} else {
	    				direction = 0;
	    				//led = 0;
	    			}

}

int main(void)
{
    SystemCoreClock = 8000000; // taktowanie 8Mhz

    HAL_Init();

    __HAL_RCC_GPIOC_CLK_ENABLE();

   GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
   gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOC, &gpio); // inicjalizacja modu³u GPIOC

   GPIO_InitTypeDef gpio_it;
   gpio_it.Mode = GPIO_MODE_IT_RISING;
   gpio_it.Pull = GPIO_PULLUP;
   gpio_it.Pin = GPIO_PIN_13;
   HAL_GPIO_Init(GPIOC, &gpio_it);

   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);



    while (1) {

    		if (direction == 1){
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
    			HAL_Delay(150);
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
    			if (led-- == 0) { // przejdz do nastepnej
    				led = 10;
    			}
    		} else if (direction == 0){
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
    			HAL_Delay(150);
    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
    			if (++led >= 10) { // przejdz do nastepnej
    			    led = 0;
    			}
    		}



   }
}

#endif

#ifdef z2

uint32_t led = 10;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin_13)
{
	HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
	if (++led >= 10) { // przejdz do nastepnej
	    led = 0;
	}
	HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET);

}

int main(void)
{
    SystemCoreClock = 8000000; // taktowanie 8Mhz

    HAL_Init();

    __HAL_RCC_GPIOC_CLK_ENABLE();

   GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
   gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOC, &gpio); // inicjalizacja modu³u GPIOC

   GPIO_InitTypeDef gpio_it;
   gpio_it.Mode = GPIO_MODE_IT_RISING;
   gpio_it.Pull = GPIO_PULLUP;
   gpio_it.Pin = GPIO_PIN_13;
   HAL_GPIO_Init(GPIOC, &gpio_it);

   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);



    while (1) {

   }
}

#endif

#ifdef z3

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin_13)
{

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

}

int main(void)
{
    SystemCoreClock = 8000000; // taktowanie 8Mhz

    HAL_Init();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

   GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
   gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOC, &gpio); // inicjalizacja modu³u GPIOC

   GPIO_InitTypeDef gpio_A; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio_A.Pin = GPIO_PIN_5;
   gpio_A.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio_A.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio_A.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOA, &gpio_A); // inicjalizacja modu³u GPIOA

   GPIO_InitTypeDef gpio_it;
   gpio_it.Mode = GPIO_MODE_IT_RISING;
   gpio_it.Pull = GPIO_PULLUP;
   gpio_it.Pin = GPIO_PIN_13;
   HAL_GPIO_Init(GPIOC, &gpio_it);

   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
   uint32_t led = 0;



    while (1) {


    		HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
    		HAL_Delay(150);
    		HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_RESET); //wy³¹cz diode
    		if (++led >= 10) { // przejdz do nastepnej
    			led = 0;
    		}




   }
}

#endif

#ifdef z4

int main(void)
{
    SystemCoreClock = 8000000; // taktowanie 8Mhz

    HAL_Init();

    __HAL_RCC_GPIOC_CLK_ENABLE();

   GPIO_InitTypeDef gpio; // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO
   gpio.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
   GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
   gpio.Mode = GPIO_MODE_OUTPUT_PP; // jako wyjœcie
   gpio.Pull = GPIO_NOPULL; // rezystory podci¹gaj¹ce s¹ wy³¹czone
   gpio.Speed = GPIO_SPEED_FREQ_LOW; // wystarcz¹ nieskie czêstotliwoœci prze³¹czania
   HAL_GPIO_Init(GPIOC, &gpio); // inicjalizacja modu³u GPIOC



   uint32_t led = 0;


    while (1) {


    			HAL_GPIO_WritePin(GPIOC, 1 << led, GPIO_PIN_SET); //w³¹cz diode
    			HAL_Delay(150);
    			HAL_GPIO_WritePin(GPIOC, 1 << (led - 3), GPIO_PIN_RESET); //wy³¹cz diode
    			if (++led >= 13) { // przejdz do nastepnej
    			    led = 0;
    			}




   }
}

#endif

