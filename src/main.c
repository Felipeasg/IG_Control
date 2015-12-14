//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "encodersbsp.h"
#include "buttonsbsp.h"
#include "ledsbsp.h"
#include "uart1bsp.h"
#include "gpiobsp.h"
#include "dacbsp.h"


#define ENABLE_CONVERSORCC  gpiobsp_output(GPIOC_11, HIGH);
#define DISABLE_CONVERSORCC gpiobsp_output(GPIOC_11, LOW);


void vApplicationTickHook( void )
{
	HAL_IncTick();
}

static void mainTask(void* pvParameters)
{
	(void)pvParameters;

	//ledsbsp_init();

	int32_t encoder1 = 0, encoder1a = 0;
	int32_t encoder2 = 0, encoder2a = 0;
	uint16_t vel = 0;

	while(!buttonsbsp_getSW1State())
	{
		ledsbsp_toogleOutputLed(LED_GREEN);
		vTaskDelay(100);
	}

	ENABLE_CONVERSORCC;
	ledsbsp_outputLed(LED_GREEN, HIGH);

	for(;;)
	{
		ledsbsp_toogleOutputLed(LED_BLUE);

		encoder1 = encoderbsp_getLEncoder();
		encoder2 = encoderbsp_getREncoder();

		printf("E1 = %ld \r\n", (encoder1 - encoder1a));
		printf("E2 = %ld \r\n", (encoder2 - encoder2a));

		encoder1a = encoder1;
		encoder2a = encoder2;

		if(buttonsbsp_getSW1State())
		{
			vel += 100;

			if(vel >= 4096)
			{
				vel = 0;
			}

			dacbsp_setValue(vel);

			while(buttonsbsp_getSW1State());
		}
		vTaskDelay(50);
	}
}

int main(void)
{
	ledsbsp_init();
	gpiobsp_init();
	uart1bsp_init();
	encodersbsp_init();
	buttonsbsp_init();
	dacbsp_init();

	DISABLE_CONVERSORCC;

	xTaskCreate(mainTask, "mt", (configMINIMAL_STACK_SIZE*3), NULL, tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();

	// Infinite loop
	for(;;);

	return 0;
}




#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
