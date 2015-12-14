/**
  *************************************************************************************
  * \file    buttonsbsp.c 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    03/05/2015
  * \brief   Small description
  *************************************************************************************
  * description
  * 
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */



/*************************************** INCLUDES *************************************/

#include "buttonsbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define LOW			GPIO_PIN_RESET
#define HIGH		GPIO_PIN_SET
#define BUTTON_CLK	__GPIOA_CLK_ENABLE()
#define BUTTON_PORT	GPIOA
#define BUTTON_PIN	GPIO_PIN_0

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void buttonsbsp_init(void)
{
	//Enable GPIO peripheral clock
	BUTTON_CLK;

	//Configure pin button as input
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = BUTTON_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);
}

GPIO_PinState buttonsbsp_getSW1State(void)
{
	return HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
}
/*************************************** EOF ******************************************/

