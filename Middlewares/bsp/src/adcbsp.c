/**
 *************************************************************************************
 * \file    adcbsp.c
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
#include "adcbsp.h"

#include "ledsbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc1;

extern TIM_HandleTypeDef htim3;

void (*adcCallBack)(__IO uint32_t* samples);

/********************************** LOCAL  VARIABLES **********************************/

__IO uint32_t ADCBuffer[9*16];
__IO uint32_t ovSample[2] = {0};

__IO float votageRMS = 0.0;

__IO uint32_t adc_temp = 0;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void adcbsp_init(void)
{
	//Start ADC
	//8 amostras das 9 tensoes
#if 1
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)ADCBuffer, 9*16);
#else
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)ADCBuffer, 9);
#endif

	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start(&hadc2);
	HAL_ADC_Start(&hadc3);

	//Start TIMER
	HAL_TIM_Base_Start(&htim3);
}

void adcbsp_registerCallBack(void (*prtrFunc)(__IO uint32_t *samples))
{
	adcCallBack = prtrFunc;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	//Time is 4.2 us without pll
	//Time is 6.04 us with pll
	UNUSED(hadc);


#if 1
	adc_temp =  ADCBuffer[0];
	adc_temp += ADCBuffer[9];
	adc_temp += ADCBuffer[18];
	adc_temp += ADCBuffer[27];
	adc_temp += ADCBuffer[36];
	adc_temp += ADCBuffer[45];
	adc_temp += ADCBuffer[54];
	adc_temp += ADCBuffer[63];
	adc_temp += ADCBuffer[72];
	adc_temp += ADCBuffer[81];
	adc_temp += ADCBuffer[90];
	adc_temp += ADCBuffer[99];
	adc_temp += ADCBuffer[108];
	adc_temp += ADCBuffer[117];
	adc_temp += ADCBuffer[126];
	adc_temp += ADCBuffer[135];

	ovSample[0] = adc_temp >> 2;

	adc_temp = 0;

	adc_temp =  ADCBuffer[1];
	adc_temp += ADCBuffer[10];
	adc_temp += ADCBuffer[19];
	adc_temp += ADCBuffer[28];
	adc_temp += ADCBuffer[37];
	adc_temp += ADCBuffer[46];
	adc_temp += ADCBuffer[55];
	adc_temp += ADCBuffer[64];
	adc_temp += ADCBuffer[73];
	adc_temp += ADCBuffer[82];
	adc_temp += ADCBuffer[91];
	adc_temp += ADCBuffer[100];
	adc_temp += ADCBuffer[109];
	adc_temp += ADCBuffer[118];
	adc_temp += ADCBuffer[127];
	adc_temp += ADCBuffer[136];

	ovSample[1] = adc_temp >> 2;

	adc_temp = 0;

	adc_temp =  ADCBuffer[2];
	adc_temp += ADCBuffer[11];
	adc_temp += ADCBuffer[20];
	adc_temp += ADCBuffer[29];
	adc_temp += ADCBuffer[38];
	adc_temp += ADCBuffer[47];
	adc_temp += ADCBuffer[56];
	adc_temp += ADCBuffer[65];
	adc_temp += ADCBuffer[74];
	adc_temp += ADCBuffer[83];
	adc_temp += ADCBuffer[92];
	adc_temp += ADCBuffer[101];
	adc_temp += ADCBuffer[110];
	adc_temp += ADCBuffer[119];
	adc_temp += ADCBuffer[128];
	adc_temp += ADCBuffer[137];

	ovSample[2] = adc_temp >> 2;

	if(adcCallBack != NULL)
	{
		adcCallBack(ovSample);
	}
#else

	if(adcCallBack != NULL)
	{
		adcCallBack(ADCBuffer);
	}
#endif
}

/*************************************** EOF ******************************************/

