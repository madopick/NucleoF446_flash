
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart2;
extern uint8_t uart2Rcv_buff[UART2_RX_BUFFER_SIZE];
extern uint8_t uart2_buff_len;

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/***************************************************************
  * @brief This function handles DMA1 stream5 global interrupt.
  **************************************************************/
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/********************************************************
  * @brief This function handles USART2 interrupt.
  ******************************************************/
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);

	if(RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))   //Judging whether it is idle interruption
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);                     //Clear idle interrupt sign (otherwise it will continue to enter interrupt)

		//Stop this DMA transmission
		HAL_UART_DMAStop(&huart2);

		//Calculate the length of the received data
		uint8_t data_length  = 255 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);


#if 0
		printf("UART2 Receive (%d) \r\n",data_length);
		for(uint8_t i = 0; i < uart2_buff_len;i++)
			printf("[%d] : %c \r\n",i,uart2Rcv_buff[i]);
		printf("\r\n");
#endif

#ifdef SHELL_CMD
		for(int i = 0; i<data_length; i++){
		  uint8_t single_char = uart2Rcv_buff[i];
		  tinysh_char_in((unsigned char)single_char);
		}
#endif

		//Zero Receiving Buffer
		memset(uart2Rcv_buff, '\0', sizeof(uart2Rcv_buff));
		uart2_buff_len = data_length;
		data_length = 0;


		//Restart to start DMA transmission of 255 bytes of data at a time
		HAL_UART_Receive_DMA(&huart2, (uint8_t*)uart2Rcv_buff, UART2_RX_BUFFER_SIZE);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
