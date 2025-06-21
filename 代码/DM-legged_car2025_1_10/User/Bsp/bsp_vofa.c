#include "bsp_vofa.h"
//#include "usbd_cdc_if.h"
//#include "usbd_core.h"
//#include "usbd_cdc.h"
uint8_t send_buf[MAX_BUFFER_SIZE];
uint16_t cnt = 0;


//�˴���Ϊdaplink���ߴ��ڵ��Դ���
vofa_t vofa_debug;
vofa_u vofa_data;
extern chassis_t chassis_move;
/**
***********************************************************************
* @brief      vofa_init(void)
* @param      NULL 
* @retval     void
* @details:   ��ʼ��vofa����
***********************************************************************
**/
void vofa_init(void)
{
    vofa_debug.tail[0] = 0x00;
    vofa_debug.tail[1] = 0x00;
    vofa_debug.tail[2] = 0x80;
    vofa_debug.tail[3] = 0x7f;   
}

/**
***********************************************************************
* @brief:      vofa_start(void)
* @param:		   void
* @retval:     void
* @details:    �������ݸ���λ��
***********************************************************************
**/
void vofa_start(void)
{
	//vofa_demo();		// demoʾ��
    vofa_send_data(0, chassis_move.v);   //�ٶ�
    vofa_send_data(1,chassis_move.x);    //λ��
    vofa_send_data(2,chassis_move.myPithR); //rollƫ����
    vofa_send_data(3,chassis_move.myPithGyroR); //����ƫ��
    //�������֡
    vofa_sendframetail();
}

/**
***********************************************************************
* @brief:      vofa_transmit(uint8_t* buf, uint16_t len)
* @param:	   void
* @retval:     void
* @details:    �޸�ͨ�Ź��ߣ�USART����USB
***********************************************************************
**/
void vofa_transmit(uint8_t* buf, uint16_t len)
{
	//CDC_Transmit_HS((uint8_t *)buf, len);  //USB����
    
    HAL_UART_Transmit(&huart10,(uint8_t*)buf,len,1000);   //USRAT����
}
/**
***********************************************************************
* @brief:      vofa_send_data(float data)
* @param[in]:  num: ���ݱ�� data: ���� 
* @retval:     void
* @details:    ���������ݲ�ֳɵ��ֽ�,��������
***********************************************************************
**/
void vofa_send_data(uint8_t num, float data) 
{
//	send_buf[cnt++] = byte0(data);
//	send_buf[cnt++] = byte1(data);
//	send_buf[cnt++] = byte2(data);
//	send_buf[cnt++] = byte3(data);
    
    vofa_data.vofadata = data;
    vofa_debug.fdata[num] = data;
    send_buf[cnt++] = vofa_data.vofa_str[0];
    send_buf[cnt++] = vofa_data.vofa_str[1];
    send_buf[cnt++] = vofa_data.vofa_str[2];
    send_buf[cnt++] = vofa_data.vofa_str[3];
}
/**
***********************************************************************
* @brief      vofa_sendframetail(void)
* @param      NULL 
* @retval     void
* @details:   �����ݰ�����֡β
***********************************************************************
**/
void vofa_sendframetail(void) 
{
	send_buf[cnt++] = vofa_debug.tail[0];
	send_buf[cnt++] = vofa_debug.tail[1];
	send_buf[cnt++] = vofa_debug.tail[2];
	send_buf[cnt++] = vofa_debug.tail[3];
	
	/* �����ݺ�֡β������� */
	vofa_transmit((uint8_t *)send_buf, cnt);
	cnt = 0;// ÿ�η�����֡β����Ҫ����
}
/**
***********************************************************************
* @brief      vofa_demo(void)
* @param      NULL 
* @retval     void
* @details:   demoʾ��
***********************************************************************
**/
void vofa_demo(void) 
{
	// Call the function to store the data in the buffer
	vofa_send_data(0, chassis_move.v);
	vofa_send_data(1, chassis_move.v_act);
	vofa_send_data(2, chassis_move.v_set);
	vofa_send_data(3, chassis_move.x);
	vofa_send_data(4, chassis_move.x_set);
	vofa_send_data(5, 0.0f);
	vofa_send_data(6, 0.0f);
	// Call the function to send the frame tail
	vofa_sendframetail();
}



