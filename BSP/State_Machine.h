/**
****************************************************************************************
* @Description:ϵͳ���״̬��ͷ�ļ�
* @Author: Haolin Wang
* @Date: 2023-06-03 17:31:33
* @LastEditTime: 2023-06-03 17:33:12
* @Note:���ڷ�ʱ��������ͬ������ͨ��
****************************************************************************************
*/

#ifndef __State_Machine_H
#define __State_Machine_H

#include "stm32f4xx_hal.h"
extern TIM_HandleTypeDef htim10;

/*����״̬��*/
typedef enum
{
    STATE_MAX3010x, // ״̬1��MAX3010x ������
    STATE_SI7021,   // ״̬2��SI7021 ����������������ʪ��
    STATE_Temp,     // ״̬3��si7021 ����������Ƥ���¶�
    STATE_MLX90614, // ״̬4��MLX90614 ������
    STATE_MHC,       // ״̬5����л�ȼ���
	STATE_BV_DPPG    // ״̬6��ѪѹѪ������
} SYSTEM_STATE;

/* �˻�����API */
bool Detection_Task(void);
void SYSTEM_STATE_Init(void);

#endif /* State_Machine_H_ */
