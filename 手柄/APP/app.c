/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : main.c
  版 本 号   : 初稿
  作    者   : LJM
  生成日期   : 2017.12.26
  最近修改   :
  功能描述   : 起始任务
               本系统编程时需要注意:
               >>凡是用了raw_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
							 临界区内不能使用延时
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/
/* 内部宏定义 ----------------------------------------------------------------*/

/* 任务相关信息定义-----------------------------------------------------------*/
#define STAR_STK_SIZE		128					//任务堆栈大小
OS_TCB StarTaskTCB;									//任务控制块
CPU_STK STAR_TASK_STK[STAR_STK_SIZE];			
/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/

/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/

/* 内部函数原型声明 ----------------------------------------------------------*/

/* 任务主体部分 --------------------------------------------------------------*/
void star_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();	
	CPU_Init();
	p_arg = p_arg;//防止编译器报警
	OS_CRITICAL_ENTER();	//进入临界区

    /*  创建任务区  */
	 switch(Robot_Mode)
	 {
		 case MATCH_MODE:{
	                   // create_IWDG_task(4);         //看门狗中断任务，防止系统崩溃带来的意外
			              //  create_command_task(5);      //决策任务，主要用于与各模块通信
			              //  create_throw_task(6);        //抛掷任务
			              // create_connect_task(7);      //交接任务
	                 //   create_motor_driver_task(8); //电机任务
                      create_hand_task(5);         //手柄任务
	                    create_key_task(10);	       //按键任务
	                    create_display_task(6);	   //显示任务
                  //    retarget_init();             //串口异步化任务
		                 }break;                       //创建比赛模式任务
		 case THROW_MODE:{
			                create_IWDG_task(4);
			               // create_throw_task(5);
	                  //  create_motor_driver_task(6);
                      create_hand_task(7);
	                    create_key_task(8);
			                create_display_task(9);
			                retarget_init();
		                 }break;                        //创建抛掷模式任务
		 case CONNECT_MODE:{ 
			                  create_IWDG_task(4);
			                  create_connect_task(5);
	                      //create_motor_driver_task(6);	  
                        create_hand_task(7);      
	                      create_key_task(8);
			                  create_display_task(9);			                  
                        retarget_init();
		                   }break;                      //创建交接模式任务
	 }
	OS_CRITICAL_EXIT();		//退出临界区
	OS_TaskSuspend((OS_TCB*)&StarTaskTCB,&err);		//挂起开始任务	    
}

void create_star_task(uint8_t prio)
{
    OS_ERR  err;
	OSTaskCreate ((OS_TCB    *)   &StarTaskTCB,			//任务控制块
								(CPU_CHAR  *)   "star_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR)    star_task,				//任务函数
								(void      *)    0,								//传入函数的参数
								(OS_PRIO    )    prio,	//任务的优先级
								(CPU_STK   *)    &STAR_TASK_STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   STAR_STK_SIZE/10,//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   STAR_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,								//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,								//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,								//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);							//指定一个变量存储错误码

}

/************** 版权所有 (C), 2017-, Mushiny **********本文件结束*************/
