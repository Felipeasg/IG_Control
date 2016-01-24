/**
 *************************************************************************************
 * \file    meassurements.c
 * \author  felip
 * \version Vx.y.z
 * \date    Jan 14, 2016
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

#include <stdint.h>
#include <math.h>

#include "stm32f4xx_hal.h"

#include "meassurements.h"
#include "speedcontroller.h"

#include "adcbsp.h"
#include "PLL_1P.h"

#include "ledsbsp.h"

#include "uart1bsp.h"

#include <string.h>
/******************************* DEFINITIONS AND MACROS *******************************/

#define ADC_BITS		14
#define ADC_COUNTS_2  	(ADC_COUNTS*ADC_COUNTS)

#define STABILITATION_TIME_MS	100

#define MY_DQ0
//#define MY_NOTCH

#define PLL_MAX RPM_TO_COUNTS_PER_MS(100)
#define PLL_MIN RPM_TO_COUNTS_PER_MS(-100)

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/


/************************************** PROTOTYPES ************************************/

void readSamples(__IO uint32_t *samples);

/********************************** GLOBAL VARIABLES **********************************/

volatile bool pllEnable = false;
__IO bool pllGo = false;

extern volatile bool plotVoltage;

extern volatile bool plotEnabled;
extern volatile uint8_t plotType;

extern __IO float targetVoltage;
extern volatile float currentSpeed;
extern volatile float targetSpeed;

/********************************** LOCAL  VARIABLES **********************************/

__IO float supplyVoltage = 3300.0F;
__IO float vCalM = 696.11F;
__IO float vCalB = -0.4435;

__IO float nMean = 100.0F;
__IO float nRMS  = 140.0F;

__IO float alphaMean = 0.0F;
__IO float betaMean = 0.0F;

__IO float alphaRMS = 0.0F;
__IO float betaRMS = 0.0F;

__IO float offsetV = 0.0F;
__IO float filteredV = 0.0F;

__IO float vRatio = 0.0F;

__IO uint32_t sampleV = 0.0F;

__IO float Vrms = 0.0F;

__IO float rmsIn = 0.0F;
__IO float rmsOut[2] = {0.0F};

__IO float meanIn = 0;
__IO float meanOut[2] = {0};

__IO float offsetVGenerator = 0.0F;
__IO float sampleVGenerator = 0.0F;
__IO float filteredVGenerator = 0.0F;

__IO float offsetVGrid = 0.0F;
__IO float sampleVGGrid = 0.0F;
__IO float filteredVGrid = 0.0F;

//Usart send data
uint32_t idx2 = 0;
char header2[4] = {0xaa,0xaa,0xaa,0xaa};
float samples2[200];
uint8_t data2[804];

//Motor com carga se aproxima mais da rede com este valor de RPM
__IO float n_rpm = RPM_TO_COUNTS_PER_MS(1828.0);

//pll
PLL_1P    pll1  = PLL_1P_DEFAULTS;
OSG			osg0 = OSG_F0_60Hz_Fs_1k;
OSG			osg1 = OSG_F0_60Hz_Fs_1k;
OSG			osg2 = OSG_F0_60Hz_Fs_1k;
FILT_2P2Z  notch = NOTCH_2P2Z_F0_0_1k_FS_1k;

float     PI_DUTY = (0);
float		PLL_Kp	= (0.9);
float		PLL_Ki = (0.05);
float 	PLL_setFreq	= (60);

float u_Q[2];
float u_D[2];
float u_Qant = 0;

float ylf[2];
float xlf[2];

float Kp = 0.0;
float Ki = 0.0;

float PI_B0 = 0.0;
float PI_B1 = 0.0;

float pllOut = 0.0;

bool pllReadyToGo = false;

/******************************* FUNCTION  IMPLEMENTATION *****************************/

void Meassurements_Init(void)
{
	adcbsp_registerCallBack(readSamples);
	adcbsp_init();

	offsetV = ADC_COUNTS/2 - 700;
	offsetVGrid = ADC_COUNTS/2 - 700;
	offsetVGenerator = ADC_COUNTS/2 - 700;

	alphaMean = (nMean - 1)/nMean;
	betaMean = 1/nMean;

	alphaRMS = (nRMS - 1)/nRMS;
	betaRMS = 1/nRMS;

	vRatio = ((vCalM * ((supplyVoltage/1000.0F) / (ADC_COUNTS))));

	//Some pll parameters
	pll1.setFreq  =  PLL_setFreq;
	pll1.Kp 	  =  PLL_Kp;
	pll1.Ki_ts 	  =  PLL_Ki;
	pll1.Notch_EN =  true;

	//    Kp    =  166.6/6;
	//    Ki    =  27755.55/6;

	Kp    =  166.6;
	Ki    =  2777.55;

	PI_B0 =  (2*Kp + Ki*Ts)/2;
	PI_B1 = -(2*Kp - Ki*Ts)/2;
}

float Meassurements_GetRMS(void)
{
	return Vrms;
}

void Meassurements_ResetPLL(void)
{
	u_Q[1] = u_Q[0] = 0;
	u_D[1] = u_D[0] = 0;

	ylf[1] = ylf[0] = 0;

	osg1.In = 0;

	osg1.out = 0;
	osg1.qout = 0;

	osg1.yn_1 = 0;
	osg1.yn_2 = 0;
	osg1.yn_3 = 0;

	osg1.xn_1 = 0;
	osg1.xn_2 = 0;
	osg1.xn_3 = 0;

	osg2.In = 0;

	osg2.out = 0;
	osg2.qout = 0;

	osg2.yn_1 = 0;
	osg2.yn_2 = 0;
	osg2.yn_3 = 0;

	osg2.xn_1 = 0;
	osg2.xn_2 = 0;
	osg2.xn_3 = 0;

	pllOut = 0;

	u_Qant = 0;

	//Seto pela interface, para manter o sincronismo com a interface eu não vou resetar quando
	//Mandar o comando de resetar o pll
//	pllGo = false;
//	pllEnable = false;

	pllReadyToGo = false;
}

void readSamples(__IO uint32_t *samples)
{
	static uint32_t count = 0;
	// Aquisiton
	sampleV = samples[0];     		//PB0
	sampleVGenerator = samples[1];  //PB1
	sampleVGGrid = samples[2];      //PA2


	// Remove offset
	offsetV = offsetV + ((((float)sampleV)-offsetV)/(ADC_COUNTS));
	filteredV = sampleV - offsetV;

	offsetVGenerator = offsetVGenerator + ((((float)sampleVGenerator)-offsetVGenerator)/ADC_COUNTS);
	filteredVGenerator = sampleVGenerator - offsetVGenerator;

	offsetVGrid = offsetVGrid + ((((float)sampleVGGrid)-offsetVGrid)/ADC_COUNTS);
	filteredVGrid = sampleVGGrid - offsetVGrid;

	if(count++ >= 2000)
	{
		count = 2000;

		//Recursive RMS
		rmsOut[0] = sqrtf(((alphaRMS*rmsOut[1]*rmsOut[1]) + (betaRMS*filteredV*filteredV)));
		rmsOut[1] = rmsOut[0];

		//Recursive average of RMS
		meanOut[0] = alphaMean*meanOut[1] + betaMean*rmsOut[0];
		meanOut[1] = meanOut[0];

		Vrms = ((vRatio * meanOut[0]) + vCalB);


#ifdef MY_DQ0

		if(pllEnable == true)
		{
			osg1.In = filteredVGenerator/ADC_COUNTS;
			osg2.In = filteredVGrid/ADC_COUNTS;

			osg1.compute(&osg1);
			osg2.compute(&osg2);

			//DQ0 Normal, no aplication note da texas ele coloca -1*uQ se fizer uma analizse
			// Ver tambem simulacao TwoOrthogonalSignalGenerator
			u_D[0] = osg2.out*osg1.out + osg2.qout*osg1.qout;
			u_Q[0] = -osg2.qout*osg1.out + osg2.out*osg1.qout;

			u_Qant = u_Q[0];
			if(pllGo == true)
			{
				//Detecta se a senoide do eixo uQ passou do semiciclo positivo para o negativo
				//para começar a fazer o controle do PLL, pois inicialmente é suposto que o gerador
				//tem uma velocidade ligeiramente menor do que a da rede, então se o erro de fase
				//e negativo ele vai tentar desacelerar para fazer o sincronismo, isto pode piorar
				//as coisas pois a frequencia do gerador em relaçao da rede já é menor, então é
				//preferivel que ele comece num erro de fase positivo para acelerar e ficar maior
				//do que a rede
				if((u_Q[0] >= 0.001) ||
						 pllReadyToGo == true)
				{
					pllReadyToGo = true;
					//PI loop filter
					//			ylf[0] = ylf[1] + 166.877556*u_Q[0] - 166.322444*u_Q[1];
					ylf[0]    = 1.0*ylf[1] + PI_B0*u_Q[0] + PI_B1*u_Q[1];

					u_Q[1]=u_Q[0];
					u_D[1]=u_D[0];

					//Ganho na saida do PI para atuar mais fortemente no controle
					pllOut = RPM_TO_COUNTS_PER_MS(ylf[0]*1.88);

					//Saturação da saida, O maximo de RPM que o PLL pode mandar o motor acelerar ou desacelerar,
					//pois o PI devido ao integrativo pode ficar muito alto
					if(pllOut >= PLL_MAX)
					{
						pllOut = PLL_MAX;
					}
					else if(ylf[0] <= PLL_MIN)
					{
						pllOut = PLL_MIN;
					}

					ylf[1]=ylf[0];


					//Transformada DQ0 igual a da simulaçao da texas testeDqappnoteTexas.m
					//Teste com tensao Vab e Vbc > 0

					//Seta o current e o target speed devido a rampa de tensão. Se for só setado o target speed o controle vai fazer uma rampa
					//E eu quero que ele seja rápido, pois o pll que está definindo seu setpoint
					/*
					 * Se eu faço:
					 *
					 * currentSpeed = n_rpm + pllOut;  --> Sincroniza em 180 graus com a rede
					 *
					 * currentSpeed = n_rpm - pllOut;  --> Sincroniza em fase com a rede
					 * */
					currentSpeed = n_rpm - pllOut;
					targetSpeed = currentSpeed;
				}
			}

#endif

#ifdef MY_NOTCH
			//	Orthogonal filter to generator sin to generate cos to PLL
			osg0.In = filteredVGenerator;
			osg0.compute(&osg0);

			//PLL
			//pll1.Mycos_1 = pll1.osg1.qout;
			//pll1.In = filteredVGrid;

			//Notch Filter
			notch.In = (filteredVGrid * osg0.qout)/ADC_COUNTS_2;

			//PI loop filter
			xlf[0] = notch.Out;

			//		    ylf[0] = ylf[1] + 166.877556*u_Q[0] - 166.322444*u_Q[1];
			ylf[0]    = 1.0*ylf[1] + PI_B0*xlf[0] + PI_B1*xlf[1];

			ylf[1]=ylf[0];
			xlf[1]=xlf[0];

			if(pllEnable)
			{
				currentSpeed = n_rpm + ylf[1];
			}
#endif

#if NORTCH
			//	Orthogonal filter to generator sin to generate cos to PLL
			pll1.osg1.In = filteredVGenerator/ADC_COUNTS;
			pll1.osg1.compute(&pll1.osg1);

			//PLL
			pll1.Mycos_1 = pll1.osg1.qout/ADC_COUNTS;
			pll1.In = filteredVGrid;

			//
			pll1.compute(&pll1);

			if(pllEnable)
			{
				currentSpeed = n_rpm + ylf[1];
			}
#endif
		}
	}

#if 1
	if(plotVoltage == true)
	{
#if 0
		samples2[idx2] = targetVoltage;
		samples2[idx2+1] = Vrms;
#endif

#if 0
		samples2[idx2] = 	sampleV;      //Vermelho
		samples2[idx2+1] = 0;  //Azul
#endif

#if 0
					samples2[idx2] = 	sampleVGenerator;      //Vermelho
					samples2[idx2+1] = sampleVGGrid;  //Azul
#endif

#if 0
					samples2[idx2] = 	filteredVGenerator;      //Vermelho
					samples2[idx2+1] = filteredVGrid;  //Azul
#endif

#if 0
		samples2[idx2] = 	pll1.In;      //Vermelho
		samples2[idx2+1] = pll1.Mycos_1;  //Azul
#endif

#if 0
		samples2[idx2] = 	0;      //Vermelho
		samples2[idx2+1] = pll1.Notch1.Out;  //Azul
#endif

#if 0
		samples2[idx2] = 	0;      //Vermelho
		samples2[idx2+1] = pll1.pi1.Out;  //Azul
#endif

#if 0
		samples2[idx2] = 	0;      //Vermelho
		samples2[idx2+1] =  RPM_TO_COUNTS_PER_MS(ylf[0]);  //Azul
#endif

#if 0
		samples2[idx2] = 	0;      //Vermelho
		samples2[idx2+1] =  pllOut;  //Azul
#endif

#if 0
		samples2[idx2] = 	0;      //Vermelho
		samples2[idx2+1] =  u_Q[0];  //Azul
#endif

#if 1
		samples2[idx2] = 	pllOut;      //Vermelho
		samples2[idx2+1] =  u_Q[0];  //Azul
#endif

#if 0
		samples2[idx2] = 	filteredVGenerator;      //Vermelho
		samples2[idx2+1] =  filteredVGrid;  //Azul
#endif

		idx2 += 2;
		if(idx2 == 200)
		{
			memcpy(data2, header2, 4);
			memcpy(&data2[4], samples2, 800);
			uart1bsp_sendData(data2, 804);

			idx2 = 0;
		}

	}
#endif

}


/*************************************** EOF ******************************************/

