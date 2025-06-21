/**
  *********************************************************************
  * @file      ps2_task.c/h
  * @brief     该任务是读取并处理ps2手柄传来的遥控数据，
	*            将遥控数据转化为期望的速度、期望的转角、期望的腿长等
  * @note       
  * @history
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *********************************************************************
  */
	
#include "ps2_task.h"
#include "cmsis_os.h"


ps2data_t ps2data;

uint16_t Handkey;	// 按键值读取，零时存储。
uint8_t Comd[2]={0x01,0x42};	//开始命令。请求数据
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键名


extern chassis_t chassis_move;
extern INS_t INS;
uint32_t PS2_TIME=10;//ps2手柄任务周期是10ms
void pstwo_task(void)
{		
	PS2_SetInit();

   while(1)
	 {
		 if(Data[1]!=0x73)
		 {
		  PS2_SetInit();
		 }

		 PS2_data_read(&ps2data);//读数据
		 PS2_data_process(&ps2data,&chassis_move,(float)PS2_TIME/1000.0f);//处理数据，设置期望数据

	   osDelay(PS2_TIME);
	 }
}

//向手柄发送命令
void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);                   //输出一位控制位 DO_H;
		}
		else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);//DO_L

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);                        //时钟拉高
		DWT_Delay(0.000005f);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
		DWT_Delay(0.000005f);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
			Data[1] = ref|Data[1];
	}
	DWT_Delay(0.000016f); 
}

/**************************************************************************
Function: Read the control of the ps2 handle
Input   : none
Output  : none
函数功能：读取PS2手柄的控制量
入口参数：无
返回  值：无
**************************************************************************/	
uint8_t reve_flag=0;
void PS2_data_read(ps2data_t *data)
{
  //读取按键键值
	data->key=PS2_DataKey(); //读取按键键值

  //读取左边遥感X轴方向的模拟量	
	data->lx=PS2_AnologData(PSS_LX); 

  //读取左边遥感Y轴方向的模拟量	
	data->ly=PS2_AnologData(PSS_LY);

  //读取右边遥感X轴方向的模拟量  
	data->rx=PS2_AnologData(PSS_RX);

  //读取右边遥感Y轴方向的模拟量  
	data->ry=PS2_AnologData(PSS_RY);

	if((data->ry<=255&&data->ry>192)||(data->ry<64&&data->ry>=0))
	{
	  data->rx=127;
	}
	if((data->rx<=255&&data->rx>192)||(data->rx<64&&data->rx>=0))
	{
	  data->ry=128;
	}
}

	
void PS2_data_process(ps2data_t *data,chassis_t *chassis,float dt)
{   
	
		
}




//判断是否为红灯模式,0x41=模拟绿灯，0x73=模拟红灯
//返回值；0，红灯模式
//		  其他，其他模式
uint8_t PS2_RedLight(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);//CLK_H
			DWT_Delay(0.000005f);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);//CLK_L
			DWT_Delay(0.000005f);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);//CLK_H
		      if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))//DI
		      Data[byte] = ref|Data[byte];
		}
        DWT_Delay(0.000016f); 
	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
}

//对读出来的PS2的数据进行处理,只处理按键部分  
//只有一个按键按下时按下为0， 未按下为1
uint8_t PS2_DataKey()
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	DWT_Delay(0.000016f); 
  PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
	DWT_Delay(0.000016f);  
}

//short poll
void PS2_ShortPoll(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	DWT_Delay(0.000016f); 
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
	DWT_Delay(0.000016f); 	
}

//进入配置
void PS2_EnterConfing(void)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	DWT_Delay(0.000016f); 
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
	DWT_Delay(0.000016f); 
}
//发送模式设置
void PS2_TurnOnAnalogMode(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	PS2_Cmd(0x03); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				   //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
	DWT_Delay(0.000016f); 
}
//振动设置
void PS2_VibrationMode(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	DWT_Delay(0.000016f); 
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
	DWT_Delay(0.000016f); 
}
//完成并保存配置
void PS2_ExitConfing(void)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//CS_L
	DWT_Delay(0.000016f); 
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//CS_H
	DWT_Delay(0.000016f); 
}

//手柄配置初始化
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	//PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		//完成并保存配置
}




