#include "bsp_usart.h"
#include "bsp.h"
#include "app.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "app_oled.h"
#include "app_motion.h"
#include "app_uart_servo.h"
#include "app_pid.h"
#include "app_bat.h"
#include "app_sbus.h"
#include "app_mecanum.h"
#include "app_rgb.h"
#include "app_flash.h"

#include "icm20948.h"
#include "app_math.h"


// 一个数表示十毫秒
uint16_t g_update = 0;
uint16_t g_oled_count = 0;
uint8_t g_oled_flag = OLED_MAX_FLAG;

/* 自动上传标志 */
uint16_t g_Auto_Report = ENABLE_AUTO_REPORT;

uint8_t g_tracing_state = 0;
uint8_t g_enable_pwm_servo = 0;

// 延迟XX毫秒，调用此函数前需要先启动FreeRTOS
void App_Delay_ms(uint16_t ms)
{
	vTaskDelay(ms / portTICK_RATE_MS);
}


// 设置自动上报数据
void Set_Auto_Report(uint16_t enable)
{
	if (enable == 0)
	{
		g_Auto_Report = 0;
		return;
	}
	g_Auto_Report = 1;
}

// 设置OLED显示标识，flag=1显示陀螺仪数据，flag=2显示电压数据。
void App_Set_OLED_Flag(uint8_t flag)
{
	g_oled_flag = flag;
}

void App_Clear_Yaw(void)
{
	
}

// 测试模式初始化
void App_Test_Mode_Init(void)
{
	MPU9250_Init(1);
	CAN_RX_Beep(0);
	Motion_Stop(1);
	UartServo_Ctrl(0xFE, 2000, 100);
	OLED_Show_Test_Mode();
	Flash_Set_TestMode(MODE_STANDARD);
}


// 初始化APP内容
void App_Init(void)
{
	PID_Param_Init();
	app_rgb_init();
	Flash_Init();
	if (Bsp_Get_TestMode() != MODE_TEST)
	{
		OLED_Show_CarType(Motion_Get_Car_Type(), VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	}

	#if PID_ASSISTANT_EN
	protocol_init();
	int32_t target_location = 500;
	int32_t cycle = 10;
	set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
	set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &target_location, 1);     // 给通道 1 发送目标值
	set_computer_value(SEND_PERIOD_CMD, CURVES_CH1, &cycle, 1);
	USART1_Send_ArrayU8("\n", 1);
	#endif

	
	#if ENABLE_INV_MEMS
	int result = ICM_20948_Init();
	if (result)
	{
		printf("ICM_INIT ERROR:%d\n", result);
		while(result)
		{
			;
		}
	}
	else
	{
		DEBUG("ICM_INIT OK\n");
	}
	#endif

	#if ENABLE_MPU9250
	MPU_Init_Ok();
	#endif
}


// 逻辑处理, 每10MS调用一次
void App_Loop(void)
{
	Beep_Timeout_Close_Handle();
	app_rgb_effects_handle();

	// static int count1 = 0;
	// count1++;
	// if (count1 > 10)
	// {
	// 	count1 = 0;
	// 	int value1 = READ_X1();
	// 	int value2 = READ_X2();
	// 	int value3 = READ_X3();
	// 	int value4 = READ_X4();
	// 	printf("Value:%d, %d, %d, %d\n", value1, value2, value3, value4);
	// }
	
}

// 发送当前固件版本号
void App_Send_Version(void)
{
	Bsp_Send_Version();
}

/* 10ms测速线程 */
void vTask_Speed(void *pvParameters)
{
	#if ENABLE_USART2 & !ENABLE_SBUS
	App_Delay_ms(2000);
	USART2_Connect_BT();
	#endif
	uint32_t lastWakeTime = xTaskGetTickCount();
	while (1)
	{
		// App_Delay_ms(10);
		vTaskDelayUntil(&lastWakeTime, 10);
		Encoder_Update_Count();
		Motion_Handle();
	}
}

/* APP处理事件 */
void vTask_App_Handle(void *pvParameters)
{
	BEEP_ON();
	App_Delay_ms(100);
	BEEP_OFF();

	while (1)
	{
		App_Delay_ms(10);
		#if ENABLE_IWDG
		IWDG_Feed();
		#endif
		// 如果电量不足，直接跳过下面APP_Loop
		if (Bat_Show_LED_Handle()) continue;
		App_Loop();
	}
}

/* 解析上位机发送过来的命令, 并处理控制的内容 */
void vTask_Control(void *pvParameters)
{
	while (System_Enable())
	{
		if (Get_CMD_Flag())
		{
			Upper_Data_Parse(Get_RxBuffer(), Get_CMD_Length());
			Clear_CMD_Flag();
		}
		SBUS_Handle();
		App_Delay_ms(1);
	}
	while (1)
	{
		App_Delay_ms(1000);
	}
}

// 自动发送数据线程
void vTask_Auto_Report(void *pvParameters)
{
	App_Delay_ms(AUTO_SEND_TIMEOUT);
	uint16_t report_count = 0;
	while (System_Enable())
	{
		if (Get_Request_Flag() > 0)
		{
			report_count = 0;
			Send_Request_Data();
		}
		if (g_Auto_Report)
		{
			if (report_count == 1)
			{
				Motion_Send_Data();
			}
			else if (report_count == 11)
			{
				#if ENABLE_INV_MEMS
				ICM20948_Send_Raw_Data();
				#endif

				#if ENABLE_MPU9250
				MPU9250_Send_Raw_Data();
				#endif
			}
			else if (report_count == 21)
			{
				#if ENABLE_INV_MEMS
				ICM20948_Send_Attitude_Data();
				#endif
			}
			else if (report_count == 31)
			{
				Encoder_Send_Count_Now();
			}
		}
		report_count++;
		if (report_count > AUTO_SEND_TIMEOUT) report_count = 0;
		App_Delay_ms(1);
		
		#if PID_ASSISTANT_EN
		receiving_process();
		#endif
	}
	
	while (1)
	{
		App_Delay_ms(1000);
	}
}

/* OLED显示任务 */
void vTask_OLED(void *pvParameters)
{
	while (System_Enable())
	{
		g_oled_count++;
		switch (g_oled_flag)
		{
		case OLED_FLAG_NO_DISPLAY:
		{
			OLED_Clear();
			OLED_Refresh();
			g_oled_flag = OLED_MAX_FLAG;
			break;
		}
		case OLED_FLAG_IMU:
		{
			if (g_oled_count >= 5)
			{
				g_oled_count = 0;
				#if ENABLE_INV_MEMS
				static float yaw = 0, roll = 0, pitch = 0;
				yaw = ICM20948_Get_Yaw_Now()*RtA;
				roll = ICM20948_Get_Roll_Now()*RtA;
				pitch = ICM20948_Get_Pitch_Now()*RtA;
				OLED_Show_IMU_Attitude(yaw, roll, pitch);
				#endif
				#if ENABLE_DEBUG_ICM_ATT
				printf("YAW:%.2f, ROLL:%.2f, Pitch:%.2f\n", yaw, roll, pitch);
				#endif
				#if ENABLE_MPU9250
				if (Bsp_Get_TestMode() == MODE_TEST)
				{
					OLED_Show_YAW(MPU_Get_Yaw_Now()*RtA);
				}
				else
				{
					OLED_Show_Test_Mode_Error();
				}
				#endif
			}
			break;
		}
		case OLED_FLAG_VOLTAGE:
		{
			if (Bsp_Get_TestMode() == MODE_TEST)
			{
				if (g_oled_count >= 100)
				{
					g_oled_count = 0;
					OLED_Show_Voltage(Bat_Voltage_Z10());
				}
			}
			else
			{
				OLED_Show_Test_Mode_Error();
			}
			break;
		}
		case OLED_FLAG_MOTOR_SPEED:
		{
			if (Bsp_Get_TestMode() == MODE_TEST)
			{
				float speed_f[4] = {0};
				Motion_Get_Motor_Speed(speed_f);
				OLED_Show_Motor_Speed(-speed_f[0], -speed_f[1], speed_f[2], speed_f[3]);
			}
			else
			{
				OLED_Show_Test_Mode_Error();
			}
			break;
		}
		case OLED_FLAG_UART_SERVO:
		{
			if (Bsp_Get_TestMode() == MODE_TEST)
			{
				OLED_Show_UART_Servo_Read(UartServo_Test_Read_State());
			}
			else
			{
				OLED_Show_Test_Mode_Error();
			}
			break;
		}
		case OLED_MAX_FLAG:
		{
			g_oled_count = 100;

			break;
		}
		default:
		{
			g_oled_flag = OLED_FLAG_NO_DISPLAY;
			break;
		}
		}
		App_Delay_ms(10);
	}


	while (1)
	{
		App_Delay_ms(1000);
	}
}

// 按键处理事件
void vTask_Key(void *pvParameters)
{
	#if ENABLE_KEY_RELEASE
	while (System_Enable())
	{
		if (Key1_State(KEY_MODE_ONE_TIME))
		{
			Beep_On_Time(50);
			DEBUG("KEY1 PRESS\n");
			static uint8_t effect1 = 5;
			RGB_Clear();
			RGB_Update();
			effect1 = (effect1 + 1) % RGB_MAX_EFFECT;
			app_rgb_set_effect(effect1, 5);
			CAN_Test_Send();

		}
		
		if (Key1_Long_Press(10)) // 长按10秒清空Flash
		{
			Beep_On_Time(1000);
			Motion_Stop(STOP_BRAKE);
			app_rgb_set_effect(0, 5);
			RGB_Clear();
			RGB_Update();

			Flash_Reset_All_Value();
			App_Delay_ms(500);
			Bsp_Reset_MCU();
		}
		App_Delay_ms(10);
	}
	#else
	static int key_state = 1;
	while (System_Enable())
	{
		if (Key1_State(KEY_MODE_ONE_TIME))
		{
			Beep_On_Time(50);
			DEBUG("KEY1 PRESS\n");

			if (key_state)
			{
				key_state = 0;
				uint16_t speed = 300;
				// Motion_Ctrl(0, 0, speed, 0);
				Motion_Ctrl(speed, 0, 0, 0);
				// Motion_Set_Pwm(speed, speed, speed, speed);

				g_tracing_state = 1;
			}
			else
			{
				key_state = 1;
				g_tracing_state = 0;
				Motion_Stop(1);
			}


			static u8 effect1 = 0;
			RGB_Clear();
			RGB_Update();
			effect1 = (effect1 + 1) % 2;
			app_rgb_set_effect(effect1, 5);
		}
		
		App_Delay_ms(10);
	}
	#endif
	while (1)
	{
		App_Delay_ms(1000);
	}
}

// 处理姿态传感器数据
void vTask_MPU(void *pvParameters)
{
	uint32_t lastWakeTime = xTaskGetTickCount();
	while (System_Enable())
	{
		#if ENABLE_INV_MEMS
		ICM20948_Read_Data_Handle();
		#endif
		#if ENABLE_MPU9250
		MPU9250_Read_Data_Handle();
		#endif
		// App_Delay_ms(10);
		vTaskDelayUntil(&lastWakeTime, 10);
	}
	while (1)
	{
		App_Delay_ms(1000);
	}
	
}


void App_Enable_PWM_Servo(void)
{
    if (g_enable_pwm_servo == 0)
    {
        g_enable_pwm_servo = 1;
	    PwmServo_Init();
        TIM7_Init();
    }
}

// 四路寻迹模块功能任务
void vTask_Tracing(void *pvParameters)
{
	while (System_Enable() && g_enable_pwm_servo == 0)
	{
		if (g_tracing_state)
		{
			Tracing_line_handle();
		}
		else
		{
			App_Delay_ms(100);
		}
	}
	while (1)
	{
		App_Delay_ms(1000);
	}
}


/* 启动FreeRTOS的任务列表，不再执行此函数后的内容 */
void App_Start_FreeRTOS(void)
{
	xTaskCreate(vTask_Speed, "Task Speed", 512, NULL, 10, NULL);
	printf("start vTask_Speed\n");
	xTaskCreate(vTask_Control, "Task Control", 128, NULL, 9, NULL);
	printf("start vTask_Control\n");
	xTaskCreate(vTask_Key, "Task KEY", 128, NULL, 8, NULL);
	printf("start vTask_Key\n");
	// xTaskCreate(vTask_Auto_Report, "Task Report", 512, NULL, 7, NULL);
	// printf("start vTask_Auto_Report\n");
	xTaskCreate(vTask_App_Handle, "Task App_Handle", 128, NULL, 4, NULL);
	printf("start vTask_App_Handle\n");
	// xTaskCreate(vTask_MPU, "Task MPU", 1024, NULL, 3, NULL);
	// printf("start vTask_MPU\n");
	#if ENABLE_OLED
	xTaskCreate(vTask_OLED, "Task OLED", 512, NULL, 1, NULL);
	printf("start vTask_OLED\n");
	#endif

	#if ENABLE_TRACING
	xTaskCreate(vTask_Tracing, "Task Tracing", 128, NULL, 5, NULL);
	printf("start vTask_Tracing\n");
	#endif
	
	printf("start vTaskStartScheduler\n");
	vTaskStartScheduler();
}
