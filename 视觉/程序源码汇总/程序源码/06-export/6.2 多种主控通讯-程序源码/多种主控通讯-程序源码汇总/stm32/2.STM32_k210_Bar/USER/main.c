#include "AllHeader.h"


char buff_com[50];
msg_k210 k210_msg;//�յ�k210��Ϣ�ṹ��


int main()
{
	SystemInit();
	
	delay_init();

	led_int();
	USART2_init(115200);
		
	LED = 0;
	
		
	while(1)
	{
		if (k210_msg.class_n != 0)//���̺Ų�Ϊ��
		{
			if(k210_msg.class_n == 2)//��������ʶ��
			{
				sprintf(buff_com,"x=%d,y=%d,w=%d,h=%d\r\n",k210_msg.x,k210_msg.y,k210_msg.w,k210_msg.h);
				USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				
				sprintf(buff_com,"str = %s\r\n",k210_msg.msg_msg);
				USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				
				k210_msg.class_n = 0;//������̺�	
			}
			
		}
	}
}

