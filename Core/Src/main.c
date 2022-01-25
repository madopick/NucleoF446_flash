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
#include "fw_cfg.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;


uint8_t uart2Rcv_buff[UART2_RX_BUFFER_SIZE];                		// UART2 RCV
uint8_t uart2_buff_len;												// UART2 RCv Length

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM1_Init(void);
static void MX_DMA_Init(void);

///UART2
static void _uart2_init(uint32_t baud);
static uint8_t _uart2_receive(UARTPeriphCallback callback, uint32_t timeout);
static uint8_t _uart2_send(uint8_t *data, uint16_t length, uint32_t timeout);

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
///fw_cfg
fwCfg_t const fwCfg_default =
{
    0x12,                //u32_crc
    0x13,                //u32_len
    0x14,                //u32_crcN
    0x15,                //u32_lenN
    0x0,                //u16_cfgProjectId
    0x3C,               //u16_cfgVer
    0x3,                //u8_cfgPartId
    0x5,                //u8_cfgAfeVer
    0x0,                //u8_reserved016
    0x0,                //u8_reserved017
    0x3C,               //u8_extReleaseByte0
    0x13,               //u8_extReleaseByte1
    0x0,                //u8_extReleaseByte2
    0x0,                //u8_extReleaseByte3
    0x0,                //u8_extReleaseByte4
    0x0,                //u8_extReleaseByte5
    0x0,                //u8_extReleaseByte6
    0x0,                //u8_extReleaseByte7
    0xFF,               //u8_sense0En
    0xFF,               //u8_sense1En
    0xFF,               //u8_sense2En
    0xFF,               //u8_sense3En
    0x1F,               //u8_sense4En
    0x0,                //u8_sense5En
    0x0,                //u8_sense6En
    0x0,                //u8_sense7En
    0xFF,               //u8_force0En
    0xFF,               //u8_force1En
    0x1,                //u8_force2En
    0x0,                //u8_force3En
    0x0,                //u8_force4En
    0x0,                //u8_force5En
    0x0,                //u8_force6En
    0x0,                //u8_force7En
    0x25,               //u8_senseLen
    0x11,               //u8_forceLen
    0x923,              //u16_scrXRes
    0x437,              //u16_scrYRes
	0x1,                //b1_orientSwap
	0x1,                //b1_orientForce
	0x0,                //b1_orientSense
	0x1,                //b1_rptResScaleEn
	0x0,                //b4_reserved036_4_7
	0x923,              //u16_rptXRes
	0x437,              //u16_rptYRes
	0x182,              //u16_scrPpi
	0x1,                //u8_orientRawdata
	0x3,                //u8_scrMultiple
	0x0,                //u8_reserved03F
};

//uint8_t __attribute__((__section__(".user_ram"))) userConfig[sizeof(fwCfg_t)];
struct fwCfg_t userConfig __attribute__ ((section (".user_ram")));



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

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart2, (uint8_t*)uart2Rcv_buff, UART2_RX_BUFFER_SIZE);

}

/********************************************
  * @name   uart2_receive
  * @brief 	UART1 Receive Function
  *******************************************/
static uint8_t _uart2_receive(UARTPeriphCallback callback, uint32_t timeout)
{
//	puts("uart2 rcv\r\n");
//	memset(uart2Rcv_buff,'\0',sizeof(uart2Rcv_buff));
//	uart2_buff_len = 0;
//
//	if(callback != NULL){
//		callback(uart2Rcv_buff, uart2_buff_len);
//	}

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







/*******************************************************************************
  * @brief  The application entry point.
  * @retval int
  ******************************************************************************/
int main(void)
{
  /// MCU Configuration
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_TIM1_Init();
  UART2Periph.init(9600);

  /* init ready */
  printf("Peripherals Init Ready\r\n");

  tinysh_init();
  /////////////////////////////////////


  /* initialize userConfig value */
  memcpy (&userConfig, &fwCfg_default, sizeof(fwCfg_t));
  printf("\r\n\nuserConfig: %ld %ld %ld %ld \r\n", userConfig.u32_crc, userConfig.u32_len, userConfig.u32_crcN, userConfig.u32_lenN);

  /* Infinite loop */
  while (1)
  {
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  HAL_Delay(1000);
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


/*********************************************
  * @name   MX_DMA_Init
  * @brief 	Enable DMA controller clock
  ********************************************/
static void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  GPIO_InitStruct.Pin 	= B1_Pin;
  GPIO_InitStruct.Mode	= GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin 	= LD2_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
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
  printf("\r\nERROR: %s, line: %d \r\n",file,line);
  while (1)
  {
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  HAL_Delay(100);
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
