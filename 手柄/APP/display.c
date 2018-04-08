/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : display.c
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

/* ���������Ϣ����-----------------------------------------------------------*/
#define DISPLAY_STK_SIZE		128
OS_TCB	DISPLAYTaskTCB;
CPU_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];

/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/
extern int Encoder_TIM2;
extern THROW_INDEX TZ1; //TZ1����
extern THROW_INDEX TZ2; //TZ2����
extern THROW_INDEX TZ3; //TZ3����
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
/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* �ڲ�����ԭ������ ----------------------------------------------------------*/
/* �������岿�� --------------------------------------------------------------*/
static void display_task(void *p_arg)
{	  
  Clear_LCD();	
	while(1)
	{	 			        
	 p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���
	
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