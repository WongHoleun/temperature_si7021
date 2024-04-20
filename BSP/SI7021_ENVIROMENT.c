/**
****************************************************************************************
* @Description: �¶ȴ�����SI7021����ģ��
* 					����ģ��IIC��Ӳ��IIC�����汾
* @Author: Haolin Wang
* @Date: 2023-03-29 16:24:36
* @LastEditTime: 2023-04-06 18:16:36
* @Note: 1.��ͨ����ʽ������ʪ��ֵʱ����Ϊ216 - 1��65535��175/65535 = 0.002670328���������������
* ��������float�ľ���Ϊ6~7λ��Ч���֣�ֱ�Ӽ���Ļ��൱�ڰ�С�����7λ֮�������ʡ�Ե��ˣ�
* ����ɼ����������ʪ�Ⱦ�����Щ��ʧ���������������ķ��������ù�ʽ����ֵ��*100��
* �����Ļ������������ʪ��ֵҲ�� *100�����������С��λ�ͻ����λ�����Ƚ�Ϊ׼ȷ��
* �ڸ������������ǰ�� *0.01��������������ֵ����
* 2. ��λ�������ûᷢ������ �޷��ش�����
****************************************************************************************
*/

#include "SI7021.h"
#include "stdio.h"
#include "main.h"

/* ģ��IIC IO���� */
#define SI7021_IIC_SCL(x)                                                                                                                                           \
	do                                                                                                                                                       \
	{                                                                                                                                                        \
		x ? HAL_GPIO_WritePin(SI7021_SCL_GPIO_Port, SI7021_SCL_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(SI7021_SCL_GPIO_Port, SI7021_SCL_Pin, GPIO_PIN_RESET); \
	} while (0) // SCL

#define SI7021_IIC_SDA(x)                                                                                                                                           \
	do                                                                                                                                                       \
	{                                                                                                                                                        \
		x ? HAL_GPIO_WritePin(SI7021_SDA_GPIO_Port, SI7021_SDA_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(SI7021_SDA_GPIO_Port, SI7021_SDA_Pin, GPIO_PIN_RESET); \
	} while (0) // SDA

#define SI7021_READ_SDA HAL_GPIO_ReadPin(SI7021_SDA_GPIO_Port, SI7021_SDA_Pin) // ��ȡSDA

#define DLY_TIM_Handle_SI7021 (&htim11) // ��ʱ��ѡ��

/**
****************************************************************************************
* @Funticon name: ΢���ʱ
* @Berif:
* @Note:
* @param {uint16_t} nus	��ʱʱ��
* @return {*}
****************************************************************************************
*/
static void delay_us(uint16_t nus)
{
	__HAL_TIM_SET_COUNTER(DLY_TIM_Handle_SI7021, 0);
	__HAL_TIM_ENABLE(DLY_TIM_Handle_SI7021);
	while (__HAL_TIM_GET_COUNTER(DLY_TIM_Handle_SI7021) < nus)
	{
	}
	__HAL_TIM_DISABLE(DLY_TIM_Handle_SI7021);
}

/**
****************************************************************************************
* @Funticon name: ����IIC��ʼ�ź�
* @Berif:
* @Note:
* @return {*}
****************************************************************************************
*/
void SI7021_iic_start(void)
{
	SI7021_IIC_SDA(1);
	SI7021_IIC_SCL(1);
	delay_us(10); // SI7021 ����4.7us
	SI7021_IIC_SDA(0);	  // START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź�
	delay_us(10); // SI7021 ����4us
	SI7021_IIC_SCL(0);	  // ǯסI2C���ߣ�׼�����ͻ��������
}

/**
****************************************************************************************
* @Funticon name: ����IICֹͣ�ź�
* @Berif:
* @Note:
* @return {*}
****************************************************************************************
*/
void SI7021_iic_stop(void)
{
	SI7021_IIC_SDA(0); // STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź�
	delay_us(10);
	SI7021_IIC_SCL(1);
	delay_us(10);
	SI7021_IIC_SDA(1); // ����I2C���߽����ź�
	delay_us(10);
}

/**
****************************************************************************************
* @Funticon name: �ȴ�Ӧ���źŵ���
* @Berif:
* @Note:
* @return {*}	1������Ӧ��ʧ�ܣ�
* 				0������Ӧ��ɹ�
****************************************************************************************
*/
uint8_t SI7021_iic_wait_ack(void)
{
	uint8_t waittime = 0;
	uint8_t rack = 0;

	SI7021_IIC_SDA(1); // �����ͷ�SDA��(��ʱ�ⲿ������������SDA��)
	delay_us(10);
	SI7021_IIC_SCL(1); // SCL=1, ��ʱ�ӻ����Է���ACK
	delay_us(10);

	while (SI7021_READ_SDA) // �ȴ�Ӧ��
	{
		waittime++;

		if (waittime > 250)
		{
			SI7021_iic_stop();
			rack = 1;
			break;
		}
	}

	SI7021_IIC_SCL(0); // SCL=0, ����ACK���
	delay_us(10);
	return rack;
}

/**
****************************************************************************************
* @Funticon name: ����ACKӦ��
* @Berif:
* @Note:
* @return {*}
****************************************************************************************
*/
void SI7021_iic_ack(void)
{
	SI7021_IIC_SDA(0); // SCL 0 -> 1  ʱ SDA = 0,��ʾӦ��
	delay_us(10);
	SI7021_IIC_SCL(1); // ����һ��ʱ��
	delay_us(10);
	SI7021_IIC_SCL(0);
	delay_us(10);
	SI7021_IIC_SDA(1); // �����ͷ�SDA��
	delay_us(10);
}

/**
****************************************************************************************
* @Funticon name: ������ACKӦ��
* @Berif:
* @Note:
* @return {*}
****************************************************************************************
*/
void SI7021_iic_nack(void)
{
	SI7021_IIC_SDA(1); // SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ��
	delay_us(10);
	SI7021_IIC_SCL(1); // ����һ��ʱ��
	delay_us(10);
	SI7021_IIC_SCL(0);
	delay_us(10);
}

/**
****************************************************************************************
* @Funticon name: IIC����һ���ֽ�
* @Berif:
* @Note:
* @param {uint8_t} data	Ҫ���͵�����
* @return {*}
****************************************************************************************
*/
void SI7021_send_byte(uint8_t data)
{
	uint8_t t;

	for (t = 0; t < 8; t++)
	{
		SI7021_IIC_SDA((data & 0x80) >> 7); // ��λ�ȷ���
		delay_us(10);
		SI7021_IIC_SCL(1);
		delay_us(10);
		SI7021_IIC_SCL(0);
		data <<= 1; // ����1λ,������һ�η���
	}
	SI7021_IIC_SDA(1); // �������, �����ͷ�SDA��
}

/**
****************************************************************************************
* @Funticon name: IIC��ȡһ���ֽ�
* @Berif: ack=1ʱ������ack; ack=0ʱ������nack
* @Note:
* @param {uint8_t} ack	�Ƿ�Ӧ��
* @return {*}
****************************************************************************************
*/
uint8_t SI7021_read_byte(uint8_t ack)
{
	uint8_t i, receive = 0;

	for (i = 0; i < 8; i++) // ����1���ֽ�����
	{
		receive <<= 1; // ��λ�����,�������յ�������λҪ����
		SI7021_IIC_SCL(1);
		delay_us(10);

		if (SI7021_READ_SDA)
		{
			receive++;
		}

		SI7021_IIC_SCL(0);
		delay_us(10);
	}

	if (!ack)
	{
		SI7021_iic_nack(); // ����nACK
	}
	else
	{
		SI7021_iic_ack(); // ����ACK
	}

	return receive;
}

/**
****************************************************************************************
* @Funticon name: ���ֽ�д�봫����
* @Berif: ���ڷ������ݵ�SI7021
* @Note:
* @param {uint8_t} REG_address	�Ĵ�����ַ
* @return {*}
****************************************************************************************
*/
void SI7021_write_command(uint8_t REG_address)
{
	SI7021_iic_start();

	SI7021_send_byte(SI7021_ADDRESS_WRITE);
	SI7021_iic_wait_ack();

	SI7021_send_byte(REG_address);
	SI7021_iic_wait_ack();

	SI7021_iic_stop();
}

/**
****************************************************************************************
* @Funticon name: ���ֽڶ�ȡ
* @Berif: ���ڶ�ȡSI7021����������
* @Note: 2���ֽ�����
* @param {uint8_t} REG_address	�Ĵ�����ַ
* @param {uint16_t} *value	����ֵ
* @return {*}
****************************************************************************************
*/
void SI7021_read_data(uint8_t REG_address, uint16_t *value)
{
	uint8_t Si7021_BUF[2] = {0}; // �������鶨��

	SI7021_iic_start(); // IIC��ʼ�ź�

	SI7021_send_byte(SI7021_ADDRESS_WRITE); // дָ��
	SI7021_iic_wait_ack();						 // �ȴ�Ӧ��

	SI7021_send_byte(REG_address);
	SI7021_iic_wait_ack(); // �ȴ�Ӧ��

	HAL_Delay(19); // �ȴ���ʪ��ת����ɣ�����Ӧ�ô���18ms

	SI7021_iic_start();
	SI7021_send_byte(SI7021_ADDRESS_READ); // ��ָ��
	SI7021_iic_wait_ack();
	Si7021_BUF[0] = SI7021_read_byte(1); // �������� ��8λ ���ݲ�����Ӧ���ź�
	Si7021_BUF[1] = SI7021_read_byte(0); // �������� ��8λ ���ݲ����ͷ�Ӧ���ź�

	SI7021_iic_stop(); // ֹͣ�ź�

	*value = ((Si7021_BUF[0] << 8) + Si7021_BUF[1]); // ���ߵ�8λ���ݺϳ�Ϊ16λ����
}

/**
****************************************************************************************
* @Funticon name: ��ȡ��ǰ�¶ȣ�ʪ�ȶ�Ӧ�¶ȣ�
* @Berif: ��ȡ��ǰͨ�����á�si7021_measure_humidity��������ɵ��¶Ȳ���
* @Note: ��ȡʪ�Ȳ��������¶ȣ�0xE0�� ʱ������ִ�в��������������ʪ�Ȳ����ڼ�������¶�ֵ��
* @param {I2C_HandleTypeDef} *hi2c
* @return {*}
****************************************************************************************
*/
int32_t SI7021_read_previous_temperature(void)
{
	uint16_t temp;

	SI7021_read_data(SI7021_READ_PREV_TEMP, &temp); // ��ȡ��ǰ�¶�
	return (temp * 17572 / 65536 - 4685);				// ʪ�ȼ��㹫ʽ  �� %RH Ϊ��λ���������ʪ��ֵ ������100��												   // �¶�ת����ʽ �ѽ�����һ�ٱ�
}

/**
****************************************************************************************
* @Funticon name: ʪ�ȼ��
* @Berif:
* @Note: ��ʼʪ�Ȳ�������Ϊ������¶�Ҳ�ᱻ������Ҫ��ȡ��ʱ�¶�ֵ��ʹ�á�SI7021_read_previous_temperature������
* @param {I2C_HandleTypeDef} *hi2c
* @return {*}
****************************************************************************************
*/
uint32_t SI7021_measure_humidity(void)
{
	uint16_t humi;

	SI7021_read_data(SI7021_MEASURE_NOHOLD, &humi); // NOHOLD_MASTERģʽ�¶�ȡʪ��
	return (humi * 12500 / 65536 - 600);				// ʪ�ȼ��㹫ʽ  �� %RH Ϊ��λ���������ʪ��ֵ ������100��
}

/**
****************************************************************************************
* @Funticon name: SI7021��ʼ��
* @Berif: ���þ������������ʼ��������SI7021
* @Note: ��ʹ�ò���ϵͳ����Ҫ���������ں�����ǰ�����Χ��·��ʼ��
* @param {I2C_HandleTypeDef} *hi2c
* @return {*}
****************************************************************************************
*/
void SI7021_init(void)
{
	SI7021_IIC_SDA(1); // ��ʼ��ƽ��Ϊ��
	SI7021_IIC_SCL(1);
	delay_us(10);

	SI7021_write_command(SI7021_RESET);
	delay_us(5);
}