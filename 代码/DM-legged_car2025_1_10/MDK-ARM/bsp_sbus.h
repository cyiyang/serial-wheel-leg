#ifndef __BSP_SBUS_H
#define __BSP_SBUS_H

#include "main.h"
#include "usart.h"
#include "chassisR_task.h"
#include "bsp_openmv.h"

#define SBUS_DATA_SIZE 25


typedef struct sbus_t
{   
    uint8_t sbus_head; //头
    uint16_t ch[16];   //数据段   0~15对应CH1~16
    uint8_t sbus_flag; //校验位
    uint8_t sbus_end;  //尾
}SBUS_t;

extern SBUS_t sbus;
extern uint8_t sbus_rx_buf[];       //缓冲数据段   
void SBUS_IT_Open(void);

//数据段处理
extern uint8_t USART5_RX_DMA_BUF[];
void SBUS_value_updata(void);
void SBUS_action(void);

#endif



