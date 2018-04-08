/*******************************************************************************

                      ��Ȩ���� (C), 2017- Mushiny.

 *******************************************************************************
  �� �� ��   : mylib.h
  �� �� ��   : ����
  ��    ��   : Ivan.Xie
  ��������   : 2017��3��28��
  ����޸�   :
  ��������   : �Զ��峣�ò�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��3��28��
    ��    ��   : Ivan.Xie
    �޸�����   : �����ļ�

*******************************************************************************/
#ifndef __MYLIB_H__
#define __MYLIB_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "sys.h"



/* ��ģ�����ⲿ�ṩ���������Ͷ��� --------------------------------------------*/
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

/* ��ģ�����ⲿ�ṩ�ĺ궨�� --------------------------------------------------*/

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
/* ��ģ�����ⲿ�ṩ�Ľӿں���ԭ������ ----------------------------------------*/
/* ��ģ�����ⲿ�ṩ�Ľӿں���ԭ������ ----------------------------------------*/

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
/************** ��Ȩ���� (C), 2017-  ***********���ļ�����***********/
