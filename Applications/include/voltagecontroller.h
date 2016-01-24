/**
  *************************************************************************************
  * \file    voltagecontroller.h 
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

#ifndef INCLUDE_VOLTAGECONTROLLER_H_
#define INCLUDE_VOLTAGECONTROLLER_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

void VoltageController_Init(void);
void VoltageController_Loop(void);
void VoltageController_Reset(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_VOLTAGECONTROLLER_H_ */

/*************************************** EOF ******************************************/
