/**
  *************************************************************************************
  * \file    motorsbsp.h 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    02/05/2015
  * \brief   This file contains the prototypes to motor control drive functions
  *************************************************************************************
  * A set of prototypes that enable acess to the motor.
  * Its possible turn on in clock and counter clockwise and also defines the pwm
  * to sendo a pin that drive the motor.
  * Are to motor to be controlled right (R) and left (L) motor
  *
  * Left Motor (PWM) -  PB6/TIM4_CH1
  * Left Motor In1   -  PB9/Output
  * Left Motor In2   -  PB8/Output
  *
  * Rigth Motor (PWM) - PB7/TIM4_CH2
  * Rigth Motor In1   - PC2/Output
  * Rigth Motor In2   - PC3/Output
  * 
  * Motors standby - PC1/Output
  *
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */

#ifndef INCLUDE_MOTORSBSP_H_
#define INCLUDE_MOTORSBSP_H_

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
 * @brief This function intialize pins connected to drive of motor and set the pins as
 * pwm.
 *
 * @return void
 */
void motorsbsp_init(void);

/**
 * @brief This function set pwm to both motor R and L
 *
 * PWM Ranges is -1000 to 1000
 * The (-) or (+) signal define the direction of rotation
 *
 * @param[in] s16_pwmLeft pwm of left motor ( -1000 < s16_pwmLeft < 1000)
 * @param[in] s16_pwmRight pwm of rigth motor ( -1000 < s16_pwmrigth < 1000)
 *
 * @param void
 */
void motorsbsp_setPWM(int16_t s16_pwmLeft, int16_t s16_pwmRight);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_MOTORSBSP_H_ */

/*************************************** EOF ******************************************/
