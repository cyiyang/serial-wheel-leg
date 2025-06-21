#ifndef __APP_BAT_H__
#define __APP_BAT_H__

#include "stm32f10x.h"

uint8_t System_Enable(void);
uint8_t Bat_State(void);
int Bat_Voltage_Z10(void);
uint8_t Bat_Show_LED_Handle(void);

#endif /* __APP_BAT_H__ */
