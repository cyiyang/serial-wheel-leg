#ifndef __CHASSISR_TASK_H
#define __CHASSISR_TASK_H

#include "main.h"
#include "dm4310_drv.h"
#include "pid.h"

#include "INS_task.h"
#include "bsp_vofa.h"

typedef struct
{
    Wheel_Motor_t wheel_motor[2];
	Joint_Motor_t joint_motor[2];
	
	float target_v;
	float v_set;//期望速度，单位是m/s
	float x_set;//期望位置，单位是m
  float v;//实际的速度,单位是m/s
	float v_act;
	float x;//实际的位移，单位是m
	
	float phi_set;
  float d_phi_set;
		
	float turn_set;//期望yaw轴弧度
	float roll_set;	//期望roll轴弧度
	
	float leg_set;//期望腿长，单位是m
	float last_leg_set;
	
	float v_filter;//滤波后的车体速度，单位是m/s
	float x_filter;//滤波后的车体位置，单位是m
	
	float myPithR;
	float myPithGyroR;
	float myRoll;

	float total_yaw;
	
	uint8_t start_flag;//启动标志
	uint8_t front_flag;
	uint8_t last_front_flag;
	uint8_t turn_flag;
	uint8_t last_turn_flag;
	uint8_t prejump_flag;
    uint8_t last_prejump_flag; //类似于一个上升沿，只有第一次从0到1时开启，持续1或者1到0时不跳
	uint8_t jump_flag;//跳跃标志
	uint8_t recover_flag;//一种情况下的倒地自起标志
	uint8_t leg_flag;
	uint8_t autoleg_flag;
	uint8_t autoturn_flag;
	uint8_t fastturn_flag;
	uint8_t movejump_flag;
    
    uint8_t openmv_flag; //循迹模式
}chassis_t;

typedef struct
{
	/*左右两腿的公共参数，固定不变*/	
	float right_l1;
	float left_l1;
	
	float left_T1;
	float right_T1;
	
	float left_F0;
	float right_F0;
	float roll_F0;
	
    float left_len;
	float right_len;
	
	//腿长变化率
	float left_len_dot;
	float right_len_dot;
} vmc_leg_t;


extern void ChassisR_init(chassis_t *chassis);
extern void ChassisR_task(void);
extern void mySaturate(float *in,float min,float max);
extern void chassisR_feedback_update(chassis_t *chassis,INS_t *ins,vmc_leg_t *vmc_leg);


#endif




