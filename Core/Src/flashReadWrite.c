/*
 * flashReadWrite.c
 *
 *  Created on: Jan 24, 2022
 *      Author: madop
 */

#include "flashReadWrite.h"


/* Private define ------------------------------------------------------------*/
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_2   											/* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_7  +  GetSectorSize(ADDR_FLASH_SECTOR_7) -1 	/* End @ of user Flash area : sector start address + sector size -1 */

#define DATA_32                 ((uint32_t)0x12345678)
#define DATA_64                 ((uint64_t)0x1234567812345678)




/* Private variables ---------------------------------------------------------*/
uint32_t FirstSector = 0, NbOfSectors = 0;
uint32_t Address = 0, SECTORError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;


/**************************************************
 *  @name 	GetSectorSize
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  *************************************************/
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }
  return sectorsize;
}


/*************************************************
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  ************************************************/
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7) */
  {
    sector = FLASH_SECTOR_7;
  }

  return sector;
}


/***************************************
 * @name   flashRead
 * @brief  read flash.
 ***************************************/
uint32_t flashRead(uint32_t addr)
{
	return (*(__IO uint32_t *)addr);
}


/*************************************************************************
 * @name   flashWrite
 * @brief  Write flash Flash area word by word
 * 		   Area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR
 *************************************************************************/
HAL_StatusTypeDef flashWrite(uint32_t addr, uint32_t data)
{
	HAL_StatusTypeDef retval = HAL_ERROR;

	Address = FLASH_USER_START_ADDR;

	while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
		{
			Address = Address + 4;
		}
		else
		{
			/* Error occurred while writing data in Flash memory.
		 	   User can add here some code to deal with this error */
			while (1)
			{
				/* Make LED3 blink (1s period) to indicate error in Write operation */
				//BSP_LED_On(LED3);
				//HAL_Delay(1000);
				//BSP_LED_Off(LED3);
				//HAL_Delay(1000);
			}
		}
	}


	/****************************************************************************
	 * Lock the Flash to disable the flash control register access (recommended
	 * to protect the FLASH memory against possible unwanted operation)
	 ****************************************************************************/
	  HAL_FLASH_Lock();

	/***********************************************************************
	* Check if the programmed data is OK
	* MemoryProgramStatus = 0: data programmed correctly
	* MemoryProgramStatus != 0: number of words not programmed correctly
	***********************************************************************/
	Address = FLASH_USER_START_ADDR;
	MemoryProgramStatus = 0x0;

	while (Address < FLASH_USER_END_ADDR)
	{
	  data32 = *(__IO uint32_t *)Address;

	  if (data32 != DATA_32)
	  {
		MemoryProgramStatus++;
	  }
	  Address = Address + 4;
	}

	/*Check if there is an issue to program data*/
	if (MemoryProgramStatus == 0)
	{
	  /* No error detected. Switch on LED1*/
	  //BSP_LED_On(LED1);
	  retval = HAL_OK;
	}
	else
	{
	  /* Error detected. Switch on LED3*/
	  //BSP_LED_On(LED3);
	  while(1);
	}

	return retval;
}


/***************************************
 * @name   flashErase
 * @brief  Erase flash.
 ***************************************/
HAL_StatusTypeDef flashErase(void)
{
	static uint8_t retry;
	HAL_StatusTypeDef retval = HAL_ERROR;

	/// Unlock the Flash to enable the flash control register access
	while(HAL_FLASH_Unlock() != HAL_OK){
		HAL_Delay(20);
		if(retry > 5){
			break;
		}
		retry++;
	}

	if(retry > 5)
		return retval;

	/*********************************************************************************************
	 * Erase the user Flash area
	 * area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR
	 *********************************************************************************************/
	FirstSector = GetSector(FLASH_USER_START_ADDR);							// Get the 1st sector to erase
	NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;			// Get the number of sector to erase from 1st sector

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = FirstSector;
	EraseInitStruct.NbSectors     = NbOfSectors;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*	Error occurred while sector erase.
	  	    SECTORError will contain the faulty sector and then to know the code error on this sector, user can call function 'HAL_FLASH_GetError()'
		 */

		/* Infinite loop */
		while (1)
		{
			/* Make LED blink (1s period) to indicate error in Erase operation */
			//BSP_LED_On(LED3);
			//HAL_Delay(1000);
			//BSP_LED_Off(LED3);
			//HAL_Delay(1000);
		}
	}


	/********************************************************************************************************
	 * Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	 *		 you have to make sure that these data are rewritten before they are accessed during code
	 *		 execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	 *		 DCRST and ICRST bits in the FLASH_CR register.
	 ********************************************************************************************************/
	  __HAL_FLASH_DATA_CACHE_DISABLE();
	  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

	  __HAL_FLASH_DATA_CACHE_RESET();
	  __HAL_FLASH_INSTRUCTION_CACHE_RESET();

	  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	  __HAL_FLASH_DATA_CACHE_ENABLE();


	return retval;
}


