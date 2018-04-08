#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include "includes.h"

typedef enum
{
	Write_Success=0,
}SERVO_ERROR;

/********�����ŷ�������ƿ�********/
typedef struct
{
	char		   *Servo_Mlcb_Name;			//�ŷ����ƿ�����
	uint8_t     Star_Id;							//��ʶ��ID
	uint8_t     Device_Id;						//�豸ID
	uint8_t     Ratio;								//���������
	uint16_t    Coder;								//����������
	SERVO_ERROR Servo_Error;					//�������
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

void Create_MLServo_CB(SERVO_MLCB		*servo_mlcb,					//�ŷ����ƿ�
											 char       	*servo_mlcb_name,			//�ŷ����ƿ�����
											 uint8_t     	 star_id,							//��ʶ��ID
											 uint8_t       ratio,								//���������
											 uint16_t      coder);							//����������

uint8_t MLServo_Write(SERVO_MLCB *Servo_MLCB, uint8_t order, uint8_t len, int32_t data);

/*****************************************��ؼ�Ҫ���*********************************************/
/*	����ٶ����	*/
uint8_t Motor_MastSpeed(SERVO_MLCB *Servo_MLCB,float data);	//	�������ٶ�����
uint8_t Motor_MinSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	�����С�ٶ�����
uint8_t Motor_SetSpeed(SERVO_MLCB *Servo_MLCB,int32_t data);	//	��������ٶ�����  
											 
uint8_t Motor_CBS(SERVO_MLCB *Servo_MLCB);
uint8_t Motor_SBS(SERVO_MLCB *Servo_MLCB);											 

/*	���ٶ�����	*/
uint8_t Motor_AccelSpeed(SERVO_MLCB *Servo_MLCB,float data);	//���õ�����ٶ�

/*	���λ������	*/
uint8_t Motor_AbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//���õ����ǰλ��Ϊ����λ��
uint8_t Motor_ToAbsolutePlace(SERVO_MLCB *Servo_MLCB,int32_t data);	//���õ�������λ�ÿ�ʼ����
uint8_t Motor_ToRelativePlace(SERVO_MLCB *Servo_MLCB,int data);	//���õ��������ϴ�����λ�ÿ�ʼ����
											 
uint8_t Motor_AbsolutePosition_Set(SERVO_MLCB *Servo_MLCB);	
uint8_t Motor_RelativePosition_Set(SERVO_MLCB *Servo_MLCB,int32_t data);

/*	�����ʼ������	*/
uint8_t Motor_P_SET(SERVO_MLCB *Servo_MLCB,int32_t data);	//���λ�ñ���P����
uint8_t Motor_Code_Set(SERVO_MLCB *Servo_MLCB,int32_t data);	//����������ֱ�������
uint8_t Motor_Speed_Mode(SERVO_MLCB *Servo_MLCB);						//���õ��Ϊ�ٶ�ģʽ
uint8_t Motor_Position_Mode(SERVO_MLCB *Servo_MLCB);					//���õ��Ϊλ��(�Ƕ�)ģʽ
uint8_t Motor_PeakCurrent_Set(SERVO_MLCB *Servo_MLCB,int32_t data);//���õ���ķ�ֵ����
uint8_t Motor_ContinuousCurrent_Set(SERVO_MLCB *Servo_MLCB,int32_t data);//���õ������������
uint8_t Motor_Save_Param(SERVO_MLCB *Servo_MLCB);					//�������ϲ���

#endif




