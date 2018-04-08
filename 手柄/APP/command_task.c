/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : command_task.c
  版 本 号   : 初稿
  作    者   : LJM
  生成日期   : 2017.12.26
  最近修改   :
  功能描述   : 主控通信任务
               本系统编程时需要注意:
               >>凡是用了raw_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/

/* 内部宏定义 ----------------------------------------------------------------*/
//状态宏

/* 任务相关信息定义-----------------------------------------------------------*/
#define COMMAND_STK_SIZE		128
OS_TCB	COMMANDTaskTCB;
CPU_STK	COMMAND_TASK_STK[COMMAND_STK_SIZE];

/* 内部常量定义---------------------------------------------------------------*/
const char connect_1=0x21;
const char throw_1=0x22;
/* 外部变量声明 --------------------------------------------------------------*/
extern OS_TCB	THROWTaskTCB;
extern OS_ERR err;
extern volatile char Photoelectric_flag;
extern volatile char GET_flag;
extern THROW_INDEX TZ1; //TZ1参数
extern char to_connect_num;
extern char to_throw_num;
extern char DI2_FLAG;
extern volatile char begin_throw;
extern SERVO_MLCB UNDER2_MLCB;
/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/
extern uint8_t place;
uint8_t send_to_underpan[8];
int calibration_speed=0;
volatile char command_flag=0;
char display[2];
char last_command=0;  //用于记录上一次执行的任务
char cali_flag=0;
char connect_num=0;
/* 内部函数原型声明 ----------------------------------------------------------*/
/* 任务主体部分 --------------------------------------------------------------*/
static void command_task(void *p_arg)
{	
  OS_ERR command_err;

//	CPU_SR_ALLOC();
	while(1)
	{	 			       
	    p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告
			switch(command_flag){
			
			/*************等待状态******************************************************/	
				case _delay:   //等待决策
        if(GET_flag){GET_flag=0;
//				while(Photoelectric_switch)
//				{connect_num++;delay_us(100);
//					if(connect_num>50){RL1_OFF;GET_flag=0;while(Send_underpan(0x21))delay_ms(1);break;}
//				  }
				}
				
		 /*******************等待调度时所有电机应该都是停止状态***********************/
				break;
			
			/***********标定状态***********************/	
			  case _calibration:   //进行标定
        while(!motor_init_flag){delay_ms(10);}
				if(motor_init_flag){	
				Motor_Position_Mode(&UNDER2_MLCB);
				delay_us(600);
				Motor_AbsolutePosition_Set(&UNDER2_MLCB);
				delay_us(600);
				cali_flag=1;
				command_flag=_delay;}				
			 break;
				
			/***********交接状态**************************************************/	
			case _connect: //进行交接		            
				break;
			
			/***********抛掷状态*************************************************/	
			case _throw:
        while(TIM3->CCR1<1900)
					{ Motor_RelativePosition_Set(&UNDER2_MLCB,0);
					  TIM_PWM_Config(TIM3,2000 , 1990 , 1940 , 1960);	
					  BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);
					  BEED_ON;delay_ms(500);BEED_OFF;delay_ms(500);} //舵机限位保护
                                                          //抛掷区TZ1、TZ2、TZ3
			  OSTaskResume(&THROWTaskTCB,&err);//开始抛掷  
			  OS_TaskSuspend((OS_TCB*)&COMMANDTaskTCB,&command_err);		//挂起调度任务
				while(Send_underpan(0x22))delay_ms(5);
			  command_flag=_delay;
				last_command=_throw;
				break;
							
			/***********前往交接点*********************************************/	
			case _ready_connect:
        TIM_PWM_Config(TIM3, 1290, 1285, 1280, 1290);//两舵机合拢 导槽工作
        while(Motor_RelativePosition_Set(&UNDER2_MLCB,163580)){delay_ms(10);} //移动距离62.7CM
			  TIM_Cmd(TIM7, ENABLE);
			  last_command=_ready_connect;
        command_flag=_delay;
				break;
			
			/***********前往抛掷点******************************************/	
			case _ready_throw:			
			  TIM_PWM_Config(TIM3,2000 , 1990 , 1940 , 1960);	//两舵机分开 准备抛掷
				Motor_RelativePosition_Set(&UNDER2_MLCB,0);
			  last_command=_ready_throw;
			  if(begin_throw==1) {
				command_flag=_throw;begin_throw=0;
				BEED_ON;delay_ms(1000);BEED_OFF; }//改变延时的值决定什么时候抛掷 0为边走边抛
        else command_flag=_delay;
				break;
			
			/***********前往摄像头定位点1启动区定位**********************/		
			case _ready_location_one:
			  TIM_PWM_Config(TIM3,2000 , 1970 , 1940 , 1960);	//两舵机分开 
				Motor_RelativePosition_Set(&UNDER2_MLCB,68400); //20CM
			  last_command=_ready_location_one;
				command_flag=_delay;
				break;
			
			/***********前往摄像头定位点2抛掷定位***********************/				
			case _ready_location_two:
				Motor_RelativePosition_Set(&UNDER2_MLCB,115200); //30CM
			  if(last_command==_ready_connect){delay_ms(500);	TIM_PWM_Config(TIM3,1980 , 1990 , 1940 , 1960);	}//两舵机分开 
			  else TIM_PWM_Config(TIM3, 1290, 1285, 1280, 1290);//两舵机合拢 导槽工作
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


