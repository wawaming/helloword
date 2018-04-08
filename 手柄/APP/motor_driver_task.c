/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny

 *******************************************************************************
  文 件 名   : motor_driver_task.c
  版 本 号   : 初稿
  作    者   : LJM
  生成日期   : 2017.12.26
  最近修改   :
  功能描述   : 电机驱动管理任务
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/

/* 内部宏定义 ----------------------------------------------------------------*/
/******底盘电机********/
//---电机1
#define DEVICE1_ID	    9
#define RATIO1		    	16
#define CODER1			    1000
//---电机2	
#define DEVICE2_ID    	12
#define RATIO2		    	14
#define CODER2		    	1000
//---电机3	
#define DEVICE3_ID	    6
#define RATIO3		    	14
#define CODER3		    	1000
////---电机4	
//#define DEVICE4_ID	    8
//#define RATIO4			    14
//#define CODER4			    1000

/* 任务相关信息定义-----------------------------------------------------------*/
OS_TCB  motor_driver_TaskTCB;							//任务控制块
#define motor_driver_STK_SIZE		256					//任务堆栈大小
CPU_STK motor_driver__STK[motor_driver_STK_SIZE];	

/* 内部常量定义---------------------------------------------------------------*/
//	定义移动电机控制块
SERVO_MLCB UNDER1_MLCB;			//第一象限电机
SERVO_MLCB UNDER2_MLCB;			//第二象限电机
SERVO_MLCB UNDER3_MLCB;			//第三象限电机
SERVO_MLCB UNDER4_MLCB;			//第四象限电机

//移动电机最大速度、加速度限制
const int motor_move_max_rpm = 500;  //
const int motor_move_acc = 300;

int sppped1=0;
int sppped2=0;
int sppped3=0;

//调试变量
int t_speed =0;

/* 内部函数原型声明 ----------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/

/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/
uint8_t motor_init_flag = 0;

/* 内部功能函数原型 ----------------------------------------------------------*/

//设置电机速度
void motor_move_1(int move_speed)
{
    Motor_SetSpeed(&UNDER1_MLCB,move_speed);
    delay_us(600);
}
void motor_move_2(int move_speed)
{
    Motor_SetSpeed(&UNDER2_MLCB,move_speed);
    delay_us(600);
}
void motor_move_3(int move_speed)
{
    Motor_SetSpeed(&UNDER3_MLCB,move_speed);
    delay_us(600);
}
void motor_move_4(int move_speed)
{
    Motor_SetSpeed(&UNDER4_MLCB,move_speed);
    delay_us(600);
}

void motor_move_setvmmps(int dstVmmps_X,int dstVmmps_Y,int dstVmmps_W)
{
    int wheel1,wheel2,wheel3,wheel4;
    
	wheel1 = (int32_t)(-dstVmmps_X + dstVmmps_Y + dstVmmps_W);
	wheel2 = (int32_t)(-(dstVmmps_X + dstVmmps_Y - dstVmmps_W));
	wheel3 = (int32_t)(-(-dstVmmps_X + dstVmmps_Y - dstVmmps_W));
	wheel4 = (int32_t)(dstVmmps_X + dstVmmps_Y + dstVmmps_W);
    
    if(wheel1 >= motor_move_max_rpm )   wheel1 = motor_move_max_rpm;
    else if(wheel1 <= -motor_move_max_rpm )  wheel1 = -motor_move_max_rpm;
    
    if(wheel2 >= motor_move_max_rpm )   wheel2 = motor_move_max_rpm;
    else if(wheel2 <= -motor_move_max_rpm )  wheel2 = -motor_move_max_rpm; 
    
    if(wheel3 >= motor_move_max_rpm )   wheel3 = motor_move_max_rpm;
    else if(wheel3 <= -motor_move_max_rpm )  wheel3 = -motor_move_max_rpm;
    
    if(wheel4 >= motor_move_max_rpm )   wheel4 = motor_move_max_rpm;
    else if(wheel4 <= -motor_move_max_rpm )  wheel4 = -motor_move_max_rpm;
    
    motor_move_1(wheel1);
    motor_move_2(wheel2);
    motor_move_3(wheel3);
    motor_move_4(wheel4);
}
void motor_move_stop(void)
{    
    motor_move_1(0);
    motor_move_2(0);
    motor_move_3(0);
    motor_move_4(0); 
}
void motor_all_Init(void)
{
	/*	底盘电机控制块设置	*/
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER1_MLCB,					
						 (char   	*)"underpan_motor1",			
						 (uint8_t      )DEVICE1_ID,											
						 (uint8_t      )RATIO1,								
						 (uint16_t     )CODER1);
	
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER2_MLCB,					
						 (char   	*)"underpan_motor2",			
						 (uint8_t      )DEVICE2_ID,											
						 (uint8_t      )RATIO2,								
						 (uint16_t     )CODER2);
	
	Create_MLServo_CB(	 (SERVO_MLCB	*)&UNDER3_MLCB,					
						 (char   	*)"underpan_motor3",			
						 (uint8_t      )DEVICE3_ID,											
						 (uint8_t      )RATIO3,								
						 (uint16_t     )CODER3);
 
}


/* 任务主体部分 --------------------------------------------------------------*/
static void motor_driver_task(void *p_arg)
{ 	
	  p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告	    
    motor_all_Init();    
    motor_init_flag = 1;
    BEED_ON;delay_ms(100);BEED_OFF;delay_ms(100);
	  BEED_ON;delay_ms(100);BEED_OFF;delay_ms(100);
	while(1)  
	{			
       	LED0(1);LED1(0);delay_ms(100);
		    LED0(0);LED1(1);delay_ms(100);	
	}
}
void  create_motor_driver_task(uint8_t prio)
{
    OS_ERR  err;
	OSTaskCreate ((OS_TCB    *) &motor_driver_TaskTCB,			//任务控制块
								(CPU_CHAR  *)   "motor_driver_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR)    motor_driver_task,				//任务函数
								(void      *)    0,								//传入函数的参数
								(OS_PRIO    )    prio,          	//任务的优先级
								(CPU_STK   *)   &motor_driver__STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   motor_driver_STK_SIZE/10,//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   motor_driver_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,								//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,								//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,								//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);							//指定一个变量存储错误码
}

/************** 版权所有 (C), 2017-, Mushiny **********本文件结束*************/

