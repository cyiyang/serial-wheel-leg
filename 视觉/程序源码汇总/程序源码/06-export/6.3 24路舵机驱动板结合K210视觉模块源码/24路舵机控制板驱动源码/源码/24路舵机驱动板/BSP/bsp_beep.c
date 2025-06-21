#include "AllHeader.h"

u32 beep_time = 0;
u32 led_time = 0;
u8 led_state = 1;


void init_led(void)
{
	//初始化蜂鸣器的引脚
  GPIO_InitTypeDef GPIO_InitStructure;
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = LED_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化BEEP_PORT*/
  	GPIO_Init(LED_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(LED_PORT,LED_PIN);
	
	

}

void led_timer(void) //定时器时间 (999+1)*(720+1)/72M = 10ms 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*定时器TIM3初始化*/
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitStructer.TIM_Period = 1000-1;//定时周期
	TIM_TimeBaseInitStructer.TIM_Prescaler = 720-1; //分频系数
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructer);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//开启更新中断

	/*定时器中断初始化*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructer.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructer);
	TIM_Cmd(TIM3, ENABLE);//定时器使能
	
}

void TIM3_IRQHandler(void) //中断，当回响信号很长是，计数值溢出后重复计数，用中断来保存溢出次数
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除中断标志
		
		led_time ++;
//3秒闪两次
		if(led_time % 60 ==0)
		{
			led_state = !led_state;
			LED(led_state);
		}
		
		if(led_time >=300 )
		{
			led_time = 0;
		}
		
	}
}


void init_beep(void)
{
	//初始化蜂鸣器的引脚
  GPIO_InitTypeDef GPIO_InitStructure;
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化BEEP_PORT*/
  	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
		
	beep_timer();//初始化定时器

}

void beep_timer(void) //定时器时间 (99+1)*(720+1)/72M = 1ms 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	NVIC_InitTypeDef NVIC_InitStructer;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/*定时器TIM2初始化*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructer.TIM_Period = 100-1;//定时周期
	TIM_TimeBaseInitStructer.TIM_Prescaler = 720-1; //分频系数
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructer);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//开启更新中断

	/*定时器中断初始化*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructer.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructer);
	TIM_Cmd(TIM2, ENABLE);//定时器使能
}

void TIM2_IRQHandler(void) //中断，当回响信号很长是，计数值溢出后重复计数，用中断来保存溢出次数
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断标志
		if(beep_time != 0)
				beep_time -- ;
	}
}


/**
* Function       
* @brief         设置蜂鸣器响的时长
* @param[in]     
* @param[out]    times:响的时长
* @retval        void
* @par History   无
*/
void set_beep(u8 times) //s为单位
{
	beep_time = times * 1000;
}

/* 以下函数放进死循环 */
//------------------------------
//蜂鸣器按照这样的频率响，TIM2中断2次完成一次流程 1s响5次
void beep_start_stop(void)
{
	if(beep_time != 0)
	{
		BEEP_ON;
		delay_ms(100);
		BEEP_OFF;
		delay_ms(100);
	}
	
	else if(beep_time == 0)
		BEEP_OFF;
}
//----------------------------


