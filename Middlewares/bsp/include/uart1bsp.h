/**
  *************************************************************************************
  * \file    uart1bsp.h 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    03/05/2015
  * \brief   This file contains the prototypes to uart1 module communication
  *************************************************************************************
  * description
  * 
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */

#ifndef INCLUDE_UART1BSP_H_
#define INCLUDE_UART1BSP_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define BAUD_RATE		115200
#define USART_TIMEOUT	100

#define STDIO_UART



/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

extern UART_HandleTypeDef huart1;

/********************************** GLOBAL FUNCTIONS **********************************/

/**
 * @brief
 *
 * @return void
 */
void uart1bsp_init(void);

uint8_t uart1bsp_getByte(void);
uint32_t uart1bsp_GetNBytes(uint8_t *buff, uint32_t u32_size);
uint32_t uart1bsp_peak(void);
void usart1bsp_clearBuffer(void);
void uart1bsp_sendData(void* data, uint32_t size);

void commandParser(void);

uint32_t usart1bsp_GetNBytes(uint8_t *buff, uint32_t u32_size);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_UART1BSP_H_ */

/*************************************** EOF ******************************************/
