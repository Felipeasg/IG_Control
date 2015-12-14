/**
  *************************************************************************************
  * \file    encodersbsp.h 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    01/05/2015
  * \brief   This file contains the prototypes functions to work with encoders
  *************************************************************************************
  * Encoders are measure the number of turn of some rotatory system.
  * This library source a set of functions to read and reset encoders to control velocity
  * position and acceleration.
  * 
  * Encoder 1 (Left) are connected on PA0 (B) and PA1 (A) - TIM 2
  * Encoder 2 (Right) are connected on PA15 (B) and PB3 (A) - TIM 5
  *
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT 2011 ENTERPRISE</center></h2>
  *************************************************************************************
  */

#ifndef INCLUDE_ENCODERSBSP_H_
#define INCLUDE_ENCODERSBSP_H_

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
 * @brief This function initilize pins and timer like encoder counter
 *
 * In this function pins are initilized as input and are conected to timers
 * to count automaticaly encoder quadrature pulses without interruption.
 * Some filter are also configureted.
 *
 * @return void
 */
void encodersbsp_init(void);

/**
 * @brief Set a offset to enconder connected on TIM2
 *
 * This funtion is util to initialize enconder with some value.
 *
 * @param[in] u32_offset
 * @return void
 */
void encodersbsp_setLEncoder(uint32_t u32_offset);

/**
 * @brief This function reset the enconder TIM2
 *
 * @return void
 */
void encoderbsp_resetLEncoder(void);

/**
 * @brief This function get the number os pulses counted by encoder
 *
 * @return The number of pulses acumulate by TIM generate by encoder
 */
uint32_t encoderbsp_getLEncoder(void);

/**
 * @brief Set a offset to enconder connected on TIM5
 *
 * This funtion is util to initialize enconder with some value.
 *
 * @param u32_offset
 * @return void
 */
void encodersbsp_setREncoder(uint32_t u32_offset);

/**
 * @brief This function reset the enconder TIM5
 *
 * @return void
 */
void encoderbsp_resetREncoder(void);

/**
 * @brief This function get the number os pulses counted by encoder
 *
 * @return The number of pulses acumulate by TIM generate by encoder
 */
uint32_t encoderbsp_getREncoder(void);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_ENCODERSBSP_H_ */

/*************************************** EOF ******************************************/
