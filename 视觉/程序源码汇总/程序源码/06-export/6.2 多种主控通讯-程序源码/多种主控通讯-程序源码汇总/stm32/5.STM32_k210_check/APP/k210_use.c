#include "AllHeader.h"

char buf_msg[100]={'\0'};
u8 new_flag = 0;
u8 r_index = 0;
u16 buf_crc = 0; //校验和
u8 tou_flag = 0; //包头收到标志
u8 len_flag = 0; //长度收到标志
u8 buf_len = 0;//长度
char data[50];//有效数据存储

void recv_k210msg(uint8_t recv_msg) //recv_msg 
{
	if (recv_msg == '$')
	{
		new_flag = 1;
		
	}
	
	if(recv_msg == '#' )
	{
		if( buf_len == r_index)
		{
			new_flag = 0;
			tou_flag = 0;
			len_flag = 0;
			
			buf_crc -= buf_msg[r_index-1];//减掉获取到的校验和
			buf_crc %= 256;
			
			if(buf_crc == buf_msg[r_index-1]) //校验正确
			{
				deal_recvmsg();//新数据接收完毕,进行处理
			}
			else //不正确
			{
				r_index = 0;
				buf_crc = 0;
			}
		}
	}
	
	if(new_flag == 1 )//要改成只不接收一次'$'
	{
		if(recv_msg == '$' && tou_flag == 0)
		{
			tou_flag = 1;
		}
		else
		{
			buf_msg[r_index++] = recv_msg;
			buf_crc += recv_msg;
			if(len_flag == 0)
			{
				buf_len = buf_msg[0];
				len_flag = 1;
			}
			
		}

	}
	
}


void deal_recvmsg(void)
{
	
	u8 index,data_i=0;//数据索引
	u8 eg_num = buf_msg[1];//例程编号
//	u8 ed_group = buf_msg[2];//例程组
	u8 number = buf_msg[3];//数据量(包含逗号)
	u8 i_duo = 0;
	//buf_len = buf_msg[0];//长度
	
	if(r_index!=buf_len)//长度不符合
	{
		buf_len = 0;
		return ;
	}
		
	
	for(index = 0 ;index<number;index++) //以逗号隔开
	{
		if(buf_msg[4+index] == 0x2c && i_duo ==0)//逗号,但不会有两个连续的逗号
		{
			i_duo = 1;
			continue;
		}
		data[data_i++]=buf_msg[4+index];//第5位是有效数据开始
		i_duo =0;
	}
	
	buf_crc = 0;//校验位清除
	r_index = 0;
	
	//USART2_Send_ArrayU8((uint8_t*)data,strlen(data));
	memset(buf_msg,0,sizeof(buf_msg));//清除旧数据
	deal_data(eg_num);

}


void deal_data(u8 egnum)//传入值是例程编号
{
	u16 x,y,w,h;
	u8 msg[20]={'\0'};
	u8 icopy = 0;
	u16 id = 999;
	switch(egnum)
	{
		//例程1、5、6
		case 1:
		case 5:
		case 6: 
			x = data[1]<<8 | data[0];
			y = data[3]<<8 | data[2];
			w = data[5]<<8 | data[4];
			h = data[7]<<8 | data[6];
			break;
		
		//例程2,3
		case 2:
		case 3:
			x = data[1]<<8 | data[0];
			y = data[3]<<8 | data[2];
			w = data[5]<<8 | data[4];
			h = data[7]<<8 | data[6];
			while(*(data+8+icopy)!='\0')
			{
				msg[icopy] = *(data+8+icopy);
				icopy ++;
			}
			break;
			
			//例程4
		case 4:
			x = data[1]<<8 | data[0];
			y = data[3]<<8 | data[2];
			w = data[5]<<8 | data[4];
			h = data[7]<<8 | data[6];
			id =data[8]<<8 | data[9];
			
			
			while(*(data+10+icopy)!='\0')
			{
				msg[icopy] = *(data+10+icopy);
				icopy ++;
			}
			
			break;
			
		case 7:
		case 8:
			x = data[1]<<8 | data[0];
			y = data[3]<<8 | data[2];
			w = data[5]<<8 | data[4];
			h = data[7]<<8 | data[6];
			id =data[8];
			
			break;
			
		case 9:
			x = data[1]<<8 | data[0];
			y = data[3]<<8 | data[2];
			w = data[5]<<8 | data[4];
			h = data[7]<<8 | data[6];
			while(*(data+8+icopy)!='\0')
			{
				msg[icopy] = *(data+8+icopy);
				icopy ++;
			}
			break;
		
		case 10:
		case 11:
			id = data[0];
			break;
	
	}
	k210_msg.class_n = egnum;
	k210_msg.x = x;
	k210_msg.y = y;
	k210_msg.w = w;
	k210_msg.h = h;
	k210_msg.id = id;
	strcpy((char*)k210_msg.msg_msg,(char*)msg);
	
	
	memset(data,0,sizeof(data));//清除data数据
	
}

