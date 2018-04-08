/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny

 *******************************************************************************
  �� �� ��   : motor_driver_task.c
  �� �� ��   : ����
  ��    ��   : LJM
  ��������   : 2017.12.26
  ����޸�   :
  ��������   : ���������������
  �����б�   :
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"

/* �ڲ��Զ����������� --------------------------------------------------------*/

/* �ڲ��궨�� ----------------------------------------------------------------*/
/******���̵��********/
//---���1
#define DEVICE1_ID	    9
#define RATIO1		    	16
#define CODER1			    1000
//---���2	
#define DEVICE2_ID    	12
#define RATIO2		    	14
#define CODER2		    	1000
//---���3	
#define DEVICE3_ID	    6
#define RATIO3		    	14
#define CODER3		    	1000
////---���4	
//#define DEVICE4_ID	    8
//#define RATIO4			    14
//#define CODER4			    1000

/* ���������Ϣ����-----------------------------------------------------------*/
OS_TCB  motor_driver_TaskTCB;							//������ƿ�
#define motor_driver_STK_SIZE		256					//�����ջ��С
CPU_STK motor_driver__STK[motor_driver_STK_SIZE];	

/* �ڲ���������---------------------------------------------------------------*/
//	�����ƶ�������ƿ�
SERVO_MLCB UNDER1_MLCB;			//��һ���޵��
SERVO_MLCB UNDER2_MLCB;			//�ڶ����޵��
SERVO_MLCB UNDER3_MLCB;			//�������޵��
SERVO_MLCB UNDER4_MLCB;			//�������޵��

//�ƶ��������ٶȡ����ٶ�����
const int motor_move_max_rpm = 500;  //
const int motor_move_acc = 300;

int sppped1=0;
int sppped2=0;
int sppped3=0;

//���Ա���
int t_speed =0;

/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/

/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/
uint8_t motor_init_flag = 0;

/* �ڲ����ܺ���ԭ�� ----------------------------------------------------------*/

//���õ���ٶ�
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
	/*	���̵�����ƿ�����	*/
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


/* �������岿�� --------------------------------------------------------------*/
static void motor_driver_task(void *p_arg)
{ 	
	  p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���	    
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
	OSTaskCreate ((OS_TCB    *) &motor_driver_TaskTCB,			//������ƿ�
								(CPU_CHAR  *)   "motor_driver_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR)    motor_driver_task,				//������
								(void      *)    0,								//���뺯���Ĳ���
								(OS_PRIO    )    prio,          	//��������ȼ�
								(CPU_STK   *)   &motor_driver__STK[0],//�����ջ����ַ
								(CPU_STK_SIZE)   motor_driver_STK_SIZE/10,//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   motor_driver_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,								//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,								//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,								//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);							//ָ��һ�������洢������
}

/************** ��Ȩ���� (C), 2017-, Mushiny **********���ļ�����*************/

