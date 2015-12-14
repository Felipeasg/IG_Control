/**
  *************************************************************************************
  * \file    flash.h 
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

#ifndef INCLUDE_FLASH_H_
#define INCLUDE_FLASH_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include "stm32f4xx_hal.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define	ADDR_FLASH_SECTOR	0x080E0000	// Endereço do setor utilizado - Setor 11 do STM32F405
#define FLASH_SECTOR		FLASH_SECTOR_11

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

uint8_t flash_write(uint32_t *buffer, uint32_t num_words);
void flash_read(uint32_t *buffer, uint32_t num_words);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_FLASH_H_ */

/*************************************** EOF ******************************************/
