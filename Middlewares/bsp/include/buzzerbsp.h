/**
  *************************************************************************************
  * \file    buzzerbsp.h 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    02/05/2015
  * \brief   This file contains the prototypes for buzzer implementations
  *************************************************************************************
  * Manager buzzer signal to generate beep sounds.
  * 
  * Buzzer is connected on PB4 Pin
  *
  * PB4 pin is also conected to TIM3_CH1
  *
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */

#ifndef INCLUDE_BUZZERBSP_H_
#define INCLUDE_BUZZERBSP_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include "stm32f4xx_hal.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define BEEP_ON		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define BEEP_OFF	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)

#define ON	0xFF
#define OFF 0x00

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

/**
 * @brief This function initialize hardware pin to drive buzzer hardware
 *
 * @return void
 */
void buzzerbsp_init(void);

/**
 * @brief This function define if beep is on or off
 *
 * @param[in] u8_beepState
 * @return void
 */
void buzzerbsp_beep(uint8_t u8_beepState);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_BUZZERBSP_H_ */

/*************************************** EOF ******************************************/
