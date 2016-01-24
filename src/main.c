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

#include "maincontroller.h"
#include "speedcontroller.h"

#include "buttonsbsp.h"
#include "ledsbsp.h"
#include "uart1bsp.h"
#include "gpiobsp.h"

#include "arm_math.h"
#include "PID.h"


#define				SET_VELOCITY				0
#define				ENABLE_VOLTAGE_CONTROL		1
#define				ENABLE_AND_GO_PLL			2
#define				DISABLE_AND_STOP_PLL		3
#define				DISABLE_VOLTAGE_CONTROL		4
#define				STOP_MOTOR					5
#define 			DISABLE_DC_DC				6
#define 			INITIAL						7

extern volatile float currentSpeed;
extern __IO float targetVoltage;

extern volatile float encoderChangeFiltred;
extern volatile int32_t encoderChange;

extern volatile float pidError;
//extern volatile float erro_a=0;
extern volatile float pidOutput;

extern arm_pid_instance_f32 SpeedPID;
extern arm_pid_instance_f32 voltagePID;
extern volatile bool plotSpeed;
extern volatile bool plotVoltage;

extern volatile bool plotEnabled;
extern volatile uint8_t plotType;

extern volatile bool enableVoltageControl;

extern volatile bool pllEnable;

extern volatile bool enableRamp;

__IO bool pllGo;

extern __IO float n_rpm;

//extern PID_t SpeedPID;

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim3;

TIM_HandleTypeDef htim4;

//__IO uint32_t ADCBuffer[9];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);

void vApplicationTickHook( void )
{
	HAL_IncTick();
}

static void commandsTask(void* pvParameters)
{
	(void)pvParameters;

	char command[200] = {0};
	uint16_t receivedBytes = 0;

	uint16_t state = 0;

	state = INITIAL;
	enableRamp  = true;

	for(;;)
	{
#if 0
		if(buttonsbsp_getSW1State() == GPIO_PIN_SET)
		{

			switch(state)
			{
			case INITIAL:

				ledsbsp_output(LED_GREEN, GPIO_PIN_SET);

				ENABLE_CONVERSORCC;

				state = SET_VELOCITY;
				break;

			case SET_VELOCITY:

				ledsbsp_output(LED_BLUE, GPIO_PIN_SET);

				enableRamp = true;
				SpeedController_SetSpeed(1827.5, 5);

				state = ENABLE_AND_GO_PLL;
				break;

			case ENABLE_AND_GO_PLL:

				ledsbsp_output(LED_ORANGE, GPIO_PIN_SET);

				pllEnable = true;
				pllGo = true;

				state = ENABLE_VOLTAGE_CONTROL;
				break;

			case ENABLE_VOLTAGE_CONTROL:

				ledsbsp_output(LED_RED, GPIO_PIN_SET);

				enableVoltageControl = true;

				state = DISABLE_AND_STOP_PLL;
				break;

			case DISABLE_AND_STOP_PLL:

				ledsbsp_output(LED_ORANGE, GPIO_PIN_RESET);

				pllEnable = false;
				pllGo = false;

				state = DISABLE_VOLTAGE_CONTROL;
				break;

			case DISABLE_VOLTAGE_CONTROL:

				ledsbsp_output(LED_RED, GPIO_PIN_RESET);

				enableVoltageControl = false;

				state = STOP_MOTOR;
				break;

			case STOP_MOTOR:

				ledsbsp_output(LED_BLUE, GPIO_PIN_RESET);

				SpeedController_SetSpeed(0.0, 5);

				state = DISABLE_DC_DC;
				break;

			case DISABLE_DC_DC:

				ledsbsp_output(LED_GREEN, GPIO_PIN_RESET);

				DISABLE_DC_DC;

				state = INITIAL;

				break;

			default:
				state = INITIAL;
				break;
			}

			while(buttonsbsp_getSW1State() == GPIO_PIN_SET);
		}

		vTaskDelay(100);
#endif

#if 1
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

			//currentSpeed = RPM_TO_COUNTS_PER_MS(atof(pch));
			//					SpeedController_SetSpeed(atof(pch), 12);
			SpeedController_SetSpeed(atof(pch), 5);

			//					printf("Target Speed = %f\n", COUNTS_PER_MS_TO_RPM(targetSpeed));
		}
		if(pch[1] == 'p')
		{
			pch = strtok (NULL," ");

			//currentSpeed = RPM_TO_COUNTS_PER_MS(atof(pch));
			n_rpm = RPM_TO_COUNTS_PER_MS(atof(pch));

			//					printf("Target Speed = %f\n", COUNTS_PER_MS_TO_RPM(targetSpeed));
		}
		if(pch[1] == 'v')
		{
			pch = strtok (NULL," ");

			targetVoltage = (atof(pch));

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
			printf("Target Speed = %f RPM\r\n", COUNTS_PER_MS_TO_RPM(currentSpeed));
			printf("---------------------------\n");
		}
	}
	else if(pch[0] == 'k')
	{
		if(pch[1] == 'p')
		{
			pch = strtok (NULL," ");

			SpeedPID.Kp = atof(pch);

			arm_pid_init_f32(&SpeedPID, true);
			//					printf("kp = %f\n", SpeedPID.Kp);
		}
		else if(pch[1] == 'i')
		{
			pch = strtok (NULL," ");

			SpeedPID.Ki = atof(pch);

			arm_pid_init_f32(&SpeedPID, true);
			//					printf("ki = %f\n", SpeedPID.Ki);
		}
		else if(pch[1] == 'd')
		{
			pch = strtok (NULL," ");

			SpeedPID.Kd = atof(pch);

			arm_pid_init_f32(&SpeedPID, true);
			//					printf("kd = %f\n", SpeedPID.Kd);
		}
	}
	else if(pch[0] == 'p')
	{
#if 1
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
		if(pch[1] == 'v')
		{
			if(plotVoltage == false)
			{
				plotVoltage = true;
			}
			else
			{
				plotVoltage = false;
			}
		}
		//PLL GO
		if(pch[1] == 'g')
		{
			if(pllGo == false)
			{
				pllGo = true;
			}
			else
			{
				pllGo = false;
			}
		}
#endif
	}
	else if(pch[0] == 'e')
	{
		if(pch[1] == 'v')
		{
			if(enableVoltageControl == false)
			{
				enableVoltageControl = true;
			}
			else
			{
				enableVoltageControl = false;
			}
		}
		if(pch[1] == 'p')
		{
			if(pllEnable == false)
			{
				pllEnable = true;
			}
			else
			{
				pllEnable = false;
			}
		}
		if(pch[1] == 'r')  //Habilita ou desabilita rampa
		{
			if(enableRamp == false)
			{
				enableRamp = true;
			}
			else
			{
				enableRamp = false;
			}
		}
	}
	else if(pch[0] == 'v')
	{
		if(pch[1] == 'p')
		{
			pch = strtok (NULL," ");

			voltagePID.Kp = atof(pch);

			arm_pid_init_f32(&voltagePID, true);
			//					printf("kp = %f\n", SpeedPID.Kp);
		}
		else if(pch[1] == 'i')
		{
			pch = strtok (NULL," ");

			voltagePID.Ki = atof(pch);

			arm_pid_init_f32(&voltagePID, true);
			//					printf("ki = %f\n", SpeedPID.Ki);
		}
		else if(pch[1] == 'd')
		{
			pch = strtok (NULL," ");

			voltagePID.Kd = atof(pch);

			arm_pid_init_f32(&voltagePID, true);
			//					printf("kd = %f\n", SpeedPID.Kd);
		}
	}
	else if(pch[0] == 'r')
	{
		if(pch[1] == 'p')
		{
			Meassurements_ResetPLL();
		}
	}


	usart1bsp_clearBuffer();

}

ledsbsp_toogle(LED_RED);
vTaskDelay(100);
#endif
	}
}

int main(void)
{
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();

	ledsbsp_init();
	gpiobsp_init();
	uart1bsp_init();
	buttonsbsp_init();
	ledsbsp_resetAllLeds();

	MainController_Init();

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

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
			|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{

	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV6;
	hadc1.Init.Resolution = ADC_RESOLUTION12b;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 3;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = EOC_SEQ_CONV;
	HAL_ADC_Init(&hadc1);

	/**Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_TRIPLEMODE_REGSIMULT;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_1;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.Rank = 3;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* ADC2 init function */
void MX_ADC2_Init(void)
{

	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV6;
	hadc2.Init.Resolution = ADC_RESOLUTION12b;
	hadc2.Init.ScanConvMode = ENABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 3;
	hadc2.Init.DMAContinuousRequests = ENABLE;
	hadc2.Init.EOCSelection = EOC_SEQ_CONV;
	HAL_ADC_Init(&hadc2);

	/**Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_TRIPLEMODE_REGSIMULT;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_1;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	HAL_ADCEx_MultiModeConfigChannel(&hadc2, &multimode);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = 3;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

}

/* ADC3 init function */
void MX_ADC3_Init(void)
{

	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV6;
	hadc3.Init.Resolution = ADC_RESOLUTION12b;
	hadc3.Init.ScanConvMode = ENABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 3;
	hadc3.Init.DMAContinuousRequests = ENABLE;
	hadc3.Init.EOCSelection = EOC_SEQ_CONV;
	HAL_ADC_Init(&hadc3);

	/**Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_TRIPLEMODE_REGSIMULT;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_1;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = 2;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = 3;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

}

/* TIM3 init function */
void MX_TIM3_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 42 - 1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
#if 1
	htim3.Init.Period = 125; //16 khz
#else
	htim3.Init.Period = 2000 -1; //1 khz
#endif
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //84Mhz
	HAL_TIM_Base_Init(&htim3);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

}

/**
 * Enable DMA controller clock
 */
void MX_DMA_Init(void)
{
	/* DMA controller clock enable */
	__DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 2);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/* TIM4 init function */
void MX_TIM4_Init(void)
{

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 84-1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 1000 - 1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim4);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&htim4);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 999;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

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
	__GPIOC_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	/* GPIO Ports Clock Enable */
	__GPIOH_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
}



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
