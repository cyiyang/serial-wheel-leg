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
	float v_set;//�����ٶȣ���λ��m/s
	float x_set;//����λ�ã���λ��m
  float v;//ʵ�ʵ��ٶ�,��λ��m/s
	float v_act;
	float x;//ʵ�ʵ�λ�ƣ���λ��m
	
	float phi_set;
  float d_phi_set;
		
	float turn_set;//����yaw�ỡ��
	float roll_set;	//����roll�ỡ��
	
	float leg_set;//�����ȳ�����λ��m
	float last_leg_set;
	
	float v_filter;//�˲���ĳ����ٶȣ���λ��m/s
	float x_filter;//�˲���ĳ���λ�ã���λ��m
	
	float myPithR;
	float myPithGyroR;
	float myRoll;

	float total_yaw;
	
	uint8_t start_flag;//������־
	uint8_t front_flag;
	uint8_t last_front_flag;
	uint8_t turn_flag;
	uint8_t last_turn_flag;
	uint8_t prejump_flag;
    uint8_t last_prejump_flag; //������һ�������أ�ֻ�е�һ�δ�0��1ʱ����������1����1��0ʱ����
	uint8_t jump_flag;//��Ծ��־
	uint8_t recover_flag;//һ������µĵ��������־
	uint8_t leg_flag;
	uint8_t autoleg_flag;
	uint8_t autoturn_flag;
	uint8_t fastturn_flag;
	uint8_t movejump_flag;
    
    uint8_t openmv_flag; //ѭ��ģʽ
}chassis_t;

typedef struct
{
	/*�������ȵĹ����������̶�����*/	
	float right_l1;
	float left_l1;
	
	float left_T1;
	float right_T1;
	
	float left_F0;
	float right_F0;
	float roll_F0;
	
    float left_len;
	float right_len;
	
	//�ȳ��仯��
	float left_len_dot;
	float right_len_dot;
} vmc_leg_t;


extern void ChassisR_init(chassis_t *chassis);
extern void ChassisR_task(void);
extern void mySaturate(float *in,float min,float max);
extern void chassisR_feedback_update(chassis_t *chassis,INS_t *ins,vmc_leg_t *vmc_leg);


#endif




