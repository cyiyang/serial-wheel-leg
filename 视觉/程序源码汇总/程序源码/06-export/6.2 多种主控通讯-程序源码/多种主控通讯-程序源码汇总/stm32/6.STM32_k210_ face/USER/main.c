#include "AllHeader.h"


char buff_com[50];
msg_k210 k210_msg;//收到k210信息结构体


int main()
{
	SystemInit();
	
	delay_init();

	led_int();
	USART2_init(115200);
		
	LED = 0;
	
		
	while(1)
	{
		if (k210_msg.class_n != 0)//例程号不为空
		{
			if(k210_msg.class_n == 6)//是人脸特征检测
			{
				sprintf(buff_com,"x=%d,y=%d,w=%d,h=%d\r\n",k210_msg.x,k210_msg.y,k210_msg.w,k210_msg.h);
				USART2_Send_ArrayU8((uint8_t*)buff_com,strlen(buff_com));
				
				k210_msg.class_n = 0;//清除例程号	
			}
			
		}
	}
}

