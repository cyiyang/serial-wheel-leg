#include "led.h"
#include "stm32f10x.h"


void led_int(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC				
}

