#include "bsp_usart1.h"
#include "chassisR_task.h"

extern UART_HandleTypeDef huart1;

extern chassis_t chassis_move;
send_data_t send_data;

rev_data_t rev_data;

void slope_following(float *target,float *set,float acc)
{
	if(*target > *set)
	{
		*set = *set + acc;
		if(*set >= *target)
		*set = *target;
	}
	else if(*target < *set)
	{
		*set = *set - acc;
		if(*set <= *target)
		*set = *target;
	}
}

void connect_usart1_init(void)
{ 
	 HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rev_data.rx,RECEIVE_DATA_SIZE*2);	
}


float vel_ratio=-0.008f;
float turn_ratio=0.02f;
float turn_ratio2=0.09f;
float leg_ratio=0.00006f;
float roll_ratio=-0.00025f;
extern chassis_t chassis_move;
extern vmc_leg_t vmc;
int reverse_flag=1;
uint8_t myflag=0;
int reverseRoll_flag=1;
int64_t movetime=0;

//蓝牙代码蓝牙有问题，遂放弃
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)
//	{
//		if(rev_data.rx[0]==0xA5)
//		{
//			rev_data.key=rev_data.rx[1];
//			
//			if(rev_data.last_key!=1&&rev_data.key==1&&chassis_move.start_flag==0) 
//			{
//				//start按键被按下
//				chassis_move.start_flag=1;
//			}
//			else if(rev_data.last_key!=1&&rev_data.key==1&&chassis_move.start_flag==1) 
//			{
//				//start按键被按下
//				chassis_move.start_flag=0;
//				chassis_move.recover_flag=0;
//			}
//			
//			if(rev_data.last_key!=2&&rev_data.key==2&&chassis_move.prejump_flag==0) 
//			{
//				//prejump按键被按下
//				chassis_move.prejump_flag=1;
//			}
//			else if(rev_data.last_key!=2&&rev_data.key==2&&chassis_move.prejump_flag==1) 
//			{
//				//prejump按键被按下
//				chassis_move.prejump_flag=0;
//			}
//			
//			if(rev_data.last_key!=4&&rev_data.key==4&&chassis_move.prejump_flag==1&&chassis_move.jump_flag==0) 
//			{
//				//只有当预跳跃标志置1，按下这个键才能开启跳跃
//				chassis_move.jump_flag=1;
//			}
//			
//			if(rev_data.last_key!=16&&rev_data.key==16&&chassis_move.autoleg_flag==0) 
//			{
//				chassis_move.autoleg_flag=1;
//			}
//			else if(rev_data.last_key!=16&&rev_data.key==16&&chassis_move.autoleg_flag==1) 
//			{
//				chassis_move.autoleg_flag=0;
//			}
//			
//			if(rev_data.last_key!=32&&rev_data.key==32&&chassis_move.autoturn_flag==0) 
//			{
//				chassis_move.autoturn_flag=1;
//			}
//			else if(rev_data.last_key!=32&&rev_data.key==32&&chassis_move.autoturn_flag==1) 
//			{
//				chassis_move.autoturn_flag=0;
//			}
//			
//			if(rev_data.last_key!=64&&rev_data.key==64&&chassis_move.fastturn_flag==0) 
//			{
//				chassis_move.fastturn_flag=1;
//			}
//			else if(rev_data.last_key!=64&&rev_data.key==64&&chassis_move.fastturn_flag==1) 
//			{
//				chassis_move.fastturn_flag=0;
//			}
//			
//			if(rev_data.last_key!=128&&rev_data.key==128&&chassis_move.movejump_flag==0) 
//			{
//				chassis_move.movejump_flag=1;
//			}
//			else if(rev_data.last_key!=128&&rev_data.key==128&&chassis_move.movejump_flag==1) 
//			{
//				chassis_move.movejump_flag=0;
//			}
//			
//			rev_data.last_key=rev_data.key;	
//		}
//		
//		 if(chassis_move.recover_flag==0
//			&&((chassis_move.myPithR<((-3.1415926f)/4.0f)&&chassis_move.myPithR>((-3.1415926f)/2.0f))
//		  ||(chassis_move.myPithR>(3.1415926f/4.0f)&&chassis_move.myPithR<(3.1415926f/2.0f))))
//			{
//			  chassis_move.recover_flag=1;//需要自起
//				chassis_move.leg_set=0.05f;//原始腿长
//			}
//			
//		if(chassis_move.start_flag==1)
//		{
//			if(rev_data.rx[0]==0xA5)
//			{			
//				if(rev_data.rx[3]!=0)
//				{
//					chassis_move.front_flag=1;
//					chassis_move.v_set=((float)((int8_t)rev_data.rx[3]))*vel_ratio;
//				  chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;
//				}
//				else
//				{
//					chassis_move.front_flag=0;
//					chassis_move.v_set=0.0f;
//				}					
//				chassis_move.last_front_flag=chassis_move.front_flag;
//				
//				if(rev_data.rx[2]!=0||chassis_move.fastturn_flag==1||chassis_move.autoturn_flag==1)
//				{
//					if(rev_data.rx[2]!=0)
//					{
//						chassis_move.turn_flag=1;
//						chassis_move.turn_set=((float)((int8_t)rev_data.rx[2]))*turn_ratio;
//					}
//				  else if(chassis_move.fastturn_flag==1)
//					{
//						chassis_move.turn_flag=1;
//						chassis_move.turn_set=-11.43f;
//					}
//					else if(chassis_move.autoturn_flag==1)
//					{
//						chassis_move.turn_flag=1;
//						chassis_move.turn_set=6.35f;
//					}
//				}
//				else 
//				{
//					chassis_move.turn_flag=0;
//				}
//				if(chassis_move.last_turn_flag==1&&chassis_move.turn_flag==0)
//				{
//				  chassis_move.turn_set=chassis_move.total_yaw;
//				}					
//				chassis_move.last_turn_flag=chassis_move.turn_flag;
//										
//				if(chassis_move.autoleg_flag==0)
//				{				
//					chassis_move.leg_set=chassis_move.leg_set+((float)((int8_t)rev_data.rx[5]))*leg_ratio; 
//				}
//				else
//				{
//					if((vmc.right_len+vmc.left_len)/2.0f>0.09f&&reverse_flag==1)
//					{
//					  reverse_flag=-1;
//					}
//					else if((vmc.right_len+vmc.left_len)/2.0f<0.055f&&reverse_flag==-1)
//					{
//					  reverse_flag=1;
//					}
//				  chassis_move.leg_set=chassis_move.leg_set+0.007f*reverse_flag;		
//				}
//				chassis_move.roll_set=chassis_move.roll_set+((float)((int8_t)rev_data.rx[4]))*roll_ratio;
//				mySaturate(&chassis_move.roll_set,-0.19f,0.19f);
//				mySaturate(&chassis_move.leg_set,0.045f,0.1f);

//				if(fabsf(chassis_move.last_leg_set-chassis_move.leg_set)>0.0001f)
//				{//遥控器控制腿长在变化
//					chassis_move.leg_flag=1;//为1标志着遥控器在控制腿长伸缩，根据这个标志可以不进行离地检测，因为当腿长在主动伸缩时，离地检测会误判为离地了 			
//				}
//				else
//				{
//				 	chassis_move.leg_flag=0;
//				}
//				chassis_move.last_leg_set=chassis_move.leg_set;
//			}	
//			
//			
//			if(chassis_move.movejump_flag==1)
//			{
//				chassis_move.v_set=100.0f*vel_ratio;
//				chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;
//				movetime++;
//				if(movetime>50)
//				{
//				  movetime=0;
//					chassis_move.jump_flag=1;
//					//chassis_move.v_set=0.0f;
//					chassis_move.movejump_flag=0;
//				}
//			}
//		}
//		
//		if(rev_data.key==8)
//		{
//		  chassis_move.roll_set=0.0f;
//		}
//		
//		HAL_UART_Receive_IT(&huart1,rev_data.rx, 8);//接收
//	}
//	
//}

