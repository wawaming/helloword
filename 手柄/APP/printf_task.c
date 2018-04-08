/*******************************************************************************

                      °æÈ¨ËùÓÐ (C), 2017- 

 *******************************************************************************
  ÎÄ ¼þ Ãû   : printf_task.c
  °æ ±¾ ºÅ   : ³õ¸å
  ×÷    Õß   : Ding
  Éú³ÉÈÕÆÚ   : 2017Äê10ÔÂ4ÈÕ
  ×î½üÐÞ¸Ä   :
  ¹¦ÄÜÃèÊö   : printfÈÎÎñÄ£¿é
	±¾ÏµÍ³±à³ÌÊ±ÐèÒª×¢Òâ:
               >>·²ÊÇÓÃÁËasy_printf()µÄtask£¬Ò»¶¨Òª×¢Òâraw_printfÊÇÐèÒª¶¯Ì¬±äÁ¿Êý×éµÄ£¬
                 ËùÒÔtaskµÄÈÎÎñ¶ÑÕ»±ØÐëÄÜÂú×ãÒªÇó£¬×îºÃÒª128»ò256ÒÔÉÏ
								 asy_printf()ÄÚÓÐÊýÊý×é»º´æ£¬ÓÐÊäÈë³¤¶ÈÏÞÖÆ£¬³¬¹ý128×Ö·û½«Ôì³ÉÊý×éÔ½½ç£¡
								 
  º¯ÊýÁÐ±í   :
  ÐÞ¸ÄÀúÊ·   :
  1.ÈÕ    ÆÚ   : 2017Äê10ÔÂ4ÈÕ
    ×÷    Õß   : Ding
    ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ
*******************************************************************************/

/* °üº¬Í·ÎÄ¼þ ----------------------------------------------------------------*/
#include "app.h"

/* ÈÎÎñÏà¹ØÐÅÏ¢¶¨Òå-----------------------------------------------------------*/
//ÈÎÎñ¶ÑÕ»´óÐ¡	
#define PRINTF_STK_SIZE 		256                 //
//ÈÎÎñ¿ØÖÆ¿é
static OS_TCB PrintfTaskTCB;
//ÈÎÎñ¶ÑÕ»	
static CPU_STK PRINTF_TASK_STK[PRINTF_STK_SIZE];
static OS_ERR err;

#define PRINTF_QMSG_SIZE 		32                 //ÄÚ½¨ÏûÏ¢¶ÓÁÐ´óÐ¡

#define PRINTF_MAX_SIZE             128        //asy_printf()ÄÚ¶¨ÒåµÄ»º´æÊý×é´óÐ¡£¬ÒòÎªÊÇ¾Ö²¿±äÁ¿Õ¼ÓÃµÄÊÇÈÎÎñ¶ÑÕ»¿Õ¼ä£¬ËùÓÐ¶ÔÈÎÎñ¶ÑÕ»µÄ´óÐ¡ÓÐÒªÇó¬×îºÃÒª128»ò256ÒÔÉÏ

#define PORT_MEMORY_SIZE 	          (4u*1024u)

OS_MEM mem_pool_printf;
unsigned int mem_pool_printf_buf[PORT_MEMORY_SIZE/4];

/* ÄÚ²¿×Ô¶¨ÒåÊý¾ÝÀàÐÍ --------------------------------------------------------*/

/* ÄÚ²¿ºê¶¨Òå ----------------------------------------------------------------*/
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/* ÄÚ²¿³£Á¿¶¨Òå---------------------------------------------------------------*/

/* Íâ²¿±äÁ¿ÉùÃ÷ --------------------------------------------------------------*/

/* Íâ²¿º¯ÊýÔ­ÐÍÉùÃ÷ ----------------------------------------------------------*/

/* ÄÚ²¿±äÁ¿ ------------------------------------------------------------------*/

/* ÄÚ²¿º¯ÊýÔ­ÐÍÉùÃ÷ ----------------------------------------------------------*/

/* ±ê×¼ÊäÈëÊä³ö --------------------------------------------------------------*/
    
PUTCHAR_PROTOTYPE
{
  /* ÎªÁËÐÞÕý¹Ù·½Çý¶¯¿âÖÐÊ×´Îµ÷ÓÃos_printfÊ±»áÂ©µôµÚÒ»¸ö×Ö·ûµÄBUG£¬ÕâÀï
   * ½«¼ì²é·¢ËÍÍê³ÉÍê³É±êÖ¾·ÅÔÚUSART_SendData()º¯ÊýÖ®Ç°.
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







/************** °æÈ¨ËùÓÐ (C), 2017- **********±¾ÎÄ¼þ½áÊø***********/
