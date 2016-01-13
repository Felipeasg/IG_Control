//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"

#include "buttonsbsp.h"
#include "ledsbsp.h"
#include "uart1bsp.h"
#include "gpiobsp.h"

#include "speedcontroller.h"

#include "arm_math.h"
#include "PID.h"

extern volatile float targetSpeed;

extern volatile float encoderChangeFiltred;
extern volatile int32_t encoderChange;

extern volatile float pidError;
//extern volatile float erro_a=0;
extern volatile float pidOutput;

extern arm_pid_instance_f32 SpeedPID;
extern volatile bool plotSpeed;
//extern PID_t SpeedPID;

void vApplicationTickHook( void )
{
	HAL_IncTick();
}

static void commandsTask(void* pvParameters)
{
	(void)pvParameters;

	char command[200] = {0};
	uint16_t receivedBytes = 0;

	for(;;)
	{
		receivedBytes = uart1bsp_peak();
		if(receivedBytes >= 2)
		{
			uart1bsp_GetNBytes((uint8_t*)command,receivedBytes);
			command[receivedBytes+1] = '\0';

			char* pch = strtok(command, " ");

			if(pch[0] == 's')
			{
				if(pch[1] == 'p')
				{
					pch = strtok (NULL," ");

					targetSpeed = RPM_TO_COUNTS_PER_MS(atof(pch));

					//					printf("Target Speed = %f\n", COUNTS_PER_MS_TO_RPM(targetSpeed));
				}
				if(pch[1] == 'i')
				{
					ENABLE_CONVERSORCC;
					//					printf("System init\n");
				}
				if(pch[1] == 's')
				{
					DISABLE_CONVERSORCC;
					//					printf("System stop\n");
				}
				if(pch[1] == 't')
				{
					printf("kp = %f\n", SpeedPID.Kp);
					printf("ki = %f\n", SpeedPID.Ki);
					printf("kd = %f\n", SpeedPID.Kd);
					printf("Vel = %f RPM volatile bool plotSpeed = false;\n", COUNTS_PER_MS_TO_RPM(encoderChangeFiltred));
					printf("Vel = %ld Counts \n", encoderChange);
					printf("Vel = %f Counts Filt. \n", encoderChangeFiltred);
					printf("Erro = %f Counts \n", pidError);
					printf("Out = %f ADC Counts \n", pidOutput);
					printf("Target Speed = %f RPM\r\n", COUNTS_PER_MS_TO_RPM(targetSpeed));
					printf("---------------------------\n");
				}
			}
			else if(pch[0] == 'k')
			{
				if(pch[1] == 'p')
				{
					pch = strtok (NULL," ");

					SpeedPID.Kp = atof(pch);

					arm_pid_init_f32(&SpeedPID, false);
					//					printf("kp = %f\n", SpeedPID.Kp);
				}
				else if(pch[1] == 'i')
				{
					pch = strtok (NULL," ");

					SpeedPID.Ki = atof(pch);

					arm_pid_init_f32(&SpeedPID, false);
					//					printf("ki = %f\n", SpeedPID.Ki);
				}
				else if(pch[1] == 'd')
				{
					pch = strtok (NULL," ");

					SpeedPID.Kd = atof(pch);

					arm_pid_init_f32(&SpeedPID, false);
					//					printf("kd = %f\n", SpeedPID.Kd);
				}
			}
			else if(pch[0] == 'p')
			{
				if(pch[1] == 's')
				{
					if(plotSpeed == false)
					{
						plotSpeed = true;
					}
					else
					{
						plotSpeed = false;
					}
				}
			}

			usart1bsp_clearBuffer();

		}

		ledsbsp_toogle(LED_RED);
		vTaskDelay(50);
	}
}

int main(void)
{
	ledsbsp_init();
	gpiobsp_init();
	uart1bsp_init();
	buttonsbsp_init();
	ledsbsp_resetAllLeds();

	speedController_Init();

	xTaskCreate(commandsTask,
			"ct",
			(configMINIMAL_STACK_SIZE*3),
			NULL,
			tskIDLE_PRIORITY,
			NULL);

	vTaskStartScheduler();

	// Infinite loop
	for(;;);

	return 0;
}




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
