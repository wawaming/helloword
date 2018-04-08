/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : key.c
  版 本 号   : 初稿
  作    者   : LJM
  生成日期   : 2017.12.26
  最近修改   :
  功能描述   : 按键任务
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
#define KEY_STK_SIZE		128
OS_TCB	KEYTaskTCB;
CPU_STK	KEY_TASK_STK[KEY_STK_SIZE];

/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/
extern volatile char command_flag;
/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/

/* 内部函数原型声明 ----------------------------------------------------------*/
/* 任务主体部分 --------------------------------------------------------------*/
extern int printf_speed[1000];
static void key_task(void *p_arg)
{	  	
	while(1)
	{ 			    
    CPU_SR_ALLOC();//使用临界区代码时需要
	  p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告		
   		 switch(Robot_Mode)
	 {
		 case MATCH_MODE:{
	                    switch(Key_Scan())
		                  {
			                      case KEY1:	{
																//Clear_LCD();
															  place=ONE;
		                     	}	break;			
			                      case KEY2:	{
																Clear_LCD();
																place=TWO;	
			                		}break;
			                      case KEY3:	{
															  Clear_LCD();
															  place=THREE;															
			                		}break;
		                      	case KEY4:	{
                                command_flag=_calibration;
	                				}break;
                      			case KEY5:	{
															for(int i_i=0;i_i<1000;i_i++){
															asy_printf("%d\n",printf_speed[i_i]);
															delay_ms(10);}
    	                		}break;
	                      		default:	break;
		                  }	
		                 }break;                       //比赛模式
		 case THROW_MODE:{
                       switch(Key_Scan())
		                  {
			                      case KEY1:	{
															           Clear_LCD();
															           place=ONE;
															           
		                     	}	break;			
			                      case KEY2:	{
															           Clear_LCD();
                                         place=TWO;
															
			                		}break;
			                      case KEY3:	{
															           Clear_LCD();
                                         place=THREE;														
			                		}break;
		                      	case KEY4:	{	
	                				}break;
                      			case KEY5:	{
    	                		}break;				
	                      		default:	break;
		                  }	
		                 }break;                        //抛掷模式
		 case CONNECT_MODE:{ 
		                    switch(Key_Scan())
		                   {
			                      case KEY1:	{
		                     	}	break;			
			                      case KEY2:	{		
			                		}break;
			                      case KEY3:	{						
			                		}break;
		                      	case KEY4:	{						
	                				}break;
                      			case KEY5:	{
    	                		}break;				
	                      		default:	break;
		                   }	
		                   }break;                      //交接模式
	 }   			
	//	LED2(1);LED3(0);delay_ms(100);	/*闪灯*/
		//LED2(0);LED3(1);delay_ms(100);		
	}
}

void create_key_task(uint8_t prio)
{
  OS_ERR  err;	
	OSTaskCreate((OS_TCB 	* )&KEYTaskTCB,		
				 (CPU_CHAR	* )"KEY test task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )prio,     	
                 (CPU_STK   * )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
}

/************/

