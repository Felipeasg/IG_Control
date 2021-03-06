/**
  *************************************************************************************
  * \file    speedcontroller.h 
  * \author  felip
  * \version Vx.y.z
  * \date    Jan 13, 2016
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

#ifndef INCLUDE_SPEEDCONTROLLER_H_
#define INCLUDE_SPEEDCONTROLLER_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include <stdbool.h>
#include "maincontroller.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define ENABLE_CONVERSORCC  gpiobsp_output(GPIOC_11, HIGH);
#define DISABLE_CONVERSORCC gpiobsp_output(GPIOC_11, LOW);

#define ENCODER_PULSES 	1024.0F//360.0F
#define OFFSET 0.0F

#define MEAN_WINDOW 30

#define RPM_TO_COUNTS_PER_MS(x)		(((x*(ENCODER_PULSES*4.0F))/(SAMPLE_FREQ*60.0F)) + OFFSET)
#define COUNTS_PER_MS_TO_RPM(x)		(((x*SAMPLE_FREQ*60.0F)/(ENCODER_PULSES*4.0F)) - OFFSET)

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/
                                      
void SpeedController_Init(void);
void SpeedController_SetSpeed(float rpmSpeed, float rampTime);
void SpeedController_Loop(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_SPEEDCONTROLLER_H_ */

/*************************************** EOF ******************************************/
