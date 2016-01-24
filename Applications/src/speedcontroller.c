/**
 *************************************************************************************
 * \file    speedcontroller.c
 * \author  felip
 * \version Vx.y.z
 * \date    Jan 13, 2016
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

#include "speedcontroller.h"
#include "encodersbsp.h"
#include "dacbsp.h"
#include "arm_math.h"
#include "PID.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define ARM_PID
//#define SIMPLE_PID

#define N 20.0F
#define ALPHA (N-1)/N
#define BETA 1/N

#define SATURATION_MAX 100000000
#define SATURATION_MIN -100000000

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

static void updateEncoderSpeed(void);
static void pidController(void);

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

volatile uint32_t encoder[2];
volatile uint32_t encoderChange = 0;
volatile float encoderChangeFiltred = 0;

volatile float currentSpeed = 0;
volatile float targetSpeed = 0;
volatile float acc = 0;
volatile float dec = 0;

#if defined(SIMPLE_PID)
PID_t SpeedPID;
#elif defined(ARM_PID)
arm_pid_instance_f32 SpeedPID;
#else
#error "Some PID is needed!!!"
#endif


volatile float pidError = 0.0;
volatile float pidOutput = 0;

static volatile float m_in[MEAN_WINDOW + 1] = {0};
static volatile float m_out[2] = {0};

static volatile float meanIn = 0;
static volatile float meanOut[2] = {0};

volatile bool enableRamp = false;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void SpeedController_Init(void)
{
	encodersbsp_init();
	dacbsp_init();

//	SpeedPID.Kp = 300;
//	SpeedPID.Ki = 0.55;
//	SpeedPID.Kd = 0.0;

	//bom
//	SpeedPID.Kp = 145.8;
//	SpeedPID.Ki = 0.38;
//	SpeedPID.Kd = 0.0;

	//bom +
//	SpeedPID.Kp = 118.4;
//	SpeedPID.Ki = 0.1;
//	SpeedPID.Kd = 0.0;

	//mbom resultados tirados com ele
//		SpeedPID.Kp = 112.4;
//		SpeedPID.Ki = 0.156;
//		SpeedPID.Kd = 0.0001;

	SpeedPID.Kp = 713.0;
	SpeedPID.Ki = 0.362;
	SpeedPID.Kd = 0.0001;

//	SpeedPID.Kp = 125.8;
//	SpeedPID.Ki = 0.30;
//	SpeedPID.Kd = 0.0;

#if defined(SIMPLE_PID)
	PID_Init(&SpeedPID);
#elif defined(ARM_PID)
	arm_pid_init_f32(&SpeedPID, true);
#endif

	dec = RPM_TO_COUNTS_PER_MS(1800)/(3500);

	dacbsp_setValue(0);

	DISABLE_CONVERSORCC;
}
static void updateEncoderSpeed(void)
{
	encoder[1] = encoderbsp_getLEncoder();
	encoderChange = (encoder[1] - encoder[0]);
	encoder[0] = encoder[1];

#if 0
	for(int i = 0; i < MEAN_WINDOW; i++)
	{
		m_in[i] = m_in[i+1];
	}

	m_in[MEAN_WINDOW] = ((float)encoderChange);

	m_out[0] = m_out[1];

	m_out[1] = m_out[0] + ((m_in[MEAN_WINDOW] - m_in[0])/MEAN_WINDOW);

	encoderChangeFiltred = m_out[1];
#endif

	meanIn = (float)encoderChange;
	meanOut[0] = ALPHA*meanOut[1] + BETA*meanIn;
	meanOut[1] = meanOut[0];

	encoderChangeFiltred = meanOut[0];

}

void updateCurrentSpeed(void)
{
	if(currentSpeed < targetSpeed)
	{
		currentSpeed += acc;
		if(currentSpeed > targetSpeed)
			currentSpeed = targetSpeed;
	}
	else if(currentSpeed > targetSpeed)
	{
		currentSpeed -= dec;
		if(currentSpeed < targetSpeed)
			currentSpeed = targetSpeed;
	}
}

static void pidController(void)
{

	if(currentSpeed > 0)
	{
#if defined(SIMPLE_PID)
		//	pidError[1] = encoderChangeFiltred - targetSpeed;
		//	pidOutput = kp*pidError[1] + ki*(pidError[1] + pidError[0]) + kd*(pidError[1] - pidError[0]);
		//	pidError[0] = pidError[1];
		pidError = encoderChangeFiltred - currentSpeed;
		pidOutput = PID_Compute(&SpeedPID, pidError);
#elif defined(ARM_PID)
		pidError = currentSpeed - encoderChangeFiltred;
		pidOutput = arm_pid_f32(&SpeedPID, pidError);
#else
#error "Some PID is needed!!!"
#endif

//		if(SpeedPID.state[2] > SATURATION_MAX)
//		{
//			SpeedPID.state[2] = SATURATION_MAX;
//		}
//		else if(SpeedPID.state[2] < SATURATION_MIN)
//		{
//			SpeedPID.state[2] = SATURATION_MAX;
//		}
		//Truncate output
		if(pidOutput > 4095)
		{
			pidOutput = 4095;
		}
		if(pidOutput < 0)
		{
			pidOutput = 0;
		}

		dacbsp_setValue((uint16_t)pidOutput);
	}
	else
	{
		dacbsp_setValue(0);
		arm_pid_reset_f32(&SpeedPID);
	}
}

void SpeedController_Loop(void)
{
	updateEncoderSpeed();
	if(enableRamp == true)
	{
		updateCurrentSpeed();
	}
	else
	{
		currentSpeed = targetSpeed;
	}
	pidController();
}

void SpeedController_SetSpeed(float rpmSpeed, float rampTime)
{
		acc = RPM_TO_COUNTS_PER_MS(rpmSpeed)/(rampTime*1000);
		targetSpeed = RPM_TO_COUNTS_PER_MS(rpmSpeed);



}

/*************************************** EOF ******************************************/

