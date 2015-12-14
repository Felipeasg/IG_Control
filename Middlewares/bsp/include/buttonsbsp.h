/**
  *************************************************************************************
  * \file    buttonsbsp.h 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    03/05/2015
  * \brief   This file contains the prototypes function to work with buttons
  *************************************************************************************
  * 
  *
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */

#ifndef INCLUDE_BUTTONSBSP_H_
#define INCLUDE_BUTTONSBSP_H_

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

/**
 * TODO: Write documentation
 * @brief
 *
 * @return
 */
void buttonsbsp_init(void);

/**
 * TODO: Write documentation
 * @brief
 *
 * @return
 */
GPIO_PinState buttonsbsp_getSW1State(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_BUTTONSBSP_H_ */

/*************************************** EOF ******************************************/
