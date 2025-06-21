#include "bsp_openmv.h"
#include "usart.h"

uint8_t  openmv_buff[5];
uint8_t Serial_RxFlag = 0;					//定义接收数据包标志位
uint8_t pRxPacket;
uint8_t RxState = 0;
uint8_t rx_buff[OPENMV_BUFF_SIZE];   //缓存区
extern chassis_t chassis_move;
openmv_t openmv;

float openmv_ground = 0.05;
float turn_openmv = 1.0;

int err_left;
int err_right;
//视觉接受代码，完成视觉功能

void open_5v_on(void) //上拉PC15,外部5v有电
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
}

void open_5v_close(void)
{
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);
}


//openmv也是比较逆天的但是解决了(字符串乱码问题) 
void openmv_init(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart7, rx_buff,OPENMV_BUFF_SIZE);    
}

//单独使用串口问题很大，主要是缓存区的问题，改为串口加DMA的形式：这里只添加修改的函数
void Openmv_data_update(void) //处理数据
{
    for(uint8_t i = 0;i < 3;i++)
    {
        openmv.way_error[i] = openmv_buff[i];    
    }
    openmv.way_flag = openmv_buff[4];

    uint16_t temp_error = (openmv.way_error[0] - '0')*100 + (openmv.way_error[1] - '0')*10 + (openmv.way_error[2] - '0');
    openmv.left_error = temp_error - 160;
    
    //对应角度关系
    openmv.real_error = openmv.left_error * openmv_ground;  //17.5cm对应320
}

void Openmv_action(void)  //循迹代码
{
    if(chassis_move.openmv_flag == 1)
    {
       chassis_move.v_set = 0.4; //给定初始速度
       chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f; 
       
       chassis_move.turn_set = openmv.real_error * turn_openmv;
                  
    }    
}







