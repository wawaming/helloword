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
	*@brief void Usart_Config()	��������
	*@param	USART_TypeDef* USARTx		��Ҫ��ʼ���Ĵ���
					uint32_t bound					���ڲ�����
	*@supplement		���������Ҫʹ���жϺ���,���������ж����ȼ�
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
	
	USART_DeInit(USARTx);						//��λ����,һ�������,��Ƭ����ʼ������ʱ,����Ҫ��λ����
	USART_InitStruct.USART_BaudRate = bound;		//����������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//�շ�ģʽ(ȫ˫��ģʽ)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //�ر���żУ��λ
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //һ��ֹͣλ
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8λ����
	USART_Init(USARTx,&USART_InitStruct);
	
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);		//�򿪴��ڽ����ж�
	USART_Cmd(USARTx,ENABLE);												//ʹ�ܴ���
}
/**
	*	@brief	fputc
	*	@param	NONE
	*	@retval NONE
*/

/***************************************************�û��Զ���ģ��**********************************************/
/***************************************************�û��Զ���ģ��**********************************************/
/***************************************************�û��Զ���ģ��**********************************************/
/**	
	*	@brief	A�����ڳ�ʼ��
	*	@supplement	A����������
								USART1:				TX1(PA9)		RX1(PA10)				Default			
								UART2	:				TX2(PA2)		RX2(PA3)				Default
								UART3	:				TX3(PB10)		RX3(PB11)				Default
								UART4	:				TX4(PA0)		RX4(PA1)					Default
								USART6:				TX6(PC6)		RX6(PC7)				Default
�ж�˵��	��	  USART3	 1 2 				UART4		0  2
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
	*	���ж�								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
   	GPIO_Config(GPIOB, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOB, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	USART_DeInit(USART3);						//��λ����
	USART_InitStruct.USART_BaudRate = 115200;		//����������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//�շ�ģʽ(ȫ˫��ģʽ)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //�ر���żУ��λ
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //һ��ֹͣλ
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8λ����
	USART_Init(USART3,&USART_InitStruct);
	
	NVIC_Config(USART3_IRQn, 1, 1);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		//�򿪴��ڽ����ж�
	USART_Cmd(USART3,ENABLE);												//ʹ�ܴ���
#endif
		
#if			UART4_OPEN
	/********************		UART4_INIT      		***********************
	*	baud	115200					*
	*	���ж�								*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);	
	
	GPIO_Config(GPIOC, GPIO_Pin_10, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOC, GPIO_Pin_11, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	USART_DeInit(UART4);						//��λ����
	USART_InitStruct.USART_BaudRate = 115200;		//����������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;		//�շ�ģʽ(ȫ˫��ģʽ)
	USART_InitStruct.USART_Parity     =   USART_Parity_No;			  //�ر���żУ��λ
	USART_InitStruct.USART_StopBits   =   USART_StopBits_1;		    //һ��ֹͣλ
	USART_InitStruct.USART_WordLength =   USART_WordLength_8b;		//8λ����
	USART_Init(UART4,&USART_InitStruct);
	
	NVIC_Config(UART4_IRQn, 0, 2);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);		//�򿪴��ڽ����ж�
	USART_Cmd(UART4,ENABLE);												//ʹ�ܴ���
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

  // ����F4��RXD��Ҫ��ΪAF������Ϊ����
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


