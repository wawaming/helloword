/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : key.c
  �� �� ��   : ����
  ��    ��   : LJM
  ��������   : 2017.12.26
  ����޸�   :
  ��������   : ��������
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
#define KEY_STK_SIZE		128
OS_TCB	KEYTaskTCB;
CPU_STK	KEY_TASK_STK[KEY_STK_SIZE];

/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/
extern volatile char command_flag;
/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* �ڲ�����ԭ������ ----------------------------------------------------------*/
/* �������岿�� --------------------------------------------------------------*/
extern int printf_speed[1000];
static void key_task(void *p_arg)
{	  	
	while(1)
	{ 			    
    CPU_SR_ALLOC();//ʹ���ٽ�������ʱ��Ҫ
	  p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���		
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
		                 }break;                       //����ģʽ
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
		                 }break;                        //����ģʽ
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
		                   }break;                      //����ģʽ
	 }   			
	//	LED2(1);LED3(0);delay_ms(100);	/*����*/
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

