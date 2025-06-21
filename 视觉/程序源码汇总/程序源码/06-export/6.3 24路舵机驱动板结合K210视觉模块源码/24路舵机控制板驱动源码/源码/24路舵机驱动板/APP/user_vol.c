#include "AllHeader.h"


/******************************
*函数功能：检测电压，异常蜂鸣器\oled处理 --更换电池后，只能复位恢复
*入口参数：无
*返回  值：无
******************************/
char buf[20]={'\0'};
void dectect_beep(void)
{ 
	u8 V_flag = 1;//电压
	float vol;
	V_flag = Bsp_Bat_State(); 
	
	if(V_flag == 0)//低压的情况 
	{
		set_beep(5);//不停的响
		OLED_Draw_Line("voltage low...", 1, false, true);
		
		led_timer(); //灯的定时器 --闪亮
	}

	if(V_flag == 1)//正常的情况
	{
		sprintf(buf,"Grop:%d  Ver:%.1f ",grop,version);
		OLED_Draw_Line(buf, 1 , false, true);
		memset(buf,0,sizeof(buf));
		vol = Bsp_Bat_Voltage_Z100()/100.0;
		sprintf(buf,"Battery vol: %.1f V ",vol);
		OLED_Draw_Line(buf, 3, false, true);
		
	}
	
	
	beep_start_stop();//蜂鸣器操作 
}



/******************************
*函数功能：检测舵机的电流，异常蜂鸣器\oled处理
*入口参数：无
*返回  值：无
******************************/
//增加该函数
void void_jutce(void)
{
	u8 I_flag = 0;//电流
	float junce;
	I_flag =J_Bsp_Bat_State(); 
	if(I_flag == 0)
	{
		junce = J_Bsp_Bat_Voltage_Z100()/100.0;
		sprintf(buf,"Current: %.2f A ",junce);
		
		OLED_Draw_Line(buf, 2, false, true);
	}
	else
	{
		junce = J_Bsp_Bat_Voltage_Z100()/100.0;
		sprintf(buf,"Current hard!: %.2f A ",junce);
		set_beep(5);//不停的响
		OLED_Draw_Line(buf, 2, false, true);
	}
	
	
	beep_start_stop();//蜂鸣器操作 
}

