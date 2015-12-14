/**
 *************************************************************************************
 * \file    gpiobsp.c
 * \author  fadriano
 * \version Vx.y.z
 * \date    14/12/2015
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

#include "gpiobsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void gpiobsp_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__GPIOC_CLK_ENABLE();


	/*Configure GPIO pins : PC11  */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

void gpiobsp_output(EN_GPIOs gpioPin, GPIO_PinState state)
{
	switch(gpioPin)
	{
	case GPIOC_11:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, state);
		break;
	default:
		break;
	}
}

void gpiobsp_toogle(EN_GPIOs gpioPin)
{
	switch(gpioPin)
	{
	case GPIOC_11:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
		break;
	}
}
/*************************************** EOF ******************************************/

