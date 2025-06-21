#include "bsp_tracing.h"
#include "app.h"
#include "app_motion.h"


#define LOW		   (0)
#define HIGH	   (1)
#define delay_ms   App_Delay_ms



void Tracing_Init(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启外设时钟*/
    RCC_APB2PeriphClockCmd(TRACING_CLK, ENABLE);
    /*选择要控制的引脚*/
    GPIO_InitStructure.GPIO_Pin = X1_PIN | X2_PIN | X3_PIN | X4_PIN;
    /*设置引脚模式为输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*调用库函数，初始化PORT*/
    GPIO_Init(TRACING_PORT, &GPIO_InitStructure);
}

void Tracing_Get_Value(int *p_iL1, int *p_iL2, int *p_iR1, int *p_iR2)
{
    *p_iL1 = READ_X1();
    *p_iL2 = READ_X2();
    *p_iR1 = READ_X3();
    *p_iR2 = READ_X4();
}


void Tracing_Line_Walking(void)
{
	int LineL1 = 1, LineL2 = 1, LineR1 = 1, LineR2 = 1;

	Tracing_Get_Value(&LineL1, &LineL2, &LineR1, &LineR2);	//获取黑线检测状态	

	if( (LineL1 == LOW || LineL2 == LOW) && LineR2 == LOW) //左大弯
    {
      	// Car_SpinLeft(7000, 7000);
        Motion_Ctrl(0, 0, 4000, 0);
		delay_ms(80);
    }
    else if ( LineL1 == LOW && (LineR1 == LOW || LineR2 == LOW)) //右大弯
	{ 
      	// Car_SpinRight(7000, 7000);
        Motion_Ctrl(0, 0, -4000, 0);
		delay_ms(80);
    }  
    else if( LineL1 == LOW ) //左最外侧检测
    {  
		// Car_SpinLeft(6800, 6800);
        Motion_Ctrl(0, 0, 3000, 0);
		delay_ms(10);
	}
    else if ( LineR2 == LOW) //右最外侧检测
    {  
		// Car_SpinRight(6800, 6800);
        Motion_Ctrl(0, 0, 3000, 0);
		delay_ms(10);
	}
    else if (LineL2 == LOW && LineR1 == HIGH) //中间黑线上的传感器微调车左转
    {   
		// Car_Left(6500);   
        Motion_Ctrl(200, 0, 2000, 0);
	}
	else if (LineL2 == HIGH && LineR1 == LOW) //中间黑线上的传感器微调车右转
    {   
		// Car_Right(6500);   
        Motion_Ctrl(200, 0, -2000, 0);
	}
    else if(LineL2 == LOW && LineR1 == LOW) // 都是黑色, 加速前进
    {  
		Motion_Ctrl(200, 0, 0, 0);
	}	
}

void Tracing_line_handle(void)
{
    Tracing_Line_Walking();
}
