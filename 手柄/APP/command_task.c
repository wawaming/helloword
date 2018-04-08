/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : command_task.c
  �� �� ��   : ����
  ��    ��   : LJM
  ��������   : 2017.12.26
  ����޸�   :
  ��������   : ����ͨ������
               ��ϵͳ���ʱ��Ҫע��:
               >>��������raw_printf()��task��һ��Ҫע��raw_printf����Ҫ��̬��������ģ�
                 ����task�������ջ����������Ҫ�����Ҫ128��256����
  �����б�   :
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"

/* �ڲ��Զ����������� --------------------------------------------------------*/

/* �ڲ��궨�� ----------------------------------------------------------------*/
//״̬��

/* ���������Ϣ����-----------------------------------------------------------*/
#define COMMAND_STK_SIZE		128
OS_TCB	COMMANDTaskTCB;
CPU_STK	COMMAND_TASK_STK[COMMAND_STK_SIZE];

/* �ڲ���������---------------------------------------------------------------*/
const char connect_1=0x21;
const char throw_1=0x22;
/* �ⲿ�������� --------------------------------------------------------------*/
extern OS_TCB	THROWTaskTCB;
extern OS_ERR err;
extern volatile char Photoelectric_flag;
extern volatile char GET_flag;
extern THROW_INDEX TZ1; //TZ1����
extern char to_connect_num;
extern char to_throw_num;
extern char DI2_FLAG;
extern volatile char begin_throw;
extern SERVO_MLCB UNDER2_MLCB;
/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/
extern uint8_t place;
uint8_t send_to_underpan[8];
int calibration_speed=0;
volatile char command_flag=0;
char display[2];
char last_command=0;  //���ڼ�¼��һ��ִ�е�����
char cali_flag=0;
char connect_num=0;
/* �ڲ�����ԭ������ ----------------------------------------------------------*/
/* �������岿�� --------------------------------------------------------------*/
static void command_task(void *p_arg)
{	
  OS_ERR command_err;

//	CPU_SR_ALLOC();
	while(1)
	{	 			       
	    p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���
			switch(command_flag){
			
			/*************�ȴ�״̬******************************************************/	
				case _delay:   //�ȴ�����
        if(GET_flag){GET_flag=0;
//				while(Photoelectric_switch)
//				{connect_num++;delay_us(100);
//					if(connect_num>50){RL1_OFF;GET_flag=0;while(Send_underpan(0x21))delay_ms(1);break;}
//				  }
				}
				
		 /*******************�ȴ�����ʱ���е��Ӧ�ö���ֹͣ״̬***********************/
				break;
			
			/***********�궨״̬***********************/	
			  case _calibration:   //���б궨
        while(!motor_init_flag){delay_ms(10);}
				if(motor_init_flag){	
				Motor_Position_Mode(&UNDER2_MLCB);
				delay_us(600);
				Motor_AbsolutePosition_Set(&UNDER2_MLCB);
				delay_us(600);
				cali_flag=1;
				command_flag=_delay;}				
			 break;
				
			/***********����״̬**************************************************/	
			case _connect: //���н���		            
				break;
			
			/***********����״̬*************************************************/	
			case _throw:
        while(TIM3->CCR1<1900)
					{ Motor_RelativePosition_Set(&UNDER2_MLCB,0);
					  TIM_PWM_Config(TIM3,2000 , 1990 , 1940 , 1960);	
					  BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);
					  BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);} //�����λ����
                                                          //������TZ1��TZ2��TZ3
			  OSTaskResume(&THROWTaskTCB,&err);//��ʼ����  
			  OS_TaskSuspend((OS_TCB*)&COMMANDTaskTCB,&command_err);		//�����������
				while(Send_underpan(0x22))delay_ms(5);
			  command_flag=_delay;
				last_command=_throw;
				break;
							
			/***********ǰ�����ӵ�*********************************************/	
			case _ready_connect:
        TIM_PWM_Config(TIM3, 1290, 1285, 1280, 1290);//�������£ ���۹���
        while(Motor_RelativePosition_Set(&UNDER2_MLCB,163580)){delay_ms(10);} //�ƶ�����62.7CM
			  TIM_Cmd(TIM7, ENABLE);
			  last_command=_ready_connect;
        command_flag=_delay;
				break;
			
			/***********ǰ��������******************************************/	
			case _ready_throw:			
			  TIM_PWM_Config(TIM3,2000 , 1990 , 1940 , 1960);	//������ֿ� ׼������
				Motor_RelativePosition_Set(&UNDER2_MLCB,0);
			  last_command=_ready_throw;
			  if(begin_throw==1) {
				command_flag=_throw;begin_throw=0;
				BEED_ON;delay_ms(1000);BEED_OFF; }//�ı���ʱ��ֵ����ʲôʱ������ 0Ϊ���߱���
        else command_flag=_delay;
				break;
			
			/***********ǰ������ͷ��λ��1��������λ**********************/		
			case _ready_location_one:
			  TIM_PWM_Config(TIM3,2000 , 1970 , 1940 , 1960);	//������ֿ� 
				Motor_RelativePosition_Set(&UNDER2_MLCB,68400); //20CM
			  last_command=_ready_location_one;
				command_flag=_delay;
				break;
			
			/***********ǰ������ͷ��λ��2������λ***********************/				
			case _ready_location_two:
				Motor_RelativePosition_Set(&UNDER2_MLCB,115200); //30CM
			  if(last_command==_ready_connect){delay_ms(500);	TIM_PWM_Config(TIM3,1980 , 1990 , 1940 , 1960);	}//������ֿ� 
			  else TIM_PWM_Config(TIM3, 1290, 1285, 1280, 1290);//�������£ ���۹���
			  last_command=_ready_location_two;
				command_flag=_delay;
				break;
			
			default:break;
		}
	 delay_ms(10);
  }
}

void create_command_task(uint8_t prio)
{
  OS_ERR  err;
	OSTaskCreate((OS_TCB 	* )&COMMANDTaskTCB,
				 (CPU_CHAR	* )"COMMAND test task", 		
                 (OS_TASK_PTR )command_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )prio,     	
                 (CPU_STK   * )&COMMAND_TASK_STK[0],	
                 (CPU_STK_SIZE)COMMAND_STK_SIZE/10,	
                 (CPU_STK_SIZE)COMMAND_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
}

/************/


