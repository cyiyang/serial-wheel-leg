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
			if(k210_msg.class_n == 10)//��������
			{
				sprintf(buff_com,"id = %c\r\n",k210_msg.id);
				USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				
				k210_msg.class_n = 0;//������̺�	
			}
			
		}
	}
}

