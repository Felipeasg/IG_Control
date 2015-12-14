/**
 *************************************************************************************
 * \file    dacbsp.c
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

#include "dacbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

static void MX_GPIO_Init(void);
static void MX_DAC_Init(void);

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

DAC_HandleTypeDef hdac;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void dacbsp_init(void)
{
	MX_GPIO_Init();
	MX_DAC_Init();
	HAL_DAC_MspInit(&hdac);

	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
}

void dacbsp_setValue(uint16_t value)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
}

/* DAC init function */
void MX_DAC_Init(void)
{

	DAC_ChannelConfTypeDef sConfig;

	/**DAC Initialization
	 */
	hdac.Instance = DAC;
	HAL_DAC_Init(&hdac);

	/**DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

}

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void)
{

	/* GPIO Ports Clock Enable */
	__GPIOH_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(hdac->Instance==DAC)
	{
		/* USER CODE BEGIN DAC_MspInit 0 */

		/* USER CODE END DAC_MspInit 0 */
		/* Peripheral clock enable */
		__DAC_CLK_ENABLE();

		/**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USER CODE BEGIN DAC_MspInit 1 */

		/* USER CODE END DAC_MspInit 1 */
	}

}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{

	if(hdac->Instance==DAC)
	{
		/* USER CODE BEGIN DAC_MspDeInit 0 */

		/* USER CODE END DAC_MspDeInit 0 */
		/* Peripheral clock disable */
		__DAC_CLK_DISABLE();

		/**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

	}
	/* USER CODE BEGIN DAC_MspDeInit 1 */

	/* USER CODE END DAC_MspDeInit 1 */

}

/*************************************** EOF ******************************************/

