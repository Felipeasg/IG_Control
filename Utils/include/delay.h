/**
  *************************************************************************************
  * \file    delay.h 
  * \author  Felipe Adriano <felipeadrianosg@gmail.com>
  * \version V1.0.0
  * \date    07/05/2015
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

#ifndef INCLUDE_DELAY_H_
#define INCLUDE_DELAY_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


#include "stm32f4xx.h"

/*************************************** INCLUDES *************************************/

/******************************* DEFINITIONS AND MACROS *******************************/

#define SYSTEM_FREQ 168

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

uint32_t micros(void);
uint32_t millis(void);
void elapse_us(uint32_t targetTime, uint32_t t0);
void delay_ms(uint32_t delay);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_DELAY_H_ */

/*************************************** EOF ******************************************/
