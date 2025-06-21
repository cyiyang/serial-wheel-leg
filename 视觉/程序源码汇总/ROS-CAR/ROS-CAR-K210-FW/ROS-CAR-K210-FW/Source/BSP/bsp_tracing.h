#ifndef __BSP_TRACING_H__
#define __BSP_TRACING_H__

#include "stm32f10x.h"

#define ENABLE_TRACING  (1)


#define TRACING_CLK     RCC_APB2Periph_GPIOC
#define TRACING_PORT    GPIOC

#define X1_PIN          GPIO_Pin_3
#define X2_PIN          GPIO_Pin_2
#define X3_PIN          GPIO_Pin_1
#define X4_PIN          GPIO_Pin_0



#define READ_X1()       GPIO_ReadInputDataBit(GPIOC, X1_PIN)
#define READ_X2()       GPIO_ReadInputDataBit(GPIOC, X2_PIN)
#define READ_X3()       GPIO_ReadInputDataBit(GPIOC, X3_PIN)
#define READ_X4()       GPIO_ReadInputDataBit(GPIOC, X4_PIN)


void Tracing_Init(void);
void Tracing_line_handle(void);
void Tracing_Get_Value(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2);
void Tracing_Line_Walking(void);
#endif
