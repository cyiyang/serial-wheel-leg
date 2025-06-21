/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp.c      
* @version      V1.0     
* @brief        驱动总入口
* @details      
* @par History  见如下说明
*                 
* version:	
*/


#include "AllHeader.h"

/**
* Function       bsp_init  
* @brief         硬件设备初始化
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void bsp_init(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	delay_init();
	delay_ms(200);//等待系统稳定
	

	
	Servo_GPIO_Init();
	TIM1_Int_Init(9, 71);			/*计数到10为100us   Tout = (9+1)*(71+1)/72M = 10us */
	USART1_init(115200); //调试串口
	//USART3_init(9600);//和上位机通信
	
	//UART5_init(9600);//---蓝牙通信默认波特率为9600

	init_led(); //灯初始化
	init_beep();  //蜂鸣器初始化
	Key_GPIO_Init();//按键初始化
  
	
	
#if	Battery_SW
	Adc_vol_init();  //检测电压
	J_Adc_init();//检测电流
#endif	

	
#if	PS_TWO
	PS2_Init();		//======ps2驱动端口初始化
	PS2_SetInit();	//======ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
	delay_ms(1000);
#endif
	
#if OLED_SW
	IIC_Init();
	i2c_scanf_addr();
	SSD1306_Init();
		OLED_Draw_Line("OLED init sucess", 1 , false, true);
//	OLED_Draw_Line("Servo angle:90 ", 1 , false, true);
//	OLED_Draw_Line("current:0.03  A", 2 , false, true);
//	OLED_Draw_Line("Battery vol: 7.2 V", 3 , false, true);
	

#endif

	//放到最后才生效，不然还是无法正常使用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁用jlink 只用SWD调试口，PA15、PB3、4做普通IO
	

}
