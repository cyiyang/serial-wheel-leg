#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "AllHeader.h"


void USART1_init(u32 baudrate);
void USART1_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length);
void USART1_Send_U8(uint8_t ch);


void USART3_init(u32 baudrate);
void USART3_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length);
void USART3_Send_U8(uint8_t ch);

void UART5_init(u32 baudrate);
void UART5_Send_U8(uint8_t ch);
void UART5_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length);

#endif
