#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ϵͳʱ�ӳ�ʼ��	
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 


//0,��֧��os
//1,֧��os
#define SYSTEM_SUPPORT_OS		1		//����ϵͳ�ļ����Ƿ�֧��OS
																	    

/* �����������Ͷ��� ----------------------------------------------------------*/
// ͨ�������ն˽��չ����ļ��̰����룬һ�㴮���ն˲���SecureCRT
typedef enum 
{ 
  VK_BTN_L     = 0x01,  // ������
  VK_BTN_R     = 0x01,  // ����Ҽ�
  VK_BACKSPACE = 0x08,  // �˸��
  VK_TAB       = 0x09,  // �Ʊ��
  VK_LF        = 0x0A,  // \n
  VK_CR        = 0x0D,  // \r
  VK_ESC       = 0x1B,  // ESC������Ҫ�жϺ����Ƿ����5B��4F
  VK_DEL       = 0x7F,  // ɾ����
  VK_LEFT      = 0x44,  // �������Ҽ�����ؽ���Ƿ��͹��� 1B 5B 4x [D
  VK_UP        = 0x41,  // ���յ��ɴ�ӡ�ַ� [A
  VK_RIGHT     = 0x43,
  VK_DOWN      = 0x42,
  VK_CTRL_C    = 0x03,  // CTRL+C+
  VK_CTRL_V    = 0x16,  // CTRL+V
  VK_CTRL_Z    = 0x1A,  // CTRL+Z
  VK_CTRL_Q    = 0x11,  // CTRL+Q
  VK_F1        = 0x50,  // F1~F4���ܼ�����ؽ���Ƿ��͹��� 1B 4F 5x
  VK_F2        = 0x51,
  VK_F3        = 0x52,
  VK_F4        = 0x53,
}VK_CODE;

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).M4ͬM3����,ֻ�ǼĴ�����ַ����.
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����

// λ��������
#ifndef BIT
#define BIT(n)                (1 << (n))
#define bit(n)                (1 << (n))
#endif

#define setbit(reg,bits)      (reg) |= (bits)
#define clearbit(reg,bits)    (reg) &= ~(bits)
#define readbit(reg,bits)     ((reg) & (bits))
#define togglebit(reg,bits)   (reg) ^= (bits)
// ��������
#define countof(arr)          (sizeof(arr)/sizeof(arr[0]))
#define MAX(a,b)              (((a) > (b)) ? (a) : (b))
#define MIN(a,b)              (((a) < (b)) ? (a) : (b))
#define ABS(Number)           (((Number) < 0) ? (-(Number)) : (Number))
#define SIGN(Number)          (((Number) < 0) ? -1 : 1)

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ 
#endif











