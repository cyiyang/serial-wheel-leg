#include "app_bat.h"
#include "app.h"
#include "app_oled.h"
#include "app_motion.h"

#include "bsp.h"

// 电池低电压检测计数阈值，乘100毫秒就是延迟时间，单位为毫秒。
// 例如：100*10=1000，即1秒。
#define BAT_CHECK_COUNT        20

uint8_t g_bat_state = 1;          // 电池低电压状态。检测到低电压后为0。只能通过复位恢复1
int Voltage_Z10 = 0;        // 电池电压值
int Voltage_Low_Count = 0;   // 低电压计数


// 查询电池电压状态，连续几秒读到低于9.6V返回0，高于9.6V返回1
uint8_t Bat_State(void)
{
	if (g_bat_state)
	{
		Voltage_Z10 = (int) (Adc_Get_Battery_Volotage() * 10);
		#if ENABLE_LOW_BATTERY_ALARM
		if (Voltage_Z10 < 96)
		{
			Voltage_Low_Count++;
			if(Voltage_Low_Count > BAT_CHECK_COUNT)
			{
				g_bat_state = 0;
			}
		}
		else
		{
			Voltage_Low_Count = 0;
		}
		#endif
	}
    // DEBUG("BAT:%d, %d", g_bat_state, Voltage_Z10);
	return g_bat_state;
}

int Bat_Voltage_Z10(void)
{
	return Voltage_Z10;
}


// 返回系统是否进入供电正常，正常返回1，不正常返回0
uint8_t System_Enable(void)
{
	return g_bat_state;
}

// 10毫秒调用一次，根据电池显示LED状态，返回0则正常，返回1则电量过低。
uint8_t Bat_Show_LED_Handle(void)
{
	static uint16_t bat_led_state = 0;
	static uint8_t result = 0;
	bat_led_state++;
	if (bat_led_state >= 10)
	{
		bat_led_state = 0;
		// Bat_State();
		// if (!System_Enable())
		// {
		// 	Bsp_Led_Show_Low_Battery();
		// 	static uint8_t alarm = 1;
		// 	if (alarm)
		// 	{
		// 		alarm = 0;
		// 		OLED_Draw_Line("Battery Low", 2, true, true);
		// 	}
		// 	Motion_Stop(STOP_BRAKE);
		// 	result = 1;
		// }
		// else
		// {
		// 	Bsp_Led_Show_State();
		// }
		Bsp_Led_Show_State();
	}
	return result;
}

