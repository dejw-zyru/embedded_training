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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

// tests
//#define resetWEL
#define blockWriting

//25AA040A instructions from page nr 7
//try with 9th bit
const uint8_t EEPROM_READ = 0b00000011; //Read data from memory array beginning at selected address
const uint8_t EEPROM_WRITE = 0b00000010;//Write data to memory array beginning at selected address
const uint8_t EEPROM_WRDI = 0b00000100; //Reset the write enable latch (disable write operations)
const uint8_t EEPROM_WREN = 0b00000110; //Set the write enable latch (enable write operations)
const uint8_t EEPROM_RDSR = 0b00000101; //Read STATUS register
const uint8_t EEPROM_WRSR = 0b00000001; //Write STATUS register

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char uart_buf[100]; //for printing to uart
  int uart_buf_len;
  char spi_buf[20];  //send and recieve data
  uint8_t addr, readAddr;
  uint8_t wip; //lets check if write in process is set 1 write is process 0 is over
  //czytamy az sie wyzeruje



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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  // CS pin should be default high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //Say something proba uarta
  uart_buf_len = sprintf(uart_buf, "SPI Test\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

  //Enable write enable latch (allow write operations)
  //aby cokolwiek wysalc wren sent
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WREN, 1, 100); //chcemy pisac do niego
  //(cast the data we wish to send to an 8bit unsigned integer pointer)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);


  //Read status register
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100); //czytamy status register
  HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  //odebrenie hex 0x02 -> ustawienie bitu wel a to znaczy ze write enable jest mzoliwe

  // Print out status register WEL write enable latch (bit 2)
  uart_buf_len = sprintf(uart_buf,
		  	  	  	  	  "Status if WEL is set: 0x%02x\r\n",
						  (unsigned int)spi_buf[0]);
  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifdef resetWEL
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WRDI, 1, 100); //chcemy pisac do niego
    //(cast the data we wish to send to an 8bit unsigned integer pointer)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);


    //Read status register
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100); //czytamy status register
    HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    //odebrenie hex 0x02 -> ustawienie bitu wel a to znaczy ze write enable jest mzoliwe

    // Print out status register
    uart_buf_len = sprintf(uart_buf,
  		  	  	  	  	  "Status if write enable latch WEL is disable (bit 2): 0x%02x\r\n",
  						  (unsigned int)spi_buf[0]);
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

#endif

#ifdef blockWriting

    spi_buf[0] |= (1<<3);
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WRSR, 1, 100); //chcemy pisac do niego
    HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_buf, 1, 100);
    //(cast the data we wish to send to an 8bit unsigned integer pointer)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    wip = 1; //czasami zapis troche zajmuje czasu czekamy az wip sie wyzeruje
      while (wip)
      {
    	  // Read status register
    	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //sprawdzic czy rozumiem wysyłane ponizej dane
    	  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100);
    	  HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
    	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    	  // Mask out WIP bit
    	  wip = spi_buf[0] & 0b00000001; //LSB spi_buf[0] it should be 0 -> sprytne porownanie
      }


    //Read status register
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100); //czytamy status register
    HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    //odebrenie hex 0x02 -> ustawienie bitu wel a to znaczy ze write enable jest mzoliwe

    // Print out status register
    uart_buf_len = sprintf(uart_buf,
  		  	  	  	  	  "block register SR: 0x%02x\r\n",
  						  (unsigned int)spi_buf[0]);
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

#endif


    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WREN, 1, 100); //chcemy pisac do niego
      //(cast the data we wish to send to an 8bit unsigned integer pointer)
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);


      //Read status register
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100); //czytamy status register
      HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

      //odebrenie hex 0x02 -> ustawienie bitu wel a to znaczy ze write enable jest mzoliwe

      // Print out status register WEL write enable latch (bit 2)
      uart_buf_len = sprintf(uart_buf,
    		  	  	  	  	  "enable rewrite register: 0x%02x\r\n",
    						  (unsigned int)spi_buf[0]);
      HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);



  // Test bytes to write to EEPROM
  /*spi_buf[0] = 0x00;
  spi_buf[1] = 0xCD;
  spi_buf[2] = 0x00;
  spi_buf[3] = 0x69;*/

    int temp=0x60;
  for (int i = 0; i<16; i++){
  	  spi_buf[i] = temp++;
  	//spi_buf[i] = 0;
    }

  // Set starting address
  addr = 0xFE;
  // read address
  readAddr = 0xFE;

  // Write 3 bytes starting at given address
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WRITE, 1, 100); //Write data to memory array beginning at selected address
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&addr, 1, 100); 	//od jakiego adresu zaczynamy pisac
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&spi_buf, 16, 100);//wyslanie 3 bajtow
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  // zeros read bytes
  for (int i = 0; i<16; i++){
	  spi_buf[i] = 0;
  }



  // Wait until WIP bit is cleared
  wip = 1; //czasami zapis troche zajmuje czasu czekamy az wip sie wyzeruje
  while (wip)
  {
	  // Read status register
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //sprawdzic czy rozumiem wysyłane ponizej dane
	  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100);
	  HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	  // Mask out WIP bit
	  wip = spi_buf[0] & 0b00000001; //LSB spi_buf[0] it should be 0 -> sprytne porownanie
  }

  // Read the 3 bytes back
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_READ, 1, 100);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&readAddr, 1, 100); 			//czytaj od tego aderes a pozneij go zwieksza
  HAL_SPI_Receive(&hspi1, (uint8_t *)&spi_buf, 16, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  // Print out bytes read
  for (int i=0;i<16;i++){
	  uart_buf_len = sprintf(uart_buf,
  		  	  	  	  	  "Status read byte nr %d: 0x%02x\r\n",
						  i,
  						  (unsigned int)spi_buf[i]);
	  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
  }


  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WREN, 1, 100); //chcemy pisac do niego
    //(cast the data we wish to send to an 8bit unsigned integer pointer)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  // Read status register
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //sprawdzic czy rozumiem wysyłane ponizej dane
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100);
  HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

  // Print out status register
  uart_buf_len = sprintf(uart_buf,
	  	  	  	  	  "zapis aby wylaczyc: 0x%02x\r\n",
					  (unsigned int)spi_buf[0]);
  HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

  //odczytanie 0 wel czyli do zapisu wyzerował sie poniewaz poprawnie zapisalismy do niego dane
#ifdef blockWriting

  	spi_buf[0] &= 0xF7;
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WRSR, 1, 100); //chcemy pisac do niego
    HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_buf, 1, 100);
    //(cast the data we wish to send to an 8bit unsigned integer pointer)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    wip = 1; //czasami zapis troche zajmuje czasu czekamy az wip sie wyzeruje
      while (wip)
      {
    	  // Read status register
    	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //sprawdzic czy rozumiem wysyłane ponizej dane
    	  HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100);
    	  HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
    	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    	  // Mask out WIP bit
    	  wip = spi_buf[0] & 0b00000001; //LSB spi_buf[0] it should be 0 -> sprytne porownanie
      }


    //Read status register
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100); //czytamy status register
    HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

    //odebrenie hex 0x02 -> ustawienie bitu wel a to znaczy ze write enable jest mzoliwe

    // Print out status register
    uart_buf_len = sprintf(uart_buf,
  		  	  	  	  	  "block register SR at the end: 0x%02x\r\n",
  						  (unsigned int)spi_buf[0]);
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);

#endif


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

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

