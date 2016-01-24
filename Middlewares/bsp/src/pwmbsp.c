/**
 *************************************************************************************
 * \file    pwmbsp.c
 * \author  felip
 * \version Vx.y.z
 * \date    Jan 14, 2016
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

#include "stm32f4xx_hal.h"
#include "pwmbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

extern TIM_HandleTypeDef htim4;

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void pwmbsp_init(void)
{
	//TIM PWM start
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void pwmbsp_setValue(uint32_t value)
{
	if(value > 1000)
		value = 1000;

	htim4.Instance->CCR4 = value;
}

/*************************************** EOF ******************************************/

