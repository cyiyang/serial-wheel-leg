#include "bsp.h"
#include "app.h"
#include "config.h"

#include "app_flash.h"
#include "app_oled.h"


static uint8_t fac_us = 0;  //us延时倍乘数
static uint16_t fac_ms = 0; //ms延时倍乘数

uint8_t g_test_mode = MODE_STANDARD;


// 系统时钟为72M，1/8为9M，既每秒钟震动9000000次，1s=1000ms=1000000us =》 1us震动9次
void SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);// 选择外部时钟  HCLK/8
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
}

/**********************************************************
** 函数名: delay_init	初始化延迟函数
** 功能描述: 初始化延迟函数,SYSTICK的时钟固定为HCLK时钟的1/8
** 输入参数: SYSCLK（单位MHz)
** 输出参数: 无
** 调用方法：如果系统时钟被设为72MHz,则调用delay_init(72)
** 系统时钟为72M，1/8为9M，既每秒钟震动9000000次，1s=1000ms=1000000us =》 1us震动9次
***********************************************************/
void delay_init(void)
{
	uint8_t SYSCLK = 72;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // 选择外部时钟  HCLK/8
	fac_us = SYSCLK / 8;
	fac_ms = (uint16_t)fac_us * 1000;
}

/**********************************************************
** 函数名: delay_ms
** 功能描述: 延时nms
** 输入参数: nms
** 输出参数: 无
** 说明：SysTick->LOAD为24位寄存器,所以,最大延时为:
		nms<=0xffffff*8*1000/SYSCLK
		SYSCLK单位为Hz,nms单位为ms
		对72M条件下,nms<=1864 
***********************************************************/
void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD = (uint32_t)nms * fac_ms; //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;			   //清空计数器
	SysTick->CTRL = 0x01;			   //开始倒数
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
	SysTick->CTRL = 0x00;						  //关闭计数器
	SysTick->VAL = 0X00;						  //清空计数器
}

/**********************************************************
** 函数名: delay_us
** 功能描述: 延时nus，nus为要延时的us数.
** 输入参数: nus
** 输出参数: 无
***********************************************************/
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = nus * fac_us; //时间加载
	SysTick->VAL = 0x00;		  //清空计数器
	SysTick->CTRL = 0x01;		  //开始倒数
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
	SysTick->CTRL = 0x00;						  //关闭计数器
	SysTick->VAL = 0X00;						  //清空计数器
}


void LED_GPIO_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	/*选择要控制的引脚*/
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*调用库函数，初始化PORT*/
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

	LED_ON();
}

//JTAG模式设置,用于设置JTAG的模式
//mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
//#define JTAG_SWD_DISABLE   0X02
//#define SWD_ENABLE         0X01
//#define JTAG_SWD_ENABLE    0X00
void Bsp_JTAG_Set(uint8_t mode)
{
	uint32_t temp;
	temp = mode;
	temp <<= 25;
	RCC->APB2ENR |= 1 << 0;	  //开启辅助时钟
	AFIO->MAPR &= 0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR |= temp;		  //设置jtag模式
}

// 发送当前版本号到主机上
void Bsp_Send_Version(void)
{
	#define LEN       7
	uint8_t data[LEN] = {0};
	uint8_t checknum = 0;
	data[0] = PTO_HEAD;
	data[1] = PTO_DEVICE_ID - 1;
	data[2] = LEN - 2;           // 数量
	data[3] = FUNC_VERSION;   // 功能字
	data[4] = VERSION_MAJOR;      // 大版本号, 组成结果：1.2
	data[5] = VERSION_MINOR;      // 小版本号

	for (uint8_t i = 2; i < LEN - 1; i++)
	{
		checknum += data[i];
	}
	data[LEN - 1] = checknum;
	USART1_Send_ArrayU8(data, LEN);
}

// LED指示单片机底层，每100毫秒调用一次,效果是LED每3秒闪2次。
void Bsp_Led_Show_State(void)
{
	static uint8_t led_flash = 0;
	led_flash++;
	if (led_flash >= 30) led_flash = 0;

	if (led_flash >= 1 && led_flash < 4)
	{
		LED_ON();
	}
	else if (led_flash >= 4 && led_flash < 7)
	{
		LED_OFF();
	}
	else if (led_flash >= 7 && led_flash < 10)
	{
		LED_ON();
	}
	else if (led_flash >= 10 && led_flash < 13)
	{
		LED_OFF();
	}
}

// 单片机指示灯显示低电量状态，LED闪烁和蜂鸣器BB
void Bsp_Led_Show_Low_Battery(void)
{
	static uint8_t led_flash_1 = 0;
	if (led_flash_1)
	{
		BEEP_ON();
		LED_ON();
		led_flash_1 = 0;
	}
	else
	{
		BEEP_OFF();
		LED_OFF();
		led_flash_1 = 1;
	}
}

// 设置工厂测试模式
void Bsp_Set_TestMode(uint16_t mode)
{
	g_test_mode = mode & 0xFF;
	if (g_test_mode > 2) g_test_mode = 0;
}

// 读取工厂测试模式状态
uint8_t Bsp_Get_TestMode(void)
{
	return g_test_mode;
}


void Bsp_Init(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置为优先级组2

	USART1_Init(USART1_BAUDRATE);
	printf("\nFirmware Version: K210-ROS-CAR-V%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	printf("Firmware Compiled: %s, %s\r\n\n", __DATE__, __TIME__);
	
	#if ENABLE_USART2
	USART2_Init(USART2_BAUDRATE);
	#endif

	#if ENABLE_USART3
	USART3_Init(USART3_BAUDRATE);
	#endif

	#if ENABLE_UART4
	UART4_Init(UART4_BAUDRATE);
	#endif

	Bsp_JTAG_Set(SWD_ENABLE);
	LED_GPIO_Init();
	Beep_GPIO_Init();
	Key_GPIO_Init();
	// Adc_Init();

#if ENABLE_OLED
	IIC_Init();
	i2c_scanf_addr();
	SSD1306_Init();
#endif

	// Flash_TestMode_Init();
	// if (g_test_mode == MODE_TEST)
	// {
	// 	DEBUG("Start Test Mode\r\n");
	// 	App_Test_Mode_Init();
	// }
	// else if (g_test_mode == MODE_STANDARD)
	// {
	// 	DEBUG("Start MPU9250 Init\r\n");
	// 	MPU9250_Init(MODE_STANDARD);
	// }
	// else // =MODE_TEMP
	// {
	// 	OLED_Show_Waiting();
	// 	MPU9250_Init(MODE_TEMP);
	// 	Flash_Set_TestMode(MODE_STANDARD);
	// 	delay_ms(100);
	// 	Bsp_Reset_MCU();
	// }

	RGB_Init();

	MOTOR_GPIO_Init();
	Motor_PWM_Init(MOTOR_MAX_PULSE, MOTOR_FREQ_DIVIDE);

	Encoder_Init();
	// CAN_Config_Init(CAN_BAUD_1000Kbps);

	#if ENABLE_TRACING
	Tracing_Init();
	#else
	// TIM7_Init();
	// PwmServo_Init();
    App_Enable_PWM_Servo();
	#endif

#if ENABLE_IWDG
	IWDG_Init();
#endif
}

// 重启单片机
void Bsp_Reset_MCU(void)
{
	printf("\r\nReset MCU\r\n");
	__set_FAULTMASK(1);
    NVIC_SystemReset();
}
