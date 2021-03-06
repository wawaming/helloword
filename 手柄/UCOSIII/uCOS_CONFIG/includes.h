/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT

#define ON	1
#define OFF	0

/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>

/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>
/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>

/*
*********************************************************************************************************
*                                              APP 
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <bsp.h>

/*
*********************************************************************************************************
*                                              USER
*********************************************************************************************************
*/
#include "function.h"
#include "PCI.h"

/*
*********************************************************************************************************
*                                              BSP
*********************************************************************************************************
*/
#include "can.h"
#include "gpio.h"
#include "IWDG.h"
#include "NVIC.h"
#include "time.h"
#include "usart.h"
#include "exti.h"
#include "I2C.h"



/*
*********************************************************************************************************
*                                              BSP
*********************************************************************************************************
*/
#include "delay.h"
#include <sys.h>	  

/*
*********************************************************************************************************
*                                              DRIVE
*********************************************************************************************************
*/
#include "rbc_lcd.h"
#include "ml_order.h"
#include "motor_control.h"



#endif
