/*******************************************************************************

                      ��Ȩ���� (C), 2017- 

 *******************************************************************************
  �� �� ��   : printf_task.c
  �� �� ��   : ����
  ��    ��   : Ding
  ��������   : 2017��10��4��
  ����޸�   :
  ��������   : printf����ģ��
	��ϵͳ���ʱ��Ҫע��:
               >>��������asy_printf()��task��һ��Ҫע��raw_printf����Ҫ��̬��������ģ�
                 ����task�������ջ����������Ҫ�����Ҫ128��256����
								 asy_printf()���������黺�棬�����볤�����ƣ�����128�ַ����������Խ�磡
								 
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��10��4��
    ��    ��   : Ding
    �޸�����   : �����ļ�
*******************************************************************************/

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"

/* ���������Ϣ����-----------------------------------------------------------*/
//�����ջ��С	
#define PRINTF_STK_SIZE 		256                 //
//������ƿ�
static OS_TCB PrintfTaskTCB;
//�����ջ	
static CPU_STK PRINTF_TASK_STK[PRINTF_STK_SIZE];
static OS_ERR err;

#define PRINTF_QMSG_SIZE 		32                 //�ڽ���Ϣ���д�С

#define PRINTF_MAX_SIZE             128        //asy_printf()�ڶ���Ļ��������С����Ϊ�Ǿֲ�����ռ�õ��������ջ�ռ䣬���ж������ջ�Ĵ�С��Ҫ�����Ҫ128��256����

#define PORT_MEMORY_SIZE 	          (4u*1024u)

OS_MEM mem_pool_printf;
unsigned int mem_pool_printf_buf[PORT_MEMORY_SIZE/4];

/* �ڲ��Զ����������� --------------------------------------------------------*/

/* �ڲ��궨�� ----------------------------------------------------------------*/
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/

/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/* ��׼������� --------------------------------------------------------------*/
    
PUTCHAR_PROTOTYPE
{
  /* Ϊ�������ٷ����������״ε���os_printfʱ��©����һ���ַ���BUG������
   * ����鷢�������ɱ�־����USART_SendData()����֮ǰ.
   */
  PrintfUART->DR = ch;
  while (readbit(PrintfUART->SR, USART_FLAG_TC) == 0);
  return ch;
}

GETCHAR_PROTOTYPE
{
  uint16_t ch;
  while (readbit(PrintfUART->SR, USART_FLAG_RXNE) == 0);
  ch = PrintfUART->DR & 0x01FF;
  return ch;
}

int asy_printf(const char *fmt, ...)
{
	 int printed;
  if (fmt[0] == 0) return 0;
	
  {
    char *buf;
    int len;
    char printf_buf[PRINTF_MAX_SIZE];
  	va_list args;
    
  	va_start(args, fmt);
  	printed = vsprintf(printf_buf, fmt, args);
    va_end(args);
    
    len = strlen(printf_buf);
		if (len > (sizeof(printf_buf)-1)) len = sizeof(printf_buf) - 1;
   
		buf=OSMemGet((OS_MEM*)&mem_pool_printf,(OS_ERR*)&err);

	  if(err==OS_ERR_NONE)
		{
			memcpy(buf, printf_buf, len);
			buf[len] = '\0';
			OSTaskQPost((OS_TCB*)&PrintfTaskTCB,(void*)buf,(OS_MSG_SIZE)len,(OS_OPT)OS_OPT_POST_FIFO,(OS_ERR*)&err);
		}	
	}
			return printed;
}


char put_char(char ch)
{
	  PrintfUART->DR = ch;
  while (readbit(PrintfUART->SR, USART_FLAG_TC) == 0);
  return ch;
}


static void hardware_init(void)
{
     
}


static void printf_task(void *p_arg)
{

	
	void *str;
  char *ch;
  uint32_t str_size;
	
	p_arg = p_arg;
	
	hardware_init();	
	while(1)
	{
		
		str=OSTaskQPend((OS_TICK     )0,
		                (OS_OPT      )OS_OPT_PEND_BLOCKING,
										(OS_MSG_SIZE*)&str_size,
		                (CPU_TS*     )0,
		                (OS_ERR*     )&err);
		 ch = (char *)str;
		while (*ch && str_size)
    {
      while (readbit(PrintfUART->SR, USART_FLAG_TC) == 0);
      PrintfUART->DR = *ch++;
      str_size --;
    }
		OSMemPut((OS_MEM*)&mem_pool_printf,(void*)str,(OS_ERR*    )&err);
	}
}



void retarget_init(void)
{
	
	   OSMemCreate((OS_MEM*    )&mem_pool_printf,
	              (CPU_CHAR*  )"printf_mem",
	              (void*      )mem_pool_printf_buf,
								(OS_MEM_QTY )PORT_MEMORY_SIZE/PRINTF_MAX_SIZE,
								(OS_MEM_SIZE)PRINTF_MAX_SIZE,
								(OS_ERR*    )&err);	
	
		OSTaskCreate((OS_TCB 	* )&PrintfTaskTCB,		
				 (CPU_CHAR	* )"printf_task", 		
                 (OS_TASK_PTR )printf_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )OS_CFG_PRIO_MAX-3,     	
                 (CPU_STK   * )&PRINTF_TASK_STK[0],	
                 (CPU_STK_SIZE)PRINTF_STK_SIZE/10,	
                 (CPU_STK_SIZE)PRINTF_STK_SIZE,		
                 (OS_MSG_QTY  )PRINTF_QMSG_SIZE,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
}







/************** ��Ȩ���� (C), 2017- **********���ļ�����***********/
