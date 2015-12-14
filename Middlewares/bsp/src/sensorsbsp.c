/**
  *************************************************************************************
  * \file    sensorsbsp.c 
  * \author  Felipe Adriano <felipeadrianosg@gmail.com>
  * \version V1.0.0
  * \date    07/05/2015
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

#include "sensorsbsp.h"
#include "delay.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define LOW		GPIO_PIN_RESET
#define HIGH	GPIO_PIN_SET
#define LINHA	GPIO_PIN_RESET	// RESET (linha branca); SET (linha preta)

#define SENSORES_CLK	__GPIOA_CLK_ENABLE(); __GPIOB_CLK_ENABLE(); __GPIOC_CLK_ENABLE(); __GPIOD_CLK_ENABLE()

#define N_EMISSORES	6
#define LF_E_PORT	GPIOD
#define LF_E_PIN	GPIO_PIN_2
#define POWER1_PORT	GPIOC
#define POWER1_PIN	GPIO_PIN_12
#define POWER2_PORT	GPIOC
#define POWER2_PIN	GPIO_PIN_11
#define RF_E_PORT	GPIOC
#define RF_E_PIN	GPIO_PIN_10
#define L_LINE_PORT	GPIOA
#define L_LINE_PIN	GPIO_PIN_14
#define R_LINE_PORT	GPIOA
#define R_LINE_PIN	GPIO_PIN_13

#define N_RECEPTORES	8
#define LINE1_PORT	GPIOC
#define LINE1_PIN	GPIO_PIN_9
#define LINE2_PORT	GPIOC
#define LINE2_PIN	GPIO_PIN_8
#define LINE3_PORT	GPIOC
#define LINE3_PIN	GPIO_PIN_7
#define LINE4_PORT	GPIOC
#define LINE4_PIN	GPIO_PIN_6
#define LINE5_PORT	GPIOB
#define LINE5_PIN	GPIO_PIN_15
#define LINE6_PORT	GPIOB
#define LINE6_PIN	GPIO_PIN_14
#define LINE7_PORT	GPIOB
#define LINE7_PIN	GPIO_PIN_13
#define LINE8_PORT	GPIOB
#define LINE8_PIN	GPIO_PIN_12

#define N_ANALOGICAS	7
#define LF_R_PORT	GPIOC
#define LF_R_PIN	GPIO_PIN_5
#define LF_R_CH		ADC_CHANNEL_15
#define L_R_PORT	GPIOC
#define L_R_PIN		GPIO_PIN_4
#define L_R_CH		ADC_CHANNEL_14
#define R_R_PORT	GPIOA
#define R_R_PIN		GPIO_PIN_7
#define R_R_CH		ADC_CHANNEL_7
#define RF_R_PORT	GPIOA
#define RF_R_PIN	GPIO_PIN_6
#define RF_R_CH		ADC_CHANNEL_6
#define G_OUTZ_PORT	GPIOB
#define G_OUTZ_PIN	GPIO_PIN_1
#define G_OUTZ_CH	ADC_CHANNEL_9
#define G_VREF_PORT	GPIOB
#define G_VREF_PIN	GPIO_PIN_0
#define G_VREF_CH	ADC_CHANNEL_8
#define VBAT_PORT	GPIOA
#define VBAT_PIN	GPIO_PIN_2
#define VBAT_CH		ADC_CHANNEL_2
#define VBAT_ALERTA 7000

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/**
 * @brief
 *
 * @param canal
 * @return
 */
uint32_t getRawADC(uint32_t canal);

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

GPIO_TypeDef* EMISSORES_PORT[N_EMISSORES] =
			{LF_E_PORT, POWER1_PORT, POWER2_PORT, RF_E_PORT,
			L_LINE_PORT, R_LINE_PORT};
const uint16_t EMISSORES_PIN[N_EMISSORES] =
			{LF_E_PIN, POWER1_PIN, POWER2_PIN, RF_E_PIN,
			L_LINE_PIN, R_LINE_PIN};

GPIO_TypeDef* RECEPTORES_PORT[N_RECEPTORES] =
			{LINE1_PORT, LINE2_PORT, LINE3_PORT, LINE4_PORT,
			LINE5_PORT, LINE6_PORT, LINE7_PORT, LINE8_PORT};
const uint16_t RECEPTORES_PIN[N_RECEPTORES] =
			{LINE1_PIN, LINE2_PIN, LINE3_PIN, LINE4_PIN,
			LINE5_PIN, LINE6_PIN, LINE7_PIN, LINE8_PIN};

GPIO_TypeDef* ANALOGICAS_PORT[N_ANALOGICAS] =
			{LF_R_PORT, L_R_PORT, R_R_PORT, RF_R_PORT,
			G_OUTZ_PORT, G_VREF_PORT, VBAT_PORT};
const uint16_t ANALOGICAS_PIN[N_ANALOGICAS] =
			{LF_R_PIN, L_R_PIN, R_R_PIN, RF_R_PIN,
			G_OUTZ_PIN, G_VREF_PIN, VBAT_PIN};

static ADC_HandleTypeDef hadc1;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void sensorsbsp_init(void)
{
	SENSORES_CLK;	// Habilita o barramento de clock do GPIO dos Sensores
	GPIO_InitTypeDef GPIO_InitStructure;


	// Configura os GPIOs dos emissores como saída push/pull
	for (int i = 0; i < N_EMISSORES; i++)
	{
		GPIO_InitStructure.Pin = EMISSORES_PIN[i];;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
		HAL_GPIO_Init(EMISSORES_PORT[i], &GPIO_InitStructure);
		HAL_GPIO_WritePin(EMISSORES_PORT[i], EMISSORES_PIN[i], LOW);
	}


	// Configura os GPIOs dos receptores como entrada sem resistor interno
	for (int i = 0; i < N_RECEPTORES; i++)
	{
		GPIO_InitStructure.Pin = RECEPTORES_PIN[i];
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(RECEPTORES_PORT[i], &GPIO_InitStructure);
	}


	// Configura os pinos analógicos
	for (int i = 0; i < N_ANALOGICAS; i++)
	{
		GPIO_InitStructure.Pin = ANALOGICAS_PIN[i];
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ANALOGICAS_PORT[i], &GPIO_InitStructure);
	}

	// Configuração do ADC
	__ADC1_CLK_ENABLE();
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION12b;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc1);

	HAL_GPIO_WritePin(LF_E_PORT, LF_E_PIN, LOW);
	HAL_GPIO_WritePin(RF_E_PORT, RF_E_PIN, LOW);
	HAL_GPIO_WritePin(POWER1_PORT, POWER1_PIN, LOW);
}

/**
 * @brief Realiza a leitura dos sensores de parede
 * 		(atualiza os sensores frontais e laterais)
 * @param lf Valor proporcional a distância do sensor frontal esquerdo
 * @param l Valor proporcional a distância do sensor diagonal esquerdo
 * @param r Valor proporcional a distância do sensor diagonal direito
 * @param rf Valor proporcional a distância do sensor frontal direito
 * @return paredes: máscara de bits indicando presença (1) ou não (0)
 * 	de paredes. O bit mais significativo representa a parede da esquerda.
 * 	Ex.: 011 = presença de parede frontal e direita.
 *
 */
int32_t sensorsbsp_getWallSensors(int32_t* lf, int32_t* l, int32_t* r, int32_t* rf)
{
	int32_t paredes = 0;



	(*lf) = getRawADC(LF_R_CH);
	(*l) = getRawADC(L_R_CH);
	(*r) = getRawADC(R_R_CH);
	(*rf) = getRawADC(RF_R_CH);

	// Registra o tempo atual
	uint32_t t0 = micros();

	// Sensor frontal esquerdo
	HAL_GPIO_WritePin(LF_E_PORT, LF_E_PIN, HIGH);
	elapse_us(60, t0);
	(*lf) = getRawADC(LF_R_CH) - (*lf);
	HAL_GPIO_WritePin(LF_E_PORT, LF_E_PIN, LOW);
	if ((*lf) < 0)
	{
		(*lf) = 0;
	}
	elapse_us(140, t0);

	// Sensor frontal direito
	HAL_GPIO_WritePin(RF_E_PORT, RF_E_PIN, HIGH);
	elapse_us(200, t0);
	(*rf) = getRawADC(RF_R_CH) - (*rf);
	HAL_GPIO_WritePin(RF_E_PORT, RF_E_PIN, LOW);
	if ((*rf) < 0)
	{
		(*rf) = 0;
	}
	elapse_us(280, t0);

	// Sensores laterais
	HAL_GPIO_WritePin(POWER1_PORT, POWER1_PIN, HIGH);
	elapse_us(340, t0);
	(*l) = getRawADC(L_R_CH) - (*l);
	(*r) = getRawADC(R_R_CH) - (*r);
	HAL_GPIO_WritePin(POWER1_PORT, POWER1_PIN, LOW);
	if ((*l) < 0)
	{
		(*l) = 0;
	}
	if ((*r) < 0)
	{
		(*r) = 0;
	}


	// Realiza a máscara de bits
	if ((*lf) > THRESHOLD || (*rf) > THRESHOLD)
	{
		paredes |= PAREDE_FRONTAL;
	}

	if ((*l) > THRESHOLD)
	{
		paredes |= PAREDE_ESQUERDA;
	}

	if ((*r) > THRESHOLD)
	{
		paredes |= PAREDE_DIREITA;
	}

	return paredes;
}

/**
 * @brief Verifica os sensores de linha
 * @param Nenhum
 * @return erro: valores entre -40 e 40 (valores negativos indicam que
 *		o robô precisa se deslocar para a esquerda)
 */
int32_t sensorsbsp_getLineSensors(void)
{
	int32_t erro = 0, soma = 0, n = 0;
	uint32_t t0 = micros();

	// Habilita os emissores
	HAL_GPIO_WritePin(L_LINE_PORT, L_LINE_PIN, HIGH);
	HAL_GPIO_WritePin(R_LINE_PORT, R_LINE_PIN, HIGH);
	elapse_us(100, t0);

	// Realiza a leitura de todos os sensores de linha, os sensores das
	// extremidades pussuem peso maior, no final é realizada a média ponderada
	if (HAL_GPIO_ReadPin(LINE1_PORT, LINE1_PIN) == LINHA)
	{
		soma += -40;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE2_PORT, LINE2_PIN) == LINHA)
	{
		soma += -30;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE3_PORT, LINE3_PIN) == LINHA)
	{
		soma += -20;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE4_PORT, LINE4_PIN) == LINHA)
	{
		soma += -10;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE5_PORT, LINE5_PIN) == LINHA)
	{
		soma += 10;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE6_PORT, LINE6_PIN) == LINHA)
	{
		soma += 20;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE7_PORT, LINE7_PIN) == LINHA)
	{
		soma += 30;
		n++;
	}
	if (HAL_GPIO_ReadPin(LINE8_PORT, LINE8_PIN) == LINHA)
	{
		soma += 40;
		n++;
	}

	// Desabilita os emissores
	HAL_GPIO_WritePin(L_LINE_PORT, L_LINE_PIN, LOW);
	HAL_GPIO_WritePin(R_LINE_PORT, R_LINE_PIN, LOW);


	// Retorna a média ou retorna a constante INFINITO indicando
	// que nenhum sensor leu linha
	if (n != 0)
	{
		erro = soma / n;
	}
	else
	{
		erro = INFINITO;
	}

	return erro;
}

int32_t sensorsbsp_getGyro(void)
{
	int32_t w = 0;

//	w = getRawADC(G_VREF_CH);
	w = (getRawADC(G_OUTZ_CH)) - 1820;//(getRawADC(G_VREF_CH));


	return w;
}

int32_t sensorsbsp_getGyroVoltage(void)
{
	int32_t w = 0;

//	w = getRawADC(G_VREF_CH);
	w = (getRawADC(G_OUTZ_CH));



	return w;
}

int32_t sensorsbsp_getGyroVref(void)
{
	int32_t w = 0;

//	w = getRawADC(G_VREF_CH);
	w = (getRawADC(G_VREF_CH));



	return w;
}

/**
  * @brief Realiza a conversão de um canal analógico
  * @param canal: Canal analógico a ser realizado a conversão
  * @return rawADC: Resultado da conversão (valor de 12 bits)
  */
uint32_t getRawADC(uint32_t canal)
{
	uint32_t rawADC;
	ADC_ChannelConfTypeDef sConfig;

	sConfig.Channel = canal;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	rawADC = HAL_ADC_GetValue(&hadc1);

	return rawADC;
}

/*************************************** EOF ******************************************/

