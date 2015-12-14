/**
  *************************************************************************************
  * \file    motorsbsp.c 
  * \author  Felipe Adriano
  * \version V1.0
  * \date    02/05/2015
  * \brief   Small description
  *************************************************************************************
  * description
  * 
  * @attention
  *
  * 
  *
  * <h2><center>&copy; COPYRIGHT </center></h2>
  *************************************************************************************
  */



/*************************************** INCLUDES *************************************/

#include "motorsbsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

/*
#define N_PINS			5
#define MOTORS_CLK		__GPIOB_CLK_ENABLE(); __GPIOC_CLK_ENABLE(); __TIM4_CLK_ENABLE()
#define PWM_AF_TIM		GPIO_AF2_TIM4
#define PWM_TIM			TIM4
#define PWM_PERIOD		999
#define PWM_PRESCALER	3
#define L_PWM_PORT		GPIOB
#define L_PWM_PIN		GPIO_PIN_6
#define L_PWM_CHANNEL	TIM_CHANNEL_1
#define L_PWM_CCR		TIM4->CCR1
#define L_IN1_PORT		GPIOB
#define L_IN1_PIN		GPIO_PIN_9
#define L_IN2_PORT		GPIOB
#define L_IN2_PIN		GPIO_PIN_8
#define R_PWM_PORT		GPIOB
#define R_PWM_PIN		GPIO_PIN_7
#define R_PWM_CHANNEL	TIM_CHANNEL_2
#define R_PWM_CCR		TIM4->CCR2
#define R_IN1_PORT		GPIOC
#define R_IN1_PIN		GPIO_PIN_2
#define R_IN2_PORT		GPIOC
#define R_IN2_PIN		GPIO_PIN_3
#define STBY_PORT		GPIOC
#define STBY_PIN		GPIO_PIN_1
*/

#define N_PINS	5
#define MOTORS_CLK	__GPIOB_CLK_ENABLE(); __GPIOC_CLK_ENABLE(); __TIM4_CLK_ENABLE()
#define PWM_AF_TIM	GPIO_AF2_TIM4
#define PWM_TIM		TIM4
#define PWM_PERIOD		999
#define PWM_PRESCALER	3
#define L_PWM_PORT	GPIOB
#define L_PWM_PIN	GPIO_PIN_6
#define L_PWM_CHANNEL	TIM_CHANNEL_1
#define L_PWM_CCR	TIM4->CCR1
#define L_IN1_PORT	GPIOB
#define L_IN1_PIN	GPIO_PIN_9
#define L_IN2_PORT	GPIOB
#define L_IN2_PIN	GPIO_PIN_8

#define R_PWM_PORT	GPIOB
#define R_PWM_PIN	GPIO_PIN_7
#define R_PWM_CHANNEL	TIM_CHANNEL_2
#define R_PWM_CCR	TIM4->CCR2
#define R_IN2_PORT	GPIOC
#define R_IN2_PIN	GPIO_PIN_2
#define R_IN1_PORT	GPIOC
#define R_IN1_PIN	GPIO_PIN_3
#define STBY_PORT	GPIOC
#define STBY_PIN	GPIO_PIN_1

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

static GPIO_TypeDef* MOTORES_PORT[N_PINS] =
			{L_IN1_PORT, L_IN2_PORT, R_IN1_PORT, R_IN2_PORT, STBY_PORT};

static const uint16_t MOTORES_PIN[N_PINS] =
			{L_IN1_PIN, L_IN2_PIN, R_IN1_PIN, R_IN2_PIN, STBY_PIN};

TIM_HandleTypeDef TimHandle;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void motorsbsp_init(void)
{
	MOTORS_CLK;

	// Configura os GPIOs do drive dos motores como sa�das push/pull
	GPIO_InitTypeDef GPIO_InitStructure;
	for (int i = 0; i < N_PINS; i++)
	{
		GPIO_InitStructure.Pin = MOTORES_PIN[i];
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(MOTORES_PORT[i], &GPIO_InitStructure);
	}
	HAL_GPIO_WritePin(STBY_PORT, STBY_PIN, GPIO_PIN_SET);

	// Configura os GPIOs de PWM como Alternate Function
	GPIO_InitStructure.Pin = L_PWM_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Alternate = PWM_AF_TIM;
	HAL_GPIO_Init(L_PWM_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = R_PWM_PIN;
	HAL_GPIO_Init(R_PWM_PORT, &GPIO_InitStructure);

	// Time base PWM COnfig
	// PWMf:	f = [(168M / 2) / (PS + 1) ] / T

	//TIM4 connected on APB1 and prescaller is 4 (the minimal!!) -> Pag. 67 - Reference Manual
                                /*   (168M/4)    */
	// timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)
	// ttf = 10500000
	// PWM_frequency = (timer_tick_frequency / (TIM_Period + 1))
	// PWMf = 10500
	TimHandle.Instance = PWM_TIM;
	TimHandle.Init.Period = PWM_PERIOD;
	TimHandle.Init.Prescaler = PWM_PRESCALER;
	TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_PWM_Init(&TimHandle);

	// Output Compare - PWM
	// TIM_Period = timer_tick_frequency / PWM_frequency - 1
	// TIM_Period = 10500000 / 10500 = 1000
	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	sConfig.Pulse = 999;
	HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, L_PWM_CHANNEL);
	HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, R_PWM_CHANNEL);

	HAL_TIM_PWM_Start(&TimHandle, L_PWM_CHANNEL);
	HAL_TIM_PWM_Start(&TimHandle, R_PWM_CHANNEL);
}

void motorsbsp_setPWM(int16_t s16_pwmLeft, int16_t s16_pwmRight)
{
	if(s16_pwmLeft < 0)
	{
		s16_pwmLeft *= -1;

		HAL_GPIO_WritePin(L_IN1_PORT, L_IN1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(L_IN2_PORT, L_IN2_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(L_IN1_PORT, L_IN1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(L_IN2_PORT, L_IN2_PIN, GPIO_PIN_RESET);
	}

	if(s16_pwmRight < 0)
	{
		s16_pwmRight *= -1;

		HAL_GPIO_WritePin(R_IN1_PORT, R_IN1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R_IN2_PORT, R_IN2_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(R_IN1_PORT, R_IN1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R_IN2_PORT, R_IN2_PIN, GPIO_PIN_RESET);
	}

	if (s16_pwmLeft > 1000)
	{
		s16_pwmLeft = 1000;
	}

	if (s16_pwmRight > 1000)
	{
		s16_pwmRight = 1000;
	}

	L_PWM_CCR = s16_pwmLeft;
	R_PWM_CCR = s16_pwmRight;
}
/*************************************** EOF ******************************************/

