/**
****************************************************************************************
* @Description:ϵͳ���״̬������
* @Author: Haolin Wang
* @Date: 2023-06-03 17:31:33
* @LastEditTime: 2023-06-03 17:33:12
* @Note:���ڷ�ʱ��������ͬ������ͨ��
****************************************************************************************
*/

#include "stdio.h"
#include "math.h"
#include "stdbool.h"
#include "State_Machine.h"
#include "SI7021.h"

SYSTEM_STATE StateMachine_system; // ״̬��

/* ȫ�ֱ��� */
float Average_Tw = 0;
float Average_He = 0;
float Average_Ts = 0;
extern bool Flag;	// �����ⲿ����
int i;
int n = 10;

static void delay_s(uint16_t ns)
{
		__HAL_TIM_SetCounter(&htim10, 0);//htim10

		__HAL_TIM_ENABLE(&htim10);

		while(__HAL_TIM_GetCounter(&htim10) < (10000 * ns));//����Ƶ��10KHz��10 000�μ�Ϊ1s
		/* Disable the Peripheral */
		__HAL_TIM_DISABLE(&htim10);
}


/**
****************************************************************************************
* @Funticon name: ״̬�������
* @Berif: ����5��״̬��״̬����ͬʱ���ж��ڼ�һֱ�ռ�������
* @Note: C �����б�ǩ(����case)����һ����䲻���Ƕ�������ı��ʽ����Ҫͨ�������Ž�����ɸ������
* @return {*}
****************************************************************************************
*/
bool Detection_Task(void)
{

    /* ״̬��ѭ�� */
	switch (StateMachine_system)
	{

//		case STATE_SI7021: // ��̬���� SI7021 ����ͨ��
//		{
//			int sum_He = 0, sum_Ts = 0;
//			uint16_t He, Ts;
//
//			/* ���� */
//			He = SI7021_measure_humidity();          // ����ʪ��
//			Ts = SI7021_read_previous_temperature(); // �����¶�
//
//			for (i = 0; i < n; i++)
//			{
//				He = SI7021_measure_humidity();          // ����ʪ��
//				Ts = SI7021_read_previous_temperature(); // �����¶�
//				sum_He += He;
//				sum_Ts += Ts;
//				delay_s(2);
//			}
//			Average_He = (0.01 * sum_He) / (float)n; // ��ƽ��ֵ
//			Average_Ts = (0.01 * sum_Ts) / (float)n; // ��ƽ��ֵ
//
//			/* ����ͨ�� */
//			printf("Environment humidity=%.2f%% \r\n", Average_He);    // Environment humidity(EHu)
//			printf("Environment temperature=%.2f \r\n", Average_Ts); // Environment temperature(ETe)
//			HAL_Delay(50);
//
//			/*��̬*/
//			StateMachine_system = STATE_SI7021; // ׼���� RTD ����ͨ��
//			break;
//		}   // case STATE_SI7021

		case STATE_Temp: // ��̬���� RTD ����ͨ��
		{
			float sum_Tw = 0;
			int Tw = 0;

			/*����*/
			for (i = 0; i < n; i++)
			{
				Tw = si7021_measure_temperature(); // Ƥ���¶�
				printf("No. %d: %.2f\r\n", i, 0.01*Tw);
				sum_Tw += Tw;
				delay_s(2);
			}
			Average_Tw = (0.01 * sum_Tw) / (float)n; // ��ƽ��ֵ

			Flag = false;	// ���б�־

			/* ����ͨ�� */
			printf("Skin temperature=%.2f \r\n", Average_Tw); // Skin temperature��STe��
			HAL_Delay(50);

			/*��̬*/
			StateMachine_system = STATE_Temp; // ׼���� MLX90614 ����ͨ��
			break;
		}   // case STATE_RTD


		default:
		{
			;
			break;
		}	// Ĭ���������

	} // switch

    return Flag;
} // bool DETECTION_Task(void)

void SYSTEM_STATE_Init(void)
{
    StateMachine_system = STATE_Temp; // ״̬����ʼ״̬
}
