/**
  *************************************************************************************
  * \file    meassurements.h 
  * \author  felip
  * \version Vx.y.z
  * \date    Jan 14, 2016
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

#ifndef INCLUDE_MEASSUREMENTS_H_
#define INCLUDE_MEASSUREMENTS_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

/******************************* DEFINITIONS AND MACROS *******************************/

#define ADC_BITS	14
#define ADC_COUNTS  	((1 << ADC_BITS) - 1)

#define STABILITATION_TIME_MS	100

#define PLOT_PLL_INPUTS 		2
#define PLOT_NORTCH_FILTER 		3
#define PLOT_VOLTAGE_CONTROL  	4


/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

void Meassurements_Init(void);
float Meassurements_GetRMS(void);
void Meassurements_ResetPLL(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_MEASSUREMENTS_H_ */

/*************************************** EOF ******************************************/
