#include "bsp_openmv.h"
#include "usart.h"

uint8_t  openmv_buff[5];
uint8_t Serial_RxFlag = 0;					//����������ݰ���־λ
uint8_t pRxPacket;
uint8_t RxState = 0;
uint8_t rx_buff[OPENMV_BUFF_SIZE];   //������
extern chassis_t chassis_move;
openmv_t openmv;

float openmv_ground = 0.05;
float turn_openmv = 1.0;

int err_left;
int err_right;
//�Ӿ����ܴ��룬����Ӿ�����

void open_5v_on(void) //����PC15,�ⲿ5v�е�
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
}

void open_5v_close(void)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);
}


//openmvҲ�ǱȽ�����ĵ��ǽ����(�ַ�����������) 
void openmv_init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart7, rx_buff,OPENMV_BUFF_SIZE);    
}

//����ʹ�ô�������ܴ���Ҫ�ǻ����������⣬��Ϊ���ڼ�DMA����ʽ������ֻ����޸ĵĺ���
void Openmv_data_update(void) //��������
{
    for(uint8_t i = 0;i < 3;i++)
    {
        openmv.way_error[i] = openmv_buff[i];    
    }
    openmv.way_flag = openmv_buff[4];

    uint16_t temp_error = (openmv.way_error[0] - '0')*100 + (openmv.way_error[1] - '0')*10 + (openmv.way_error[2] - '0');
    openmv.left_error = temp_error - 160;
    
    //��Ӧ�Ƕȹ�ϵ
    openmv.real_error = openmv.left_error * openmv_ground;  //17.5cm��Ӧ320
}

void Openmv_action(void)  //ѭ������
{
    if(chassis_move.openmv_flag == 1)
    {
       chassis_move.v_set = 0.4; //������ʼ�ٶ�
       chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f; 
       
       chassis_move.turn_set = openmv.real_error * turn_openmv;
                  
    }    
}







