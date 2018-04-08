//////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************/
/**	
	*	@brief	CAN接收数据处理
	*/
/********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////

#include "CANReceive.h"


/*******************************************************************************************
*																			*	全局变量段	*																			 *
********************************************************************************************/
/*	摄像头偏差	*/
extern int16_t Camera_Offset;

/*	底盘电机速度	*/
extern int Under_Motor1[2];
extern int Under_Motor2[2];
extern int Under_Motor3[2];
extern int Under_Motor4[2];
extern uint8_t CAN_RE[8];
/**
	*	@brief
	*	@param
	*/
void CAN_Recive_handle(void)
{
	uint8_t i;
	INT_CHAR_UNION under_motor;
	CanRxMsg	CAMERA_Msg;
	CAMERA_Msg.StdId = 0x00;
	CAMERA_Msg.RTR = 0x00;
	CAMERA_Msg.IDE = 0x00;
	CAMERA_Msg.ExtId = 0x00;
	CAMERA_Msg.DLC = 0x00;
	CAMERA_Msg.Data[0] = 0x00;
	CAMERA_Msg.Data[1] = 0x00;
	CAMERA_Msg.Data[2] = 0x00;
	CAMERA_Msg.Data[3] = 0x00;
	CAMERA_Msg.Data[4] = 0x00;
	CAMERA_Msg.Data[5] = 0x00;
	CAMERA_Msg.Data[6] = 0x00;
	CAMERA_Msg.Data[7] = 0x00;
	
	CAN_Receive(CAN1, CAN_FIFO0, &CAMERA_Msg);
#if 1
	/*	*	*	*	*	*	*	*	*	摄像头数据接收	*	*	*	*	*	*	*	*	*	*/
	
#endif
}