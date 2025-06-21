#include "AllHeader.h"

extern u8 RxBuffer[PTO_MAX_BUF_LEN];

//USART1 ---- 与PC通信，输出调试信息
void USART1_init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启接收中断       
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_Cmd(USART1, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
 * @Brief: UART1发送数据
 * @Note: 
 * @Parm: ch:待发送的数据 
 * @Retval: 
 */
void USART1_Send_U8(uint8_t ch)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USART1, ch);
}

/**
 * @Brief: UART1发送数据
 * @Note: 
 * @Parm: BufferPtr:待发送的数据  Length:数据长度
 * @Retval: 
 */
void USART1_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length)
{
	while (Length--)
	{
		USART1_Send_U8(*BufferPtr);
		BufferPtr++;
	}
}

//串口中断服务函数
void USART1_IRQHandler(void)
{
	uint8_t Rx1_Temp = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Rx1_Temp = USART_ReceiveData(USART1);
		USART1_Send_U8(Rx1_Temp);
	}
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(DEBUG_USARTx, (uint8_t)ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET)
		;
	return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET)
		;
	return (int)USART_ReceiveData(DEBUG_USARTx);
}






//USART3
void USART3_init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);//部分重映像
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);  
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启接收中断       
	//USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_Cmd(USART3, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


//发送一个字符
void USART3_Send_U8(uint8_t ch)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USART3, ch);
}

//发送一个字符串
/**
 * @Brief: UsART3发送数据
 * @Note: 
 * @Parm: BufferPtr:待发送的数据  Length:数据长度
 * @Retval: 
 */
void USART3_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length)
{
	while (Length--)
	{
		USART3_Send_U8(*BufferPtr);
		BufferPtr++;
	}
}




//串口中断服务函数
void USART3_IRQHandler(void)
{
	uint8_t Rx3_Temp;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Rx3_Temp = USART_ReceiveData(USART3);
//		USART3_Send_U8(Rx3_Temp);
//    deal_bluetooth(Rx3_Temp);   //蓝牙处理
		//printf("%c",Rx3_Temp);
		Upper_Data_Receive(Rx3_Temp);      //接受K210数据
		if(Get_CMD_Flag())
		{
			deal_K210(Get_RxBuffer(), Get_CMD_Length());   //处理K210数据
			Clear_CMD_Flag();  //清楚标志位
		}
	}
}


//UART5
void UART5_init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE);
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure); 
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);  
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); //开启接收中断       
	//USART_ClearFlag(UART5,USART_FLAG_TC);
	USART_Cmd(UART5, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


//发送一个字符
void UART5_Send_U8(uint8_t ch)
{
	while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(UART5, ch);
}

//发送一个字符串
/**
 * @Brief: UsART3发送数据
 * @Note: 
 * @Parm: BufferPtr:待发送的数据  Length:数据长度
 * @Retval: 
 */
void UART5_Send_ArrayU8(uint8_t *BufferPtr, uint16_t Length)
{
	while (Length--)
	{
		UART5_Send_U8(*BufferPtr);
		BufferPtr++;
	}
}

//串口中断服务函数
void UART5_IRQHandler(void)
{
	uint8_t Rx5_Temp;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		Rx5_Temp = USART_ReceiveData(UART5);
//		UART5_Send_U8(Rx5_Temp);
//		printf("%c",Rx5_Temp);
		deal_bluetooth(Rx5_Temp);
	}
}


