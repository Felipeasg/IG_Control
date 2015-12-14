/**
 *************************************************************************************
 * \file    uart1bsp.c
 * \author  Felipe Adriano
 * \version V1.0
 * \date    03/05/2015
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

#include "uart1bsp.h"

/******************************* DEFINITIONS AND MACROS *******************************/

#define USARTx_IRQn			USART1_IRQn
#define USARTx_IRQHandler	USART1_IRQHandler

#define USARTx_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define USARTx_TX_DMA_STREAM             DMA2_Stream7

/************************* TYPEDEFS, CLASSES AND STRUCTURES ***************************/

/************************************** PROTOTYPES ************************************/

/********************************** GLOBAL VARIABLES **********************************/

/********************************** LOCAL  VARIABLES **********************************/

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_tx;

/******************************* FUNCTION  IMPLEMENTATION *****************************/


void uart1bsp_init(void)
{
	__GPIOB_CLK_ENABLE();	// Habilita o barramento de clock do GPIOA
	__USART1_CLK_ENABLE();	// Habilita o barramento de clock da USART1
	__DMA2_CLK_ENABLE();

	// Configura os GPIOs da USART1 como Alternate Function
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Configura��o do perif�rico USART
	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUD_RATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

	HAL_NVIC_SetPriority(USARTx_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(USARTx_IRQn);


	/*##-3- Configure the DMA streams ##########################################*/
	/* Configure the DMA handler for Transmission process */
	hdma_tx.Instance                 = USARTx_TX_DMA_STREAM;
	hdma_tx.Init.Channel             = USARTx_TX_DMA_CHANNEL;
	hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                = DMA_NORMAL;
	hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

	HAL_DMA_Init(&hdma_tx);

	/* Associate the initialized DMA handle to the the UART handle */
	__HAL_LINKDMA(&huart1, hdmatx, hdma_tx);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(huart1.hdmatx);
}


uint8_t uart1bsp_getByte(void)
{
	uint8_t u8_data;

	HAL_UART_Receive(&huart1, &u8_data, 1, 100);

	return u8_data;
}

void uart1bsp_sendData(void* data, uint32_t size)
{
	HAL_UART_DMAResume(&huart1);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)data, size);
}

#ifdef STDIO_UART

/**
 * @brief _write redefinition to printf uart dump - stdio.h
 */
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1, ptr, len, USART_TIMEOUT);

	return len;
}


/**
 * @brief _read redefinition to scanf uart - stdio.h
 */
int _read (int file, char *ptr, int len)
{
	HAL_UART_Receive(&huart1, ptr, len, USART_TIMEOUT);
	return len;
}

#endif

/*************************************** EOF ******************************************/

