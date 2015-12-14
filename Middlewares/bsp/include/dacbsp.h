/**
  *************************************************************************************
  * \file    dacbsp.h 
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

#ifndef DACBSP_H_
#define DACBSP_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include "stm32f4xx_hal.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

void dacbsp_init(void);
void dacbsp_setValue(uint16_t value);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* DACBSP_H_ */

/*************************************** EOF ******************************************/
