#include "AllHeader.h"

#define RX_LEN 10

char rxbuff[RX_LEN];//一包数据的存储量
u8 recv_i = 0;
u8 rx_flag = 0;//有效数据包的位置
u8 newdata_flag = 0;//完成一包数据接收的标志

/* 命令接收缓存 */
u8 RxBuffer[PTO_MAX_BUF_LEN];
/* 接收数据下标 */
u8 RxIndex = 0;
/* 接收状态机 */
u8 RxFlag = 0;
/* 新命令接收标志 */
u8 New_CMD_flag;
/* 新命令数据长度 */
u8 New_CMD_length;





//接收蓝牙的信息并做处理  //$A090#  :第一路的舵机90度
void deal_bluetooth(u8 Rx_temp)  //把尾（#）去掉
{
		if(Rx_temp == '$')
	{
		rx_flag = 1;
	}
	if(Rx_temp == '#')
	{
		rxbuff[recv_i] = '\0';
		recv_i = 0;
		rx_flag = 0;
		deal_data();
		memset(rxbuff,0,RX_LEN);
	}
	if(rx_flag == 1 )
	{
		rxbuff[recv_i] = Rx_temp;
		recv_i ++;
	}

}


//处理包的数据
u8 deal_data(void) //传过来的是字符类型
{
	u8 duo_num = 0;
	u8 Hor,column;
	u16 angle = 0;
	duo_num = rxbuff[1] - 64; 
	Hor = (duo_num-1)/8;//得到组别 0.1.2     -1:数组是从0开始
	column=(duo_num-1)%8; //一组里的对应的舵机位置
	
	angle = (rxbuff[2]-48)*100 + (rxbuff[3]-48)*10 + rxbuff[4]-48;
	
	if(angle <= 0)
	{
		angle = 0;
	}
	if(angle >= 180)
	{
		angle = 180;
	}
	
	Angle_J[Hor][column] = angle;	
	
	return 0;
}


// 获取接收的数据
uint8_t* Get_RxBuffer(void)
{
	return (uint8_t*)RxBuffer;
}

// 获取命令长度
uint8_t Get_CMD_Length(void)
{
	//printf("L=%c\r\n",New_CMD_length);
	return New_CMD_length;
}

// 获取命令标志
uint8_t Get_CMD_Flag(void)
{
	return New_CMD_flag;
}

void Clear_CMD_Flag(void)
{
	#if ENABLE_CLEAR_RXBUF
	for (uint8_t i = 0; i < New_CMD_length; i++)
	{
		RxBuffer[i] = 0;
	}
	#endif
	New_CMD_length = 0;
	New_CMD_flag = 0;
}

//清除RxBuffer中所有值为0
void Clear_RxBuffer(void)
{
	for (uint8_t i = 0; i < PTO_MAX_BUF_LEN; i++)
	{
		RxBuffer[i] = 0;
	}
}

//将接收的数据拆分
void Upper_Data_Receive(uint8_t Rx_Temp)
{
	switch (RxFlag)
	{
	case 0:
		if (Rx_Temp == PTO_HEAD)
		{
			RxBuffer[0] = PTO_HEAD;
			RxFlag = 1;
		}
		break;

	case 1:
		if (Rx_Temp == PTO_DEVICE_ID)
		{
			RxBuffer[1] = PTO_DEVICE_ID;
			RxFlag = 2;
			RxIndex = 2;
		}
		else
		{
			RxFlag = 0;
			RxBuffer[0] = 0x0;
		}
		break;

	case 2:
		New_CMD_length = Rx_Temp + 2;
	  
		if (New_CMD_length >= PTO_MAX_BUF_LEN)
		{
			RxIndex = 0;
			RxFlag = 0;
			RxBuffer[0] = 0;
			RxBuffer[1] = 0;
			New_CMD_length = 0;
			break;
		}
		RxBuffer[RxIndex] = Rx_Temp;
		RxIndex=3;
		RxFlag = 3;
		break;

	case 3:
		RxBuffer[RxIndex] = Rx_Temp;
		RxIndex++;
		if (RxIndex >= New_CMD_length)
		{
			New_CMD_flag = 1;
			RxIndex = 0;
			RxFlag = 0;
		}
		break;	

	default:
		break;
	}
}

//设置舵机的角度
void PwmServo_Set_Angle_All(uint8_t angle_s1, uint8_t angle_s2, uint8_t angle_s3, uint8_t angle_s4)
{
	if (angle_s1 <= 180)
	{
		Angle_J[0][0] = angle_s1;    //S1舵机---X轴

	}

	if (angle_s2 <= 180) 
	{
		Angle_J[0][1] = angle_s2;    //S2舵机---Y轴

	}

	if (angle_s3 <= 180)
	{
		Angle_J[0][2] = angle_s3;    //无用

	}

	if (angle_s4 <= 180)
	{
		Angle_J[0][3] = angle_s4;    //无用

	}
}


//
void deal_K210(uint8_t *data_buf, uint8_t num)
{
	uint8_t func_id = *(data_buf + 3);
	if(func_id==FUNC_PWM_SERVO_ALL)
	{
		uint8_t angle_s1 = *(data_buf + 4);
		uint8_t angle_s2 = *(data_buf + 5);
		uint8_t angle_s3 = *(data_buf + 6);
		uint8_t angle_s4 = *(data_buf + 7);
//		printf("%s",RxBuffer);
		printf("all Servo:%d, %d, %d, %d\n", angle_s1, angle_s2, angle_s3, angle_s4);  //打印K210输出的信息
		PwmServo_Set_Angle_All(angle_s1, angle_s2, angle_s3, angle_s4);
		Clear_RxBuffer();
	}
}



