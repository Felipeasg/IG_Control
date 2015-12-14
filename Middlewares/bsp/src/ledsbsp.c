/**
  *************************************************************************************
  * \file    ledsbsp.c 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    01/05/2015
  * \brief   This is the implementation to ledsbsp
  *************************************************************************************
  * 
  * This is the implementation of the ledsbps.
  * Important details about implementation should go in comments.
  *
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT 2011 ENTERPRISE</center></h2>
  *************************************************************************************
  */



/*************************************** INCLUDES *************************************/

#include "ledsbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void ledsbsp_init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
	  __GPIOD_CLK_ENABLE();
	  __GPIOC_CLK_ENABLE();

	  /*Configure GPIO pins : PDs */
	  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  /*Configure GPIO pins : PC11  */
	  GPIO_InitStruct.Pin = GPIO_PIN_11;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

void ledsbsp_outputLed(EN_Leds en_led, GPIO_PinState en_pinstate)
{
	switch(en_led)
	{
	case LED_GREEN:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, en_pinstate);
		break;
	case LED_ORANGE:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, en_pinstate);
		break;
	case LED_RED:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, en_pinstate);
		break;
	case LED_BLUE:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, en_pinstate);
		break;
	case PIN_PC11:
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, en_pinstate);
		break;

	default:
		break;
	}
}

void ledsbsp_setAllLeds(void)
{
	ledsbsp_outputLed(LED_GREEN, HIGH);
	ledsbsp_outputLed(LED_ORANGE, HIGH);
	ledsbsp_outputLed(LED_RED, HIGH);
	ledsbsp_outputLed(LED_BLUE, HIGH);
//	ledsbsp_outputLed(LED5, HIGH);
}

void ledsbsp_resetAllLeds(void)
{
	ledsbsp_outputLed(LED_GREEN, LOW);
	ledsbsp_outputLed(LED_ORANGE, LOW);
	ledsbsp_outputLed(LED_RED, LOW);
	ledsbsp_outputLed(LED_BLUE, LOW);
//	ledsbsp_outputLed(LED5, LOW);
}

void ledsbsp_toogleOutputLed(EN_Leds en_led)
{
	switch(en_led)
		{
		case LED_GREEN:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			break;
		case LED_ORANGE:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			break;
		case LED_RED:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			break;
		case LED_BLUE:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			break;
		case PIN_PC11:
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
			break;

		default:
			break;
		}
}

/*************************************** EOF ******************************************/

