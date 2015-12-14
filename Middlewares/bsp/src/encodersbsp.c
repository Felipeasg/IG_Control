/**
  *************************************************************************************
  * \file    encodersbsp.c
  * \author  root
  * \version V1.0
  * \date    01/05/2015
  * \brief   Here are the implementation of functions to work with encoders
  *************************************************************************************
  * This implementations contains the code to manipulate encoders.
  * More detail can be obtained in the comments into each function
  *
  * @attention
  *
  *
  *
  * <h2><center>&copy; COPYRIGHT 2011 ENTERPRISE</center></h2>
  *************************************************************************************
  */



/*************************************** INCLUDES *************************************/

#include "encodersbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

static TIM_HandleTypeDef mst_htim2;
static TIM_HandleTypeDef mst_htim5;


/******************************* FUNCTION  IMPLEMENTATION *****************************/

void encodersbsp_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_Encoder_InitTypeDef sConfig;

	__TIM2_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	__TIM5_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	mst_htim2.Instance = TIM2;
	mst_htim2.Init.Prescaler = 0;
	mst_htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	mst_htim2.Init.Period = 0xffffffff;
	mst_htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 3;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 3;
	HAL_TIM_Encoder_Init(&mst_htim2, &sConfig);


	mst_htim5.Instance = TIM5;
	mst_htim5.Init.Prescaler = 0;
	mst_htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	mst_htim5.Init.Period = 0xffffffff;
	mst_htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 3;
	sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 3;
	HAL_TIM_Encoder_Init(&mst_htim5, &sConfig);

	HAL_TIM_Encoder_Start(&mst_htim2, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&mst_htim2, TIM_CHANNEL_2);

	HAL_TIM_Encoder_Start(&mst_htim5, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&mst_htim5, TIM_CHANNEL_2);

}

void encodersbsp_setLEncoder(uint32_t u32_offset)
{
	TIM2->CNT = u32_offset;
}

void encoderbsp_resetLEncoder(void)
{
	TIM2->CNT = 0;
}

uint32_t encoderbsp_getLEncoder(void)
{
	return TIM2->CNT;
}

void encodersbsp_setREncoder(uint32_t u32_offset)
{
	TIM5->CNT = u32_offset;
}

void encoderbsp_resetREncoder(void)
{
	TIM5->CNT = 0;
}

uint32_t encoderbsp_getREncoder(void)
{
	return TIM5->CNT;
}
/*************************************** EOF ******************************************/
