/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_timer.h	
* @author       
* @version      V1.0
* @date         
* @brief        定时器
* @details      
* @par History  见如下说明
*                 
* version:		
*/

#include "AllHeader.h"



/**
* Function       TIM1_Int_Init
* @author        
* @date             
* @brief         定时器1初始化接口
* @param[in]     arr：自动重装值。psc：时钟预分频数
* @param[out]    void
* @retval        void
* @par History   这里时钟选择为APB1的2倍，而APB1为36M
*/
void TIM1_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	//定时器TIM1初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = (psc-1); //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim   //36Mhz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;    //重复计数关闭
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE ); //使能指定的TIM1中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM1, ENABLE);  //使能TIMx					 
}
/**
* Function       TIM1_Int_Init
* @author        
* @date             
* @brief         定时器1中断服务程序: 主要控制6路舵机运行
* @param[in]     arr：自动重装值。psc：时钟预分频数
* @param[out]    void
* @retval        void
* @par History   这里时钟选择为APB1的2倍，而APB1为36M
*/
int num = 0;
int times = 10;

void TIM1_UP_IRQHandler(void)   //TIM1中断
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM1更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIM1更新中断标志 
		num++;
	

		#ifdef USE_SERVO_J1
		if(num <= (Angle_J[0][0] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J1_PORT, Servo_J1_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J1_PORT, Servo_J1_PIN );		//将舵机接口电平置高
		}
		#endif	   	  

		#ifdef USE_SERVO_J2
		if(num <= (Angle_J[0][1] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J2_PORT, Servo_J2_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J2_PORT, Servo_J2_PIN );		//将舵机接口电平置高
		}
		#endif

		#ifdef USE_SERVO_J3

		if(num <= (Angle_J[0][2] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J3_PORT, Servo_J3_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J3_PORT, Servo_J3_PIN );		//将舵机接口电平置高
		}
		#endif

		#ifdef USE_SERVO_J4
		if(num <= (Angle_J[0][3] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J4_PORT, Servo_J4_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J4_PORT, Servo_J4_PIN );		//将舵机接口电平置高
		}
		#endif

		#ifdef USE_SERVO_J5
		if(num <= (Angle_J[0][4] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J5_PORT, Servo_J5_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J5_PORT, Servo_J5_PIN );		//将舵机接口电平置高
		}
		#endif

		/*
		
		#ifdef USE_SERVO_J6
		if(num <= (Angle_J[0][5] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J6_PORT, Servo_J6_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J6_PORT, Servo_J6_PIN );		//将舵机接口电平置高
		}
		#endif
		
		#ifdef USE_SERVO_J7
		if(num <= (Angle_J[0][6] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J7_PORT, Servo_J7_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J7_PORT, Servo_J7_PIN );		//将舵机接口电平置高
		}
		#endif
		
		#ifdef USE_SERVO_J8
		if(num <= (Angle_J[0][7] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J8_PORT, Servo_J8_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J8_PORT, Servo_J8_PIN );		//将舵机接口电平置高
		}
		#endif
		
		
		
		
		#ifdef USE_SERVO_J9
		if(num <= (Angle_J[1][0] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J9_PORT, Servo_J9_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J9_PORT, Servo_J9_PIN );		//将舵机接口电平置高
		}
		#endif
		
		#ifdef USE_SERVO_J10
		if(num <= (Angle_J[1][1] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J10_PORT, Servo_J10_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J10_PORT, Servo_J10_PIN );		//将舵机接口电平置高
		}
		#endif
		
		#ifdef USE_SERVO_J11
		if(num <= (Angle_J[1][2] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J11_PORT, Servo_J11_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J11_PORT, Servo_J11_PIN );		//将舵机接口电平置高
		}
		#endif
		#ifdef USE_SERVO_J12
		if(num <= (Angle_J[1][3] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J12_PORT, Servo_J12_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J12_PORT, Servo_J12_PIN );		//将舵机接口电平置高
		}
		#endif
		
		#ifdef USE_SERVO_J13
		if(num <= (Angle_J[1][4] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J13_PORT, Servo_J13_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J13_PORT, Servo_J13_PIN );		//将舵机接口电平置高
		}
		#endif
		
		
		#ifdef USE_SERVO_J14
		if(num <= (Angle_J[1][5] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J14_PORT, Servo_J14_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J14_PORT, Servo_J14_PIN );		//将舵机接口电平置高
		}
		#endif
		
		#ifdef USE_SERVO_J15
		if(num <= (Angle_J[1][6] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J15_PORT, Servo_J15_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J15_PORT, Servo_J15_PIN );		//将舵机接口电平置高
		}
		#endif	
		
		#ifdef USE_SERVO_J16
		if(num <= (Angle_J[1][7] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J16_PORT, Servo_J16_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J16_PORT, Servo_J16_PIN );		//将舵机接口电平置高
		}
		#endif	
		
		#ifdef USE_SERVO_J17
		if(num <= (Angle_J[2][0] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J17_PORT, Servo_J17_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J17_PORT, Servo_J17_PIN );		//将舵机接口电平置高
		}
		#endif	
				
		#ifdef USE_SERVO_J18
		if(num <= (Angle_J[2][1] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J18_PORT, Servo_J18_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J18_PORT, Servo_J18_PIN );		//将舵机接口电平置高
		}
		#endif	
				
						
		#ifdef USE_SERVO_J19
		if(num <= (Angle_J[2][2] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J19_PORT, Servo_J19_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J19_PORT, Servo_J19_PIN );		//将舵机接口电平置高
		}
		#endif	
						
		#ifdef USE_SERVO_J20
		if(num <= (Angle_J[2][3] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J20_PORT, Servo_J20_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J20_PORT, Servo_J20_PIN );		//将舵机接口电平置高
		}
		#endif	
						
		#ifdef USE_SERVO_J21
		if(num <= (Angle_J[2][4] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J21_PORT, Servo_J21_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J21_PORT, Servo_J21_PIN );		//将舵机接口电平置高
		}
		#endif

		#ifdef USE_SERVO_J22
		if(num <= (Angle_J[2][5] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J22_PORT, Servo_J22_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J22_PORT, Servo_J22_PIN );		//将舵机接口电平置高
		}
		#endif

		#ifdef USE_SERVO_J23
		if(num <= (Angle_J[2][6] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J23_PORT, Servo_J23_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J23_PORT, Servo_J23_PIN );		//将舵机接口电平置高
		}
		#endif		
		
		#ifdef USE_SERVO_J24
		if(num <= (Angle_J[2][7] * 11 + 500)/times)
		{
			GPIO_SetBits(Servo_J24_PORT, Servo_J24_PIN );		//将舵机接口电平置高
		}
		else
		{
			GPIO_ResetBits(Servo_J24_PORT, Servo_J24_PIN );		//将舵机接口电平置高
		}
		#endif	
    */
		if(num == 200) //200*100=20ms  20ms一个周期
		{
			num = 0;
		}
		
	}
}
