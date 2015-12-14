/**
  *************************************************************************************
  * \file    delay.c 
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



/*************************************** INCLUDES *************************************/

#include "delay.h"


/******************************* DEFINITIONS AND MACROS *******************************/

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

/******************************* FUNCTION  IMPLEMENTATION *****************************/

/**
  * @brief Realiza a leitura do tempo passado desde o inicio da operação
  * @param Nenhum
  * @return microsegundos desde o inicio da operação
  */
uint32_t micros(void)
{
	return (HAL_GetTick()*1000 + 1000 - SysTick->VAL/SYSTEM_FREQ);
}


/**
  * @brief Realiza a leitura do tempo passado desde o inicio da operação
  * @param Nenhum
  * @return milisegundos desde o inicio da operação
  */
uint32_t millis(void)
{
	return HAL_GetTick();
}


/**
  * @brief Aguarda um tempo em microsegundos em relação a um tempo inicial
  * @param targetTime: Tempo que será aguardado em microsegundos
  * @param t0: Tempo inicial em microsegundos
  * @return Nenhum
  */
void elapse_us(uint32_t targetTime, uint32_t t0)
{
	while ((micros() - t0) < targetTime);
}


/**
  * @brief Gera um atraso
  * @param delay: Tempo do atraso em milisegundos
  * @return Nenhum
  */
void delay_ms(uint32_t delay)
{
	uint32_t ms_start = HAL_GetTick();

	while ((HAL_GetTick() - ms_start) < delay);
}


/*************************************** EOF ******************************************/

