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
extern THROW_INDEX TZ1; //TZ1参数
extern THROW_INDEX TZ2; //TZ2参数
extern THROW_INDEX TZ3; //TZ3参数

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


/*           外部中断服务函数            */
void EXTI0_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/*	用户函数	*/
		EXTI_ClearITPendingBit(EXTI_Line0);
		
	}
	OSIntExit();			//退出临界区
}

/****************光纤对管*****************/
/****************************************/
volatile char Photoelectric_flag=0;
volatile char GET_flag=0;
void EXTI2_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{
		/*	用户函数	*/
		/***********已经到达交接位置开始交接****************/
		if(Photoelectric_flag==0){			
		if(Photoelectric_switch){RL1_OFF;GET_flag=1;Photoelectric_flag=1;while(Send_underpan(0x21));} //已经检测到绳并且完成夹取
    else {Photoelectric_flag=0;}
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	OSIntExit();			//退出临界区
}


void EXTI3_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line3) != RESET)
	{ 
		/*	用户函数	*/
    		
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	OSIntExit();			//退出临界区
}

 /***************抛掷点定位行程开关行程开关*****************/
/*********************************************************/
extern int calibration_speed;
char DI2_FLAG=0;
void EXTI4_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line4) != RESET)
	{ 
		/*	用户函数	*/
		
		if(!travel_switch_throw){
     calibration_speed=0;
     DI2_FLAG=1;			
      //平移机构停止
     }
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	OSIntExit();			//退出临界区
}


 /****************交接点定位行程开关 外部中断6*****************/
/*************************************************************/
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	if(EXTI_GetFlagStatus(EXTI_Line5) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line5) ;
	}
	/****************交接点定位行程开关*****************/
/****************************************/
	if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
	{
		/*	用户代码	*/
		
		if(!travel_switch_connect){
   	calibration_speed=0;	
    command_flag=_delay;
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetFlagStatus(EXTI_Line7) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)
	{
		/*	用户函数	*/
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	OSIntExit();			//退出临界区
}


/**
	*	@brief	EXTI15_10_IRQHandler()
	*	@param	NONE
	*	@retval	NONE
	*/
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();		//进入中断处理程序
	
	if(EXTI_GetFlagStatus(EXTI_Line10) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line11) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line12) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line12);		
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line13) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line14) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetFlagStatus(EXTI_Line15) != RESET)
	{
		/*	用户代码	*/
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
	OSIntExit();			//退出临界区
}








int _entercode_5;
void TIM5_IRQHandler(void)
{
	uint16_t i;
	OS_ERR err;
	OSIntEnter();		//进入中断处理程序
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
	OSIntEnter();		//进入中断处理程序
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
    
	OSIntEnter();		//进入中断处理程序	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{

		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);       
	}	
	OSIntExit();			//退出临界区
}
/************************** 摄像头中断接收程序  **************************************/
void USART2_IRQHandler(void)
{
	
	OSIntEnter();		//进入中断处理程序
	/**********************发送完成中断************************/
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		{
			/*		用户函数		*/
  
		}
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
	}
	OSIntExit();			//退出临界区
}

/**
  * @brief	USART3_IRQHandler
	*	@param	NONE
	*	@retval	NONE
  */
extern HandlesStruct UART_CH;	//手柄数据包
extern OS_SEM	Handle_Sem;			//手柄任务信号量
void USART3_IRQHandler(void)
{
	uint8_t ch;
	static uint8_t i=0;
	static uint8_t step =0;
	static uint8_t usart_ch[4]={0};
	OS_ERR err;
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		{
			/*		用户函数		*/
		
		}
		USART_ClearITPendingBit(USART3, USART_IT_TC);
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		 RL2_ON;
		{
			/*		用户函数		*/
			ch = USART_ReceiveData(USART3);
			switch(step)
			{
				case 0:									{	
			/*	帧头	*/								if(ch==0x40)		step++;
																}break;				
				case 1:									{
			/*	按键	*/								if(ch==0x40)		;
																	else{
																		usart_ch[0] = ch;
																		step++;
																	}
																}break;				
				case 2:									{
			/*	左右摇杆	*/						if(ch==0x40)		step=1;
																	else{
																		usart_ch[1] = ch;
																		step++;
																	}
																}break;
				case 3:									{
			/*	帧尾	*/								if(ch==0x40)			step=1;
																	else if(ch!=0x23)	step=0;
																	else if(ch==0x23)		
																	{
																		UART_CH.KEY = usart_ch[0];
																		UART_CH.LEFT_RIGHT =  usart_ch[1];
																		UART_CH.UP_DOWN    =  usart_ch[2];
																		UART_CH.BEFORE_RIGHT = usart_ch[3];
																		UART_CH.finish_flag = 1;//数据有效
																		OSSemPost(	(OS_SEM	*)&Handle_Sem,					//需要释放的信号量
																								(OS_OPT	 )OS_OPT_POST_1,		//释放给等待此信号量的任务中具有最高优先级者
																								(OS_ERR	*)&err);						//错误代码返回
																	}
																}break;				
				default:								{}break;
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}	
	OSIntExit();			//退出临界区
}

void UART4_IRQHandler(void)
{   
	OSIntEnter();		//进入中断处理程序
	
	/**********************发送完成中断************************/
	if(USART_GetITStatus(UART4, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_TC);
		{
			/*		用户函数		*/
		}
		
	}
	/**********************接收完成中断*****************************/
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	
               
    }
	OSIntExit();			//退出临界区
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
	OSIntEnter();					//进入中断处理程序
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		/*	用户函数	*/
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
int32_t Encoder_TIM4=0;			//通过TIM4编码器模式捕捉的计数
void TIM4_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		/*	用户函数	*/
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


int Encoder_TIM8=0;			//通过TIM4编码器模式捕捉的计数
void TIM8_UP_TIM13_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
	if(TIM_GetFlagStatus(TIM8,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearFlag(TIM8, TIM_FLAG_Update);
		/*	用户函数	*/
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
	OSIntEnter();					//进入中断处理程序
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		if(CAN_Receive_Msg(&underpan_RxMsg,CAN_Filter_FIFO0)){
    display[1]=underpan_RxMsg.Data[1];
    display[0]=underpan_RxMsg.Data[0];
			switch(display[0]){
				case 0x11:{command_flag=_ready_connect;}break;//移动到交接位置 
				case 0x12:{command_flag=_ready_location_two;}break;//移动到定位位置   
				case 0x13:{command_flag=_ready_location_one;}break;//移动到定位位置 3 TZ1 5 6 TZ2  8 TZ3  
				case 0x14:{command_flag=_ready_throw;
					switch(display[1]){
			      case 3:{begin_throw=1;place=ONE;}break;//TZ1区开始抛掷
				    case 5:{begin_throw=1;place=TWO;}break;//TZ2区开始抛掷 
				    case 6:{begin_throw=1;place=TWO;}break;//TZ2区开始抛掷
				    case 8:{begin_throw=1;place=THREE;}break;//TZ3区开始抛掷
				    default:{begin_throw=0;}break;//不进行抛掷 等待命令
				    }
				  }break;//移动到投掷位置 
				case 0x15:{command_flag=_connect;}break;//交接 21
				case 0x16:{command_flag=_throw;}break;//投掷  22
				default:{command_flag=_delay;}break;//错误消息 继续等待 
			}
	}
}
	OSIntExit();
}


void CAN1_SCE_IRQHandler(void)
{
	OSIntEnter();					//进入中断处理程序
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









