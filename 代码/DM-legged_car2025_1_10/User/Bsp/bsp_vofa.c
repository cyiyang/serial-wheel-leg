#include "bsp_vofa.h"
//#include "usbd_cdc_if.h"
//#include "usbd_core.h"
//#include "usbd_cdc.h"
uint8_t send_buf[MAX_BUFFER_SIZE];
uint16_t cnt = 0;


//此代码为daplink无线串口调试代码
vofa_t vofa_debug;
vofa_u vofa_data;
extern chassis_t chassis_move;
/**
***********************************************************************
* @brief      vofa_init(void)
* @param      NULL 
* @retval     void
* @details:   初始化vofa接收
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
* @details:    发送数据给上位机
***********************************************************************
**/
void vofa_start(void)
{
	//vofa_demo();		// demo示例
    vofa_send_data(0, chassis_move.v);   //速度
    vofa_send_data(1,chassis_move.x);    //位移
    vofa_send_data(2,chassis_move.myPithR); //roll偏航角
    vofa_send_data(3,chassis_move.myPithGyroR); //重心偏移
    //最后数据帧
    vofa_sendframetail();
}

/**
***********************************************************************
* @brief:      vofa_transmit(uint8_t* buf, uint16_t len)
* @param:	   void
* @retval:     void
* @details:    修改通信工具，USART或者USB
***********************************************************************
**/
void vofa_transmit(uint8_t* buf, uint16_t len)
{
	//CDC_Transmit_HS((uint8_t *)buf, len);  //USB方案
    
    HAL_UART_Transmit(&huart10,(uint8_t*)buf,len,1000);   //USRAT方案
}
/**
***********************************************************************
* @brief:      vofa_send_data(float data)
* @param[in]:  num: 数据编号 data: 数据 
* @retval:     void
* @details:    将浮点数据拆分成单字节,联合体拆分
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
* @details:   给数据包发送帧尾
***********************************************************************
**/
void vofa_sendframetail(void) 
{
	send_buf[cnt++] = vofa_debug.tail[0];
	send_buf[cnt++] = vofa_debug.tail[1];
	send_buf[cnt++] = vofa_debug.tail[2];
	send_buf[cnt++] = vofa_debug.tail[3];
	
	/* 将数据和帧尾打包发送 */
	vofa_transmit((uint8_t *)send_buf, cnt);
	cnt = 0;// 每次发送完帧尾都需要清零
}
/**
***********************************************************************
* @brief      vofa_demo(void)
* @param      NULL 
* @retval     void
* @details:   demo示例
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



