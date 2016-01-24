/**
 *************************************************************************************
 * \file    voltagecontroller.c
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



/*************************************** INCLUDES *************************************/

#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "voltagecontroller.h"
#include "arm_math.h"
#include "meassurements.h"
#include "pwmbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define SATURATION_MAX 15000000
#define SATURATION_MIN -2000000

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

arm_pid_instance_f32 voltagePID;

__IO float targetVoltage = 223.0;
__IO float rmsVoltage = 0.0;
__IO float voltageError = 0;
__IO float output = 0;
__IO float pwm = 0;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void VoltageController_Init(void)
{
	Meassurements_Init();
	pwmbsp_init();
	pwmbsp_setValue(0);

	voltagePID.Kp = -2.8;
	voltagePID.Ki = -0.015;
	voltagePID.Kd = 0.0;

	arm_pid_init_f32(&voltagePID, true);

	targetVoltage = 223.0;
}

void VoltageController_Loop(void)
{
	rmsVoltage = Meassurements_GetRMS();

	if(rmsVoltage > 100)
	{
		voltageError = targetVoltage - rmsVoltage;

		//	if(voltageError < 10)
		//	{
		//efetuar controle somente quando o erro é negativo pois senao o integrativo aumenta para o lado oposto
		//e fode o controle
		output = arm_pid_f32(&voltagePID, voltageError);

		pwm = output;

		//	pwm = 1000 + output;
		//	if(pwm > 1000)
		//		pwm = 1000;
		//	if(pwm < 0)
		//		pwm = 0;

//		if(voltagePID.state[2] > SATURATION_MAX)
//		{
//			voltagePID.state[2] = SATURATION_MAX;
//		}
//		else if(voltagePID.state[2] < SATURATION_MIN)
//		{
//			voltagePID.state[2] = SATURATION_MAX;
//		}

		if(pwm > 850)
			pwm = 850;
		if(pwm < 0)
			pwm = 0;

		pwmbsp_setValue((uint32_t)(pwm));
	}
	else
	{
		pwmbsp_setValue(0);
		arm_pid_reset_f32(&voltagePID);
	}
	//	}
	//	else
	//	{
	//		pwmbsp_setValue(0);
	//	}

}

void VoltageController_Reset(void)
{
	pwmbsp_setValue(0);
	arm_pid_reset_f32(&voltagePID);
}

/*************************************** EOF ******************************************/

