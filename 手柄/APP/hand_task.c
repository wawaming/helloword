/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : hand_task.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2017.11.5 ������
  ����޸�   :
  ��������   : �����ֱ�����
               ��ϵͳ���ʱ��Ҫע��:
               >>��������raw_printf()��task��һ��Ҫע��raw_printf����Ҫ��̬��������ģ�
                 ����task�������ջ����������Ҫ�����Ҫ128��256����
								ʹ����ʱ���ܴ��ٽ���
  �����б�   :
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"

/* �ڲ��Զ����������� --------------------------------------------------------*/
/* �ڲ��궨�� ----------------------------------------------------------------*/

/* ���������Ϣ����-----------------------------------------------------------*/
#define HAND_STK_SIZE		512					//�����ջ��С
CPU_STK HAND_TASK_STK[HAND_STK_SIZE];	
OS_TCB  hand_TaskTCB;							//������ƿ�

/*�ź�������---------------------------------------------------------------*/
   OS_SEM	Handle_Sem;
/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/
   HandlesStruct UART_CH;
/* �ⲿ����ԭ������ ----------------------------------------------------------*/
OS_ERR	HandleControlTaskErr	 = OS_ERR_NONE;
extern OS_TCB	THROWTaskTCB;
OS_ERR err;
extern THROW_INDEX TZ1; //TZ1����
extern THROW_INDEX TZ2; //TZ2����
extern THROW_INDEX TZ3; //TZ3����
extern int calibration_speed;
extern volatile char command_flag;
extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern int RL_NUM;
/* �ڲ����� ------------------------------------------------------------------*/
int TIM3_CCR1=0;
/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/* �������岿�� --------------------------------------------------------------*/

extern int printf_flag;
u8 HanComd[2]={0x01,0x42};	//��ʼ�����������
u8 HandData[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
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
	};	//����ֵ�밴����            //����
void PS2_Cmd(u8 Comd)
{
    volatile u16 ref=0x01;  //volatile�ı�����˵��������ܻᱻ���벻���ظı䣬�������������Ͳ���ȥ�������������ֵ�ˡ�
	  HandData[1] = 0;
    for(ref=0x01;ref<0x0100;ref<<=1)
	  {
	     if(ref&Comd)
		  {
			DO_H;                   //���һλ����λ
		  }
		   else DO_L;

		   CLK_H;                        //ʱ������
		   DELAY_TIME;
		   CLK_L;
	     DELAY_TIME;
	
		   CLK_H;
		    if(DI)
			   HandData[1] = ref|HandData[1];   //ID�ţ���0X73D) �� PS2_RedLight���ж��ֱ��ķ���ģʽ��Ҳ�����ж� ID �� Data[1]��ֵ��
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
 minddata[16]=(1<<10)+(1<<12);//��ϼ�L1+��
 minddata[17]=(1<<10)+(1<<14);//��ϼ�L1+X
 minddata[18]=(1<<10)+(1<<15);//��ϼ�L1+��
 minddata[19]=(1<<10)+(1<<13);//��ϼ�L1+O
}
void PS2_ReadData(void)
{
	u8 index;
  u16 Handkey;
	u16 hand_number=555;
	
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(HanComd[0]);  //��ʼ����
	PS2_Cmd(HanComd[1]);  //��������
	for(byte=2;byte<9;byte++)          //��ʼ�������� ��8���ֽڣ�8�У���
	 {
		 for(ref=0x01;ref<0x100;ref<<=1)    //8��λ
		  {
			  CLK_H;
			  DELAY_TIME;
			  CLK_L;
			  DELAY_TIME;
			  CLK_H;
		        if(DI)
		         HandData[byte] = ref|HandData[byte];  //�γɱ�־��
		   }
          delay_us(16);
	 }
	CS_H;
}
u16 Handba_RX,Handba_RY,Handba_LX,Handba_LY;//�ֱ�ҡ��
u8 PS2_DataKey()
{
	u8 index;
  u16 Handkey;
	u16 hand_number=555;
	PS2_ClearData();
	PS2_ReadData();
	Handkey=(HandData[4]<<8)|HandData[3];     //����16������  ����Ϊ0�� δ����Ϊ1
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
	p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���	    
	while(1)
	{        
       CPU_SR_ALLOC();
	      p_arg = p_arg;
	while(1)
	{
		u16 i=PS2_DataKey(); 
		if(Handba_RX!=128){}//ҡ����
		if(Handba_RY!=128){}//ҡ��
		if(Handba_LX!=128){}//ҡ����
		if(Handba_LY!=128){}//ҡ��
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
	OSTaskCreate ((OS_TCB    *) &hand_TaskTCB,			//������ƿ�
								(CPU_CHAR  *)   "hand_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR)    hand_task,				//������
								(void      *)    0,								//���뺯���Ĳ���
								(OS_PRIO    )    prio,          	//��������ȼ�
								(CPU_STK   *)   &HAND_TASK_STK[0],//�����ջ����ַ
								(CPU_STK_SIZE)   HAND_STK_SIZE/10,//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   HAND_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,								//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,								//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,								//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);							//ָ��һ�������洢������
}

/************** ��Ȩ���� (C), 2017-, Mushiny **********���ļ�����*************/

