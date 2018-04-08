/*******************************************************************************

                      版权所有 (C), 2017- 

 *******************************************************************************
  文 件 名   : mylib.c
  版 本 号   : 初稿
  作    者   : Ding
  生成日期   : 2017年9月29日
  最近修改   :
  功能描述   : mylib
  函数列表   :
  修改历史   :
  1.日    期   : 2017年9月29日
    作    者   : Ding
    修改内容   : 创建文件
*******************************************************************************/

/* 包含头文件 ----------------------------------------------------------------*/
#include "os.h"
#include "mylib.h"
#include <string.h>

void myfifo_init(ST_FIFO *pfifo, uint8_t *buf, uint16_t size)
{
  pfifo->buf = buf;
  pfifo->size = size;
  pfifo->read = 0;
  pfifo->write = 0;
}

void myfifo_flush(ST_FIFO *pfifo)
{
  ENTER_CRITICAL();
  
  pfifo->read = 0;
  pfifo->write = 0;

  EXIT_CRITICAL();
}

void myfifo_put(ST_FIFO *pfifo, uint8_t dat)
{  
  ENTER_CRITICAL();
  
  pfifo->buf[pfifo->write] = dat;
  if (++(pfifo->write) >= pfifo->size)
    pfifo->write = 0;
  
  EXIT_CRITICAL();
}

uint8_t myfifo_get(ST_FIFO *pfifo, uint8_t *dat)
{
  uint8_t ret = 0;

  ENTER_CRITICAL();
  if (pfifo->write != pfifo->read)
  {
    ret = 1;
    *dat = pfifo->buf[pfifo->read];
    if (++(pfifo->read) >= pfifo->size)
      pfifo->read = 0;
  }
  EXIT_CRITICAL();
  
  return ret;
}

uint8_t myfifo_isempty(ST_FIFO *pfifo)
{
  uint8_t ret = 0;

  if (pfifo->write == pfifo->read)
    ret = 1;
  
  return ret;
}

/* 二维队列 ------------------------------------------------------------------*/
void myfifo2d_init(ST_FIFO2D *pfifo, uint8_t *buf, uint16_t *len_tab, uint16_t size, uint16_t buf_len)
{
  pfifo->buf = buf;
  pfifo->len_tab = len_tab;
  pfifo->size = size;
  pfifo->buf_len = buf_len;
  pfifo->read = 0;
  pfifo->write = 0;
  // 为防止队列的数组是动态变量数组，这里增加一个清零的处理
  memset((uint8_t *)pfifo->buf, 0, pfifo->buf_len * pfifo->size);
  memset((uint8_t *)pfifo->len_tab, 0, sizeof(pfifo->len_tab[0]) * pfifo->size);
}

void myfifo2d_flush(ST_FIFO2D *pfifo)
{
  ENTER_CRITICAL();
  
  pfifo->read = 0;
  pfifo->write = 0;
  memset((uint8_t *)pfifo->buf, 0, pfifo->buf_len * pfifo->size);
  memset((uint8_t *)pfifo->len_tab, 0, sizeof(pfifo->len_tab[0]) * pfifo->size);
  
  EXIT_CRITICAL();
}

void myfifo2d_put(ST_FIFO2D *pfifo, uint8_t *dat, uint16_t len)
{  
  ENTER_CRITICAL();

  len = len < pfifo->buf_len ? len : pfifo->buf_len;
  memcpy(&pfifo->buf[pfifo->write * pfifo->buf_len], dat, len);
  pfifo->len_tab[pfifo->write] = len;
  if (++(pfifo->write) >= pfifo->size)
    pfifo->write = 0;
  
  EXIT_CRITICAL();
}

uint8_t myfifo2d_get(ST_FIFO2D *pfifo, uint8_t *dat, uint16_t *len)
{
  uint8_t ret = 0;

  ENTER_CRITICAL();
  if (pfifo->write != pfifo->read)
  {
    ret = 1;
    if (pfifo->len_tab[pfifo->read] > pfifo->buf_len) pfifo->len_tab[pfifo->read] = pfifo->buf_len;
    memcpy(dat, &pfifo->buf[pfifo->read * pfifo->buf_len], pfifo->len_tab[pfifo->read]);
    *len = pfifo->len_tab[pfifo->read];
    memset(&pfifo->buf[pfifo->read * pfifo->buf_len], 0, pfifo->len_tab[pfifo->read]);
    pfifo->len_tab[pfifo->read] = 0;
    if (++(pfifo->read) >= pfifo->size)
      pfifo->read = 0;
  }
  EXIT_CRITICAL();
  
  return ret;
}

uint8_t myfifo2d_isempty(ST_FIFO2D *pfifo)
{
  uint8_t ret = 0;

  if (pfifo->write == pfifo->read)
    ret = 1;
  
  return ret;
}


// 求哪个bit位为1
uint8_t mylib_getbitId(uint32_t val, uint8_t size)
{
  uint8_t i = 0;
  if (val)
  {
    if (size > 4) size = 4;
    for (i=0; i<size*8; i++)
      if (readbit(val, bit(i))) break;
  }
  return i;
}

