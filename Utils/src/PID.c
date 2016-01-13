/**
 *************************************************************************************
 * \file    PID.c
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


/*************************************** INCLUDES *************************************/

#include "PID.h"

#include <string.h>

/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void PID_Init(PID_t *pxPID)
{
	float kp, ki, kd;

	kp = pxPID->Kp;
	ki = pxPID->Ki;
	kd = pxPID->Kd;

	//Checa parametros:
	//Ponteiro nulo?
	if((PID_t *)0 == pxPID)
	{
		//encerra funcao
		return;
	}
	//Limpa bloco de controle:
	memset((uint8_t*)pxPID, 0 , sizeof(PID_t));

	pxPID->Kp = kp;
	pxPID->Ki = ki;
	pxPID->Kd = kd;
}

float PID_Compute(PID_t *pxPID, float in)
{
	//Variaveis auxiliares:
	float dSaida = 0;
	float dAcumulador = 0;

	//Checa parametros:
	//checa por ponteiro nulo:
	if((PID_t *)0 == pxPID)
	{
		//encerra funcao:
		return(0);
	}
	//computa o erro corrente:
	pxPID->wErro[0] = in - pxPID->wRealimentacao;
	//integra erro
	dAcumulador +=pxPID->Ki * pxPID->wErro[0];
	//Calcula termo derivativo:
	dSaida = pxPID->Kd * (pxPID->wErro[0] - pxPID->wErro[1]);
	//Calcula termo proporcional:
	dSaida += pxPID->Kp * pxPID->wErro[0];
	//soma o integral e encerra a computacao:
	dSaida += dAcumulador ;
	//escala valor para 16bits sem sinal:
	pxPID->wSaida = dSaida;
	//atualiza o historico de erro:
	pxPID->wErro[1] = pxPID->wErro[0];
	//retorna variavel de saida:
	return(pxPID->wSaida);
}
/*************************************** EOF ******************************************/

