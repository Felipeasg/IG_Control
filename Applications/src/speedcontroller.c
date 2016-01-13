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
#include "PID.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define ARM_PID
//#define SIMPLE_PID

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

static void speedController_Task(void* pvParameters);
static void updateEncoderSpeed(void);
static void pidController(void);

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

volatile int32_t encoder[2];
volatile int32_t encoderChange = 0;
volatile float encoderChangeFiltred = 0;
volatile int32_t count = 0;
volatile bool plotSpeed = false;

volatile float targetSpeed = 0;

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

uint32_t idx = 0;
char header[4] = {0xaa,0xaa,0xaa,0xaa};
float samples[200];
uint8_t data[804];

/******************************* FUNCTION  IMPLEMENTATION *****************************/

static void speedController_Task(void* pvParameters)
{
	(void)pvParameters;

	TickType_t xLastWakeTime = xTaskGetTickCount();

	ledsbsp_output(LED_GREEN, HIGH);

	for(;;)
	{
		updateEncoderSpeed();
		pidController();

		if(plotSpeed == true)
		{
			samples[idx] = COUNTS_PER_MS_TO_RPM(targetSpeed);
			samples[idx+1] = COUNTS_PER_MS_TO_RPM(encoderChangeFiltred);

			idx += 2;
			if(idx == 200)
			{
				memcpy(data, header, 4);
				memcpy(&data[4], samples, 800);
				uart1bsp_sendData(data, 804);

				idx = 0;
			}
		}

		count++;
		if(count > LED_BLINK_TIME_IN_100_MS)
		{
			count = 0;
			ledsbsp_toogle(LED_BLUE);
		}

		vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME);
	}
}

static void updateEncoderSpeed(void)
{
	encoder[1] = -encoderbsp_getLEncoder();
	encoderChange = (encoder[1] - encoder[0]);
	encoder[0] = encoder[1];

	for(int i = 0; i < MEAN_WINDOW; i++)
	{
		m_in[i] = m_in[i+1];
	}

	m_in[MEAN_WINDOW] = ((float)encoderChange);

	m_out[0] = m_out[1];

	m_out[1] = m_out[0] + ((m_in[MEAN_WINDOW] - m_in[0])/MEAN_WINDOW);

	encoderChangeFiltred = m_out[1];

}

static void pidController(void)
{

#if defined(SIMPLE_PID)
//	pidError[1] = encoderChangeFiltred - targetSpeed;
//	pidOutput = kp*pidError[1] + ki*(pidError[1] + pidError[0]) + kd*(pidError[1] - pidError[0]);
//	pidError[0] = pidError[1];
	pidError = encoderChangeFiltred - targetSpeed;
	pidOutput = PID_Compute(&SpeedPID, pidError);
#elif defined(ARM_PID)
	pidError = encoderChangeFiltred - targetSpeed;
	pidOutput = arm_pid_f32(&SpeedPID, pidError);
#else
#error "Some PID is needed!!!"
#endif

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

void speedController_Init()
{
	encodersbsp_init();
	dacbsp_init();

	SpeedPID.Kp = 1000.0;
	SpeedPID.Ki = 1.02;
	SpeedPID.Kd = 0.0;

#if defined(SIMPLE_PID)
	PID_Init(&SpeedPID);
#elif defined(ARM_PID)
	arm_pid_init_f32(&SpeedPID, true);
#endif

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
	rpmSpeed = rampTime;
	rampTime = rpmSpeed;
}

/*************************************** EOF ******************************************/

