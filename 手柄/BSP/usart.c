/*************************************************************************************
*	@file			usart.c
* @author	 	yun
*	@supplement		
*	@version 	V1.0
*	@date			2016/5/27
* @brief		NONE
*************************************************************************************/

#include "usart.h"
/**
	*@brief void Usart_Config()	串口配置
	*@param	USART_TypeDef* USARTx		需要初始化的串口
					uint32_t bound					串口波特率
	*@supplement		如果串口需要使能中断函数,还需配置中断优先级
	*@retval NONE
	*/
void Usart_Config(USART_TypeDef* USARTx, uint32_t bound)
{
	USART_InitTypeDef USART_InitStruct;
	
	if(USARTx==USART1){	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		
	//	GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	}
	if(USARTx==USART2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		
		GPIO_Config(GPIOA, GPIO_Pin_2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOA, GPIO_Pin_3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	}
	if(USARTx==USART3){	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		
		GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
		GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);

	}
	
	USART_DeInit(USARTx);						//复位串口,一般情况下,单片机初始化配置时,都需要复位外设
	USART_InitStruct.USART_BaudRate = bound;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(USARTx,&USART_InitStruct);
	
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(USARTx,ENABLE);												//使能串口
}
/**
	*	@brief	fputc
	*	@param	NONE
	*	@retval NONE
*/

/***************************************************用户自定义模块**********************************************/
/***************************************************用户自定义模块**********************************************/
/***************************************************用户自定义模块**********************************************/
/**	
	*	@brief	A车串口初始化
	*	@supplement	A车串口数据
								USART1:				TX1(PA9)		RX1(PA10)				Default			
								UART2	:				TX2(PA2)		RX2(PA3)				Default
								UART3	:				TX3(PB10)		RX3(PB11)				Default
								UART4	:				TX4(PA0)		RX4(PA1)					Default
								USART6:				TX6(PC6)		RX6(PC7)				Default
中断说明	：	  USART3	 1 2 				UART4		0  2
	*	@param	NONE
	*	@retval	NONE
	*/
#define		USART1_OPEN	 0
#define		USART2_OPEN	 0
#define		USART3_OPEN	 1
#define		UART4_OPEN	 0
void A_Usart_Config(void)
{
	USART_InitTypeDef USART_InitStruct;

#if			USART3_OPEN
	/********************		UART3_INIT      		***********************
	*	baud	115200					*
	*	开中断								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
   	GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	USART_DeInit(USART3);						//复位串口
	USART_InitStruct.USART_BaudRate = 115200;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(USART3,&USART_InitStruct);
	
	NVIC_Config(USART3_IRQn, 1, 1);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(USART3,ENABLE);												//使能串口
#endif
		
#if			UART4_OPEN
	/********************		UART4_INIT      		***********************
	*	baud	115200					*
	*	开中断								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);	
	
	GPIO_Config(GPIOC, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOC, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	USART_DeInit(UART4);						//复位串口
	USART_InitStruct.USART_BaudRate = 115200;		//波特率设置
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//收发模式(全双工模式)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //关闭奇偶校验位
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //一个停止位
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8位数据
	USART_Init(UART4,&USART_InitStruct);
	
	NVIC_Config(UART4_IRQn, 0, 2);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);		//打开串口接收中断
	USART_Cmd(UART4,ENABLE);												//使能串口
#endif
}

 void printf_uart_init(void)
{

	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* Enable USART clock */
  RCC_APB1PeriphClockCmd(PrintfUART_RCC, ENABLE);

	/* Connect USART pins to AF */
  GPIO_PinAFConfig(PrintfUART_TXD_GPIO, mylib_getbitId(PrintfUART_TXD_PIN, 2), PrintfUART_AF);
  GPIO_PinAFConfig(PrintfUART_RXD_GPIO, mylib_getbitId(PrintfUART_RXD_PIN, 2), PrintfUART_AF);

	 /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Pin = PrintfUART_TXD_PIN;
  GPIO_Init(PrintfUART_TXD_GPIO, &GPIO_InitStructure);

  // 对于F4，RXD需要设为AF不能设为输入
  GPIO_InitStructure.GPIO_Pin = PrintfUART_RXD_PIN;
  GPIO_Init(PrintfUART_RXD_GPIO, &GPIO_InitStructure);
	
	  /* Enable the USART OverSampling by 8 */
//  USART_OverSampling8Cmd(PrintfUART, ENABLE);  
	
	
	USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = PrintfUART_BAUD;
  USART_Init(PrintfUART, &USART_InitStructure);
  
	
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = PrintfUART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(PrintfUART, USART_IT_RXNE, DISABLE);
  /* Enable USART */
  USART_Cmd(PrintfUART, ENABLE);
	

}


