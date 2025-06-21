#include "AllHeader.h"

/*********************************************************     
**********************************************************/	 
//使用的不是硬件SPI
#define DELAY_TIME  delay_us(5); 

u16 Handkey;	// 按键值读取，零时存储。
u8 Comd[2]={0x01,0x42};	//开始命令。请求数据
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
u16 MASK[]={
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
	};	
 /**************************************************************************
函数功能：以下是PS2接收器模块的初始化代码
入口参数：无
返回  值：无
按键值与按键明
手柄接口初始化    输入  DI->PA6 
                 输出  DO->PA7    CS->PA4  CLK->PA5
**************************************************************************/
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                           //定义GPIO_InitStructure结构体
	//DI
	RCC_APB2PeriphClockCmd(PS_RCC_DI,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;                     //下拉输入模式
	GPIO_InitStructure.GPIO_Pin=PS_PIN_DI;													
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;                //50MHZ  
	GPIO_Init(PS_PORT_DI,&GPIO_InitStructure);                          
	
	//DO
	RCC_APB2PeriphClockCmd(PS_RCC_DO,ENABLE);           //开启GPIOB时钟
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;                 //复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=PS_PIN_DO;  //DO口 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;             	 //50MHZ
	GPIO_Init(PS_PORT_DO,&GPIO_InitStructure);                         

	//CS
	RCC_APB2PeriphClockCmd(PS_RCC_CS,ENABLE);           //开启GPIOB时钟
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;                 //复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=PS_PIN_CS;  //CS口 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;             	 //50MHZ
	GPIO_Init(PS_PORT_CS,&GPIO_InitStructure);   
	
	//SCK
	RCC_APB2PeriphClockCmd(PS_RCC_CLK,ENABLE);           //开启GPIOB时钟
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;                 //复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin=PS_PIN_CLK;  //CLK口 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;             	 //50MHZ
	GPIO_Init(PS_PORT_CLK,&GPIO_InitStructure);   
	
	
}
/**************************************************************************
函数功能：向手柄发送命令
入口参数：CMD指令
返回  值：无
**************************************************************************/
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //输出一位控制位
		}
		else DO_L;
		CLK_H;                        //时钟拉高
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		if(DI) //为高电平的时候
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}
/**************************************************************************
函数功能：判断是否为红灯模式,0x41=模拟绿灯，0x73=模拟红灯
入口参数：CMD指令
返回  值：0，红灯模式  其他，其他模式
**************************************************************************/
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;
}
/**************************************************************************
函数功能：读取手柄数据
入口参数：无
返回  值：无
**************************************************************************/
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME;
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;
}
/**************************************************************************
函数功能：对读出来的PS2的数据进行处理,只处理按键部分
入口参数：CMD指令
返回  值：无  
//只有一个按键按下时按下为0， 未按下为1
**************************************************************************/
u8 PS2_DataKey()
{
	u8 index;
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
/**************************************************************************
函数功能：向手柄发送命令
入口参数：得到一个摇杆的模拟量	 范围0~256
返回  值：无
**************************************************************************/
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}
//清除数据缓冲区
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
函数功能: 手柄震动函数，
Calls:		 void PS2_Cmd(u8 CMD);
入口参数: motor1:右侧小震动电机 0x00关，其他开
	        motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
返回  值:无
******************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	CS_L;
	delay_us(16);
    PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
/**************************************************************************
函数功能：short poll
入口参数：无
返回  值：无
**************************************************************************/
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
/**************************************************************************
函数功能：进入配置
入口参数：无
返回  值：无
**************************************************************************/
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
/**************************************************************************
函数功能：发送模式设置
入口参数：无
返回  值：无
**************************************************************************/
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
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
	CS_H;
	delay_us(16);
}
/**************************************************************************
函数功能：振动设置
入口参数：无
返回  值：无
**************************************************************************/
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
/**************************************************************************
函数功能：完成并保存配置
入口参数：无
返回  值：无
**************************************************************************/
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
/**************************************************************************
函数功能：手柄配置初始化
入口参数：无
返回  值：无
**************************************************************************/
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


