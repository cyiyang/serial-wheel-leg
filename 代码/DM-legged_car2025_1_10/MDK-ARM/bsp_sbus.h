#ifndef __BSP_SBUS_H
#define __BSP_SBUS_H

#include "main.h"
#include "usart.h"
#include "chassisR_task.h"
#include "bsp_openmv.h"

#define SBUS_DATA_SIZE 25


typedef struct sbus_t
{   
    uint8_t sbus_head; //ͷ
    uint16_t ch[16];   //���ݶ�   0~15��ӦCH1~16
    uint8_t sbus_flag; //У��λ
    uint8_t sbus_end;  //β
}SBUS_t;

extern SBUS_t sbus;
extern uint8_t sbus_rx_buf[];       //�������ݶ�   
void SBUS_IT_Open(void);

//���ݶδ���
extern uint8_t USART5_RX_DMA_BUF[];
void SBUS_value_updata(void);
void SBUS_action(void);

#endif



