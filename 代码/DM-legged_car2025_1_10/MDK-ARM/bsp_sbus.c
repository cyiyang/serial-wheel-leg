#include "bsp_sbus.h"

#define SBUS_DATA_SIZE 25

uint8_t sbus_rx_sta = 0;                 // sbus ����״̬��0��δ��ɣ�1�������һ֡����
uint8_t sbus_rx_buf[SBUS_DATA_SIZE];     // ����sbus���ݻ�����---DMA����
uint8_t sbus_connect_flag = 0;			    // �����Ƿ�����
SBUS_t sbus;                            // SBUS �ṹ��ʵ����
uint8_t USART5_RX_DMA_BUF[SBUS_DATA_SIZE];

//���ݿ��ƶ�Ӧ��ϵ
float sbus_vel_ratio = -0.008f;
float sbus_turn_ratio = 0.02f;
float sbus_turn_ratio2 = 0.09f;
float sbus_leg_ratio = 0.00004f;  
float sbus_roll_ratio = -0.00025f;
extern chassis_t chassis_move;
extern vmc_leg_t vmc;
int sbus_reverse_flag=1;
uint8_t sbus_myflag=0;
int sbus_reverseRoll_flag=1;
int64_t sbus_movetime=0;
float sbustobluetooth[4];


void SBUS_IT_Open(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart5, USART5_RX_DMA_BUF,SBUS_DATA_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_uart5_rx, DMA_IT_HT);		   		// �ֶ��ر�DMA_IT_HT�ж�
}

void SBUS_value_updata(void) //ԭͨ����һ   200~1000~1800
{
    if(sbus.ch[6] == 1800) //CH7
    {
        chassis_move.start_flag = 1;
        chassis_move.recover_flag = 0;
    }
    else 
    {
        chassis_move.start_flag = 0;
    }
    
    if(sbus.ch[4] == 200 && chassis_move.start_flag == 1)//CH5
    {
          chassis_move.autoleg_flag = 0;
    }
    else if(sbus.ch[4] == 1000 && chassis_move.start_flag == 1)  
    {       
         //chassis_move.roll_set=0.0f; //�����Ϊ0
         chassis_move.autoleg_flag = 1;     //�̶��߶�    
    }
    else if(sbus.ch[4] == 1800 && chassis_move.start_flag == 1)
    {
        chassis_move.prejump_flag = 1;
    }
    
    if(sbus.ch[7] >= 1000 && chassis_move.start_flag == 1) //CH8
    {
        chassis_move.autoturn_flag = 1;
    }
    
    if(sbus.ch[5] >= 1000 && chassis_move.start_flag == 1) //CH6
    {
        chassis_move.openmv_flag = 1;
    }
    
     if(chassis_move.recover_flag==0
    &&((chassis_move.myPithR<((-3.1415926f)/4.0f)&&chassis_move.myPithR>((-3.1415926f)/2.0f))
  ||(chassis_move.myPithR>(3.1415926f/4.0f)&&chassis_move.myPithR<(3.1415926f/2.0f))))
    {
      chassis_move.recover_flag=1;//��Ҫ����
      chassis_move.leg_set=0.05f;//ԭʼ�ȳ�
    }
    
    //ģ������������ӳ�䷽ʽ 200-1800 תΪ -128~128
    for(int sbus_i = 0 ; sbus_i <= 3 ; sbus_i++)
    {
        sbustobluetooth[0+sbus_i] = (float)(sbus.ch[0+sbus_i] - 1000)*0.16f;
    }
}


void SBUS_action(void)  //SBUS��Ӧ��ң�ػ�ͨ��
{
    if(chassis_move.start_flag  == 1)
    {
        chassis_move.front_flag=1;
        chassis_move.v_set=(sbustobluetooth[2])*sbus_vel_ratio; //CH3
        chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;
    }
    else
    {
        chassis_move.front_flag = 0;
        chassis_move.v_set = 0.0f;   
    }
    chassis_move.last_front_flag = chassis_move.front_flag;
    
    if(chassis_move.start_flag == 1 && chassis_move.autoturn_flag == 1 && chassis_move.openmv_flag == 0) //û��ѭ����������ת��
    {
            chassis_move.turn_flag = 1;
            chassis_move.turn_set = (sbustobluetooth[3])*sbus_turn_ratio;

    }else 
    {
        chassis_move.turn_flag = 0;
    }
    
    if(chassis_move.last_turn_flag == 1 && chassis_move.turn_flag == 0)
    {
        chassis_move.turn_set=chassis_move.total_yaw;
    }
    chassis_move.last_turn_flag = chassis_move.turn_flag;
    
        if(chassis_move.autoleg_flag == 0)
        {				
            chassis_move.leg_set = chassis_move.leg_set + (sbustobluetooth[1])*sbus_leg_ratio;  //�ȳ�ֵ
            //chassis_move.roll_set = 0.0f;  //CH5���м�Ҳʹ��������ƽ
        }else 
        {
            chassis_move.leg_set = 0.05f;
        }
        
        if(chassis_move.openmv_flag == 1 && chassis_move.start_flag == 1 && chassis_move.turn_flag == 1) //��ת��Ȼ������ѭ��
        {
           // Openmv_action(); //��ѭ���Ĵ���
        }       
              
        chassis_move.roll_set=chassis_move.roll_set+(sbustobluetooth[0])*sbus_roll_ratio;
        mySaturate(&chassis_move.roll_set,-0.19f,0.19f);
        mySaturate(&chassis_move.leg_set,0.045f,0.1f);

        if((fabsf(chassis_move.last_leg_set-chassis_move.leg_set)>0.0001f) && chassis_move.autoleg_flag == 0)//ң���������ȳ��ڱ仯�������߼���ͬ
        {
            chassis_move.leg_flag=1;//Ϊ1��־��ң�����ڿ����ȳ����������������־���Բ�������ؼ�⣬��Ϊ���ȳ�����������ʱ����ؼ�������Ϊ����� 			
        }
        else
        {
            chassis_move.leg_flag=0;
        }
        chassis_move.last_leg_set=chassis_move.leg_set;
        
        if(chassis_move.last_prejump_flag != 1 && chassis_move.prejump_flag == 1 && chassis_move.start_flag == 1) //�Ƿ���Ծ
        {
            chassis_move.jump_flag = 1; 
        }
        chassis_move.last_prejump_flag = chassis_move.prejump_flag;
        
       if(chassis_move.movejump_flag==1) //���ٱ�����Ծ���ܵ�һ��ʱ��������Ծ
       {
        chassis_move.v_set=100.0f*sbus_vel_ratio;
        chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;
        sbus_movetime++;
        if(sbus_movetime>50)
         {
            sbus_movetime=0;
            chassis_move.jump_flag=1;
            chassis_move.v_set=0.0f;
            chassis_move.movejump_flag=0;
         }
       }   
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{  
    if(huart->Instance == UART5)
    {   
        sbus_rx_sta = 1;
        if ( USART5_RX_DMA_BUF[0] != 0x0F || USART5_RX_DMA_BUF[SBUS_DATA_SIZE - 1] != 0x00)
        {
           sbus_connect_flag = 0;
        }
        else
        {
           sbus_connect_flag = 1;
        }
        
        if( sbus_connect_flag == 1)
        {
         /*  SBUS�����ź�----ͨ��STM32�ķ�����  */
            sbus.ch[0] = ((int16_t)USART5_RX_DMA_BUF[1] >> 0 | ((int16_t)USART5_RX_DMA_BUF[2] << 8)) & 0x7FF;
            sbus.ch[1] = ((int16_t)USART5_RX_DMA_BUF[2] >> 3 | ((int16_t)USART5_RX_DMA_BUF[3] << 5)) & 0x7FF;
            sbus.ch[2] = ((int16_t)USART5_RX_DMA_BUF[3] >> 6 | ((int16_t)USART5_RX_DMA_BUF[4] << 2) | ((int16_t)USART5_RX_DMA_BUF[5] << 10)) & 0x7FF;
            sbus.ch[3] = ((int16_t)USART5_RX_DMA_BUF[5] >> 1 | ((int16_t)USART5_RX_DMA_BUF[6] << 7)) & 0x7FF;
            sbus.ch[4] = ((int16_t)USART5_RX_DMA_BUF[6] >> 4 | ((int16_t)USART5_RX_DMA_BUF[7] << 4)) & 0x7FF;
            sbus.ch[5] = ((int16_t)USART5_RX_DMA_BUF[7] >> 7 | ((int16_t)USART5_RX_DMA_BUF[8] << 1) | ((int16_t)USART5_RX_DMA_BUF[9] << 9)) & 0x7FF;
            sbus.ch[6] = ((int16_t)USART5_RX_DMA_BUF[9] >> 2 | ((int16_t)USART5_RX_DMA_BUF[10] << 6)) & 0x7FF;
            sbus.ch[7] = ((int16_t)USART5_RX_DMA_BUF[10] >> 5 | ((int16_t)USART5_RX_DMA_BUF[11] << 3)) & 0x7FF;
            
            sbus.ch[8] = ((int16_t)USART5_RX_DMA_BUF[12] >> 0 | ((int16_t)USART5_RX_DMA_BUF[13] << 8)) & 0x7FF;
            sbus.ch[9] = ((int16_t)USART5_RX_DMA_BUF[13] >> 3 | ((int16_t)USART5_RX_DMA_BUF[14] << 5)) & 0x7FF;
            sbus.ch[10] = ((int16_t)USART5_RX_DMA_BUF[14] >> 6 | ((int16_t)USART5_RX_DMA_BUF[15] << 2) | ((int16_t)USART5_RX_DMA_BUF[16] << 10)) & 0x7FF;
            sbus.ch[11] = ((int16_t)USART5_RX_DMA_BUF[16] >> 1 | ((int16_t)USART5_RX_DMA_BUF[17] << 7)) & 0x7FF;
            sbus.ch[12] = ((int16_t)USART5_RX_DMA_BUF[17] >> 4 | ((int16_t)USART5_RX_DMA_BUF[18] << 4)) & 0x7FF;
            sbus.ch[13] = ((int16_t)USART5_RX_DMA_BUF[18] >> 7 | ((int16_t)USART5_RX_DMA_BUF[19] << 1) | ((int16_t)USART5_RX_DMA_BUF[20] << 9)) & 0x7FF;
            sbus.ch[14] = ((int16_t)USART5_RX_DMA_BUF[20] >> 2 | ((int16_t)USART5_RX_DMA_BUF[21] << 6)) & 0x7FF;
            sbus.ch[15] = ((int16_t)USART5_RX_DMA_BUF[21] >> 5 | ((int16_t)USART5_RX_DMA_BUF[22] << 3)) & 0x7FF;
            
            sbus.sbus_flag = (USART5_RX_DMA_BUF[23]&0x04) >> 2;
            
            if(sbus.sbus_flag == 0) // ��������һ֡
            {
               SBUS_value_updata();
               SBUS_action();
            }
                       
            sbus_connect_flag = 0; //��һ�ν���           
        } 
              
        SBUS_IT_Open();
    } 
    
    //�Ӿ�Ҳ����DMA���ܶ���������
//    if(huart->Instance == UART7)
//    {   
//       
//       for(uint8_t i= 0;i<OPENMV_BUFF_SIZE;i++)
//        {    
//            uint8_t RxData = rx_buff[i];	
//            /*ʹ��״̬����˼·�����δ������ݰ��Ĳ�ͬ����*/
//		
//		/*��ǰ״̬Ϊ0���������ݰ���ͷ*/
//			if(RxData == ':' && RxState == 0)		//�������ȷʵ�ǰ�ͷ��������һ�����ݰ��Ѵ������
//			{
//				RxState = 1;			//����һ��״̬
//				pRxPacket = 0;			//���ݰ���λ�ù���
//			}
//            else if(RxData != '@' && RxState == 1)
//            {
//                openmv_buff[pRxPacket] = RxData;
//                pRxPacket ++; 
//            }
//            else if(RxData == '@')
//            {
//                RxState = 0;
//                pRxPacket = 0;
//            }
//        
//     }
//        Openmv_data_update();
//        Openmv_action();
//     
//       HAL_UARTEx_ReceiveToIdle_DMA(&huart7, rx_buff,OPENMV_BUFF_SIZE);
//    }
}
	
	
	
	
