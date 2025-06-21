#include "AllHeader.h"

char buf_msg[100]={'\0'};
u8 new_flag = 0;
u8 r_index = 0;
u16 buf_crc = 0; //У���
u8 tou_flag = 0; //��ͷ�յ���־
u8 len_flag = 0; //�����յ���־
u8 buf_len = 0;//����
char data[50];//��Ч���ݴ洢

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
			
			buf_crc -= buf_msg[r_index-1];//������ȡ����У���
			buf_crc %= 256;
			
			if(buf_crc == buf_msg[r_index-1]) //У����ȷ
			{
				deal_recvmsg();//�����ݽ������,���д���
			}
			else //����ȷ
			{
				r_index = 0;
				buf_crc = 0;
			}
		}
	}
	
	if(new_flag == 1 )//Ҫ�ĳ�ֻ������һ��'$'
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
	
	u8 index,data_i=0;//��������
	u8 eg_num = buf_msg[1];//���̱��
//	u8 ed_group = buf_msg[2];//������
	u8 number = buf_msg[3];//������(��������)
	u8 i_duo = 0;
	//buf_len = buf_msg[0];//����
	
	if(r_index!=buf_len)//���Ȳ�����
	{
		buf_len = 0;
		return ;
	}
		
	
	for(index = 0 ;index<number;index++) //�Զ��Ÿ���
	{
		if(buf_msg[4+index] == 0x2c && i_duo ==0)//����,�����������������Ķ���
		{
			i_duo = 1;
			continue;
		}
		data[data_i++]=buf_msg[4+index];//��5λ����Ч���ݿ�ʼ
		i_duo =0;
	}
	
	buf_crc = 0;//У��λ���
	r_index = 0;
	
	//USART2_Send_ArrayU8((uint8_t*)data,strlen(data));
	memset(buf_msg,0,sizeof(buf_msg));//���������
	deal_data(eg_num);

}


void deal_data(u8 egnum)//����ֵ�����̱��
{
	u16 x,y,w,h;
	u8 msg[20]={'\0'};
	u8 icopy = 0;
	u16 id = 999;
	switch(egnum)
	{
		//����1��5��6
		case 1:
		case 5:
		case 6: 
			x = data[1]<<8 | data[0];
			y = data[3]<<8 | data[2];
			w = data[5]<<8 | data[4];
			h = data[7]<<8 | data[6];
			break;
		
		//����2,3
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
			
			//����4
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
	
	
	memset(data,0,sizeof(data));//���data����
	
}

