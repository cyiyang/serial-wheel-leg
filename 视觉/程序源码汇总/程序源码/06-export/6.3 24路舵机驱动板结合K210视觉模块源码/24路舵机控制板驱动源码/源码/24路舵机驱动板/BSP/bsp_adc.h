#ifndef __BSP_ADC_H 
#define __BSP_ADC_H

#include "AllHeader.h"


void Adc_vol_init(void);
u8 Bsp_Bat_State(void);
u8 juice_check(void);
u8 Bsp_System_Enable(void);
int Bsp_Bat_Voltage_Z100(void);

#endif
