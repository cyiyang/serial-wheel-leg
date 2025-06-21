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
			if(k210_msg.class_n == 7)//�ǿ��ּ��
			{
				sprintf(buff_com,"x=%d,y=%d,w=%d,h=%d\r\n",k210_msg.x,k210_msg.y,k210_msg.w,k210_msg.h);
				USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				
				if(k210_msg.id == 'Y' || k210_msg.id == 'y')
				{
					sprintf(buff_com,"Yes\r\n");
					USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				}
				else
				{
					sprintf(buff_com,"NO\r\n");
					USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				}
				
				k210_msg.class_n = 0;//������̺�	
			}
			
		}
	}
}

