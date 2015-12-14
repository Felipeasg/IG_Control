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

#include "stm32f4xx_hal.h"

#define ENABLE_CONVERSORCC  ledsbsp_outputLed(PIN_PC11, HIGH);
#define DISABLE_CONVERSORCC ledsbsp_outputLed(PIN_PC11, LOW);

DAC_HandleTypeDef hdac;

static void MX_GPIO_Init(void);
static void MX_DAC_Init(void);

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

			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, vel);

			while(buttonsbsp_getSW1State());
		}
		vTaskDelay(50);
	}
}

int main(void)
{
	ledsbsp_init();
	uart1bsp_init();
	encodersbsp_init();
	buttonsbsp_init();

	DISABLE_CONVERSORCC;

	MX_GPIO_Init();
	MX_DAC_Init();
	HAL_DAC_MspInit(&hdac);

	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);


	xTaskCreate(mainTask, "mt", (configMINIMAL_STACK_SIZE*3), NULL, tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();

	// Infinite loop
	for(;;);

	return 0;
}


/* DAC init function */
void MX_DAC_Init(void)
{

  DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization
    */
  hdac.Instance = DAC;
  HAL_DAC_Init(&hdac);

    /**DAC channel OUT1 config
    */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hdac->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspInit 0 */

  /* USER CODE END DAC_MspInit 0 */
    /* Peripheral clock enable */
    __DAC_CLK_ENABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }

}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{

  if(hdac->Instance==DAC)
  {
  /* USER CODE BEGIN DAC_MspDeInit 0 */

  /* USER CODE END DAC_MspDeInit 0 */
    /* Peripheral clock disable */
    __DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

  }
  /* USER CODE BEGIN DAC_MspDeInit 1 */

  /* USER CODE END DAC_MspDeInit 1 */

}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
