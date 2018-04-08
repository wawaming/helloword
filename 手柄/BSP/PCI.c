#include "PCI.h"
#include "app.h"
/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : PCI.c
  �� �� ��   : ����
  ��    ��   : LJM
  ��������   : 2017.12.26
  ����޸�   :
  ��������   : �����ʼ���ļ�
               ��ϵͳ���ʱ��Ҫע��:
               >>��������raw_printf()��task��һ��Ҫע��raw_printf����Ҫ��̬��������ģ�
                 ����task�������ջ����������Ҫ�����Ҫ128��256����
  �����б�   :
*******************************************************************************/
///////////////////////////////////////////////////////////
/************************ȫ�ֱ���************************/

uint8_t Robot_Mode=1;

///////////////////////////////////////////////////////////

/**
	*	@brief	���԰���ɨ�����		
						���԰����ǲ���Ҫ��ʼ���ģ����ⲿ�Ѿ�������Ӳ���������������������
						��Ϊio�ڲ���ʲô״̬�������Գ�ȥio�ĵ�ƽ���
	*	@param 	NONE
	* @retval NONE
	*/


void KEY_Init(void)
{
  GPIO_Config(GPIOF, GPIO_Pin_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_5, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_3, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
	GPIO_Config(GPIOF, GPIO_Pin_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);
}

uint8_t Key_Scan(void)
{
	if(!KEY1_CHECK)      return KEY1;
	else if(!KEY2_CHECK) return KEY2;
	else if(!KEY3_CHECK) return KEY3;
	else if(!KEY4_CHECK) return KEY4;
	else if(!KEY5_CHECK) return KEY5;
	else return 0;
}


/**
	*	@brief	����������	
	*	@param 	NONE
	* @retval NONE
	*/
void BEED_Init(void)
{
	GPIO_Config(GPIOD, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP);
	BEED_OFF;
}


/**
	*	@brief	��ˮ�Ƶ���	
	*	@param 	NONE
	* @retval NONE
	*/
void LED_Init(void)
{
	
	GPIO_Config(GPIOB, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);
	GPIO_Config(GPIOE, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);
	GPIO_Config(GPIOE, GPIO_Pin_0,  GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);
	GPIO_Config(GPIOC, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);

	LED0(0);
	LED1(0);
	LED2(0);
	LED3(0);
}

/**************************************************************************************
**
	*	@brief	Encoder_Init(void)
	*	@param 	NONE
	*	@supplement GPIOA0	GPIOA1	һ��		�ж� 0 2
								GPIOD12	GPIOD13	һ��		�ж� 0 1 
	* @retval NONE
	*
***************************************************************************************/
void Encoder_Init(void)
{	
	/*	TIM4�ı�����ģʽ	ͨ��1��2*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_Config(GPIOD,GPIO_Pin_12|GPIO_Pin_13,GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
//	TIM_ENCODER_Config(TIM4,0);
//	NVIC_Config(TIM4_IRQn, 0, 1);
	
	/*	TIM2�ı�����ģʽ	ͨ��1��2*/
	GPIO_Config(GPIOA, GPIO_Pin_0|GPIO_Pin_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	
	TIM_ENCODER_Config(TIM2,0);
	NVIC_Config(TIM2_IRQn, 0, 2);
	
	/*	TIM8�ı�����ģʽ	ͨ��1��2*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
	GPIO_Config(GPIOC, GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);
	TIM_ENCODER_Config(TIM8,0);
	NVIC_Config(TIM8_UP_TIM13_IRQn , 0, 3);
	
}


void TIM4_Init(void)
{

	TIM_Timing_Config(TIM4, 5, TIM_MS, ON);	
	NVIC_Config(TIM4_IRQn, 0, 1);
	TIM_Cmd(TIM4,ENABLE);
}

/***************************************************************************************
**
	*	@brief	TIM3_Init(void)
	*	@param
	*	@supplement	
	*	@retval	NONE
****************************************************************************************/
void TIM9_Init(void)
{
	TIM_Timing_Config(TIM9, 5, TIM_MS, ON);	
	NVIC_Config(TIM1_BRK_TIM9_IRQn, 0, 2);
	TIM_Cmd(TIM9, ENABLE);
}
/***************************************************************************************
**
	*	@brief	TIM5_Init(void)
	*	@param
	*	@supplement	
	*	@retval	NONE
****************************************************************************************/
void TIM5_Init(void)
{
	TIM_Timing_Config(TIM5, 20, TIM_MS, ON);	
	NVIC_Config(TIM5_IRQn, 0, 0);
	TIM_Cmd(TIM5, DISABLE);
}

/***************************************************************************************
**
	*	@brief	TIM7_Init(void)
	*	@param
	*	@supplement	
	*	@retval	NONE
****************************************************************************************/
void TIM7_Init(void)
{
	TIM_Timing_Config(TIM7, 800, TIM_MS, ON);	//
	NVIC_Config(TIM7_IRQn, 0, 3);
	TIM_Cmd(TIM7, DISABLE);
}


/**
	*	@brief  ���׵���
	*	@param 	NONE
	* @retval NONE
	*/
void RELAVY_Init(void)
{
	GPIO_Config(GPIOE, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������  �س�
	GPIO_Config(GPIOG, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������	 �ͷ�
	GPIO_Config(GPIOE, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������  װ��
	GPIO_Config(GPIOE, GPIO_Pin_8, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������
	GPIO_Config(GPIOB, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������
	GPIO_Config(GPIOB, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������
	GPIO_Config(GPIOB, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������
	GPIO_Config(GPIOF, GPIO_Pin_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������	
	GPIO_Config(GPIOF, GPIO_Pin_12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������
	GPIO_Config(GPIOF, GPIO_Pin_13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������	
	GPIO_Config(GPIOF, GPIO_Pin_14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������
	GPIO_Config(GPIOF, GPIO_Pin_15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);    //�����������ͨ�������	
}


/**
	*	@brief	��ʱ��1 ��ˢ��� pwm��� 
	*	@param 	NONE
	* @retval NONE
	*/
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  ������÷���ģʽ
	PS2_Cmd(0xEE); //Ox03�������ã�������ͨ��������MODE������ģʽ��
				   //0xEE������������ã���ͨ��������MODE������ģʽ��
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
/*��������ģʽ��*/
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
/*��������ģʽ��*/
void PS2_VibrationMode_on(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0XEE);//���С�������
	PS2_Cmd(0XEE);//�ұߴ���������ֵΪAA
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);	
}
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
	PS2_VibrationMode_on();	//������ģʽ
	PS2_ExitConfing();		//��ɲ���������
}
void Model_Aircraft_Init(void)
{
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_Config(GPIOA, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_DOWN);//�������DATA
	GPIO_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);//���CMD
	GPIO_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);//��������CS
	GPIO_Config(GPIOA, GPIO_Pin_11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_DOWN);//ʱ��CLK
	
	
	
//	TIM_Timing_Config(TIM1, 20000, TIM_US, ON);	
//	TIM_PWM_Config(TIM1,1400, 1000,  1000, 1000);
}
/*�������ֱ�����ģ��*/









/***************************************************************************************
**
	*	@brief	TIM3_Init(void)
	*	@param
	*	@supplement	����PWM�����,���ƶ��       50HZ��һͨ��
	*	@retval	������TIM3�����жϴ���
****************************************************************************************/
void SERVO_Init(void)
{
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_Config(GPIOB, GPIO_Pin_4, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//ͨ��1
	GPIO_Config(GPIOB, GPIO_Pin_5, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//2
	GPIO_Config(GPIOC, GPIO_Pin_8, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//3
	GPIO_Config(GPIOC, GPIO_Pin_9, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP);//4
	TIM_Timing_Config(TIM3, 20000, TIM_US,ON);	//20MS����
	TIM_PWM_Config(TIM3,2000 , 1970 , 1940 , 1960);	
}

/**
	*	@brief  ��� 
	*	@param 	NONE
	* @retval NONE
	*/
void DI_Init(void)
{
	GPIO_Config(GPIOG, GPIO_Pin_13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI6
	GPIO_Config(GPIOG, GPIO_Pin_15, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI7
	GPIO_Config(GPIOB, GPIO_Pin_8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI8
	GPIO_Config(GPIOD, GPIO_Pin_2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI1    //�����������������
	GPIO_Config(GPIOD, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI2
	GPIO_Config(GPIOD, GPIO_Pin_6, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI3
	GPIO_Config(GPIOG, GPIO_Pin_9, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI4
	GPIO_Config(GPIOG, GPIO_Pin_11, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP);//DI5
  Exti_config(GPIOG, EXTI_Line13|EXTI_Line15|EXTI_Line9|EXTI_Line11, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOB, EXTI_Line8, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOD, EXTI_Line4|EXTI_Line6, EXTI_Trigger_Rising_Falling);
	Exti_config(GPIOD, EXTI_Line2, EXTI_Trigger_Rising_Falling);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource13);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource15);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);
	NVIC_Config(EXTI15_10_IRQn, 2, 0);
	NVIC_Config(EXTI9_5_IRQn, 2,3);
	NVIC_Config(EXTI2_IRQn, 1, 2);
	NVIC_Config(EXTI4_IRQn, 2, 2);

}


/**
	*	@brief	ģʽѡ����
	*	@param 	NONE
	* @retval NONE
	*/
void Mode_Init(void)
{
	int mode_flag=0;
	while(mode_flag==0)
	{
		switch(Key_Scan())
		{
			case KEY1:	{
				           mode_flag=1;
				           Robot_Mode=MATCH_MODE;
		     	}	break;			
			case KEY2:	{
                 	 mode_flag=1;
                   Robot_Mode=THROW_MODE;
					}break;
			case KEY3:	{		
                 	 mode_flag=1;	
                   Robot_Mode=CONNECT_MODE;				
					}break;
			case KEY4:	{		
                  BEED_ON;delay_ms(1000);BEED_OFF;				
					}break;
			case KEY5:	{
				          BEED_ON;delay_ms(1000);BEED_OFF;
    			}break;				
			default:	break;
		}
     	LCD_Write_string("select the mode",1, 0);	
		  LCD_Write_string("KEY1 : match mode",1, 2);
		  LCD_Write_string("KEY2 : throw mode",1, 4);
		  LCD_Write_string("KEY3 : connect mode",1, 6);
    
	} 	
}



void BSP_Init(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     //�жϷ�������
  delay_init(168);                                	//ʱ�ӳ�ʼ��	
	//CAN_Config(CAN_Mode_Normal);			            //---CAN_Mode_LoopBack ----CAN_Mode_Normal
	//NVIC_Config(CAN1_RX0_IRQn, 2, 1);                   //---FIFO0 RX_Interrupt
 // IWDG_Init(IWDG_Prescaler_64,256);
//	Encoder_Init();
	//KEY_Init();
  LED_Init();	
  ROC_LCD_Init();
	BEED_Init();
	//A_Usart_Config();	  //��ͨ���ڳ�ʼ������
 //	TIM5_Init();  
 // TIM7_Init();
 // TIM4_Init();   
	//printf_uart_init();//�����첽����ʼ��
//	DI_Init();
	Model_Aircraft_Init();//�ֱ���ʼ��
	//SERVO_Init();
	PS2_SetInit();
//	RELAVY_Init();
	//RL1_ON;
	//Mode_Init();//ģʽѡ��
}
