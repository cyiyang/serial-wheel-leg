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
	u16 x; //横坐标
	u16 y; //纵坐标
	u16 w; //宽度
	u16 h; //长度
	u16 id; //标签
	u8 class_n;//例程编号
	u8 msg_msg[20]; //有效数据位
}msg_k210;


/********k210通信相关************/
void recv_k210msg(uint8_t recv_msg);
void deal_recvmsg(void);
void deal_data(u8 egnum);

extern msg_k210 k210_msg;//收到k210信息结构体
#endif


