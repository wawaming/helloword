/*******************************************************************************

                      版权所有 (C), 2017- 

 *******************************************************************************
  文 件 名   : printf_task.c
  版 本 号   : 初稿
  作    者   : Ding
  生成日期   : 2017年10月4日
  最近修改   :
  功能描述   : printf任务模块
	本系统编程时需要注意:
               >>凡是用了asy_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
								 asy_printf()内有数数组缓存，有输入长度限制，超过128字符将造成数组越界！
								 
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月4日
    作    者   : Ding
    修改内容   : 创建文件
*******************************************************************************/

/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 任务相关信息定义-----------------------------------------------------------*/
//任务堆栈大小	
#define PRINTF_STK_SIZE 		256                 //
//任务控制块
static OS_TCB PrintfTaskTCB;
//任务堆栈	
static CPU_STK PRINTF_TASK_STK[PRINTF_STK_SIZE];
static OS_ERR err;

#define PRINTF_QMSG_SIZE 		32                 //内建消息队列大小

#define PRINTF_MAX_SIZE             128        //asy_printf()内定义的缓存数组大小，因为是局部变量占用的是任务堆栈空间，所有对任务堆栈的大小有要求詈靡�128或256以上

#define PORT_MEMORY_SIZE 	          (4u*1024u)

OS_MEM mem_pool_printf;
unsigned int mem_pool_printf_buf[PORT_MEMORY_SIZE/4];

/* 内部自定义数据类型 --------------------------------------------------------*/

/* 内部宏定义 ----------------------------------------------------------------*/
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/

/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/

/* 内部函数原型声明 ----------------------------------------------------------*/

/* 标准输入输出 --------------------------------------------------------------*/
    
PUTCHAR_PROTOTYPE
{
  /* 为了修正官方驱动库中首次调用os_printf时会漏掉第一个字符的BUG，这里
   * 将检查发送完成完成标志放在USART_SendData()函数之前.
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







/************** 版权所有 (C), 2017- **********本文件结束***********/
