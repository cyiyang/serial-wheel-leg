/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         bsp_servo.h	
* @author       
* @version      V1.0
* @date         
* @brief        24个舵机控制驱动头文件
* @details      
* @par History  见如下说明
*                 
* 
*/

#ifndef __BSP_SERVO_H__
#define __BSP_SERVO_H__

#include "AllHeader.h"


#define Servo_J1_PIN		GPIO_Pin_0
#define Servo_J2_PIN		GPIO_Pin_1
#define Servo_J3_PIN		GPIO_Pin_2
#define Servo_J4_PIN		GPIO_Pin_3


#define Servo_J5_PIN		GPIO_Pin_4
#define Servo_J6_PIN		GPIO_Pin_5
#define Servo_J7_PIN		GPIO_Pin_0
#define Servo_J8_PIN		GPIO_Pin_1

#define Servo_J9_PIN		GPIO_Pin_15
#define Servo_J10_PIN		GPIO_Pin_3
#define Servo_J11_PIN		GPIO_Pin_10
#define Servo_J12_PIN		GPIO_Pin_11

#define Servo_J13_PIN		GPIO_Pin_6
#define Servo_J14_PIN		GPIO_Pin_7
#define Servo_J15_PIN		GPIO_Pin_8
#define Servo_J16_PIN		GPIO_Pin_9

#define Servo_J17_PIN		GPIO_Pin_6
#define Servo_J18_PIN		GPIO_Pin_7
#define Servo_J19_PIN		GPIO_Pin_8
#define Servo_J20_PIN		GPIO_Pin_9

#define Servo_J21_PIN		GPIO_Pin_8
#define Servo_J22_PIN		GPIO_Pin_14
#define Servo_J23_PIN		GPIO_Pin_15
#define Servo_J24_PIN		GPIO_Pin_11

#define Servo_J1_PORT		GPIOA
#define Servo_J2_PORT		GPIOA
#define Servo_J3_PORT		GPIOA
#define Servo_J4_PORT		GPIOA

#define Servo_J5_PORT		GPIOB
#define Servo_J6_PORT		GPIOB
#define Servo_J7_PORT		GPIOB
#define Servo_J8_PORT		GPIOB

#define Servo_J9_PORT		GPIOA
#define Servo_J10_PORT	GPIOB
#define Servo_J11_PORT	GPIOB
#define Servo_J12_PORT	GPIOB

#define Servo_J13_PORT	GPIOB
#define Servo_J14_PORT	GPIOB
#define Servo_J15_PORT	GPIOB
#define Servo_J16_PORT	GPIOB

#define Servo_J17_PORT	GPIOC
#define Servo_J18_PORT	GPIOC
#define Servo_J19_PORT	GPIOC
#define Servo_J20_PORT	GPIOC

#define Servo_J21_PORT	GPIOA
#define Servo_J22_PORT	GPIOB
#define Servo_J23_PORT	GPIOB
#define Servo_J24_PORT	GPIOA

#define Servo_J1_RCC		RCC_APB2Periph_GPIOA
#define Servo_J2_RCC		RCC_APB2Periph_GPIOA
#define Servo_J3_RCC  	RCC_APB2Periph_GPIOA
#define Servo_J4_RCC		RCC_APB2Periph_GPIOA

#define Servo_J5_RCC		RCC_APB2Periph_GPIOB
#define Servo_J6_RCC		RCC_APB2Periph_GPIOB
#define Servo_J7_RCC		RCC_APB2Periph_GPIOB
#define Servo_J8_RCC		RCC_APB2Periph_GPIOB

#define Servo_J9_RCC		RCC_APB2Periph_GPIOA
#define Servo_J10_RCC		RCC_APB2Periph_GPIOB
#define Servo_J11_RCC  	RCC_APB2Periph_GPIOB
#define Servo_J12_RCC		RCC_APB2Periph_GPIOB

#define Servo_J13_RCC		RCC_APB2Periph_GPIOB
#define Servo_J14_RCC		RCC_APB2Periph_GPIOB
#define Servo_J15_RCC		RCC_APB2Periph_GPIOB
#define Servo_J16_RCC		RCC_APB2Periph_GPIOB

#define Servo_J17_RCC		RCC_APB2Periph_GPIOC
#define Servo_J18_RCC		RCC_APB2Periph_GPIOC
#define Servo_J19_RCC  	RCC_APB2Periph_GPIOC
#define Servo_J20_RCC		RCC_APB2Periph_GPIOC

#define Servo_J21_RCC		RCC_APB2Periph_GPIOA
#define Servo_J22_RCC		RCC_APB2Periph_GPIOB
#define Servo_J23_RCC		RCC_APB2Periph_GPIOB
#define Servo_J24_RCC		RCC_APB2Periph_GPIOA



void Servo_J1(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J2(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J3(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J4(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J5(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J6(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J7(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
void Servo_J8(int v_iAngle);/*定义一个脉冲函数，用来模拟方式产生PWM值*/
//9-24路舵机自行增加即可，本工程不采用以上的方法，而是使用定时器的方式进行。

void front_detection(void);
void left_detection(void);
void right_detection(void);



#endif

