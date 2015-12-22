/**
  *************************************************************************************
  * \file    speedcontroller.c 
  * \author  fadriano
  * \version Vx.y.z
  * \date    22/12/2015
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
#include "ledsbsp.h"

#include "arm_math.h"

/******************************* DEFINITIONS AND MACROS *******************************/



/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

static void speedController_Task(void* pvParameters);

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

volatile int32_t encoder = 0, encoderOld = 0;
volatile int32_t encoderChange = 0;
volatile float encoderChangeFiltred = 0;
volatile int32_t count = 0;

volatile float targetSpeed = 0;

volatile float kp = 10;
volatile float ki = 1.8;
volatile float kd = 0.0;

volatile float erro = 0, erro_a=0;
volatile float out = 0;

arm_pid_instance_f32 SpeedPID;

static volatile float m_in[MEAN_WINDOW + 1] = {0};
static volatile float m_out[2] = {0};

/******************************* FUNCTION  IMPLEMENTATION *****************************/

static void speedController_Task(void* pvParameters)
{
	(void)pvParameters;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	ledsbsp_output(LED_GREEN, HIGH);

	for(;;)
	{
		encoder = -encoderbsp_getLEncoder();
		encoderChange = (encoder - encoderOld);

		encoderOld = encoder;

		for(int i = 0; i < MEAN_WINDOW; i++)
		{
			m_in[i] = m_in[i+1];
		}

		m_in[MEAN_WINDOW] = (float)encoderChange;

		m_out[0] = m_out[1];

		m_out[1] = m_out[0] + ((m_in[MEAN_WINDOW] - m_in[0])/MEAN_WINDOW);

		//  *output = lp_yv[1];
		encoderChangeFiltred = m_out[1];


		erro = encoderChangeFiltred - targetSpeed;

//		posPWM[i] = arm_pid_f32(&SpeedPID[i], posErr[i]);
//		out = kp*erro + ki*(erro + erro_a) + kd*(erro - erro_a);
		out = arm_pid_f32(&SpeedPID, erro);

		erro_a = erro;

		if(out > 4095)
		{
			out = 4095;
		}
		if(out < 0)
		{
			out = 0;
		}

		dacbsp_setValue((uint16_t)out);

		count++;
		if(count > 100)
		{
			count = 0;
			ledsbsp_toogle(LED_BLUE);
		}

		vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME);
	}
}

void speedController_Init()
{
	encodersbsp_init();
	dacbsp_init();

	SpeedPID.Ki = 0.85;
	SpeedPID.Kp = 80.0;
	SpeedPID.Kd = 0.0;

	arm_pid_init_f32(&SpeedPID, true);

	dacbsp_setValue(0);
	DISABLE_CONVERSORCC;
	xTaskCreate(speedController_Task,
				"mt",
				(configMINIMAL_STACK_SIZE*3),
				NULL,
				tskIDLE_PRIORITY + 1,
				NULL);
}


void speedController_SetSpeed(float rpmSpeed, float rampTime)
{

}

/*************************************** EOF ******************************************/

