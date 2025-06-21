#ifndef K210_MSG_DEAL_H
#define K210_MSG_DEAL_H

#include <stdio.h>
#include <string.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct k210_msg_struct
{
  uint16_t x; //横坐标
	uint16_t y; //纵坐标
	uint16_t w; //宽度
	uint16_t h; //长度
	uint16_t id; //标签
	uint8_t class_n;//例程编号
	uint8_t msg_msg[20]; //有效数据位
}msg_k210;

extern msg_k210 k210_msg ;

void recv_k210msg(uint8_t recv_msg);
void deal_recvmsg(void);

#ifdef __cplusplus
}
#endif

#endif
