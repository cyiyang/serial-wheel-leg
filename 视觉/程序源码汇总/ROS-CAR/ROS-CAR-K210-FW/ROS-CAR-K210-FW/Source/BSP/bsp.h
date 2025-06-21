#ifndef __BSP_H__
#define __BSP_H__

#include "stdio.h"
#include "stdint.h"

#include "stm32f10x.h"
#include "bsp_common.h"

#include "yb_debug.h"
#include "bsp_beep.h"
#include "bsp_timer.h"
#include "bsp_wdg.h"
#include "bsp_usart.h"
#include "bsp_io_i2c.h"
#include "bsp_motor.h"
#include "bsp_key.h"
#include "bsp_encoder.h"
#include "bsp_pwmServo.h"
#include "bsp_ssd1306.h"
#include "bsp_spi.h"
#include "bsp_adc.h"
#include "bsp_can.h"
#include "bsp_rgb.h"
#include "bsp_mpu9250.h"
#include "bsp_tracing.h"



#define VERSION_MAJOR          0x02
#define VERSION_MINOR          0x04
#define VERSION_PATCH          0x02


//JTAG模式设置定义
#define JTAG_SWD_DISABLE       0X02
#define SWD_ENABLE             0X01
#define JTAG_SWD_ENABLE        0X00


// 模式定义
#define MODE_STANDARD          (0)
#define MODE_TEST              (1)
#define MODE_TEMP              (2)


void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);


void Bsp_Init(void);

void Bsp_JTAG_Set(uint8_t mode);

void Bsp_Send_Version(void);

void Bsp_Led_Show_State(void);
void Bsp_Led_Show_Low_Battery(void);

void Bsp_Set_TestMode(uint16_t mode);
uint8_t Bsp_Get_TestMode(void);


void Bsp_Reset_MCU(void);

#endif
