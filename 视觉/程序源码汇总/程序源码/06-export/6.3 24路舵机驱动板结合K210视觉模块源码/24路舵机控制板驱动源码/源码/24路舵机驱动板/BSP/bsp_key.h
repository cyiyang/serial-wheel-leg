#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "AllHeader.h"

//  引脚定义
#define KEY1_GPIO_PORT GPIOB
#define KEY1_GPIO_PIN  GPIO_Pin_12
#define KEY1_GPIO_CLK  RCC_APB2Periph_GPIOB


// 按键状态，与实际电平相反。
#define KEY1_PRESS      1
#define KEY2_PRESS      2
#define KEY_RELEASE     0

#define KEY_MODE_ONE_TIME   1
#define KEY_MODE_ALWAYS     0


uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Key_GPIO_Init(void);
uint8_t Key1_State(uint8_t mode);
uint8_t Key1_Long_Press(uint16_t timeout);

//Test
uint8_t key_test(void);



#endif

