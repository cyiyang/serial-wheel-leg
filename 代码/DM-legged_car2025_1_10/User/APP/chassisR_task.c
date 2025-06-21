/**
  *********************************************************************
  * @file      chassisR_task.c/h
  * @brief     该任务控制底盘两个的电机DM3510和腿部两个电机DM3507
  * @note       
  * @history
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *********************************************************************
  */
	
#include "chassisR_task.h"
#include "fdcan.h"
#include "cmsis_os.h"
#include <math.h>

extern INS_t INS;
																
chassis_t chassis_move;
vmc_leg_t vmc;

uint32_t CHASSR_TIME=1;	

float Turn_Kp=1.0f;//0.3f;
float Turn_Kd=-0.05f;//0.04f;
//转向环
float Turn(float Angle,float Gyro)
{  
	 float tor3;

	 if(chassis_move.turn_flag==1)
	 {
		 tor3=(chassis_move.turn_set-Gyro)*Turn_Kd;
	 }
	 else if(chassis_move.turn_flag==0)
	 {
	   tor3=Turn_Kp*(chassis_move.turn_set-Angle)-Gyro*Turn_Kd; //PD转向环
	 } 
	 return tor3;
}

float turn_T=0.0f;

float leg_kp= 400.0f;
float leg_kd= -40.0f;	 

float mg=3.0f;


float roll_kp=-100.0f;
float roll_kd=1.0f;

int jump_time;
int finish_time;
//float Poly_Coefficient[8][4]={
//{3.515e-12, -7.2537e-13, 4.388e-14, -0.010351},
//{26.7451, -7.9462, 0.83722, -0.087871},
//{-65.1454, 23.0838, -3.4635, -0.6282},
//{4.0412, -1.3881, 0.028802, -0.06891},
//{3.4115e-12, -7.2121e-13, 4.481e-14, -0.010351},
//{26.7451, -7.9462, 0.83722, -0.087871},
//{-65.1454, 23.0838, -3.4635, -0.6282},
//{4.0412, -1.3881, 0.028802, -0.06891}
//};

float Poly_Coefficient[8][4] = 
{
{-1.0876e-12, 2.0279e-13, -1.1429e-14, -0.014639},
{30.5314, -9.0685, 0.95411, -0.099303},
{-84.4462, 29.5798, -4.3324, -0.63839},
{4.1482, -1.413, 0.01555, -0.068828},
{-2.2255e-13, 1.9945e-14, -2.241e-16, -0.014639},
{30.5314, -9.0685, 0.95411, -0.099303},
{-84.4462, 29.5798, -4.3324, -0.63839},
{4.1482, -1.413, 0.01555, -0.068828}
};

float LQR_K_calc(float *coe,float len)
{ 
  return coe[0]*len*len*len+coe[1]*len*len+coe[2]*len+coe[3];
}


//float lqr_k[8]={ 
//        -0.0104f ,  -0.0624f  , -0.7520f,   -0.0704f,
//   -0.0104f,   -0.0624f,   -0.7520f,   -0.0704f
//}; //第一版

//腿最低的K值
float lqr_k[8]={ 
        -0.0146f ,  -0.0668f  , -0.8179f,   -0.0732f,
        -0.0146f ,  -0.0668f  , -0.8179f,   -0.0732f
};

void ChassisR_task(void)
{
	while(INS.ins_flag==0)
	{//等待加速度收敛
	  osDelay(1);	
	}

    ChassisR_init(&chassis_move);
    
	while(1)
	{	
		chassisR_feedback_update(&chassis_move,&INS,&vmc);//更新数据
        
        //vofa_start();
		
		for(int i=0;i<4;i++)
		{
			lqr_k[i]=LQR_K_calc(&Poly_Coefficient[i][0],vmc.left_len);	
		}
		for(int j=4;j<8;j++)
		{
			lqr_k[j]=LQR_K_calc(&Poly_Coefficient[j][0],vmc.right_len);	
		}
		
        //插值腿长 y = kx + b 获得不同腿长的机械中值
		//chassis_move.phi_set=0.25f*((vmc.left_len+vmc.right_len)/2.0f)/0.0455f - 0.635f;//腿长变化导致机械中值发生变化
		chassis_move.phi_set = 0.6800f*((vmc.left_len+vmc.right_len)/2.0f) - 0.2716f;     //参数修改见matlab
       
        
		chassis_move.wheel_motor[0].wheel_T=lqr_k[0]*(chassis_move.x_set-chassis_move.x)
																		+lqr_k[1]*(0.7f*chassis_move.v_set-chassis_move.v)
																		+lqr_k[2]*(chassis_move.phi_set-chassis_move.myPithR)
																		+lqr_k[3]*(chassis_move.d_phi_set-chassis_move.myPithGyroR);

		chassis_move.wheel_motor[1].wheel_T=lqr_k[4]*(chassis_move.x_set-chassis_move.x)
																		+lqr_k[5]*(0.7f*chassis_move.v_set-chassis_move.v)
																		+lqr_k[6]*(chassis_move.phi_set-chassis_move.myPithR)
																		+lqr_k[7]*(chassis_move.d_phi_set-chassis_move.myPithGyroR);
	
	
		chassis_move.wheel_motor[0].wheel_T=0.0f-chassis_move.wheel_motor[0].wheel_T;
		
		turn_T= Turn(chassis_move.total_yaw,INS.Gyro[2]); //转向差值
		
		chassis_move.wheel_motor[0].wheel_T=chassis_move.wheel_motor[0].wheel_T - turn_T;
		chassis_move.wheel_motor[1].wheel_T=chassis_move.wheel_motor[1].wheel_T - turn_T;
		
		mySaturate(&chassis_move.wheel_motor[0].wheel_T,-0.18f,0.18f);
		mySaturate(&chassis_move.wheel_motor[1].wheel_T,-0.18f,0.18f);
		
		vmc.roll_F0=roll_kp*(chassis_move.roll_set-chassis_move.myRoll)+roll_kd*INS.Gyro[0];

	if(chassis_move.jump_flag==1||chassis_move.jump_flag==2||chassis_move.jump_flag==3)
	{
    if(chassis_move.jump_flag==1)
		{//压缩阶段
		  vmc.left_F0=mg+leg_kp*(0.04f-vmc.left_len) + leg_kd*vmc.left_len_dot;			
		  vmc.right_F0=mg+leg_kp*(0.04f-vmc.right_len) - leg_kd*vmc.right_len_dot;
			 if(vmc.left_len<0.05f&&vmc.right_len<0.05f)
			 {
				jump_time++;
			 }
			 if(jump_time>=4)
			 {  
				 jump_time=0;
				 chassis_move.jump_flag=2;//压缩完毕进入上升加速阶段
			 }			 
		}
		else if(chassis_move.jump_flag==2)
		{//上升加速阶段			
		   vmc.left_F0=mg+leg_kp*(0.3f-vmc.left_len) + leg_kd*vmc.left_len_dot;			
		   vmc.right_F0=mg+leg_kp*(0.3f-vmc.right_len) - leg_kd*vmc.right_len_dot;
			
			 if(vmc.left_len>0.085f&&vmc.right_len>0.085f)
			 {
				jump_time++;
			 }
			 if(jump_time>=2)
			 {  
				 jump_time=0;
				 chassis_move.jump_flag=3;//上升完毕进入缩腿阶段
			 }	 
		}
		else if(chassis_move.jump_flag==3)
		{//缩腿阶段
			vmc.left_F0=leg_kp*(0.01f-vmc.left_len) + leg_kd*vmc.left_len_dot;			
		    vmc.right_F0=leg_kp*(0.01f-vmc.right_len) - leg_kd*vmc.right_len_dot;
			chassis_move.x_set=chassis_move.x;
		  if(vmc.left_len<0.045f&&vmc.right_len<0.045f)
		  {
			 jump_time++;
		  }
		  if(jump_time>=2)
		  { 
			 jump_time=0;
			 chassis_move.leg_set=0.06f;
			 chassis_move.last_leg_set=0.06f;
			 chassis_move.jump_flag=4;//缩腿完毕	
		  }
		}
	}	
	else
	{
		vmc.left_F0=mg+leg_kp*(chassis_move.leg_set-vmc.left_len) + leg_kd*vmc.left_len_dot;			
		vmc.right_F0=mg+leg_kp*(chassis_move.leg_set-vmc.right_len) - leg_kd*vmc.right_len_dot;
	}
	
	if(vmc.left_len>0.055f&&vmc.right_len>0.055f&&chassis_move.jump_flag==4)
	{
	  chassis_move.jump_flag=0;
	}

	 if(chassis_move.recover_flag==0)		
	 {//倒地自起不需要检测是否离地	
		 //离地检测	                                          
			if((vmc.left_F0<0.1f&&vmc.right_F0<0.1f&&chassis_move.leg_flag == 1&&chassis_move.jump_flag!=1&&chassis_move.jump_flag!=2)
					||chassis_move.jump_flag==3||chassis_move.jump_flag==4)
			{//离地了
				chassis_move.wheel_motor[0].wheel_T=0.0f;
				chassis_move.wheel_motor[1].wheel_T=0.0f;
				chassis_move.x=0.0f;
				chassis_move.x_set=chassis_move.x;
				chassis_move.turn_set=chassis_move.total_yaw;
			}
			else
			{		
				vmc.left_F0=vmc.left_F0+vmc.roll_F0;
				vmc.right_F0=vmc.right_F0-vmc.roll_F0;
			}
	 }
	 else if(chassis_move.recover_flag==1)
	 {		 
		  vmc.left_F0=0.0f;			
		  vmc.right_F0=0.0f;
	 }
	  
		//关节电机
		vmc.left_T1=-2.0f*vmc.left_l1*arm_sin_f32(chassis_move.joint_motor[0].para.pos)*vmc.left_F0;	
		vmc.right_T1=2.0f*vmc.right_l1*arm_sin_f32(chassis_move.joint_motor[1].para.pos)*vmc.right_F0;
		vmc.right_T1=0.0f-vmc.right_T1;
			
		if(chassis_move.jump_flag==1||chassis_move.jump_flag==2||chassis_move.jump_flag==3)
		{//跳跃的时候需要更大扭矩
			mySaturate(&vmc.left_T1,-5.0f,5.0f);   //根据重量修改扭矩
			mySaturate(&vmc.right_T1,-5.0f,5.0f);
		}	
		else
		{//不跳跃的时候
			mySaturate(&vmc.left_T1,-3.0f,3.0f);
			mySaturate(&vmc.right_T1,-3.0f,3.0f);
		}	
		
		if(chassis_move.start_flag==1)	
		{
			mit_ctrl3(&hfdcan1,0x03,  0.0f, 0.0f, 0.0f,  0.0f,vmc.left_T1);//左边关节电机
			mit_ctrl3(&hfdcan2,0x01,  0.0f, 0.0f, 0.0f,  0.0f,vmc.right_T1);//右边关节电机
			
			mit_ctrl2(&hfdcan1,0x04, 0.0f, 0.0f,0.0f, 0.0f,chassis_move.wheel_motor[0].wheel_T);//左边轮毂电机
			mit_ctrl2(&hfdcan2,0x02, 0.0f, 0.0f,0.0f, 0.0f,chassis_move.wheel_motor[1].wheel_T);//右边轮毂电机
			osDelay(CHASSR_TIME);
		}
		else if(chassis_move.start_flag==0)	
		{
			chassis_move.turn_set=chassis_move.total_yaw;
			chassis_move.roll_set=0.0f;
			chassis_move.leg_set=0.05f;//原始腿长
			chassis_move.x=0.0f;
			chassis_move.x_set=chassis_move.x;
			
			mit_ctrl3(&hfdcan1,0x03, 0.0f, 0.0f,0.0f, 0.0f,0.0f);//左边关节电机	
			mit_ctrl3(&hfdcan2,0x01, 0.0f, 0.0f,0.0f, 0.0f,0.0f);//右边关节电机	
					
			mit_ctrl2(&hfdcan1,0x04, 0.0f, 0.0f,0.0f, 0.0f, 0.0f);//左边轮毂电机	
			mit_ctrl2(&hfdcan2,0x02, 0.0f, 0.0f,0.0f, 0.0f, 0.0f);//右边轮毂电机
			osDelay(CHASSR_TIME);
		}
	}
}

void ChassisR_init(chassis_t *chassis)
{
	//左边
	joint_motor_init(&chassis->joint_motor[0],0x03,MIT_MODE);//发送id为1
	wheel_motor_init(&chassis->wheel_motor[0],0x04,MIT_MODE);//发送id为2
	//右边
	joint_motor_init(&chassis->joint_motor[1],0x01,MIT_MODE);//发送id为3
	wheel_motor_init(&chassis->wheel_motor[1],0x02,MIT_MODE);//发送id为4
	
	vmc.right_l1=0.06507f;//第一杆的长度，单位m
	vmc.left_l1=0.06507f;//第一杆的长度，单位m
	chassis_move.phi_set=-0.4f;
	chassis_move.leg_set=0.05f;
	
	for(int j=0;j<10;j++)
	{//左边
    enable_motor_mode(&hfdcan1,chassis->joint_motor[0].para.id,chassis->joint_motor[0].mode);//左边关节电机
	  osDelay(1);
	}
	for(int j=0;j<10;j++)
	{//左边
    enable_motor_mode(&hfdcan1,chassis->wheel_motor[0].para.id,chassis->wheel_motor[0].mode);//左边轮毂电机
	  osDelay(1);
	}

	for(int j=0;j<10;j++)
	{//右边
    enable_motor_mode(&hfdcan2,chassis->joint_motor[1].para.id,chassis->joint_motor[1].mode);//右边关节电机
	  osDelay(1);
	}
	for(int j=0;j<10;j++)
	{//右边
    enable_motor_mode(&hfdcan2,chassis->wheel_motor[1].para.id,chassis->wheel_motor[1].mode);//右边轮毂电机
	  osDelay(1);
	}
}

int64_t time=0;
float averr[10]={0.0f};
void chassisR_feedback_update(chassis_t *chassis,INS_t *ins,vmc_leg_t *vmc_leg)
{
	chassis->myPithR=ins->Pitch;
	chassis->myPithGyroR=0.0f-ins->Gyro[1];
	
	chassis->total_yaw=ins->YawTotalAngle;
	chassis->v_act=((chassis->wheel_motor[1].para.vel-chassis->wheel_motor[0].para.vel)/2.0f)*0.03375f;//0.03375m是轮子半径
	//chassis->v=((chassis->wheel_motor[1].para.vel-chassis->wheel_motor[0].para.vel)/2.0f)*0.03375f;//0.03375m是轮子半径
	averr[0]=averr[1];
	averr[1]=averr[2];
	averr[2]=averr[3];
	averr[3]=averr[4];
	averr[4]=averr[5];
	averr[5]=averr[6];
	averr[6]=averr[7];
	averr[7]=averr[8];
	averr[8]=averr[9];
	averr[9]=chassis->v_act;
	
	chassis->v=0.1f*averr[0]+0.1f*averr[1]+0.1f*averr[2]+0.1f*averr[3]
						+0.1f*averr[4]+0.1f*averr[5]+0.1f*averr[6]+0.1f*averr[7]
						+0.1f*averr[8]+0.1f*averr[9];//对速度进行均值滤波
	
	chassis->x=chassis->x+chassis->v*0.001f;//控制周期为1ms
	
	//连杆1和连杆2近似等长
	//轮子在竖直方向运动近似为一条直线
	vmc_leg->left_len= 2.0f*vmc_leg->left_l1
											*arm_cos_f32(chassis->joint_motor[0].para.pos);//余弦定理
	vmc_leg->left_len_dot = -2.0f*vmc_leg->left_l1*arm_sin_f32(chassis->joint_motor[0].para.pos)
													*chassis->joint_motor[0].para.vel;//直接求导

	vmc_leg->right_len= 2.0f*vmc_leg->right_l1
											*arm_cos_f32(-chassis->joint_motor[1].para.pos);//余弦定理
	vmc_leg->right_len_dot = -2.0f*vmc_leg->right_l1*arm_sin_f32(chassis->joint_motor[1].para.pos)
													*(-chassis->joint_motor[1].para.vel);//直接求导
	
	if(INS.Roll>0)//imu需要反装
	{
		chassis->myRoll=INS.Roll-3.1415926f;
	}
	else if(INS.Roll<=0)
	{
	  chassis->myRoll=INS.Roll+3.1415926f;
	}
	
	if(ins->Pitch<(3.1415926f/4.0f)&&ins->Pitch>(-3.1415926f/4.0f))
	{//根据pitch角度判断倒地自起是否完成
		chassis->recover_flag=0;
	}

}


void mySaturate(float *in,float min,float max)
{
  if(*in < min)
  {
    *in = min;
  }
  else if(*in > max)
  {
    *in = max;
  }
}





		
