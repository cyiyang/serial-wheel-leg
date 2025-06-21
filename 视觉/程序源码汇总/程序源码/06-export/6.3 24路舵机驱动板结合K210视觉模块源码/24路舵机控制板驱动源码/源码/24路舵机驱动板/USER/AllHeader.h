#ifndef __AllHeader_H
#define __AllHeader_H


#define DEBUG_USARTx USART1  //打印信息串口

#define bool _Bool
#define true 1
#define false 0

#include <stdio.h>
#include "string.h"
#include "stm32f10x.h"
#include "bsp.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "bsp_gpio.h"
#include "bsp_servo.h"
#include "bsp_ps2.h"
#include "delay.h"
#include "bsp_timer.h"
#include "bsp_beep.h"
#include "bsp_adc.h"
#include "bsp_juice_adc.h"
#include "bsp_usart.h"
#include "bsp_common.h"
#include "oled_iic.h"
#include "oled_fonts.h"
#include "bsp_oled.h"
#include "bsp_key.h"


#define GROUP_NUM 3 //24路舵机很3组，一组8个
#define DUOJI_NUM 8  //定义每组的舵机数
extern u8 grop ;//按键的组别数，主要应用在手柄
extern float version;//版本号


/*定义需要初始化的舵机宏定义开关*/
//1-8（第一组）
#define USE_SERVO_J1
#define USE_SERVO_J2
#define USE_SERVO_J3
#define USE_SERVO_J4
#define USE_SERVO_J5
#define USE_SERVO_J6
#define USE_SERVO_J7
#define USE_SERVO_J8

//9-16（第二组）
#define USE_SERVO_J9
#define USE_SERVO_J10
#define USE_SERVO_J11
#define USE_SERVO_J12
#define USE_SERVO_J13
#define USE_SERVO_J14
#define USE_SERVO_J15
#define USE_SERVO_J16

//16-24（第三组）
#define USE_SERVO_J17
#define USE_SERVO_J18
#define USE_SERVO_J19
#define USE_SERVO_J20
#define USE_SERVO_J21
#define USE_SERVO_J22
#define USE_SERVO_J23
#define USE_SERVO_J24

#define PTO_MAX_BUF_LEN           (20)
#define PTO_HEAD                  (0xFF)
#define PTO_DEVICE_ID             (0xFC)
#define FUNC_PWM_SERVO_ALL        (0x04)



/*ps2 手柄的定义开关 1：打开 0：关闭*/
#define PS_TWO 0

/*蜂鸣器的定义开关 1：打开 0：关闭*/
#define BEEP_SW 1

/*电压电流检测定义开关 1：打开 0：关闭*/
#define Battery_SW 1

/*oled的定义开关 1：打开 0：关闭*/
#define OLED_SW 1




//定时器输出pwm，实现角度控制
extern int Angle_J[GROUP_NUM][DUOJI_NUM];

void void_jutce(void);

void dectect_beep(void);
void test_ps2(void);
void user_douji(void);
void deal_bluetooth(u8 Rx_temp);
uint8_t* Get_RxBuffer(void);
uint8_t Get_CMD_Length(void);
uint8_t Get_CMD_Flag(void);
void Clear_CMD_Flag(void);
void Clear_RxBuffer(void);
void Upper_Data_Receive(uint8_t Rx_Temp);
void PwmServo_Set_Angle_All(uint8_t angle_s1, uint8_t angle_s2, uint8_t angle_s3, uint8_t angle_s4);
void deal_K210(uint8_t *data_buf, uint8_t num);
u8 deal_data(void);
#endif


