#ifndef _CANRECEIVE_H
#define _CANRECEIVE_H

#include "includes.h"


/*	����������	*/
typedef	union
{
	int 	int_data;
	char	char_data[4];
}INT_CHAR_UNION;









/*	CAN �������ݴ���	*/
void CAN_Recive_handle(void);
#endif