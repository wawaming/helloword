/*******************************************************************************

                      ��Ȩ���� (C), 2017- .

 *******************************************************************************
  �� �� ��   : shell_task.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2017��10��5��
  ����޸�   :
  ��������   : shell����ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��10��5��
    ��    ��   : 
    �޸�����   : �����ļ�

  2.��    ��   : 2017��10��5�� 
    ��    ��   : 
*******************************************************************************/

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"
//#include "cli.h"

/* �ڲ��Զ����������� --------------------------------------------------------*/

/* �ڲ��궨�� ----------------------------------------------------------------*/


#define CMD_SIZE                    8

/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/

/* �ⲿ����ԭ������ ----------------------------------------------------------*/

/* �ڲ����� ------------------------------------------------------------------*/

/* ���������Ϣ����-----------------------------------------------------------*/
//�����ջ��С	
#define SHELL_STK_SIZE            512
//������ƿ�
static OS_TCB ShellTaskTCB;
//�����ջ	
static CPU_STK SHELL_TASK_STK[SHELL_STK_SIZE];
static OS_ERR err;

static OS_SEM sem_printfuart_rx;
static uint8_t PrintfUART_RxBuf[PrintfUART_RxBufSize];
static uint16_t PrintfUART_RxId = 0;
static ST_FIFO printfuart_rxfifo;
static uint8_t rxfifo_buf[32];

static uint8_t cmd_tab[CMD_SIZE][PrintfUART_RxBufSize];
static uint8_t cmd_write_id = 0;
static uint8_t cmd_read_id = 0;
static uint8_t cmd_recall_flag = 0;
static uint8_t edit_flag = 0;
static uint8_t edit_id = 0;

/* �ڲ�����ԭ������ ----------------------------------------------------------*/
static void hardware_init(void)
{
  USART_ITConfig(PrintfUART, USART_IT_RXNE, ENABLE);
}

static void _clearline(uint16_t len)
{
  uint8_t i;
  for (i=0; i<len; i++)
  {
	//	put_char(VK_BACKSPACE);
    put_char(VK_BACKSPACE);
    put_char(' ');
    put_char(VK_BACKSPACE);
  }
}

static void _string_insert(uint8_t * buf, uint8_t ch, uint8_t after_len)
{
  uint8_t i;                                 
  if (after_len > 0)
  {
    for (i=after_len; i; i--) {*(buf+i) = *(buf+i-1);}
    *buf = ch;
    *(buf+after_len+1) = '\0';                                   //��ʵ�Ĳ����ַ���ʵ�ǹ����Ƶ����
    for (i=0; i<after_len+1; i++) {put_char(*(buf+i));}           //���ƻ�ȥ
    for (i=0; i<after_len; i++) {put_char(VK_BACKSPACE);}  
  }
  else
  {
    *buf = ch;
    put_char(*buf);
  }
}

static void _string_delete(uint8_t * buf, uint8_t after_len)
{
  uint8_t i;
  if (after_len > 0)
  {
    for (i=0; i<after_len; i++) {*(buf+i) = *(buf+i+1);}
    *(buf+i) = '\0';
    for (i=0; i<after_len; i++) {put_char(*(buf+i));}
    put_char(' ');
    for (i=0; i<after_len+1; i++) {put_char(VK_BACKSPACE);}
  }
  else
  {
    *buf = '\0';
    put_char(' ');
    put_char(VK_BACKSPACE);
  }
}

static void shell_task(void *arg)
{
	

  uint8_t ch;
  char printbuf[PrintfUART_RxBufSize] = {0};
	
  //raw_semaphore_create(&sem_printfuart_rx, "sem_printfuart_rx", 0);
	OSSemCreate((OS_SEM*)&sem_printfuart_rx,(CPU_CHAR*)"printfuart_rx_sem",(OS_SEM_CTR)0,(OS_ERR*)&err);
	
  myfifo_init(&printfuart_rxfifo, rxfifo_buf, countof(rxfifo_buf));
  hardware_init();
  

  asy_printf("\r\n------------------- NCURoboteam -----------------------\r\n");
  asy_printf(    ">>>>>>>>>>>>>>>>> Welcome to D! <<<<<<<<<<<<<<<<<<<<<\r\n");
  asy_printf(LOGO_STRING);
  //GPIO_Off(EN_EXT12V_GPIO, EN_EXT12V_PIN);
  while(1)
  {
		OSSemPend(&sem_printfuart_rx,0,OS_OPT_PEND_BLOCKING,NULL,&err);
		 if (myfifo_get(&printfuart_rxfifo, &ch))
    {
      if (isprint(ch))//C �⺯�� int isprint(int c) ����������ַ��Ƿ��ǿɴ�ӡ�ġ��ɴ�ӡ�ַ��Ƿǿ����ַ����ַ���
      {
        if (cmd_recall_flag) // ��������¼�����������������ַ�������������޸�
          {
            cmd_recall_flag = 0;
          } 
        if (edit_flag == 0)
        {
          PrintfUART_RxBuf[PrintfUART_RxId++] = ch;
          if (PrintfUART_RxId >= PrintfUART_RxBufSize) PrintfUART_RxId = PrintfUART_RxBufSize-1;
          put_char(ch);
        }
        else
        {
          _string_insert(&PrintfUART_RxBuf[edit_id], ch, PrintfUART_RxId-edit_id);
          PrintfUART_RxId ++;
          edit_id ++;
        }
      }
      else if (ch == VK_BACKSPACE  ||  ch == VK_DEL)
      {
        if (cmd_recall_flag) {cmd_recall_flag = 0; } // ��������¼����������������ɾ����������������޸�
        if (edit_flag == 0)
        {
          if (PrintfUART_RxId)
          {
            PrintfUART_RxId --;
            PrintfUART_RxBuf[PrintfUART_RxId] = '\0';
            put_char(VK_BACKSPACE);
            put_char(' ');                            //����ʾ��Ҫ�����ۼ�
            put_char(VK_BACKSPACE);
          }
        }
        else
        {
          if (edit_id)
          {
            put_char(VK_BACKSPACE);  // ��Ϊ��ʱ�����±�Ϊ1����0����Ҫ�˸�
            _string_delete(&PrintfUART_RxBuf[edit_id-1], PrintfUART_RxId-edit_id);
            PrintfUART_RxId --;
            edit_id --;
          }
        }
      }
      else if (ch == '\n')
      {
        put_char('\n');
        if (PrintfUART_RxId)
        {
          int res;
          PrintfUART_RxBuf[PrintfUART_RxId++] = '\0';
          if (!cmd_recall_flag)
          {
            strncpy((char *)cmd_tab[cmd_write_id], (char *)PrintfUART_RxBuf, PrintfUART_RxId);
            cmd_read_id = cmd_write_id;
            cmd_write_id ++;
            if (cmd_write_id >= CMD_SIZE) cmd_write_id = 0;
          }
          if (PrintfUART_RxBuf[0]=='?' && PrintfUART_RxId==2)
          {
            strncpy((char *)PrintfUART_RxBuf, "help\0", 5);
          }
          do {
            res = rsh_process_command((const CPU_INT08S *)PrintfUART_RxBuf, (CPU_INT08S *)printbuf, (size_t)sizeof(printbuf));
            if (strlen(printbuf) > 0) asy_printf("%s\r\n", printbuf);
          } while (res == 0);
        }
        asy_printf(LOGO_STRING);
        memset(PrintfUART_RxBuf, 0, PrintfUART_RxId);
        PrintfUART_RxId = 0;
        cmd_recall_flag = 0;
        edit_flag = 0;
      }
      else if (ch == VK_ESC)
      { // �����ַ�
        while (OSSemPend(&sem_printfuart_rx,6,OS_OPT_PEND_BLOCKING,NULL,&err) != OS_ERR_TIMEOUT)
        {
          myfifo_get(&printfuart_rxfifo, &ch);
        }
        myfifo_get(&printfuart_rxfifo, &ch);  // ȡ�����һ���ַ�
        
        switch (ch)
        {
          case VK_UP:     // ���ϼ����Ϸ����ѭ������
            { 
              if (PrintfUART_RxId)
              { // �����ǰ��
                _clearline(PrintfUART_RxId);
                memset(PrintfUART_RxBuf, 0, PrintfUART_RxId);
              }
              if (cmd_recall_flag || (cmd_read_id == cmd_write_id))
              { // �ܿ��׸�
                if (cmd_read_id)
                {
                  cmd_read_id --;
                }
                else 
                {
                  cmd_read_id = CMD_SIZE - 1;
                  while (strlen((char *)cmd_tab[cmd_read_id]) == 0)
                  {
                    if (cmd_read_id)
                      cmd_read_id --;
                    else 
                      break;
                  }
                }
              }
              if (strlen((char *)cmd_tab[cmd_read_id]) > 0)
              {
                cmd_recall_flag = 1;
                strcpy((char *)PrintfUART_RxBuf, (char *)cmd_tab[cmd_read_id]);
                PrintfUART_RxId = strlen((char *)PrintfUART_RxBuf);
                asy_printf("%s", (char *)PrintfUART_RxBuf);
              }
            }
            break;
          case VK_DOWN:     // ���¼����·������ѭ������
            if (PrintfUART_RxId)
            { // �����ǰ��
              _clearline(PrintfUART_RxId);
              memset(PrintfUART_RxBuf, 0, PrintfUART_RxId);
            }
            if (cmd_read_id != cmd_write_id)
            { // ��û�������������
              if (cmd_recall_flag)
              {
                cmd_read_id ++;
                if (cmd_read_id >= CMD_SIZE)
                  cmd_read_id = 0;
                if (cmd_read_id == cmd_write_id)
                {
                  PrintfUART_RxId = 0;
                  break;
                }
              }
              else
              {
                cmd_recall_flag = 1;
              }
              strcpy((char *)PrintfUART_RxBuf, (char *)cmd_tab[cmd_read_id]);
              PrintfUART_RxId = strlen((char *)PrintfUART_RxBuf);
              asy_printf("%s", (char *)PrintfUART_RxBuf);
            }
            else
            {
              PrintfUART_RxId = 0;
            }
            break;
          case VK_LEFT:     // ��������ƶ����
            edit_flag = 1;
            if (edit_id) 
            {
              edit_id --;
              put_char(VK_BACKSPACE);
            }
            break;
          case VK_RIGHT:    // ��������ƶ����
            edit_flag = 1;
            if (edit_id < PrintfUART_RxId)
            {
              put_char(PrintfUART_RxBuf[edit_id++]);
            }
            break;
          case VK_ESC:      // ESC��
            if (PrintfUART_RxId)
            {
              _clearline(PrintfUART_RxId);
              memset(PrintfUART_RxBuf, 0, PrintfUART_RxId);
            }
            if (PrintfUART_RxId == 0)
              rsh_process_key(ch);
            else
              PrintfUART_RxId = 0;
            break;
          case VK_F1:       // F1~F4���ܼ�
          case VK_F2:
          case VK_F3:
          case VK_F4:
            rsh_process_key(ch);
            break;
          default:
            break;
        }
        
      }
      else if (ch == VK_CTRL_C)
      { // ʵ��linux/windows���ն�����CTRL+C��ֹ������
        rsh_process_key(ch);
      }
      
      if (!edit_flag)
        edit_id = PrintfUART_RxId;
    }
   // led_toggle(LED_COM);
	}
	
}

void create_shell_task(uint8_t prio)
{
//  raw_task_create(&shell_task_obj, (RAW_U8  *)"shell_task", 0,
//                   prio, 0,  shell_task_stack, 
//                   SHELL_STACK_SIZE ,  shell_task, 1); 
	
		OSTaskCreate((OS_TCB 	* )&ShellTaskTCB,		
				 (CPU_CHAR	* )"shell_task", 		
                 (OS_TASK_PTR )shell_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )prio,     	
                 (CPU_STK   * )&SHELL_TASK_STK[0],	
                 (CPU_STK_SIZE)SHELL_STK_SIZE/10,	
                 (CPU_STK_SIZE)SHELL_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
	
	
	
}

/* ��ص��жϺ��� */
void PrintfUART_IRQHandler(void)
{
  uint8_t ch;    
  OSIntEnter();
  if (readbit(PrintfUART->SR,USART_FLAG_RXNE))  // �����ж�
  {
    myfifo_put(&printfuart_rxfifo, (uint8_t)PrintfUART->DR);
    //raw_semaphore_put(&sem_printfuart_rx);
      
		OSSemPost(&sem_printfuart_rx,OS_OPT_POST_1,&err);
  }
  OSIntExit();
}


