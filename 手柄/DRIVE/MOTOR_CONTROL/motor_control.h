#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include "includes.h"

typedef enum
{
	Write_Success=0,
}SERVO_ERROR;

/********铭朗伺服电机控制块********/
typedef struct
{
	char		   *Servo_Mlcb_Name;			//伺服控制块名字
	uint8_t     Star_Id;							//标识符ID
	uint8_t     Device_Id;						//设备ID
	uint8_t     Ratio;								//电机传动比
	uint16_t    Coder;								//编码器线数
	SERVO_ERROR Servo_Error;					//发送情况
}SERVO_MLCB;

typedef struct
{
	unsigned Servo_ID:7;
	unsigned Data_Len:4;
	unsigned Data_Format:2;
	unsigned Data_Type:2;
	uint8_t  Front_Data[4];
	int32_t  Last_Data;
}Servo_Struct;

uint8_t Servo_Set_Function(Servo_Struct *Servo_Structure);
uint8_t Servo_Param_Set( uint8_t servo_id,uint8_t order,uint8_t len,int32_t data);

void Create_MLServo_CB(SERVO_MLCB		*servo_mlcb,					//伺服控制块
											 char       	*servo_mlcb_name,			//伺服控制块名字
											 uint8_t     	 star_id,							//标识符ID
											 uint8_t       ratio,								//电机传动比
											 uint16_t      coder);							//编码器线数

uint8_t MLServo_Write(SERVO_MLCB *Servo_MLCB, uint8_t order, uint8_t len, int32_t data);

/*****************************************相关简要设计*********************************************/
/*	电机速度设计	*/
uint8_t Motor_MastSpeed(SERVO_MLCB *Servo_MLCB,float data);	//	电机最大速度设置
uint8_t Motor_MinSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	电机最小速度设置
uint8_t Motor_SetSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	电机运行速度设置  
											 
uint8_t Motor_CBS(SERVO_MLCB *Servo_MLCB);
uint8_t Motor_SBS(SERVO_MLCB *Servo_MLCB);											 

/*	加速度设置	*/
uint8_t Motor_AccelSpeed(SERVO_MLCB *Servo_MLCB,float data);	//设置电机加速度

/*	电机位置设置	*/
uint8_t Motor_AbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//设置电机当前位置为绝对位置
uint8_t Motor_ToAbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//设置电机向绝对位置开始运行
uint8_t Motor_ToRelativePlace(SERVO_MLCB *Servo_MLCB,int data);	//设置电机相对于上次设置位置开始运行
											 
uint8_t Motor_AbsolutePosition_Set(SERVO_MLCB *Servo_MLCB);	
uint8_t Motor_RelativePosition_Set(SERVO_MLCB *Servo_MLCB,int32_t data);

/*	电机初始化设置	*/
uint8_t Motor_P_SET(SERVO_MLCB *Servo_MLCB,int32_t data);	//电机位置比例P设置
uint8_t Motor_Code_Set(SERVO_MLCB *Servo_MLCB,int32_t data);	//电机编码器分辨率设置
uint8_t Motor_Speed_Mode(SERVO_MLCB *Servo_MLCB);						//设置电机为速度模式
uint8_t Motor_Position_Mode(SERVO_MLCB *Servo_MLCB);					//设置电机为位置(角度)模式
uint8_t Motor_PeakCurrent_Set(SERVO_MLCB *Servo_MLCB,int32_t data);//设置电机的峰值电流
uint8_t Motor_ContinuousCurrent_Set(SERVO_MLCB *Servo_MLCB,int32_t data);//设置电机的连续电流
uint8_t Motor_Save_Param(SERVO_MLCB *Servo_MLCB);					//保存以上参数

#endif




