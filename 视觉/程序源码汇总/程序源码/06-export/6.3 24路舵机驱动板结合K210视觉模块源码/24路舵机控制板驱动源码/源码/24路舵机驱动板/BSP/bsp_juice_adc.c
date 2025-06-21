#include "AllHeader.h"


// 电池低电压检测计数阈值，乘50毫秒就是延迟时间，单位为毫秒。
// 例如：50*20=1000，即1秒。
#define Junce_CHECK_COUNT        20
int Voltage_JUNCE = 0;        // 电池电压值
int Jucet_Low_Count = 0;   // 电流过载计数
u8 J_bat_state = 1;

void J_Adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PC1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}			

//获得ADC值
//ch:通道值 0~3
u16 J_Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

// 获得 ADC 多次测量平均值, ch:通道值 ; times:测量次数
u16 J_Adc_Get_Average(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for (t = 0; t < times; t++)
	{
		temp_val += J_Get_Adc(ch);
		// delay_ms(5);
	}
	return temp_val / times;
}

// 获得测得原始电压值
float J_Adc_Get_Measure_Volotage(void)
{
	u16 adcx;
	float temp;
	adcx = J_Adc_Get_Average(11, 1);	//ADC Channel 1  ADC Channel 11  
	temp = (float)adcx * (33.0f / 4096);
	return temp;
}

// 获得实际电池分压前电压
float J_Adc_Get_Battery_Volotage(void)
{
	float temp;
	temp = J_Adc_Get_Measure_Volotage();
	return temp;
}


// 超载：1  ，正常：0
u8 J_Bsp_Bat_State(void)
{

	Voltage_JUNCE = (int) (J_Adc_Get_Battery_Volotage() * 100);
	if (Voltage_JUNCE > 1600)
	{
		Jucet_Low_Count++;
		if(Jucet_Low_Count > Junce_CHECK_COUNT)
		{
			return 1;

		}
	}
	else
	{
		Jucet_Low_Count = 0;
	}
	return 0;
}

//返回当前测量的电压
int J_Bsp_Bat_Voltage_Z100(void)
{
	return Voltage_JUNCE;
}


// 返回系统是否进入供电正常，正常返回1，不正常返回0
u8 J_Bsp_System_Enable(void)
{
	return J_bat_state;
}


