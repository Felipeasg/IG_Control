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

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "maincontroller.h"
#include "speedcontroller.h"
#include "voltagecontroller.h"
#include "meassurements.h"
#include "ledsbsp.h"
#include "uart1bsp.h"
#include "PLL_1P.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define LED_BLINK_TIME_IN_100_MS	(100/SAMPLE_TIME)

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

static void mainControlTask(void* pvParameters);

/********************************** GLOBAL VARIABLES **********************************/

extern volatile float currentSpeed;
extern volatile float encoderChangeFiltred;

extern __IO float targetVoltage;
extern __IO float rmsVoltage;
extern __IO float voltageError;

extern __IO uint32_t sampleV;
extern __IO float sampleVGGrid;
extern __IO float sampleVGenerator;

extern __IO float filteredVGenerator;

extern PLL_1P    pll1;

/********************************** LOCAL  VARIABLES **********************************/

volatile int32_t count = 0;
volatile bool plotSpeed = false;
volatile bool plotVoltage = false;
volatile uint8_t plotType = 0;
volatile bool plotEnabled  = false;
volatile bool enableVoltageControl = false;

uint32_t idx = 0;
char header[4] = {0xaa,0xaa,0xaa,0xaa};
float samples[200];
uint8_t data[804];

/******************************* FUNCTION  IMPLEMENTATION *****************************/

static void mainControlTask(void* pvParameters)
{
	(void)pvParameters;

	TickType_t xLastWakeTime = xTaskGetTickCount();


	for(;;)
	{

		SpeedController_Loop();
		if(enableVoltageControl == true)
		{
			VoltageController_Loop();
		}
		else
		{
			VoltageController_Reset();
		}

		if(plotSpeed== true)
		{

			samples[idx] = COUNTS_PER_MS_TO_RPM(currentSpeed);
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
#if 0
		if(plotVoltage == true)
		{
			//			samples[idx] = targetVoltage;
			//			samples[idx+1] = Meassurements_GetRMS();

			//			samples[idx] = 	sampleVGenerator;      //Vermelho
			//			samples[idx+1] = sampleVGGrid;  //Azul

			//			samples[idx] = 	pll1.osg1.In;      //Vermelho
			//			samples[idx+1] = pll1.osg1.qout;  //Azul

			//			samples[idx] = 	pll1.In;      //Vermelho
			//			samples[idx+1] = pll1.Mycos_1;  //Azul

			samples[idx] = 	0;      //Vermelho
			samples[idx+1] = pll1.Notch1.Out;  //Azul

			//			samples[idx+1] = ((pll1.wo/2*M_PI)*(120/4));  //Azul

			idx += 2;
			if(idx == 200)
			{
				memcpy(data, header, 4);
				memcpy(&data[4], samples, 800);
				uart1bsp_sendData(data, 804);

				idx = 0;
			}
		}
#endif

#if 0
		count++;
		if(count > LED_BLINK_TIME_IN_100_MS)
		{
			count = 0;
			ledsbsp_toogle(LED_BLUE);
		}
#endif
		vTaskDelayUntil(&xLastWakeTime, SAMPLE_TIME);
	}
}

void MainController_Init()
{
	SpeedController_Init();
	VoltageController_Init();

	xTaskCreate(mainControlTask,
			"mt",
			(configMINIMAL_STACK_SIZE*3),
			NULL,
			tskIDLE_PRIORITY + 1,
			NULL);
}


/*************************************** EOF ******************************************/

