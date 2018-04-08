/*******************************************************************************

                      版权所有 (C), 2017- Mushiny.

 *******************************************************************************
  文 件 名   : main.h
  版 本 号   : 初稿
  作    者   : hzh
  生成日期   :2017.11.5
  最近修改   :
  功能描述   : 应用程序接口声明模块
  函数列表   :
  修改历史   :
*******************************************************************************/
#ifndef _APP_H
#define _APP_H

/* 包含头文件 ----------------------------------------------------------------*/
#include "includes.h"
#include "sys.h"
#include "mylib.h"
#include <rsh.h>

/* 本模块向外部提供的宏定义 --------------------------------------------------*/
#define LOGO_STRING                     "NCURoboteam-A Robot# "
#define _MAIN_DEBUG_                  1
#define _CLI_ENABLE_                  1

/* 本模块向外部提供的宏定义 --------------------------------------------------*/
typedef volatile struct
{
  uint8_t  control_num;     //当前控制次数	
	uint8_t  control_final;   //最终停止次数
	float    P;               //闭环P值
  float    D;               //闭环D值
	float    Xtime;           //理论x轴时间
	int      turn;            //加速旋转圈数
	int      speed;           //抛掷速度
	int      Vspeed;          //理论y轴速度
	int      Aspeed;          //调整y轴速度
	int      point;           //释放点
	int      init_point;      //初始位置
	int      target_distance; //减速距离
	int      real_distance;   //实际位置
	int      theo_distance;   //理论位置
	int      offe_distance;   //偏差距离
	int      offe_lastance;   //偏差距离
	int      time;            //停止时间
}THROW_INDEX;  //抛掷参数

 typedef struct {
	/*****************跟踪微分器参数**********************/
  float r;//快速因子 时间尺度
	float h;//滤波因子 积分时间
	float n;
  float h0;//h0=n*h；
	
	float x1;//跟踪微分器跟踪信号
	float x2;//跟踪微分器微分跟踪信号
	float fh;
/********************非线性组合参数*********************/
	float b1;
	float theta;
	float b2;
	float alphal;
	
/**********************输出信号************************/
  float u0;	
	float e0;
	float e1;
	float e2;
	
}adrc;


/* 本模块向外部提供的接口常量声明 --------------------------------------------*/

#define Pi 3.14159f               //π
#define TIME_cyc 20.0f            //控制周期 ms
#define cycle   64000          //输出轴旋转一圈，编码器返回的脉冲值
#define throw_speed_max   200     //正向最大速度
#define throw_speed_min  -200     //反向最大速度  
#define CAN_Delay        50     //CAN延时次数

#define ONE    1  //TZ1状态.
#define TWO    2  //TZ2状态
#define THREE  3  //TZ3状态 


#define _delay 0        //等待调度状态
#define _calibration 1  //标定状态
#define _connect 2      //交接状态
#define _throw 3        //抛掷状态
#define _ready_connect 4//前往交接点
#define _ready_throw 5  //前往抛掷点
#define _ready_location_one 6//前往定位点
#define _ready_location_two 7//前往定位点
/* 本模块向外部提供的接口变量声明 --------------------------------------------*/
extern uint8_t motor_init_flag;
extern uint8_t throw_flag;
extern uint8_t Robot_Mode;
extern uint8_t place;
/* 本模块向外部提供的接口函数原型声明 ----------------------------------------*/
void BSP_Init(void);
void retarget_init(void);
int  asy_printf(const char *fmt, ...);
char put_char(char ch);

void motor_all_Init(void);
void motor_move_setvmmps(int dstVmmps_X,int dstVmmps_Y,int dstVmmps_W);
void motor_move_stop(void);
void IWDG_Feed(void);
void throw_init(uint8_t area);
void throw_ball(uint8_t area);
void throw_stop(uint8_t area);
void throw_adjust(void);

void create_star_task(uint8_t prio);
void create_IWDG_task(uint8_t prio);
void create_motor_driver_task(uint8_t prio);
void create_hand_task(uint8_t prio);
void create_key_task(uint8_t prio);
void create_display_task(uint8_t prio);
void create_command_task(uint8_t prio);
void create_throw_task(uint8_t prio);
void create_connect_task(uint8_t prio);

/* 全局配置区 ----------------------------------------*/


//位操作声明
#ifndef BIT
#define BIT(n)                (1 << (n))
#define bit(n)                (1 << (n))
#endif

#define setbit(reg,bits)      (reg) |= (bits)
#define clearbit(reg,bits)    (reg) &= ~(bits)
#define readbit(reg,bits)     ((reg) & (bits))
#define togglebit(reg,bits)   (reg) ^= (bits)

// 调试宏，printf状态跟踪调试
//各串口寄存器地址
#define UART1_DR_Addr         (USART1_BASE + 0x04)
#define UART2_DR_Addr         (USART2_BASE + 0x04)
#define UART3_DR_Addr         (USART3_BASE + 0x04)
#define UART4_DR_Addr         (UART4_BASE  + 0x04)
#define UART5_DR_Addr         (UART5_BASE  + 0x04)
#define UART6_DR_Addr         (USART6_BASE + 0x04)

#define SYSTEM_DEBUG                  1
#define DEBUG_TRACE                   asy_printf
#define os_printf                     asy_printf

// Printf UART 蓝牙
#define PrintfUART_AF                 GPIO_AF_UART4
#define PrintfUART_RCC                RCC_APB1Periph_UART4

#define PrintfUART_TXD_GPIO           GPIOC
#define PrintfUART_TXD_PIN            GPIO_Pin_10

#define PrintfUART_RXD_GPIO           GPIOC
#define PrintfUART_RXD_PIN            GPIO_Pin_11

#define PrintfUART                    UART4
#define PrintfUART_DR_Addr            UART4_DR_Addr
#define PrintfUART_BAUD               115200
#define PrintfUART_TxBufSize          128
#define PrintfUART_RxBufSize          128
#define PrintfUART_IRQn               UART4_IRQn
#define PrintfUART_IRQHandler         UART4_IRQHandler



/*以下为手柄测试模块*/
#define  DI    PAin(8)                //PB12输入  接DATA线

#define  DO_H  PAout(9)=1              //命令位拉高  对应CMD
#define  DO_L  PAout(9)=0              //命令位低

#define  CS_H  PAout(10)=1              // ATT线
#define  CS_L  PAout(10)=0              //CS拉低

#define  CLK_H PAout(11)=1              //时钟拉高
#define  CLK_L PAout(11)=0              //时钟拉低
#define DELAY_TIME  delay_us(5); 
//These are our button constants按钮对应的常量
#define PSB_SELECT      1
#define PSB_L3          2        //左摇杆
#define PSB_R3          3        //右摇杆
#define PSB_START       4  
#define PSB_PAD_UP      5        //上下左右键
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9        //左2
#define PSB_R2          10       //右2
#define PSB_L1          11       //左1
#define PSB_R1          12       //右1
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define L1_SANJIAO      17
#define L1_CHA          18
#define L1_ZFX          19
#define L1_QQ           20
void PS2_Cmd(u8 CMD);
void PS2_ClearData();
void PS2_ReadData(void);
u8 PS2_DataKey();


#endif
/************** 版权所有 (C), 2017- Mushiny. ***********本文件结束***********/


