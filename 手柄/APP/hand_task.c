/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : hand_task.c
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2017.11.5 星期六
  最近修改   :
  功能描述   : 无线手柄任务
               本系统编程时需要注意:
               >>凡是用了raw_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
								使用延时不能打开临界区
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/
/* 内部宏定义 ----------------------------------------------------------------*/

/* 任务相关信息定义-----------------------------------------------------------*/
#define HAND_STK_SIZE		512					//任务堆栈大小
CPU_STK HAND_TASK_STK[HAND_STK_SIZE];	
OS_TCB  hand_TaskTCB;							//任务控制块

/*信号量申明---------------------------------------------------------------*/
   OS_SEM	Handle_Sem;
/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/
   HandlesStruct UART_CH;
/* 外部函数原型声明 ----------------------------------------------------------*/
OS_ERR	HandleControlTaskErr	 = OS_ERR_NONE;
extern OS_TCB	THROWTaskTCB;
OS_ERR err;
extern THROW_INDEX TZ1; //TZ1参数
extern THROW_INDEX TZ2; //TZ2参数
extern THROW_INDEX TZ3; //TZ3参数
extern int calibration_speed;
extern volatile char command_flag;
extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern int RL_NUM;
/* 内部变量 ------------------------------------------------------------------*/
int TIM3_CCR1=0;
/* 内部函数原型声明 ----------------------------------------------------------*/

/* 任务主体部分 --------------------------------------------------------------*/

extern int printf_flag;
u8 HanComd[2]={0x01,0x42};	//开始命令。请求数据
u8 HandData[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
int test_hand=10,test_num=0;
u16 minddata[20]={0};
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明            //按键
void PS2_Cmd(u8 Comd)
{
    volatile u16 ref=0x01;  //volatile的变量是说这变量可能会被意想不到地改变，这样，编译器就不会去假设这个变量的值了。
	  HandData[1] = 0;
    for(ref=0x01;ref<0x0100;ref<<=1)
	  {
	     if(ref&Comd)
		  {
			DO_H;                   //输出一位控制位
		  }
		   else DO_L;

		   CLK_H;                        //时钟拉高
		   DELAY_TIME;
		   CLK_L;
	     DELAY_TIME;
	
		   CLK_H;
		    if(DI)
			   HandData[1] = ref|HandData[1];   //ID号（如0X73D) 在 PS2_RedLight中判断手柄的发送模式，也就是判断 ID 即 Data[1]的值。
	   }
}
void PS2_ClearData()
{
	u8 a;
	u16 b;
	for(a=0;a<9;a++)
		HandData[a]=0x00;
	for(b=0;b<16;b++)
	{
	minddata[b]=1<<b;
	}
 minddata[16]=(1<<10)+(1<<12);//组合键L1+△
 minddata[17]=(1<<10)+(1<<14);//组合键L1+X
 minddata[18]=(1<<10)+(1<<15);//组合键L1+□
 minddata[19]=(1<<10)+(1<<13);//组合键L1+O
}
void PS2_ReadData(void)
{
	u8 index;
  u16 Handkey;
	u16 hand_number=555;
	
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(HanComd[0]);  //开始命令
	PS2_Cmd(HanComd[1]);  //请求数据
	for(byte=2;byte<9;byte++)          //开始接受数据 （8个字节（8行））
	 {
		 for(ref=0x01;ref<0x100;ref<<=1)    //8个位
		  {
			  CLK_H;
			  DELAY_TIME;
			  CLK_L;
			  DELAY_TIME;
			  CLK_H;
		        if(DI)
		         HandData[byte] = ref|HandData[byte];  //形成标志？
		   }
          delay_us(16);
	 }
	CS_H;
}
u16 Handba_RX,Handba_RY,Handba_LX,Handba_LY;//手柄摇杆
u8 PS2_DataKey()
{
	u8 index;
  u16 Handkey;
	u16 hand_number=555;
	PS2_ClearData();
	PS2_ReadData();
	Handkey=(HandData[4]<<8)|HandData[3];     //这是16个按键  按下为0， 未按下为1
	Handba_RX=HandData[5];
	Handba_RY=HandData[6];
	Handba_LX=HandData[7];
	Handba_LY=HandData[8];
	hand_number=~Handkey;
	test_num=Handkey;
	for(index=0;index<20;index++)
  {
	if(hand_number==minddata[index])
	{
	test_hand=index+1;	
	return index+1;
	}
	}
	return 33;
}


static void hand_task(void *p_arg)
{ 	
	p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告	    
	while(1)
	{        
       CPU_SR_ALLOC();
	      p_arg = p_arg;
	while(1)
	{
		u16 i=PS2_DataKey(); 
		if(Handba_RX!=128){}//摇杆右
		if(Handba_RY!=128){}//摇杆
		if(Handba_LX!=128){}//摇杆左
		if(Handba_LY!=128){}//摇杆
		switch(i)
		{
		  case 33:break;
		  case PSB_SELECT:{
			     
			}break;
      case PSB_L3:    {
			     
			}break; 
		  case PSB_R3:    {
			           
			}break;
		  case PSB_START: {
			     
			}break;
		  case PSB_PAD_UP:{
			     
			}break;
		  case PSB_PAD_RIGHT:{
			     
			}break;
		  case PSB_PAD_DOWN:{
			    
			}break;
		  case PSB_PAD_LEFT:{
			     
			}break;
		  case PSB_L2:    {
			      
			}break;
		  case PSB_R2:    {
			      
			}break;
		  case PSB_L1:    {
			      
			}break;
		  case PSB_R1:    {
			      
			}break;
		  case PSB_GREEN: {
			     
			}break;
		  case PSB_RED:   {
			
			}break;
		  case PSB_BLUE:  {
			
			}break;	
		  case PSB_PINK:  {
			
			}break;	
		  case L1_SANJIAO:{
			     
			}break;	
		  case L1_CHA:    {
			       
			}break;	
		  case L1_ZFX:    {
			        
			}break;	
		  case L1_QQ:     {
			        
			}break;				
			default:break;
		}
		
		
		delay_ms(100);
	}
}
}
void create_hand_task(uint8_t prio)
{
  OS_ERR  err;
	OSTaskCreate ((OS_TCB    *) &hand_TaskTCB,			//任务控制块
								(CPU_CHAR  *)   "hand_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR)    hand_task,				//任务函数
								(void      *)    0,								//传入函数的参数
								(OS_PRIO    )    prio,          	//任务的优先级
								(CPU_STK   *)   &HAND_TASK_STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   HAND_STK_SIZE/10,//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   HAND_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,								//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,								//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,								//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);							//指定一个变量存储错误码
}

/************** 版权所有 (C), 2017-, Mushiny **********本文件结束*************/

