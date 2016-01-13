/**
 *************************************************************************************
 * \file    PID.h
 * \author  fadriano
 * \version Vx.y.z
 * \date    Dec 23, 2015
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

#ifndef INCLUDE_PID_H_
#define INCLUDE_PID_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus


/*************************************** INCLUDES *************************************/

#include <stdint.h>

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	//Acumulador para as multiplicacoes
	float dAcumulador;
	//Guarda a realimentacao
	float wRealimentacao;
	//historico do erro
	float wErro[2];
	//Variavel de saida:
	float wSaida;
}PID_t;

/********************************** GLOBAL VARIABLES **********************************/


/********************************** GLOBAL FUNCTIONS **********************************/

void PID_Init(PID_t *pxPID);
float PID_Compute(PID_t *pxPID, float wSetPoint);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* INCLUDE_PID_H_ */

/*************************************** EOF ******************************************/
