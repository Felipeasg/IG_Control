/**
  *************************************************************************************
  * \file    buzzerbsp.c 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    02/05/2015
  * \brief   These file contains the implementations of buzzer library
  *************************************************************************************
  * Here are the detais about the function to manipulate buzzer beeps sounds.
  * 
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */



/*************************************** INCLUDES *************************************/

#include "buzzerbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void buzzerbsp_init(void)
{
	__GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void buzzerbsp_beep(uint8_t u8_beepState)
{
	if(u8_beepState == ON)
	{
		BEEP_ON;
	}
	else
	{
		BEEP_OFF;
	}
}
/*************************************** EOF ******************************************/

