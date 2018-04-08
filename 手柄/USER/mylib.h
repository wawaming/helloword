/*******************************************************************************

                      版权所有 (C), 2017- Mushiny.

 *******************************************************************************
  文 件 名   : mylib.h
  版 本 号   : 初稿
  作    者   : Ivan.Xie
  生成日期   : 2017年3月28日
  最近修改   :
  功能描述   : 自定义常用操作库
  函数列表   :
  修改历史   :
  1.日    期   : 2017年3月28日
    作    者   : Ivan.Xie
    修改内容   : 创建文件

*******************************************************************************/
#ifndef __MYLIB_H__
#define __MYLIB_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "sys.h"



/* 本模块向外部提供的数据类型定义 --------------------------------------------*/
typedef struct
{
  uint8_t *buf;
  uint16_t size;
  uint16_t read;
  uint16_t write;
}ST_FIFO;

typedef struct
{
  uint8_t *buf;
  uint16_t *len_tab;
  uint16_t size;
  uint16_t buf_len;
  uint16_t read;
  uint16_t write;
}ST_FIFO2D;

/* 本模块向外部提供的宏定义 --------------------------------------------------*/

#define USE_OS

#ifdef USE_OS
#define ENTER_CRITICAL()       CPU_SR_ALLOC();OS_CRITICAL_ENTER()   //RAW_SR_ALLOC();RAW_CPU_DISABLE()
#define EXIT_CRITICAL()        OS_CRITICAL_EXIT()    //RAW_CPU_ENABLE()
#define MY_ASSERT(a)           RAW_ASSERT(a)
#else
#define ENTER_CRITICAL()
#define EXIT_CRITICAL()
#define MY_ASSERT(a) 
#endif
/* 本模块向外部提供的接口函数原型声明 ----------------------------------------*/
/* 本模块向外部提供的接口函数原型声明 ----------------------------------------*/

void myfifo_init(ST_FIFO *pfifo, uint8_t *buf, uint16_t size);
void myfifo_flush(ST_FIFO *pfifo);
void myfifo_put(ST_FIFO *pfifo, uint8_t dat);
uint8_t myfifo_get(ST_FIFO *pfifo, uint8_t *dat);
uint8_t myfifo_isempty(ST_FIFO *pfifo);

void myfifo2d_init(ST_FIFO2D *pfifo, uint8_t *buf, uint16_t *len_tab, uint16_t size, uint16_t buf_len);
void myfifo2d_flush(ST_FIFO2D *pfifo);
void myfifo2d_put(ST_FIFO2D *pfifo, uint8_t *dat, uint16_t len);
uint8_t myfifo2d_get(ST_FIFO2D *pfifo, uint8_t *dat, uint16_t *len);
uint8_t myfifo2d_isempty(ST_FIFO2D *pfifo);

uint8_t mylib_getbitId(uint32_t val, uint8_t size);

#endif
/************** 版权所有 (C), 2017-  ***********本文件结束***********/
