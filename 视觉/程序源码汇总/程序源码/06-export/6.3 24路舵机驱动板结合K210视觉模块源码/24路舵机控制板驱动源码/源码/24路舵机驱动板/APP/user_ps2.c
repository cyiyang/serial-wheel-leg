#include "AllHeader.h"

 
//------------接线说明---------------
//PS2接收器--------STM32F1主板1引脚
//左->右 DI开始
//    VCC     -----------    V3.3 
//    GND     -----------    GND  
//    MISO=DI、DAT      --------- PA6
//    MOSI=DO、CMD      ----------PA7
//    CS      -----------    PA4
//    CLK     -----------    PA5 
 
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;


void user_douji(void)
{
	//摇杆暂时不用
		PS2_LX=PS2_AnologData(PSS_LX);      
		PS2_LY=PS2_AnologData(PSS_LY);
		PS2_RX=PS2_AnologData(PSS_RX);
		PS2_RY=PS2_AnologData(PSS_RY);
	
		PS2_KEY=PS2_DataKey();
	
		switch(PS2_KEY)//按键处理
		{
			/**************************/
			case PSB_SELECT: //控s1上增大
					if(Angle_J[grop][0] >= 180)
							Angle_J[grop][0] = 180;
					else 
						Angle_J[grop][0] +=10;//加10度的向上加
					break;
			case PSB_START: //控s1上减少
				if(Angle_J[grop][0] <= 0)
							Angle_J[grop][0] = 0;
					else 
						Angle_J[grop][0] -=10;//加10度的向下减
				  break;
					
			/**************************/		
			case PSB_L3: //控s2上增大
					if(Angle_J[grop][1] >= 180)
							Angle_J[grop][1] = 180;
					else 
						Angle_J[grop][1] +=10;//加10度的向上加
					break;
			case PSB_R3: //控s2上减少
					if(Angle_J[grop][1] <= 0)
							Angle_J[grop][1] = 0;
					else 
						Angle_J[grop][1] -=10;//加10度的向下减
				  break;
					
			/**************************/
			case PSB_PAD_UP: //控s3上增大
					if(Angle_J[grop][2] >= 180)
							Angle_J[grop][2] = 180;
					else 
						Angle_J[grop][2] +=10;//加10度的向上加
					break;
			case PSB_PAD_DOWN: //控s3上减少
					if(Angle_J[grop][2] <= 0)
							Angle_J[grop][2] = 0;
					else 
						Angle_J[grop][2] -=10;//加10度的向下减
				  break;
					
			/**************************/
			case PSB_PAD_RIGHT: //控s4上增大
					if(Angle_J[grop][3] >= 180)
							Angle_J[grop][3] = 180;
					else 
						Angle_J[grop][3] +=10;//加10度的向上加
					break;
			case PSB_PAD_LEFT: //控s4上减少
					if(Angle_J[grop][3] <= 0)
							Angle_J[grop][3] = 0;
					else 
						Angle_J[grop][3] -=10;//加10度的向下减
				  break;
					
			/**************************/
			case PSB_L2: //控s5上增大
					if(Angle_J[grop][4] >= 180)
							Angle_J[grop][4] = 180;
					else 
						Angle_J[grop][4] +=10;//加10度的向上加
					break;				
			case PSB_R2: //控s5上减少
					if(Angle_J[grop][4] <= 0)
							Angle_J[grop][4] = 0;
					else 
						Angle_J[grop][4] -=10;//加10度的向下减
					break;				
			
			/**************************/
			case PSB_L1: //控s6上增大
					if(Angle_J[grop][5] >= 180)
							Angle_J[grop][5] = 180;
					else 
						Angle_J[grop][5] +=10;//加10度的向上加
					break;				
			case PSB_R1: //控s6上减少
					if(Angle_J[grop][5] <= 0)
							Angle_J[grop][5] = 0;
					else 
						Angle_J[grop][5] -=10;//加10度的向下减
					break;		
			
			/**************************/
			case PSB_GREEN: //控s7上增大
					if(Angle_J[grop][6] >= 180)
							Angle_J[grop][6] = 180;
					else 
						Angle_J[grop][6] +=10;//加10度的向上加
					break;		
			case PSB_BLUE: //控s7上减少
					if(Angle_J[grop][6] <= 0)
							Angle_J[grop][6] = 0;
					else 
						Angle_J[grop][6] -=10;//加10度的向下减
					break;		
					
			/**************************/
			case PSB_RED: //控s8上增大
					if(Angle_J[grop][7] >= 180)
							Angle_J[grop][7] = 180;
					else 
						Angle_J[grop][7] +=10;//加10度的向上加
					break;		
			case PSB_PINK: //控s8上减少
					if(Angle_J[grop][7] <= 0)
							Angle_J[grop][7] = 0;
					else 
						Angle_J[grop][7] -=10;//加10度的向下减
					break;		
		}

}


//测试手柄的程序
#if 0
void test_ps2(void)
{ 
	while(1)
		{		
			PS2_LX=PS2_AnologData(PSS_LX);      
			PS2_LY=PS2_AnologData(PSS_LY);
			PS2_RX=PS2_AnologData(PSS_RX);
			PS2_RY=PS2_AnologData(PSS_RY);
			PS2_KEY=PS2_DataKey();	
 		  printf("%d     PS2左X轴:",PS2_LX);   //打印左边X轴的值
			printf("%d     PS2左Y轴:",PS2_LY);   //打印右边Y轴的值
		  printf("%d     PS2右X轴:",PS2_RX);   //打印左边X轴的值
			printf("%d     PS2右Y轴:",PS2_RY);   //打印右边Y轴的值
			printf("%d \r\nPS2按键:",PS2_KEY);   //打印按键的值
			delay_ms(100);										   //延时100m秒钟
			switch(PS2_KEY)
			{
				case 5:
				case 6:
				case 7:
				case 8:
					BEEP_ON;
					break;
				
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
					BEEP_OFF;
					break;
				
			}
			delay_ms(150);
		} 
}
#endif



