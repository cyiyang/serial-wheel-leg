#ifndef __DM4310_DRV_H__
#define __DM4310_DRV_H__
#include "main.h"
#include "fdcan.h"
#include "can_bsp.h"

#define MIT_MODE 			0x000
#define POS_MODE			0x100
#define SPEED_MODE		0x200

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -280.0f
#define V_MAX 280.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -1.0f
#define T_MAX 1.0f

//轮毂3510
#define P_MIN2 -12.5f
#define P_MAX2 12.5f
#define V_MIN2 -280.0f
#define V_MAX2 280.0f
#define KP_MIN2 0.0f
#define KP_MAX2 500.0f
#define KD_MIN2 0.0f
#define KD_MAX2 5.0f
#define T_MIN2 -1.0f
#define T_MAX2 1.0f

//小关节3507
#define P_MIN3 -12.5f
#define P_MAX3 12.5f
#define V_MIN3 -50.0f
#define V_MAX3 50.0f
#define KP_MIN3 0.0f
#define KP_MAX3 500.0f
#define KD_MIN3 0.0f
#define KD_MAX3 5.0f
#define T_MIN3 -10.0f
#define T_MAX3 10.0f

typedef struct 
{
	uint16_t id;
	uint16_t state;
	int p_int;
	int v_int;
	int t_int;
	int kp_int;
	int kd_int;
	float pos;
	float vel;
	float tor;
	float Kp;
	float Kd;
	float Tmos;
	float Tcoil;
}motor_fbpara_t;


typedef struct
{
	uint16_t mode;
	motor_fbpara_t para;
}Joint_Motor_t ;

typedef struct
{
	uint16_t mode;
	float wheel_T;//轮毂电机的输出扭矩，单位为N
	
	motor_fbpara_t para;	
}Wheel_Motor_t ;


extern void dm4310_fbdata(Joint_Motor_t *motor, uint8_t *rx_data,uint32_t data_len);
extern void dm3510_fbdata(Wheel_Motor_t *motor, uint8_t *rx_data,uint32_t data_len);
extern void dm3507_fbdata(Joint_Motor_t *motor, uint8_t *rx_data,uint32_t data_len);

extern void enable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);
extern void disable_motor_mode(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);

//4310关节电机
extern void mit_ctrl(hcan_t* hcan, uint16_t motor_id, float pos, float vel,float kp, float kd, float torq);
extern void pos_speed_ctrl(hcan_t* hcan,uint16_t motor_id, float pos, float vel);
extern void speed_ctrl(hcan_t* hcan,uint16_t motor_id, float _vel);

//3510电机
extern void mit_ctrl2(hcan_t* hcan, uint16_t motor_id, float pos, float vel,float kp, float kd, float torq);
//3507关节电机
extern void mit_ctrl3(hcan_t* hcan, uint16_t motor_id, float pos, float vel,float kp, float kd, float torq);

extern void joint_motor_init(Joint_Motor_t *motor,uint16_t id,uint16_t mode);
extern void wheel_motor_init(Wheel_Motor_t *motor,uint16_t id,uint16_t mode);
	
extern float Hex_To_Float(uint32_t *Byte,int num);//十六进制到浮点数
extern uint32_t FloatTohex(float HEX);//浮点数到十六进制转换

extern float uint_to_float(int x_int, float x_min, float x_max, int bits);
extern int float_to_uint(float x_float, float x_min, float x_max, int bits);


extern void save_motor_zero(hcan_t* hcan, uint16_t motor_id, uint16_t mode_id);

#endif /* __DM4310_DRV_H__ */



