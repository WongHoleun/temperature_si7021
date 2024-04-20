/**
****************************************************************************************
* @Description:系统检测状态机程序
* @Author: Haolin Wang
* @Date: 2023-06-03 17:31:33
* @LastEditTime: 2023-06-03 17:33:12
* @Note:用于分时段启动不同传感器通信
****************************************************************************************
*/

#include "stdio.h"
#include "math.h"
#include "stdbool.h"
#include "State_Machine.h"
#include "SI7021.h"

SYSTEM_STATE StateMachine_system; // 状态机

/* 全局变量 */
float Average_Tw = 0;
float Average_He = 0;
float Average_Ts = 0;
extern bool Flag;	// 声明外部变量
int i;
int n = 10;

static void delay_s(uint16_t ns)
{
		__HAL_TIM_SetCounter(&htim10, 0);//htim10

		__HAL_TIM_ENABLE(&htim10);

		while(__HAL_TIM_GetCounter(&htim10) < (10000 * ns));//计数频率10KHz，10 000次即为1s
		/* Disable the Peripheral */
		__HAL_TIM_DISABLE(&htim10);
}


/**
****************************************************************************************
* @Funticon name: 状态机程序段
* @Berif: 具有5种状态的状态机，同时在中断期间一直收集样本。
* @Note: C 语言中标签(例如case)的下一条语句不能是定义变量的表达式，需要通过大括号将语句变成复合语句
* @return {*}
****************************************************************************************
*/
bool Detection_Task(void)
{

    /* 状态机循环 */
	switch (StateMachine_system)
	{

//		case STATE_SI7021: // 现态：与 SI7021 进行通信
//		{
//			int sum_He = 0, sum_Ts = 0;
//			uint16_t He, Ts;
//
//			/* 动作 */
//			He = SI7021_measure_humidity();          // 环境湿度
//			Ts = SI7021_read_previous_temperature(); // 环境温度
//
//			for (i = 0; i < n; i++)
//			{
//				He = SI7021_measure_humidity();          // 环境湿度
//				Ts = SI7021_read_previous_temperature(); // 环境温度
//				sum_He += He;
//				sum_Ts += Ts;
//				delay_s(2);
//			}
//			Average_He = (0.01 * sum_He) / (float)n; // 求平均值
//			Average_Ts = (0.01 * sum_Ts) / (float)n; // 求平均值
//
//			/* 串口通信 */
//			printf("Environment humidity=%.2f%% \r\n", Average_He);    // Environment humidity(EHu)
//			printf("Environment temperature=%.2f \r\n", Average_Ts); // Environment temperature(ETe)
//			HAL_Delay(50);
//
//			/*次态*/
//			StateMachine_system = STATE_SI7021; // 准备与 RTD 建立通信
//			break;
//		}   // case STATE_SI7021

		case STATE_Temp: // 现态：与 RTD 进行通信
		{
			float sum_Tw = 0;
			int Tw = 0;

			/*动作*/
			for (i = 0; i < n; i++)
			{
				Tw = si7021_measure_temperature(); // 皮肤温度
				printf("No. %d: %.2f\r\n", i, 0.01*Tw);
				sum_Tw += Tw;
				delay_s(2);
			}
			Average_Tw = (0.01 * sum_Tw) / (float)n; // 求平均值

			Flag = false;	// 运行标志

			/* 串口通信 */
			printf("Skin temperature=%.2f \r\n", Average_Tw); // Skin temperature（STe）
			HAL_Delay(50);

			/*次态*/
			StateMachine_system = STATE_Temp; // 准备与 MLX90614 建立通信
			break;
		}   // case STATE_RTD


		default:
		{
			;
			break;
		}	// 默认情况处理

	} // switch

    return Flag;
} // bool DETECTION_Task(void)

void SYSTEM_STATE_Init(void)
{
    StateMachine_system = STATE_Temp; // 状态机初始状态
}
