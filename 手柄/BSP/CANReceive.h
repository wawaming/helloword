#ifndef _CANRECEIVE_H
#define _CANRECEIVE_H

#include "includes.h"


/*	定义联合体	*/
typedef	union
{
	int 	int_data;
	char	char_data[4];
}INT_CHAR_UNION;









/*	CAN 接收数据处理	*/
void CAN_Recive_handle(void);
#endif