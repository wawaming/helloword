/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : display.c
  版 本 号   : 初稿
  作    者   : LJM
  生成日期   : 2017.12.26
  最近修改   :
  功能描述   : 状态显示任务
               本系统编程时需要注意:
               >>凡是用了raw_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/
/* 内部宏定义 ----------------------------------------------------------------*/

/* 任务相关信息定义-----------------------------------------------------------*/
#define DISPLAY_STK_SIZE		128
OS_TCB	DISPLAYTaskTCB;
CPU_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];

/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/
extern int Encoder_TIM2;
extern THROW_INDEX TZ1; //TZ1参数
extern THROW_INDEX TZ2; //TZ2参数
extern THROW_INDEX TZ3; //TZ3参数
extern int angle,past_cycle;
extern int real_speed;
extern char USART3_Flag;
extern volatile char command_flag;
extern volatile char Photoelectric_flag;
extern volatile char GET_flag;
extern int display[8];
extern int TIM3_CCR1;
extern int RL_NUM;
extern int _entercode_3,_entercode_4,_entercode_5;
extern char cali_flag;
extern int Encoder;
extern int test_hand,test_num;
extern 	u16 Handba_RX,Handba_RY,Handba_LX,Handba_LY;
/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/

/* 内部函数原型声明 ----------------------------------------------------------*/
/* 任务主体部分 --------------------------------------------------------------*/
static void display_task(void *p_arg)
{	  
  Clear_LCD();	
	while(1)
	{	 			        
	 p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告
	
					LCD_Write_string("  test_hand",0, 0);
					LCD_Display_int(test_hand,15,0);			
					LCD_Write_string("  test_num",0, 1);
					LCD_Display_int(test_num,15,1);
					LCD_Write_string("  Handba_RX",0, 2);
					LCD_Display_int(Handba_RX,15,2); 		
					LCD_Write_string("  Handba_RY",0, 3);
					LCD_Display_int(Handba_RY,15,3);
					LCD_Write_string("  Handba_LX",0, 4);
					LCD_Display_int(Handba_LX,15,4);	
          LCD_Write_string("  Handba_LY",0, 5);
					LCD_Display_int(Handba_LY,15,5);		
	 delay_ms(10);
  }
}

void create_display_task(uint8_t prio)
{
  OS_ERR  err;	
	OSTaskCreate((OS_TCB 	* )&DISPLAYTaskTCB,		
				 (CPU_CHAR	* )"DISPALY test task", 		
                 (OS_TASK_PTR )display_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )prio,     	
                 (CPU_STK   * )&DISPLAY_TASK_STK[0],	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE/10,	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
}

/************/