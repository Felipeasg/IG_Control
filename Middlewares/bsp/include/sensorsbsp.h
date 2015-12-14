/**
  *************************************************************************************
  * \file    sensorsbsp.h 
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

#ifndef BSP_SENSORSBSP_H_
#define BSP_SENSORSBSP_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include "stm32f4xx.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define INFINITO	8888		// Valor que indica a não identificação de linha

#define THRESHOLD	1000
#define PAREDE_ESQUERDA	0b100
#define PAREDE_FRONTAL	0b010
#define PAREDE_DIREITA	0b001

// Constantes para calibrar o valor da tensão medida
#define VBAT_RAW	3434
#define VBAT_V		8270

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

/**
 * @brief
 *
 * @return void
 */
void sensorsbsp_init(void);

/**
 * @brief
 *
 * @param
 * @param
 * @param
 * @param
 * @return
 */
int32_t sensorsbsp_getWallSensors(int32_t* lf, int32_t* l, int32_t* r, int32_t* rf);

/**
 * @brief
 *
 * @return
 */
int32_t sensorsbsp_getLineSensors();

/**
 * @brief
 *
 * @return
 */
int32_t sensorsbsp_getGyro();

/**
 * @brief
 *
 * @return
 */
int32_t sensorsbsp_getGyroVoltage(void);

int32_t sensorsbsp_getGyroVref(void);



#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* BSP_SENSORSBSP_H_ */

/*************************************** EOF ******************************************/
