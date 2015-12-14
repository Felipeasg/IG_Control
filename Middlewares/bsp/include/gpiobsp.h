/**
  *************************************************************************************
  * \file    gpiobsp.h 
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

#ifndef GPIOBSP_H_
#define GPIOBSP_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include "stm32f4xx_hal.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define LOW		GPIO_PIN_RESET
#define HIGH	GPIO_PIN_SET

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

typedef enum {
	GPIOC_11
}EN_GPIOs;

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/
                                      
void gpiobsp_init(void);
void gpiobsp_output(EN_GPIOs gpioPin, GPIO_PinState state);
void gpiobsp_toogle(EN_GPIOs gpioPin);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* GPIOBSP_H_ */

/*************************************** EOF ******************************************/
