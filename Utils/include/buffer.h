#ifndef BUFFER_H_
#define BUFFER_H_

/******************************************************************************
 *                                 I N C L U D E S                            *
 *****************************************************************************/

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t* data_array;            //ponteiro para vetor dos dados

    uint32_t array_size;            //tamanho da array alocada para o buffer.
                                    //Por questao de eficiencia, array size deve
                                    //ser uma potencia de 2.

    uint32_t get_idx;               //indice de leitura do buffer

    uint32_t put_idx;               //indice de escrita do buffer

    uint32_t length;                //numero de elementos presentes no buffer

} TBuffer;

/**
 * void initBuffer(TBuffer *buffer, uint8_t *array, int32_t size )
 * Descricao - Inicializa os ponteiros do buffer
 * Parametros - buffer: ponteiro para o objeto buffer a ser inicializado
 *              array: ponteiro para o vetor de dados deste buffer
 *              size: comprimento do vetor de dados em bytes
 * Exemplo - initBuffer(&buffer,&array,128 )
 */
void initBuffer(TBuffer *buffer, uint8_t *array, int32_t size);

/**
 * int32_t putByte(TBuffer* buffer, uint8_t ch)
 * Descricao - Acrescenta um Byte ao buffer
 * Parametros - buffer: buffer q sera acrescido
 *              ch: byte q sera acrescentado
 * Retorno - (1) se o elemento foi acrescentado
 *           (-1) se o elemento nao foi acrescentado (buffer cheio)
 * Exemplo - putByte(&buffer, 'A')
 */
int32_t bufferPutByte(TBuffer* buffer, uint8_t ch);

/**
 * u_long getByte(TBuffer *Buffer, uint8_t *data)
 * Descricao - Retira um uint8_t do Buffer
 * Parametros - buffer: Buffer de origem do byte
 *              data: onde o dado deve ser escrito
 * Retorno - (1) se o elemento foi retirado
 *           (-1) se o elemento nao foi retirado (buffer vazio)
 * Exemplo - getByte(&buffer,&data_ptr )
 */
int32_t bufferGetByte(TBuffer *buffer, uint8_t *data);

/**
 * u_long getLength(TBuffer *buffer)
 * Descricao - Retorna o numero de bytes no buffer
 * Parametros - buffer: Buffer
 * Retorno - o numero de elementos do buffer
 * Exemplo - getLength(&buffer)
 */
uint32_t bufferGetLength(TBuffer *buffer);

uint32_t bufferGetN(TBuffer *buffer, uint8_t *data, int32_t n);

uint32_t bufferPutN(TBuffer *buffer, uint8_t *data, int32_t n);

#endif /*BUFFER_H_*/
