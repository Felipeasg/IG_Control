/**
  *************************************************************************************
  * \file    pwmbsp.h 
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

#ifndef BSP_INCLUDE_PWMBSP_H_
#define BSP_INCLUDE_PWMBSP_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** GLOBAL FUNCTIONS **********************************/

void pwmbsp_init(void);
void pwmbsp_setValue(uint32_t value);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* BSP_INCLUDE_PWMBSP_H_ */

/*************************************** EOF ******************************************/
