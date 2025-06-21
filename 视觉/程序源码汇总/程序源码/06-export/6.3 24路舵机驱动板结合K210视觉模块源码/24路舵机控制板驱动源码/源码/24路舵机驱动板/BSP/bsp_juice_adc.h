#ifndef __BSP_JUICE_ADC_H 
#define __BSP_JUICE_ADC_H

#include "AllHeader.h"

void J_Adc_init(void);
void J_Adc_init(void);
float J_Adc_Get_Battery_Volotage(void);
u8 J_Bsp_Bat_State(void);
int J_Bsp_Bat_Voltage_Z100(void);
u8 J_Bsp_System_Enable(void);


#endif
