/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : throw_task.c
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
uint8_t place=0;
uint8_t throw_flag=0;
int angle,past_cycle;
/* 任务相关信息定义-----------------------------------------------------------*/
#define THROW_STK_SIZE		128
OS_TCB	THROWTaskTCB;
CPU_STK	THROW_TASK_STK[THROW_STK_SIZE];

/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/

THROW_INDEX TZ1; //TZ1参数
THROW_INDEX TZ2; //TZ2参数
THROW_INDEX TZ3; //TZ3参数

extern int Encoder_TIM2;
int RL_NUM=0;
int acc_speed=3;
int _entercode_3,_entercode_4;
extern SERVO_MLCB UNDER1_MLCB;
extern OS_TCB	COMMANDTaskTCB;
uint8_t PixyTxMsg[8];

/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/

/* 内部函数原型声明 ----------------------------------------------------------*/

/************************ADRC参数数组**********************/
const float ADRC_PARM[7]={ 
/*时间尺度r    积分时间h    平滑因子n     非线性跟踪信号比例b1     theta    非线性跟踪信号比例b1        alphal     */ 
    800,           0.08,        2,               250,                 250,          0.001,              0.002    };

		
		
/************************ADRC参数初始化**********************/
		
void ADRC_Init (adrc* ADRC_INIT){
		ADRC_INIT->r=ADRC_PARM[0];
		ADRC_INIT->h=	ADRC_PARM[1];
		ADRC_INIT->n=ADRC_PARM[2];
		ADRC_INIT->b1=ADRC_PARM[3];
		ADRC_INIT->theta=ADRC_PARM[4];
		ADRC_INIT->b2=ADRC_PARM[5];	
		ADRC_INIT->alphal=ADRC_PARM[6];
}

int Sign(float input){
 int  output=0;
    if(input>1E-6) output=1;
    else if(input<-1E-6) output=-1;
    else output=0;
    return output;
}

int fsg(float a,float b){
int output=0;
	output=(Sign(a+b)-Sign(a-b))/2;
	return output;
}

/*********最速跟踪微分器TD fhan函数实现*******/
/*函数名称: void fhan()
  函数功能：跟踪微分器 获取系统的跟踪量和跟踪微分量
  输入：ADRC参数列表  目标值
  输出：跟踪量：x1  跟踪微分量：x2   */

void fhan(adrc* ADRC_INIT,float except_data){
float d0,a0,y,a1,a2,a;
float x1_offect=0;
x1_offect=ADRC_INIT->x1-except_data;
ADRC_INIT->h0=ADRC_INIT->n*ADRC_INIT->h*ADRC_INIT->h;
d0=ADRC_INIT->r*ADRC_INIT->h0*ADRC_INIT->h0; //d0=r*h0^2
a0=ADRC_INIT->x2*ADRC_INIT->h0;
y=x1_offect+a0;
a1=sqrt(d0*d0+8*d0*abs(y));
a2=a0+Sign(y)*(a1-d0)/2;	
a=(a0+y)*fsg(y,d0)+a2*(1-fsg(y,d0));
ADRC_INIT->fh=-ADRC_INIT->r*(a/d0)*fsg(a,d0)-ADRC_INIT->r*Sign(a)*(1-fsg(a,d0));
ADRC_INIT->x1+=ADRC_INIT->h*ADRC_INIT->x2;
ADRC_INIT->x2+=ADRC_INIT->h*ADRC_INIT->fh;	
}



float Nolinear_conbination(adrc* ADRC_INIT,float error){
int s=fsg(error,ADRC_INIT->theta);
ADRC_INIT->u0=error*s/(pow(ADRC_INIT->theta,1-ADRC_INIT->alphal))+(pow(abs(error),ADRC_INIT->alphal)\
	            *Sign(error)*(1-s));
	return ADRC_INIT->u0;
}

float TD_Nolinear_PID(adrc* ADRC_INIT,float execpt,float Real){
float output;
/***先得到输入的跟踪量和跟踪微分量x1和x2*****/
fhan(ADRC_INIT,execpt);
float X1=	ADRC_INIT->x1;
float X2=	ADRC_INIT->x2;	
/****得到输出量的跟踪量和跟踪微分量v1和v2****/
fhan(ADRC_INIT,Real);
float V1=	ADRC_INIT->x1;
float V2=	ADRC_INIT->x2;
/*****得到跟踪偏差量和跟踪微分偏差量********/
float e1=	X1-V1;
float e2=X2-V2;	
Real=ADRC_INIT->b1*Nolinear_conbination(ADRC_INIT,e1)+ADRC_INIT->b2*  \
	                               Nolinear_conbination(ADRC_INIT,e2);
output=Real;
return output;
}



/* 
   抛掷参数初始化函数
*/
void throw_init(uint8_t area)
{
	if(Robot_Mode==MATCH_MODE)    //初始化比赛模式参数,若是调试模式，忽略本函数功能
	{
		switch(area)
		{	
			case ONE:{
 	             	TZ1.P=0.000011f;
	              TZ1.D=0.004f;
	              TZ1.control_num=0;
	              //TZ1.speed=140;
	              //TZ1.point=3200;
                TZ1.turn=2;
                TZ1.offe_lastance=0;

			         }break;          //投掷TZ1区参数
      case TWO:{
				        TZ2.P=0.000011f;
	              TZ2.D=0.004f;
	              TZ2.control_num=0;
	              TZ2.speed=180;
	              TZ2.point=500;
                TZ2.turn=2;
                TZ2.offe_lastance=0;		
		          	}break;         //投掷TZ2区参数
     	case THREE:{
				         TZ3.P=0.000011f;
	               TZ3.D=0.004f;
	               TZ3.control_num=0;
	               TZ3.speed=200;
	               TZ3.point=300;
                 TZ3.turn=2;
                 TZ3.offe_lastance=0;		
		          	}break;	        //投掷TZ3区参数
      default:	break;			
		}
	}
	else{
	     TZ1.control_num=0;
	     TZ2.control_num=0;
	     TZ3.control_num=0;
		   TZ1.offe_lastance=0;
		   TZ2.offe_lastance=0;
		   TZ3.offe_lastance=0;
	    }
}

/*
   抛掷函数
*/

extern int printf_flag;
int throw_speed=0;
int throw_point=0;
extern int real_speed;

void throw_ball(uint8_t area)
{
	 int  enterthrow=0,hole_circle=0,plus_minus=0;;
	 OS_ERR err;
	 CPU_SR_ALLOC();	
	 OS_CRITICAL_ENTER();	//进入临界区
	 switch(area)
	 {
		 case ONE:{
			         if(TZ1.speed<0){plus_minus=-1;}
							 else if(TZ1.speed>0){plus_minus=1;}
							 
							
							 Motor_AccelSpeed(&UNDER1_MLCB,300);delay_us(600);
							 Motor_SetSpeed(&UNDER1_MLCB,TZ1.speed);
							
							 
							 hole_circle=Encoder_TIM2*65536+TIM2->CNT+(cycle*TZ1.turn*plus_minus);
							 while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){IWDG_Feed();}//此处需强行进行喂狗，否则系统强制复位
							 enterthrow=Encoder_TIM2*65536+TIM2->CNT;
							 while((((plus_minus*enterthrow)%cycle)<=(TZ1.point-3))||((plus_minus*enterthrow)%cycle)>=(TZ1.point+3))
							 {enterthrow=Encoder_TIM2*65536+TIM2->CNT;IWDG_Feed();}						 
							 RL1_ON;
			         RL_NUM++;
							 TZ1.init_point=Encoder_TIM2*65536+TIM2->CNT;_entercode_4=TZ1.init_point;
							 			 
		 }break;  
		 case TWO:{
			         if(TZ2.speed<0){plus_minus=-1;} //58550 200 2 58480
							 else if(TZ2.speed>0){plus_minus=1;}
							 Motor_SetSpeed(&UNDER1_MLCB,TZ2.speed);
							 hole_circle=Encoder_TIM2*65536+TIM2->CNT+(cycle*TZ2.turn*plus_minus);
							 while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){IWDG_Feed();}//此处需强行进行喂狗，否则系统强制复位
							 enterthrow=Encoder_TIM2*65536+TIM2->CNT; 
							 while((((plus_minus*enterthrow)%cycle)<=(TZ2.point-3))||((plus_minus*enterthrow)%cycle)>=(TZ2.point+3))
							 {enterthrow=Encoder_TIM2*65536+TIM2->CNT;IWDG_Feed();}
							 TZ2.init_point=Encoder_TIM2*65536+TIM2->CNT;
							 RL1_ON;RL_NUM++;
		 }break;
		 case THREE:{
			          if(TZ3.speed<0){plus_minus=-1;}
							  else if(TZ3.speed>0){plus_minus=1;}
								Motor_SetSpeed(&UNDER1_MLCB,TZ3.speed);
							  hole_circle=Encoder_TIM2*65536+TIM2->CNT+(cycle*TZ3.turn*plus_minus);
							  while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){IWDG_Feed();}//此处需强行进行喂狗，否则系统强制复位
							  enterthrow=Encoder_TIM2*65536+TIM2->CNT;
							  while((((plus_minus*enterthrow)%cycle)<=(TZ3.point-3))||((plus_minus*enterthrow)%cycle)>=(TZ3.point+3))
							  {enterthrow=Encoder_TIM2*65536+TIM2->CNT;IWDG_Feed();}
								TZ3.init_point=Encoder_TIM2*65536+TIM2->CNT;
							  RL1_ON;RL_NUM++;
		 }break;
	 }	 
	 OS_CRITICAL_EXIT();
}

/*
   抛掷闭环函数
   当前 位置 速度 时间 V_max=throw_speed
	 X轴时间 Xtime=π*控制周期*次数/减速总时间 
	 Y轴速度 Vspeed =V_Max*（cos(time_to_x)+1）
*/
void throw_stop(uint8_t area)
{
	switch(area)
		{
		case ONE:{		
	             TZ1.target_distance=cycle-TZ1.point;              //计算停止距离	
	             TZ1.time=TZ1.target_distance*2/ABS_int(TZ1.speed);  //计算控制时间
	             TZ1.control_final=TZ1.time/TIME_cyc;                //计算控制次数
	             TZ1.Xtime=Pi*TIME_cyc*TZ1.control_num/TZ1.time;     //计算当前时间
	             TZ1.Vspeed=(int)(TZ1.speed/2*(cos(TZ1.Xtime)+1));   //计算理论速度
	             TZ1.theo_distance=TZ1.init_point+(TZ1.speed/2)*(sin(TZ1.Xtime)+TZ1.Xtime);   //计算理论位置
	             TZ1.real_distance=Encoder_TIM2*65536+TIM2->CNT;     //编码器读数	
	             TZ1.offe_distance=TZ1.theo_distance-TZ1.real_distance;        //计算位置偏差	
               TZ1.Aspeed=TZ1.Vspeed+(int)(TZ1.P*TZ1.offe_distance+TZ1.D*(TZ1.offe_distance-TZ1.offe_lastance));//计算调整速度
               TZ1.offe_lastance=TZ1.offe_distance;
               if(TZ1.Aspeed>throw_speed_max){TZ1.Aspeed=throw_speed_max;}	  //限制最大值
	             if(TZ1.Aspeed<throw_speed_min){TZ1.Aspeed=throw_speed_min;}	  //限制最小值
	             Motor_SetSpeed(&UNDER1_MLCB,TZ1.Aspeed);                      //速度调节
							 
               TZ1.control_num++;
		         }break;
		case TWO:{			
			         TZ2.target_distance=cycle-TZ2.point;              //计算停止距离	
	             TZ2.time=TZ2.target_distance*2/ABS_int(TZ2.speed);  //计算控制时间
	             TZ2.control_final=TZ2.time/TIME_cyc;                //计算控制次数
	             TZ2.Xtime=Pi*TIME_cyc*TZ2.control_num/TZ2.time;     //计算当前时间
	             TZ2.Vspeed=(int)(TZ2.speed/2*(cos(TZ2.Xtime)+1));   //计算理论速度
	             TZ2.theo_distance=TZ2.init_point+(TZ2.speed/2)*(sin(TZ2.Xtime)+TZ2.Xtime);   //计算理论位置
	             TZ2.real_distance=Encoder_TIM2*65536+TIM2->CNT;     //编码器读数	
	             TZ2.offe_distance=TZ2.theo_distance-TZ2.real_distance;        //计算位置偏差	
               TZ2.Aspeed=TZ2.Vspeed+(int)(TZ2.P*TZ2.offe_distance+TZ2.D*(TZ2.offe_distance-TZ2.offe_lastance));//计算调整速度
               TZ2.offe_lastance=TZ2.offe_distance;
               if(TZ2.Aspeed>throw_speed_max){TZ2.Aspeed=throw_speed_max;}	  //限制最大值
	             if(TZ2.Aspeed<throw_speed_min){TZ2.Aspeed=throw_speed_min;}	  //限制最小值
	             Motor_SetSpeed(&UNDER1_MLCB,TZ2.Aspeed);                      //速度调节
               TZ2.control_num++;
		         }break;
		case THREE:{
			         TZ3.target_distance=cycle-TZ3.point;                //计算停止距离	
	             TZ3.time=TZ3.target_distance*2/ABS_int(TZ3.speed);  //计算控制时间
	             TZ3.control_final=TZ3.time/TIME_cyc;                //计算控制次数
	             TZ3.Xtime=Pi*TIME_cyc*TZ3.control_num/TZ3.time;     //计算当前时间
	             TZ3.Vspeed=(int)(TZ3.speed/2*(cos(TZ3.Xtime)+1));   //计算理论速度
	             TZ3.theo_distance=TZ3.init_point+(TZ3.speed/2)*(sin(TZ3.Xtime)+TZ3.Xtime);   //计算理论位置
	             TZ3.real_distance=Encoder_TIM2*65536+TIM2->CNT;     //编码器读数	
	             TZ3.offe_distance=TZ3.theo_distance-TZ3.real_distance;        //计算位置偏差	
               TZ3.Aspeed=TZ3.Vspeed+(int)(TZ3.P*TZ3.offe_distance+TZ3.D*(TZ3.offe_distance-TZ3.offe_lastance));//计算调整速度
               TZ3.offe_lastance=TZ3.offe_distance;
               if(TZ3.Aspeed>throw_speed_max){TZ3.Aspeed=throw_speed_max;}	  //限制最大值
	             if(TZ3.Aspeed<throw_speed_min){TZ3.Aspeed=throw_speed_min;}	  //限制最小值
	             Motor_SetSpeed(&UNDER1_MLCB,TZ3.Aspeed);                       //速度调节
               TZ3.control_num++;
		         }break;
		default:	break;	
		}
}


void throw_stop1(uint8_t area)
{
	int decelerate,enterstop;
	Motor_AccelSpeed(&UNDER1_MLCB,acc_speed);delay_us(600);
	switch(area)
		{
		case ONE:{		
	             decelerate=cycle-((TZ1.speed*TZ1.speed)/(60*60*2*acc_speed))*cycle;
               TZ1.control_final=TZ1.speed*1000/acc_speed/60/TIME_cyc;
               while(enterstop%cycle)
               {enterstop=Encoder_TIM2*65536+TIM2->CNT;IWDG_Feed();}
               Motor_SetSpeed(&UNDER1_MLCB,0);			
               TZ1.control_num++;
		         }break;
		case TWO:{			
			        
               TZ2.control_num++;
		         }break;
		case THREE:{
			         
               TZ3.control_num++;
		         }break;
		default:	break;	
		}
}

/*   
    位置补偿函数
*/
void throw_adjust(void)
{
	        
				  delay_us(600);
			    Motor_Position_Mode(&UNDER1_MLCB);
				  delay_us(600);
				  Motor_MastSpeed(&UNDER1_MLCB,50.0f);
				  delay_us(600);
				  Motor_AccelSpeed(&UNDER1_MLCB,50.0f);
          delay_us(600);
					past_cycle=(int)(ABS_float((float)Encoder_TIM2*65536+TIM2->CNT)/((float)cycle)+0.5f);//四舍五入取最近的整圈
					angle=ABS_int(Encoder_TIM2*65536+TIM2->CNT)-past_cycle*cycle;//偏差角度对应的脉冲	
					Motor_ToRelativePlace(&UNDER1_MLCB,-angle);
}


void throw_AbsolutePosition(void)
{
	        BEED_OFF;
				  delay_us(600);
			    Motor_Position_Mode(&UNDER1_MLCB);
				  delay_us(600);
				  Motor_MastSpeed(&UNDER1_MLCB,300.0f);
				  delay_us(600);
				  Motor_AccelSpeed(&UNDER1_MLCB,13.0f);
          delay_us(600);
//					past_cycle=(int)(ABS_float((float)Encoder_TIM2*65536+TIM2->CNT)/((float)cycle)+0.5f);//四舍五入取最近的整圈
//					angle=ABS_int(Encoder_TIM2*65536+TIM2->CNT)-past_cycle*cycle;//偏差角度对应的脉冲	
//					Motor_ToRelativePlace(&UNDER1_MLCB,-angle);
	        Motor_RelativePosition_Set(&UNDER1_MLCB,0);
}



/* 任务主体部分 --------------------------------------------------------------*/

static void throw_task(void *p_arg)
{	  	
	OS_ERR err;
  OS_TaskSuspend((OS_TCB*)&THROWTaskTCB,&err);		//挂起抛掷任务
	while(1)
	{	 			        
	 p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告

		 switch(Robot_Mode)
	 {
		 case MATCH_MODE:{
	                    Motor_Speed_Mode(&UNDER1_MLCB);delay_us(600);					            
					            Motor_MastSpeed(&UNDER1_MLCB,400);delay_us(600);
				            	Motor_AccelSpeed(&UNDER1_MLCB,300);delay_us(600);
			                throw_init(place);
                      printf_flag=1;		 
			                throw_ball(place);
			                throw_stop(place);
			                TIM_Cmd(TIM5, ENABLE);
			                while(throw_flag!=1){BEED_ON;}
											printf_flag=0;
									  	throw_adjust();
                      BEED_OFF;											
                      throw_flag=0;
											OSTaskResume(&COMMANDTaskTCB,&err);//开始调度任务  												
		                 }break;                       //比赛模式
		 case THROW_MODE:{
			                Motor_Speed_Mode(&UNDER1_MLCB);delay_us(600);					            
					            Motor_MastSpeed(&UNDER1_MLCB,400);delay_us(600);
				            	Motor_AccelSpeed(&UNDER1_MLCB,300);delay_us(600);
			                throw_init(place);
                      printf_flag=1;			 
			                throw_ball(place);
			                throw_stop(place);
			                printf_flag=0;
			                TIM_Cmd(TIM5, ENABLE);
			                while(throw_flag!=1){}
										  throw_adjust(); 
                      throw_flag=0;
		                 }break;                        //抛掷模式
		 case CONNECT_MODE:{ 
		
		                   }break;                      //交接模式
	 }
 
    OS_TaskSuspend((OS_TCB*)&THROWTaskTCB,&err);		//挂起抛掷任务		 
	
  }
}

void create_throw_task(uint8_t prio)
{
  OS_ERR  err;	
	OSTaskCreate((OS_TCB 	* )&THROWTaskTCB,		
				 (CPU_CHAR	* )"THROW  task", 		
                 (OS_TASK_PTR )throw_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )prio,     	
                 (CPU_STK   * )&THROW_TASK_STK[0],	
                 (CPU_STK_SIZE)THROW_STK_SIZE/10,	
                 (CPU_STK_SIZE)THROW_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
}

/************/

