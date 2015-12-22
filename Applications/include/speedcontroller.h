/**
  *************************************************************************************
  * \file    speedcontroller.h 
  * \author  fadriano
  * \version Vx.y.z
  * \date    22/12/2015
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

#ifndef SPEEDCONTROLLER_H_
#define SPEEDCONTROLLER_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"

#include "gpiobsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define ENABLE_CONVERSORCC  gpiobsp_output(GPIOC_11, HIGH);
#define DISABLE_CONVERSORCC gpiobsp_output(GPIOC_11, LOW);

#define ENCODER_PULSES 	1024.0F//360.0F
#define OFFSET 0.0F
#define SAMPLE_FREQ	1000.0F //Hz
#define SAMPLE_TIME	1 //ms

#define MEAN_WINDOW 16

#define RPM_TO_COUNTS_PER_MS(x)		(((x*(ENCODER_PULSES*4.0F))/(SAMPLE_FREQ*60.0F)) + OFFSET)
#define COUNTS_PER_MS_TO_RPM(x)		(((x*SAMPLE_FREQ*60.0F)/(ENCODER_PULSES*4.0F)) - OFFSET)

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

void speedController_Init(void);
void speedController_SetSpeed(float rpmSpeed, float rampTime);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* SPEEDCONTROLLER_H_ */

/*************************************** EOF ******************************************/
