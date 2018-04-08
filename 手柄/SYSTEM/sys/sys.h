#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//系统时钟初始化	
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 


//0,不支持os
//1,支持os
#define SYSTEM_SUPPORT_OS		1		//定义系统文件夹是否支持OS
																	    

/* 基本数据类型定义 ----------------------------------------------------------*/
// 通过串口终端接收过来的键盘按键码，一般串口终端采用SecureCRT
typedef enum 
{ 
  VK_BTN_L     = 0x01,  // 鼠标左键
  VK_BTN_R     = 0x01,  // 鼠标右键
  VK_BACKSPACE = 0x08,  // 退格键
  VK_TAB       = 0x09,  // 制表键
  VK_LF        = 0x0A,  // \n
  VK_CR        = 0x0D,  // \r
  VK_ESC       = 0x1B,  // ESC键，还要判断后面是否跟着5B或4F
  VK_DEL       = 0x7F,  // 删除键
  VK_LEFT      = 0x44,  // 上下左右键，监控结果是发送过来 1B 5B 4x [D
  VK_UP        = 0x41,  // 接收到可打印字符 [A
  VK_RIGHT     = 0x43,
  VK_DOWN      = 0x42,
  VK_CTRL_C    = 0x03,  // CTRL+C+
  VK_CTRL_V    = 0x16,  // CTRL+V
  VK_CTRL_Z    = 0x1A,  // CTRL+Z
  VK_CTRL_Q    = 0x11,  // CTRL+Q
  VK_F1        = 0x50,  // F1~F4功能键，监控结果是发送过来 1B 4F 5x
  VK_F2        = 0x51,
  VK_F3        = 0x52,
  VK_F4        = 0x53,
}VK_CODE;

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

// 位操作声明
#ifndef BIT
#define BIT(n)                (1 << (n))
#define bit(n)                (1 << (n))
#endif

#define setbit(reg,bits)      (reg) |= (bits)
#define clearbit(reg,bits)    (reg) &= ~(bits)
#define readbit(reg,bits)     ((reg) & (bits))
#define togglebit(reg,bits)   (reg) ^= (bits)
// 常见操作
#define countof(arr)          (sizeof(arr)/sizeof(arr[0]))
#define MAX(a,b)              (((a) > (b)) ? (a) : (b))
#define MIN(a,b)              (((a) < (b)) ? (a) : (b))
#define ABS(Number)           (((Number) < 0) ? (-(Number)) : (Number))
#define SIGN(Number)          (((Number) < 0) ? -1 : 1)

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 
#endif











