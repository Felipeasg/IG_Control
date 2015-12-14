/**
 *************************************************************************************
 * \file    flash.c
 * \author  Felipe Adriano <felipeadrianosg@gmail.com>
 * \version V1.0.0
 * \date    07/05/2015
 * \brief   Small description
 *************************************************************************************
 * description
 *
 * @attention
 *
 *
 *
 * <h2><center>&copy; COPYRIGHT 2011 ENTERPRISE</center></h2>
 *************************************************************************************
 */



/*************************************** INCLUDES *************************************/

#include "flash.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

/**
 * @brief Função para gravar um buffer na Flash
 * @param buffer com os dados a serem gravados
 * @param num_words: tamanho do buffer
 * @return 0: erro durante a gravação; 1: gravação realizada com sucesso
 */
uint8_t flash_write(uint32_t *buffer, uint32_t num_words)
{
	uint32_t startAddress = ADDR_FLASH_SECTOR;

	HAL_FLASH_Unlock();

	FLASH_Erase_Sector(FLASH_SECTOR, VOLTAGE_RANGE_3);

	for (uint32_t i = 0; i < num_words; i++)
	{
		if (HAL_FLASH_Program(TYPEPROGRAM_WORD,(uint32_t)(startAddress+(i*4)),*buffer)!= HAL_OK)
		{
			HAL_FLASH_Lock();
			return 0;
		}
		buffer++;
	}

	HAL_FLASH_Lock();
	return 1;
}

/**
 * @brief Função para ler o buffer gravado na Flash pela writeFlash
 * @param buffer para receber os dados
 * @param num_words (tamanho do buffer)
 * @return nenhum
 */
void flash_read(uint32_t *buffer, uint32_t num_words)
{
	uint32_t startAddress = ADDR_FLASH_SECTOR;

	for (uint32_t i = 0; i < num_words; i++)
	{
		buffer[i] = *(uint32_t *)(startAddress + (i*4));
	}
}

/*************************************** EOF ******************************************/

