/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart2;

///UART2
static void _uart2_init(uint32_t baud);
static uint8_t _uart2_receive(UARTPeriphCallback callback, uint32_t timeout);
static uint8_t _uart2_send(uint8_t *data, uint16_t length, uint32_t timeout);



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART2_UART_Init(void);


///PRINTF
#ifdef __GNUC__
	/* With GCC, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/******************************************************************
  * @name   PUTCHAR_PROTOTYPE
  * @brief  Retargets the C library printf function to the USART.
  *****************************************************************/
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);				//Use UART2
	return ch;
}


/* Private user code ---------------------------------------------------------*/
///UART 2
const struct UARTPeriph_s UART2Periph =
{
        .init 	 = _uart2_init,
        .receive = _uart2_receive,
        .send 	 = _uart2_send
};


/********************************************
  * @name   _uart2_init
  * @brief 	UART1 Initialization Function
  *******************************************/
static void _uart2_init(uint32_t baud)
{
  huart2.Instance 						= USART2;
  huart2.Init.BaudRate 					= baud;
  huart2.Init.WordLength 				= UART_WORDLENGTH_8B;
  huart2.Init.StopBits 					= UART_STOPBITS_1;
  huart2.Init.Parity 					= UART_PARITY_NONE;
  huart2.Init.Mode 						= UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl 				= UART_HWCONTROL_NONE;
  huart2.Init.OverSampling 				= UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  //__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  //HAL_UART_Receive_DMA(&huart2, (uint8_t*)huart2Rcv_buff, UART2_RX_BUFFER_SIZE);

}

/********************************************
  * @name   _lpuart1_receive
  * @brief 	LPUART1 Receive Function
  *******************************************/
static uint8_t _uart2_receive(UARTPeriphCallback callback, uint32_t timeout)
{
	//puts("puart2 rcv\r\n");
//	memset(puart2Rcv_buff,'\0',sizeof(huart2Rcv_buff));
//	f_lpuart1 = 0;
//	lpuart2_buff_len = 0;
//
//	while (f_lpuart1 == 0){
//		DelayMs(100);
//	}
//
//	if(callback != NULL){
//		callback(lpuartRcv_buff, lpuart1_buff_len);
//	}

	//printf("exit from lpuart1 rcv\r\n");

	return HAL_OK;
}

/********************************************
  * @name   _puart2_send
  * @brief 	LPUART1 Send Function
  *******************************************/
static uint8_t _uart2_send(uint8_t *data, uint16_t length, uint32_t timeout)
{
	if (HAL_UART_Transmit(&huart2, data, length, timeout)!= HAL_OK)
	{
		//char strdat[100];
		//sprintf(strdat,"LPUART1 TX Fail: %s\r\n",data);
		//puts(strdat);
		//puts("LPUART1 TX Fail!!!\r\n");
		return HAL_ERROR;
	}
	return HAL_OK;
}







/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();

  /* Infinite loop */
  while (1)
  {

  }
}

/******************************************************
  * @brief System Clock Configuration (180 MHz)
  * @retval None
  *****************************************************/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage*/
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure. */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }

  /** Activate the Over-Drive mode */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }

  /** Initializes the CPU, AHB and APB buses clocks*/
  RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 	= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }
}


/********************************************
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  *******************************************/
static void MX_CRC_Init(void)
{
  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }
}

/********************************************
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  *******************************************/
static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance 					= TIM1;
  htim1.Init.Prescaler 				= 0;
  htim1.Init.CounterMode 			= TIM_COUNTERMODE_UP;
  htim1.Init.Period 				= 65535;
  htim1.Init.ClockDivision 			= TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter 		= 0;
  htim1.Init.AutoReloadPreload 		= TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource 	= TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode 	= TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }
}


/******************************************
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  *****************************************/
static void MX_USART2_UART_Init(void)
{
  huart2.Instance 				= USART2;
  huart2.Init.BaudRate 			= 115200;
  huart2.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart2.Init.StopBits 			= UART_STOPBITS_1;
  huart2.Init.Parity 			= UART_PARITY_NONE;
  huart2.Init.Mode 				= UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
  huart2.Init.OverSampling 		= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
	  Error_Handler(__FILE__, __LINE__);
  }
}

/*****************************************
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  ****************************************/
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}



/*******************************************************************
  * @name   Error_Handler
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  ******************************************************************/
void Error_Handler(char * file, int line)
{
  __disable_irq();
  printf("Error: %s, line: %d \r\n",file,line);
  while (1)
  {
	  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
	  //HAL_Delay(100);
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
