/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "tiny_sh.h"
#include "flashReadWrite.h"
#include "fw_cfg.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/



/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(char * file, int line);


/* Exported Variables --------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
#define UART2_RX_BUFFER_SIZE				255

#define B1_Pin 								GPIO_PIN_13
#define B1_GPIO_Port 						GPIOC
#define USART_TX_Pin 						GPIO_PIN_2
#define USART_TX_GPIO_Port 					GPIOA
#define USART_RX_Pin 						GPIO_PIN_3
#define USART_RX_GPIO_Port 					GPIOA
#define LD2_Pin 							GPIO_PIN_5
#define LD2_GPIO_Port 						GPIOA
#define TMS_Pin 							GPIO_PIN_13
#define TMS_GPIO_Port 						GPIOA
#define TCK_Pin 							GPIO_PIN_14
#define TCK_GPIO_Port 						GPIOA
#define SWO_Pin 							GPIO_PIN_3
#define SWO_GPIO_Port 						GPIOB


#define SHELL_CMD


/************************************************************
 * @brief   UART callback typedef
 *
 * @param   data The data pointer
 * @param   length The data length
 ************************************************************/
typedef void (*UARTPeriphCallback)(uint8_t *data, uint16_t length);

/************************************************************
 * @brief   UART peripheral structure.
 *
 * There several instance of UART in this application:
 * UART1 :
 * LUART1:
 ************************************************************/
extern const struct UARTPeriph_s
{
    void (*init)(uint32_t baud);
    uint8_t (*receive)(UARTPeriphCallback callback, uint32_t timeout);
    uint8_t (*send)(uint8_t *data, uint16_t length, uint32_t timeout);
}UART2Periph;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
