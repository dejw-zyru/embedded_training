/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t us);

int __io_putchar(int ch)
{
	if (ch == '\n') {
		__io_putchar('\r');
}
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 1;
}

HAL_StatusTypeDef wire_reset(void);

void write_bit(int value);

int read_bit(void);

void wire_write(uint8_t byte);

uint8_t wire_read(void);

uint8_t wire_crc(const uint8_t* data, int len);

uint8_t byte_crc(uint8_t crc, uint8_t byte);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void readROM(void); //task 1 z 17 oneWIRE
void readScratchpad(void);
void ReadTemp(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Myfking code
  //readROM();
  //readScratchpad(); //without reset retun 85stC
  ReadTemp();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void delay_us(uint32_t us)  //opznienie us
{
	__HAL_TIM_SET_COUNTER(&htim6, 0); //zerowanie licznika
	while (__HAL_TIM_GET_COUNTER(&htim6) < us) {} //jak doliczy do us
}

HAL_StatusTypeDef wire_reset(void) // sekwencja reset
{
	int rc;
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET); //master
	delay_us(480);
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
	delay_us(70);
	rc = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);	//slave musi powrocic na stan wysoki
	delay_us(410);

	if (rc == 0)
			return HAL_OK;
	else
			return HAL_ERROR;
}

void write_bit(int value) {

if (value) { //zapisanie 1
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
	delay_us(6);
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
	delay_us(64);
} else { //zapisanie 0
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
	delay_us(60);
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
	delay_us(10);
	}
}

int read_bit(void)
{
  int rc;
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
  delay_us(6);
  HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
  delay_us(9);
  rc = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin); // jak to trwa ze on tak długo mierzy
  delay_us(55);
  return rc;
}

void wire_write(uint8_t byte) //przesylanie od najmniej znaczacego bitu, przesyłanie całego bajtu
{
  int i;
  for (i = 0; i < 8; i++) {
    write_bit(byte & 0x01);
    byte >>= 1;
  }
}

uint8_t wire_read(void) //czytanie calego bajta
{
  uint8_t value = 0;
  int i;
  for (i = 0; i < 8; i++) {
    value >>= 1;
    if (read_bit()) // w zaleznosci czy przeczytasz 0 czy nie to rob or albo i nie
      value |= 0x80;  // odczyt 1 0b1000 0000 mamy 1 wpis do value i przesuwamy, tak jak zawsze
  }
  return value;
}

uint8_t byte_crc(uint8_t crc, uint8_t byte) //algorytm CRC
{
  int i;
  for (i = 0; i < 8; i++) {
    uint8_t b = crc ^ byte;
    crc >>= 1;
    if (b & 0x01)
      crc ^= 0x8c;
    byte >>= 1;
  }
  return crc;
}

uint8_t wire_crc(const uint8_t* data, int len) //wykonanie CRC na zadanej liczbie bajtow
{
  int i;
    uint8_t crc = 0;

    for (i = 0; i < len; i++)
      crc = byte_crc(crc, data[i]);

    return crc;
}

void readROM(void) { //odczytywanie adresu
	HAL_TIM_Base_Start(&htim6);

	HAL_StatusTypeDef rc = wire_reset();

	wire_write(0x33); // Read ROM

	  int i;
	  uint8_t rom_code[8];
	  for (i = 0; i < 8; i++)
	    rom_code[i] = wire_read(); //czujnik odsyła 8 bajtow

	  uint8_t crc = wire_crc(rom_code, 7); // 7 bo do petli
}

void readScratchpad(void){
	HAL_TIM_Base_Start(&htim6);

	wire_reset();
	wire_write(0xcc); //skip rom
	wire_write(0xbe); //read scrachpad

	int i;
	uint8_t scratchpad[9];
	for (i = 0; i < 9; i++)
	  scratchpad[i] = wire_read();

	uint8_t crc = wire_crc(scratchpad, 8);
}

void ReadTemp(void) {

	HAL_TIM_Base_Start(&htim6);

	wire_reset();
	wire_write(0xcc);
	wire_write(0x44);

	HAL_Delay(750);

	wire_reset();
	wire_write(0xcc);
	wire_write(0xbe);

	int i;
	uint8_t scratchpad[9];
	for (i = 0; i < 9; i++)
	  scratchpad[i] = wire_read();

	uint8_t crc = wire_crc(scratchpad, 8);

	uint32_t temp = (uint32_t)(scratchpad[1] << 8);

	temp |= scratchpad[0];

	//float solveTemp = temp/16;


}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
