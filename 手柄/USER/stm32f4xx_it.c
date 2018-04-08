/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "app.h"
/** @addtogroup Template_Project
  * @{
  */
extern THROW_INDEX TZ1; //TZ1����
extern THROW_INDEX TZ2; //TZ2����
extern THROW_INDEX TZ3; //TZ3����

extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern OS_TCB	THROWTaskTCB;
extern volatile char command_flag;
int  Encoder_TIM2=0;
extern uint8_t throw_flag;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/*           �ⲿ�жϷ�����            */
void EXTI0_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line0);
		
	}
	OSIntExit();			//�˳��ٽ���
}

/****************���˶Թ�*****************/
/****************************************/
volatile char Photoelectric_flag=0;
volatile char GET_flag=0;
void EXTI2_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{
		/*	�û�����	*/
		/***********�Ѿ����ｻ��λ�ÿ�ʼ����****************/
		if(Photoelectric_flag==0){			
		if(Photoelectric_switch){RL1_OFF;GET_flag=1;Photoelectric_flag=1;while(Send_underpan(0x21));} //�Ѿ���⵽��������ɼ�ȡ
    else {Photoelectric_flag=0;}
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	OSIntExit();			//�˳��ٽ���
}


void EXTI3_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line3) != RESET)
	{ 
		/*	�û�����	*/
    		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	OSIntExit();			//�˳��ٽ���
}

 /***************�����㶨λ�г̿����г̿���*****************/
/*********************************************************/
extern int calibration_speed;
char DI2_FLAG=0;
void EXTI4_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{ 
		/*	�û�����	*/
		
		if(!travel_switch_throw){
     calibration_speed=0;
     DI2_FLAG=1;			
      //ƽ�ƻ���ֹͣ
     }
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	OSIntExit();			//�˳��ٽ���
}


 /****************���ӵ㶨λ�г̿��� �ⲿ�ж�6*****************/
/*************************************************************/
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	if(EXTI_GetFlagStatus(EXTI_Line5) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line5) ;
	}
	/****************���ӵ㶨λ�г̿���*****************/
/****************************************/
	if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
	{
		/*	�û�����	*/
		
		if(!travel_switch_connect){
   	calibration_speed=0;	
    command_flag=_delay;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetFlagStatus(EXTI_Line7) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	OSIntExit();			//�˳��ٽ���
}


/**
	*	@brief	EXTI15_10_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();		//�����жϴ������
	
	if(EXTI_GetFlagStatus(EXTI_Line10) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line11) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line12) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line12);		
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line13) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line14) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetFlagStatus(EXTI_Line15) != RESET)
	{
		/*	�û�����	*/
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
	OSIntExit();			//�˳��ٽ���
}








int _entercode_5;
void TIM5_IRQHandler(void)
{
	uint16_t i;
	OS_ERR err;
	OSIntEnter();		//�����жϴ������
	if(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)!=RESET)
	{		
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//		TZ1.control_num++;
//		if(TZ1.control_num>90)			{ throw_flag=1; TIM_Cmd(TIM5, DISABLE);	}
		switch(place)
		{
			case ONE:{
			if(TZ1.control_num<TZ1.control_final)			{throw_stop(place);IWDG_Feed();}
			else{
				   Motor_SetSpeed(&UNDER1_MLCB,0);			
           throw_flag=1;
				   _entercode_5=Encoder_TIM2*65536+TIM2->CNT;
           TIM_Cmd(TIM5, DISABLE);	TZ1.control_num=0;
		    	}
			}break;
			case TWO:{
				if(TZ2.control_num<TZ2.control_final){throw_stop(place);IWDG_Feed();}
		  	else{				  
				  Motor_SetSpeed(&UNDER1_MLCB,0);					
					TIM_Cmd(TIM5, DISABLE);
          throw_flag=1;_entercode_5=Encoder_TIM2*65536+TIM2->CNT;TZ2.control_num=0;		
			    }
			}break;
			case THREE:{
					if(TZ3.control_num<TZ3.control_final){throw_stop(place);IWDG_Feed();}
			else{			     
				   Motor_SetSpeed(&UNDER1_MLCB,0);	
				   throw_flag=1;
					 TIM_Cmd(TIM5, DISABLE);_entercode_5=Encoder_TIM2*65536+TIM2->CNT;				   			   
			}
			}break;
			default:{if(TZ1.control_num<TZ1.control_final)			{throw_stop(place);IWDG_Feed();}
			else{
				   Motor_SetSpeed(&UNDER1_MLCB,0);			
           throw_flag=1;_entercode_5=Encoder_TIM2*65536+TIM2->CNT;
           TIM_Cmd(TIM5, DISABLE);TZ3.control_num=0;	
		    	}
			}break;
		}
	}	
	
	OSIntExit();
}



void TIM7_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();		//�����жϴ������
	if(TIM_GetFlagStatus(TIM7,TIM_FLAG_Update)!=RESET)
	{	
		
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		Photoelectric_flag=1;
		TIM_Cmd(TIM7, DISABLE);
	}
	OSIntExit();
}



void USART1_IRQHandler(void)     
{
    
	OSIntEnter();		//�����жϴ������	
	/**********************��������ж�************************/
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{

		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);       
	}	
	OSIntExit();			//�˳��ٽ���
}
/************************** ����ͷ�жϽ��ճ���  **************************************/
void USART2_IRQHandler(void)
{
	
	OSIntEnter();		//�����жϴ������
	/**********************��������ж�************************/
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		{
			/*		�û�����		*/
  
		}
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
	}
	OSIntExit();			//�˳��ٽ���
}

/**
  * @brief	USART3_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */
extern HandlesStruct UART_CH;	//�ֱ����ݰ�
extern OS_SEM	Handle_Sem;			//�ֱ������ź���
void USART3_IRQHandler(void)
{
	uint8_t ch;
	static uint8_t i=0;
	static uint8_t step =0;
	static uint8_t usart_ch[4]={0};
	OS_ERR err;
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		{
			/*		�û�����		*/
		
		}
		USART_ClearITPendingBit(USART3, USART_IT_TC);
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		 RL2_ON;
		{
			/*		�û�����		*/
			ch = USART_ReceiveData(USART3);
			switch(step)
			{
				case 0:									{	
			/*	֡ͷ	*/								if(ch==0x40)		step++;
																}break;				
				case 1:									{
			/*	����	*/								if(ch==0x40)		;
																	else{
																		usart_ch[0] = ch;
																		step++;
																	}
																}break;				
				case 2:									{
			/*	����ҡ��	*/						if(ch==0x40)		step=1;
																	else{
																		usart_ch[1] = ch;
																		step++;
																	}
																}break;
				case 3:									{
			/*	֡β	*/								if(ch==0x40)			step=1;
																	else if(ch!=0x23)	step=0;
																	else if(ch==0x23)		
																	{
																		UART_CH.KEY = usart_ch[0];
																		UART_CH.LEFT_RIGHT =  usart_ch[1];
																		UART_CH.UP_DOWN    =  usart_ch[2];
																		UART_CH.BEFORE_RIGHT = usart_ch[3];
																		UART_CH.finish_flag = 1;//������Ч
																		OSSemPost(	(OS_SEM	*)&Handle_Sem,					//��Ҫ�ͷŵ��ź���
																								(OS_OPT	 )OS_OPT_POST_1,		//�ͷŸ��ȴ����ź����������о���������ȼ���
																								(OS_ERR	*)&err);						//������뷵��
																	}
																}break;				
				default:								{}break;
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}	
	OSIntExit();			//�˳��ٽ���
}

void UART4_IRQHandler(void)
{   
	OSIntEnter();		//�����жϴ������
	
	/**********************��������ж�************************/
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		{
			/*		�û�����		*/
		}
		
	}
	/**********************��������ж�*****************************/
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	
               
    }
	OSIntExit();			//�˳��ٽ���
}

int Encoder,lat_Encoder=0;
int real_speed=0;
int printf_speed[1000]={0};
int printf_i=0;
int printf_flag=0;





/**
	*	@brief	void TIM2_IRQHandler()
	*	@param	NONE
	*	@retval NONE
	*/

void TIM2_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		/*	�û�����	*/
		if(TIM2->CNT==65535)
		{
			Encoder_TIM2--;
		}
		if(TIM2->CNT==0)
		{
			Encoder_TIM2++;
		}
	}
	OSIntExit();
}


/**
	*	@brief	void TIM4_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
int32_t Encoder_TIM4=0;			//ͨ��TIM4������ģʽ��׽�ļ���
void TIM4_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		/*	�û�����	*/
		Encoder=Encoder_TIM2*65536 + TIM2->CNT-lat_Encoder;
		lat_Encoder=Encoder_TIM2*65536 + TIM2->CNT;
		real_speed=Encoder*200*60/(4000*16);
		if(printf_flag==1&&printf_i<1000){
		printf_speed[printf_i++]=real_speed;}
		else {
		printf_i=0;}
	}
	OSIntExit();
}


int Encoder_TIM8=0;			//ͨ��TIM4������ģʽ��׽�ļ���
void TIM8_UP_TIM13_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(TIM_GetFlagStatus(TIM8,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM8, TIM_FLAG_Update);
		/*	�û�����	*/
		if(TIM8->CNT==65535)
		{
			Encoder_TIM8--;
		}
		if(TIM8->CNT==0)
		{
			Encoder_TIM8++;
		}
	}
	
	
	OSIntExit();
}

/**********************************************************************************************************/
/*																*	CAN_HANDLER	*																													*/
/**********************************************************************************************************/
/**
	*
	*/
extern char display[2];
CanRxMsg underpan_RxMsg;
volatile char begin_throw=0;

void CAN1_RX0_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		if(CAN_Receive_Msg(&underpan_RxMsg,CAN_Filter_FIFO0)){
    display[1]=underpan_RxMsg.Data[1];
    display[0]=underpan_RxMsg.Data[0];
			switch(display[0]){
				case 0x11:{command_flag=_ready_connect;}break;//�ƶ�������λ�� 
				case 0x12:{command_flag=_ready_location_two;}break;//�ƶ�����λλ��   
				case 0x13:{command_flag=_ready_location_one;}break;//�ƶ�����λλ�� 3 TZ1 5 6 TZ2  8 TZ3  
				case 0x14:{command_flag=_ready_throw;
					switch(display[1]){
			      case 3:{begin_throw=1;place=ONE;}break;//TZ1����ʼ����
				    case 5:{begin_throw=1;place=TWO;}break;//TZ2����ʼ���� 
				    case 6:{begin_throw=1;place=TWO;}break;//TZ2����ʼ����
				    case 8:{begin_throw=1;place=THREE;}break;//TZ3����ʼ����
				    default:{begin_throw=0;}break;//���������� �ȴ�����
				    }
				  }break;//�ƶ���Ͷ��λ�� 
				case 0x15:{command_flag=_connect;}break;//���� 21
				case 0x16:{command_flag=_throw;}break;//Ͷ��  22
				default:{command_flag=_delay;}break;//������Ϣ �����ȴ� 
			}
	}
}
	OSIntExit();
}


void CAN1_SCE_IRQHandler(void)
{
	OSIntEnter();					//�����жϴ������
	if(CAN_GetITStatus(CAN1,CAN_IT_BOF ) != RESET)	 /* Bus-off */
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF );
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_EWG ) != RESET)	 /* Error warning */
	{	
		CAN_ClearITPendingBit(CAN1, CAN_IT_EWG );
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_EPV) != RESET)  	 /* Error passive */
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_EPV );
	}	
	OSIntExit();	
}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/









