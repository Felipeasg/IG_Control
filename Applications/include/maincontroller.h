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

//#define SAMPLE_FREQ	1000.0F //Hz
#define SAMPLE_TIME	1 //ms
#define SAMPLE_FREQ 1/(SAMPLE_TIME/1000.0)

#define PLOT_SPEED	1

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

void MainController_Init();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* SPEEDCONTROLLER_H_ */

/*************************************** EOF ******************************************/
