#ifndef __BSP_VOFA_H
#define __BSP_VOFA_H
#include "stdint.h"
#include "chassisR_task.h"
#include "usart.h"

#define MAX_BUFFER_SIZE 1024
#define CH_COUNT  4
//#define byte0(dw_temp)     (*(char*)(&dw_temp))       //USB�Ĵ���-->USB����ʵ������˭�������ߵ���
//#define byte1(dw_temp)     (*((char*)(&dw_temp) + 1))
//#define byte2(dw_temp)     (*((char*)(&dw_temp) + 2))
//#define byte3(dw_temp)     (*((char*)(&dw_temp) + 3))

typedef struct Frame    //�ٷ�����ṹ��
{    
    float fdata[CH_COUNT];
    unsigned char tail[4];//β֡
}vofa_t;

typedef union Data
{
    uint8_t vofa_str[4];
    float vofadata;
}vofa_u;

extern vofa_u vofa_data;
extern vofa_t vofa_debug;
void vofa_init(void);
void vofa_start(void);
void vofa_send_data(uint8_t num, float data); 
void vofa_sendframetail(void);
void vofa_demo(void);

extern uint8_t send_buf[];

#endif // DEBUG--->�������
