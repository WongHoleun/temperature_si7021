/**
****************************************************************************************
* @Description:系统检测状态机头文件
* @Author: Haolin Wang
* @Date: 2023-06-03 17:31:33
* @LastEditTime: 2023-06-03 17:33:12
* @Note:用于分时段启动不同传感器通信
****************************************************************************************
*/

#ifndef __State_Machine_H
#define __State_Machine_H

#include "stm32f4xx_hal.h"
extern TIM_HandleTypeDef htim10;

/*定义状态机*/
typedef enum
{
    STATE_MAX3010x, // 状态1：MAX3010x 传感器
    STATE_SI7021,   // 状态2：SI7021 传感器测量环境温湿度
    STATE_Temp,     // 状态3：si7021 传感器测量皮肤温度
    STATE_MLX90614, // 状态4：MLX90614 传感器
    STATE_MHC,       // 状态5：代谢热计算
	STATE_BV_DPPG    // 状态6：血压血流计算
} SYSTEM_STATE;

/* 人机交互API */
bool Detection_Task(void);
void SYSTEM_STATE_Init(void);

#endif /* State_Machine_H_ */
