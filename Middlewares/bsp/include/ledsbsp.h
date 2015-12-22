/**
  *************************************************************************************
  * \file    ledsbsp.h 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    01/05/2015
  * \brief   This is the prototypes of ledsbsp library
  *************************************************************************************
  * 
  * This contains the prototypes to manipulates the leds on board.
  *
  * The led on board are showed bellow with their respective pins:
  *
  *  	  LED1 - PB5
  *  	  LED2 - PC0
  *  	  LED3 - PA5
  *  	  LED4 - PB10
  *  	  LED5 - PB11
  *
  * Leds are useful to debug your code or to show some state of
  * sensors on the board
  *
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT 2011 ENTERPRISE</center></h2>
  *************************************************************************************
  */

#ifndef BSP_LEDSBSP_H_
#define BSP_LEDSBSP_H_

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
	LED_GREEN,
	LED_ORANGE,
	LED_RED,
	LED_BLUE,
	PIN_PC11
}EN_Leds;
/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

/**
 * @brief This function initialize the gpio pins of STM32 as output pins
 *  	  to drive the leds
 *
 * @return void
 */
void ledsbsp_init(void);

/**
 * @brief Set the respective led indicate by en_led and its state
 * 		  indicated by en_pinstate
 *
 *
 * @param[in] en_led The led to be configured HIGH or LOW (LED1, LED2, LED3, LED4 or LED5)
 * @param[in] u8_state the state of a led (HIGH or LOW)
 * @return void
 */
void ledsbsp_output(EN_Leds en_led, GPIO_PinState en_pinstate);

/**
 * @brief Turn HIGH all the leds of the board
 *
 * @return void
 */
void ledsbsp_setAllLeds(void);

/**
 * @brief Turn LOW all the leds of the board
 */
void ledsbsp_resetAllLeds(void);

/**
 * @brief This function invert the state of some led connect on a pin
 *
 * @param[in] en_led
 * @return void
 */
void ledsbsp_toogle(EN_Leds en_led);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* BSP_LEDSBSP_H_ */

/*************************************** EOF ******************************************/
