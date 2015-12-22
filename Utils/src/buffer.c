#include "buffer.h"



/**
 * void initBuffer(TBuffer *buffer, uint8_t *array, int32_t size )
 * Descricao - Inicializa os ponteiros do buffer
 * Parametros - buffer: ponteiro para o objeto buffer a ser inicializado
 *              array: ponteiro para o vetor de dados deste buffer
 *              size: comprimento do vetor de dados em bytes
 * Exemplo - initBuffer(&buffer,&array,128 )
 */
void initBuffer(TBuffer *buffer, uint8_t *array, int32_t size ){
    buffer->data_array = array;
    buffer->array_size = size-1;
    buffer->get_idx = 0;
    buffer->put_idx = 0;
    buffer->length = 0;
}


/**
 * int32_t putByte(TBuffer* buffer, uint8_t ch)
 * Descricao - Acrescenta um Byte ao buffer
 * Parametros - buffer: buffer q sera acrescido
 *              ch: byte q sera acrescentado
 * Retorno - (1) se o elemento foi acrescentado
 *           (-1) se o elemento nao foi acrescentado (buffer cheio)
 * Exemplo - putByte(&buffer, 'A')
 */
int32_t bufferPutByte(TBuffer* buffer, uint8_t ch) {

    if(buffer->length > buffer->array_size)
        return -1;



    int32_t ptr = (buffer->put_idx + buffer->get_idx) & buffer->array_size;

    *(buffer->data_array + ptr) = (uint8_t) ch;
    buffer->put_idx++;
    buffer->length++;

    return 1;

}



/**
 * u_long getByte(TBuffer *Buffer, uint8_t *data)
 * Descricao - Retira um uint8_t do Buffer
 * Parametros - buffer: Buffer de origem do byte
 *              data: onde o dado deve ser escrito
 * Retorno - (1) se o elemento foi retirado
 *           (-1) se o elemento nao foi retirado (buffer vazio)
 * Exemplo - getByte(&buffer,&data_ptr )
 */
int32_t bufferGetByte(TBuffer *buffer, uint8_t *data) {

    /* Testa se tem novo caractere */
    if (buffer->put_idx > 0)
    {

        *data = *(buffer->data_array + buffer->get_idx);
        buffer->get_idx = (buffer->get_idx + 1) & buffer->array_size;

        buffer->put_idx--;
        buffer->length--;

        return 1;

    }  else

        return -1;

}

/**
 * u_long getLength(TBuffer *buffer)
 * Descricao - Retorna o numero de bytes no buffer
 * Parametros - buffer: Buffer
 * Retorno - o numero de elementos do buffer
 * Exemplo - getLength(&buffer)
 */
uint32_t bufferGetLength(TBuffer *buffer) {

    return buffer->length;


}


uint32_t bufferPutN(TBuffer *buffer, uint8_t *data, int32_t n){

    int32_t count;
    int32_t length = buffer->length;


    if (n > (buffer->array_size + 1 - length)) {
        n = buffer->array_size + 1 - length;
    }


    for(count = n;count>0;count--) {

        int32_t ptr = (buffer->put_idx + buffer->get_idx) & buffer->array_size;

        *(buffer->data_array + ptr) = (uint8_t) *data++;
        buffer->put_idx++;

    }

    buffer->length += n;

    return n;


}


uint32_t bufferGetN(TBuffer *buffer, uint8_t *data, int32_t n){

    int32_t got;

    if(buffer->length > (buffer->array_size+1)) {
        buffer->length = 0;
    }

    if (n > buffer->length)
        n = buffer->length;

    got = n;

    for(;n>0;n--){

        *data++ = *(buffer->data_array + buffer->get_idx);


        buffer->put_idx = (buffer->put_idx - 1) & buffer->array_size;
        buffer->get_idx = (buffer->get_idx + 1) & buffer->array_size;
    }

    buffer->length -= got;


    return got;

}
