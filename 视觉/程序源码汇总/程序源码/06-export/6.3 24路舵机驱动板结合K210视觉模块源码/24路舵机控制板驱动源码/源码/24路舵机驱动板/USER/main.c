#include "AllHeader.h"
/*
	ʵ��ģ�飺
	24·���������
	K210�Ӿ�ģ��
	��ά��̨
	
	ʵ����Ĵ��룺
	usart3����K210���͵�����
  TIM1�ж��п��ƶ��pwm���
*/


u8 grop = 0;
float version = 1.1;

int main(void)
{	
	u8 Dnum;
	u8 Doup;

	//����Ƕȳ�ʼ��
	for(Doup = 0;Doup <GROUP_NUM;Doup++)
	{  
		for(Dnum = 0;Dnum <DUOJI_NUM;Dnum++)
		{
			Angle_J[Doup][Dnum] = 90;   //�����ж���������ýǶ�
		}
	}
	
	bsp_init();//Ӳ����ʼ�� 
	USART3_init(115200);//����λ��ͨ��
	
	while (1)
	{
		dectect_beep();//����ѹ
		
		void_jutce(); //������
	}
 								    
}


