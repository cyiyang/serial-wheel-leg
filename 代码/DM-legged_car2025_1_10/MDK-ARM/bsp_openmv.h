#ifndef __BSP_OPENMV_H
#define __BSP_OPENMV_H

#include "main.h"
#include "chassisR_task.h"

#define OPENMV_BUFF_SIZE 7     //���ش�СΪ320 * 240

extern uint8_t openmv_buff[];  
extern uint8_t rx_buff[];

extern uint8_t RxState;		//�����ʾ��ǰ״̬��״̬�ľ�̬����
extern uint8_t pRxPacket;	//�����ʾ��ǰ��������λ�õľ�̬����

typedef struct openmv_data
{
    uint8_t way_error[3];
    uint8_t way_flag;
    int left_error;
    float real_error;
}openmv_t;

extern openmv_t openmv;
void open_5v_on(void);
void open_5v_close(void);
void openmv_init(void);

void Openmv_data_update(void);
void Openmv_action(void);

#endif

