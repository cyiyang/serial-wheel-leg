/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_gpio.c      
* @version      V1.0       
* @brief        驱动gpio源文件
* @details      
* @par History  见如下说明
*                 
* version:	
*/

#include "AllHeader.h"


/**
* Function       Servo_GPIO_Init
* @author        
* @date            
* @brief         需要用到的舵机初始化接口
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/

void Servo_GPIO_Init(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
	
	
	
#ifdef USE_SERVO_J1
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J1_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J1_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J1_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J2
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J2_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J2_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	/*调用库函数，初始化Servo_J2_PORT*/
  	GPIO_Init(Servo_J2_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J3
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J3_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J3_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	/*调用库函数，初始化Servo_J3_PORT*/
  	GPIO_Init(Servo_J3_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J4
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J4_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J4_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J4_PORT*/
  	GPIO_Init(Servo_J4_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J5
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J5_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J5_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J5_PORT*/
  	GPIO_Init(Servo_J5_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J6
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J6_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J6_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J6_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J7
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J7_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J7_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J7_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J8
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J8_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J8_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J8_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J9
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J9_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J9_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J9_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J10
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J10_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J10_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J10_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J11
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J11_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J11_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J11_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J12
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J12_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J12_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J12_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J13
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J13_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J13_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J13_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J14
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J14_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J14_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J14_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J15
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J15_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J15_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J15_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J16
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J16_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J16_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J16_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J17
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J17_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J17_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J17_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J18
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J18_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J18_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J18_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J19
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J19_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J19_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J19_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J20
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J20_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J20_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J20_PORT, &GPIO_InitStructure);		  
#endif
 
#ifdef USE_SERVO_J21
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J21_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J21_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J21_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J22
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J22_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J22_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J22_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J23
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J23_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J23_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J23_PORT, &GPIO_InitStructure);		  
#endif

#ifdef USE_SERVO_J24
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J24_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J24_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化Servo_J1_PORT*/
  	GPIO_Init(Servo_J24_PORT, &GPIO_InitStructure);		  
#endif
}

