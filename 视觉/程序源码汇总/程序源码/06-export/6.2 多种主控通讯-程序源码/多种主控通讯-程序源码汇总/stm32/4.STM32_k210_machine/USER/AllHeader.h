#ifndef __AllHeader_H
#define __AllHeader_H

#define DEBUG_USARTx USART1



#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "stdint.h"
#include "bsp_common.h"
#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_pwr.h"
//#include "stm32f10x_bkp.h"

#include "delay.h"
#include "bsp_usart.h"
#include "led.h"

typedef struct msg_buf
{
	u16 x; //������
	u16 y; //������
	u16 w; //���
	u16 h; //����
	u16 id; //��ǩ
	u8 class_n;//���̱��
	u8 msg_msg[20]; //��Ч����λ
}msg_k210;


/********k210ͨ�����************/
void recv_k210msg(uint8_t recv_msg);
void deal_recvmsg(void);
void deal_data(u8 egnum);

extern msg_k210 k210_msg;//�յ�k210��Ϣ�ṹ��
#endif


