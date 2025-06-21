#include "AllHeader.h"
/*
	实验模块：
	24路舵机驱动板
	K210视觉模块
	二维云台
	
	实验核心代码：
	usart3接收K210发送的数据
  TIM1中断中控制舵机pwm输出
*/


u8 grop = 0;
float version = 1.1;

int main(void)
{	
	u8 Dnum;
	u8 Doup;

	//舵机角度初始化
	for(Doup = 0;Doup <GROUP_NUM;Doup++)
	{  
		for(Dnum = 0;Dnum <DUOJI_NUM;Dnum++)
		{
			Angle_J[Doup][Dnum] = 90;   //将所有舵机调整到该角度
		}
	}
	
	bsp_init();//硬件初始化 
	USART3_init(115200);//和上位机通信
	
	while (1)
	{
		dectect_beep();//检测电压
		
		void_jutce(); //检测电流
	}
 								    
}


