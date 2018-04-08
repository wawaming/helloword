/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : throw_task.c
  �� �� ��   : ����
  ��    ��   : LJM
  ��������   : 2017.12.26
  ����޸�   :
  ��������   : ״̬��ʾ����
               ��ϵͳ���ʱ��Ҫע��:
               >>��������raw_printf()��task��һ��Ҫע��raw_printf����Ҫ��̬��������ģ�
                 ����task�������ջ����������Ҫ�����Ҫ128��256����
  �����б�   :
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"


/* �ڲ��Զ����������� --------------------------------------------------------*/
/* �ڲ��궨�� ----------------------------------------------------------------*/
uint8_t place=0;
uint8_t throw_flag=0;
int angle,past_cycle;
/* ���������Ϣ����-----------------------------------------------------------*/
#define THROW_STK_SIZE		128
OS_TCB	THROWTaskTCB;
CPU_STK	THROW_TASK_STK[THROW_STK_SIZE];

/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/

THROW_INDEX TZ1; //TZ1����
THROW_INDEX TZ2; //TZ2����
THROW_INDEX TZ3; //TZ3����

extern int Encoder_TIM2;
int RL_NUM=0;
int acc_speed=3;
int _entercode_3,_entercode_4;
extern SERVO_MLCB UNDER1_MLCB;
extern OS_TCB	COMMANDTaskTCB;
uint8_t PixyTxMsg[8];

/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/************************ADRC��������**********************/
const float ADRC_PARM[7]={ 
/*ʱ��߶�r    ����ʱ��h    ƽ������n     �����Ը����źű���b1     theta    �����Ը����źű���b1        alphal     */ 
    800,           0.08,        2,               250,                 250,          0.001,              0.002    };

		
		
/************************ADRC������ʼ��**********************/
		
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

/*********���ٸ���΢����TD fhan����ʵ��*******/
/*��������: void fhan()
  �������ܣ�����΢���� ��ȡϵͳ�ĸ������͸���΢����
  ���룺ADRC�����б�  Ŀ��ֵ
  �������������x1  ����΢������x2   */

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
/***�ȵõ�����ĸ������͸���΢����x1��x2*****/
fhan(ADRC_INIT,execpt);
float X1=	ADRC_INIT->x1;
float X2=	ADRC_INIT->x2;	
/****�õ�������ĸ������͸���΢����v1��v2****/
fhan(ADRC_INIT,Real);
float V1=	ADRC_INIT->x1;
float V2=	ADRC_INIT->x2;
/*****�õ�����ƫ�����͸���΢��ƫ����********/
float e1=	X1-V1;
float e2=X2-V2;	
Real=ADRC_INIT->b1*Nolinear_conbination(ADRC_INIT,e1)+ADRC_INIT->b2*  \
	                               Nolinear_conbination(ADRC_INIT,e2);
output=Real;
return output;
}



/* 
   ����������ʼ������
*/
void throw_init(uint8_t area)
{
	if(Robot_Mode==MATCH_MODE)    //��ʼ������ģʽ����,���ǵ���ģʽ�����Ա���������
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

			         }break;          //Ͷ��TZ1������
      case TWO:{
				        TZ2.P=0.000011f;
	              TZ2.D=0.004f;
	              TZ2.control_num=0;
	              TZ2.speed=180;
	              TZ2.point=500;
                TZ2.turn=2;
                TZ2.offe_lastance=0;		
		          	}break;         //Ͷ��TZ2������
     	case THREE:{
				         TZ3.P=0.000011f;
	               TZ3.D=0.004f;
	               TZ3.control_num=0;
	               TZ3.speed=200;
	               TZ3.point=300;
                 TZ3.turn=2;
                 TZ3.offe_lastance=0;		
		          	}break;	        //Ͷ��TZ3������
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
   ��������
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
	 OS_CRITICAL_ENTER();	//�����ٽ���
	 switch(area)
	 {
		 case ONE:{
			         if(TZ1.speed<0){plus_minus=-1;}
							 else if(TZ1.speed>0){plus_minus=1;}
							 
							
							 Motor_AccelSpeed(&UNDER1_MLCB,300);delay_us(600);
							 Motor_SetSpeed(&UNDER1_MLCB,TZ1.speed);
							
							 
							 hole_circle=Encoder_TIM2*65536+TIM2->CNT+(cycle*TZ1.turn*plus_minus);
							 while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){IWDG_Feed();}//�˴���ǿ�н���ι��������ϵͳǿ�Ƹ�λ
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
							 while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){IWDG_Feed();}//�˴���ǿ�н���ι��������ϵͳǿ�Ƹ�λ
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
							  while((Encoder_TIM2*65536+TIM2->CNT)!=hole_circle){IWDG_Feed();}//�˴���ǿ�н���ι��������ϵͳǿ�Ƹ�λ
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
   �����ջ�����
   ��ǰ λ�� �ٶ� ʱ�� V_max=throw_speed
	 X��ʱ�� Xtime=��*��������*����/������ʱ�� 
	 Y���ٶ� Vspeed =V_Max*��cos(time_to_x)+1��
*/
void throw_stop(uint8_t area)
{
	switch(area)
		{
		case ONE:{		
	             TZ1.target_distance=cycle-TZ1.point;              //����ֹͣ����	
	             TZ1.time=TZ1.target_distance*2/ABS_int(TZ1.speed);  //�������ʱ��
	             TZ1.control_final=TZ1.time/TIME_cyc;                //������ƴ���
	             TZ1.Xtime=Pi*TIME_cyc*TZ1.control_num/TZ1.time;     //���㵱ǰʱ��
	             TZ1.Vspeed=(int)(TZ1.speed/2*(cos(TZ1.Xtime)+1));   //���������ٶ�
	             TZ1.theo_distance=TZ1.init_point+(TZ1.speed/2)*(sin(TZ1.Xtime)+TZ1.Xtime);   //��������λ��
	             TZ1.real_distance=Encoder_TIM2*65536+TIM2->CNT;     //����������	
	             TZ1.offe_distance=TZ1.theo_distance-TZ1.real_distance;        //����λ��ƫ��	
               TZ1.Aspeed=TZ1.Vspeed+(int)(TZ1.P*TZ1.offe_distance+TZ1.D*(TZ1.offe_distance-TZ1.offe_lastance));//��������ٶ�
               TZ1.offe_lastance=TZ1.offe_distance;
               if(TZ1.Aspeed>throw_speed_max){TZ1.Aspeed=throw_speed_max;}	  //�������ֵ
	             if(TZ1.Aspeed<throw_speed_min){TZ1.Aspeed=throw_speed_min;}	  //������Сֵ
	             Motor_SetSpeed(&UNDER1_MLCB,TZ1.Aspeed);                      //�ٶȵ���
							 
               TZ1.control_num++;
		         }break;
		case TWO:{			
			         TZ2.target_distance=cycle-TZ2.point;              //����ֹͣ����	
	             TZ2.time=TZ2.target_distance*2/ABS_int(TZ2.speed);  //�������ʱ��
	             TZ2.control_final=TZ2.time/TIME_cyc;                //������ƴ���
	             TZ2.Xtime=Pi*TIME_cyc*TZ2.control_num/TZ2.time;     //���㵱ǰʱ��
	             TZ2.Vspeed=(int)(TZ2.speed/2*(cos(TZ2.Xtime)+1));   //���������ٶ�
	             TZ2.theo_distance=TZ2.init_point+(TZ2.speed/2)*(sin(TZ2.Xtime)+TZ2.Xtime);   //��������λ��
	             TZ2.real_distance=Encoder_TIM2*65536+TIM2->CNT;     //����������	
	             TZ2.offe_distance=TZ2.theo_distance-TZ2.real_distance;        //����λ��ƫ��	
               TZ2.Aspeed=TZ2.Vspeed+(int)(TZ2.P*TZ2.offe_distance+TZ2.D*(TZ2.offe_distance-TZ2.offe_lastance));//��������ٶ�
               TZ2.offe_lastance=TZ2.offe_distance;
               if(TZ2.Aspeed>throw_speed_max){TZ2.Aspeed=throw_speed_max;}	  //�������ֵ
	             if(TZ2.Aspeed<throw_speed_min){TZ2.Aspeed=throw_speed_min;}	  //������Сֵ
	             Motor_SetSpeed(&UNDER1_MLCB,TZ2.Aspeed);                      //�ٶȵ���
               TZ2.control_num++;
		         }break;
		case THREE:{
			         TZ3.target_distance=cycle-TZ3.point;                //����ֹͣ����	
	             TZ3.time=TZ3.target_distance*2/ABS_int(TZ3.speed);  //�������ʱ��
	             TZ3.control_final=TZ3.time/TIME_cyc;                //������ƴ���
	             TZ3.Xtime=Pi*TIME_cyc*TZ3.control_num/TZ3.time;     //���㵱ǰʱ��
	             TZ3.Vspeed=(int)(TZ3.speed/2*(cos(TZ3.Xtime)+1));   //���������ٶ�
	             TZ3.theo_distance=TZ3.init_point+(TZ3.speed/2)*(sin(TZ3.Xtime)+TZ3.Xtime);   //��������λ��
	             TZ3.real_distance=Encoder_TIM2*65536+TIM2->CNT;     //����������	
	             TZ3.offe_distance=TZ3.theo_distance-TZ3.real_distance;        //����λ��ƫ��	
               TZ3.Aspeed=TZ3.Vspeed+(int)(TZ3.P*TZ3.offe_distance+TZ3.D*(TZ3.offe_distance-TZ3.offe_lastance));//��������ٶ�
               TZ3.offe_lastance=TZ3.offe_distance;
               if(TZ3.Aspeed>throw_speed_max){TZ3.Aspeed=throw_speed_max;}	  //�������ֵ
	             if(TZ3.Aspeed<throw_speed_min){TZ3.Aspeed=throw_speed_min;}	  //������Сֵ
	             Motor_SetSpeed(&UNDER1_MLCB,TZ3.Aspeed);                       //�ٶȵ���
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
    λ�ò�������
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
					past_cycle=(int)(ABS_float((float)Encoder_TIM2*65536+TIM2->CNT)/((float)cycle)+0.5f);//��������ȡ�������Ȧ
					angle=ABS_int(Encoder_TIM2*65536+TIM2->CNT)-past_cycle*cycle;//ƫ��Ƕȶ�Ӧ������	
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
//					past_cycle=(int)(ABS_float((float)Encoder_TIM2*65536+TIM2->CNT)/((float)cycle)+0.5f);//��������ȡ�������Ȧ
//					angle=ABS_int(Encoder_TIM2*65536+TIM2->CNT)-past_cycle*cycle;//ƫ��Ƕȶ�Ӧ������	
//					Motor_ToRelativePlace(&UNDER1_MLCB,-angle);
	        Motor_RelativePosition_Set(&UNDER1_MLCB,0);
}



/* �������岿�� --------------------------------------------------------------*/

static void throw_task(void *p_arg)
{	  	
	OS_ERR err;
  OS_TaskSuspend((OS_TCB*)&THROWTaskTCB,&err);		//������������
	while(1)
	{	 			        
	 p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���

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
											OSTaskResume(&COMMANDTaskTCB,&err);//��ʼ��������  												
		                 }break;                       //����ģʽ
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
		                 }break;                        //����ģʽ
		 case CONNECT_MODE:{ 
		
		                   }break;                      //����ģʽ
	 }
 
    OS_TaskSuspend((OS_TCB*)&THROWTaskTCB,&err);		//������������		 
	
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

