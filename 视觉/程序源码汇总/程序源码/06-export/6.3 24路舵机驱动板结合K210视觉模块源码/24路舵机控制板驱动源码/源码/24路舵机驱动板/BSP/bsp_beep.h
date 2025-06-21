#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "AllHeader.h"

#define BEEP_RCC   RCC_APB2Periph_GPIOA
#define BEEP_PORT  GPIOA
#define BEEP_PIN   GPIO_Pin_12

#define LED_RCC   RCC_APB2Periph_GPIOC
#define LED_PORT  GPIOC
#define LED_PIN   GPIO_Pin_13

#define BEEP_ON  GPIO_SetBits(BEEP_PORT,BEEP_PIN)
#define BEEP_OFF  GPIO_ResetBits(BEEP_PORT,BEEP_PIN)


#define LED(x) (x>0)?GPIO_SetBits(LED_PORT,LED_PIN):GPIO_ResetBits(LED_PORT,LED_PIN);

void init_beep(void);
void beep_timer(void);
void set_beep(u8 times);
void beep_start_stop(void);


void init_led(void);
void led_timer(void);
void led_control(void);

#endif
