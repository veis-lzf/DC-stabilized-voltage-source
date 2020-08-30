#ifndef __ADC1_H__
#define __ADC1_H__

#include "stm8s_adc1.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"

/* ADC�������� */
#define ADC_MAX 1024 // ADC����
#define VREF 3.33f // �ο���ѹ
#define TIMES 10 // ��������
#define RATIO 4 // ���������ѹ��������
#define OFFSET 0.025

/* ADC���IO���� */
#define ADC_GPIO_PORT GPIOD
#define ADC_PIN GPIO_PIN_2

/* ADC������������ */
typedef struct _adc_s_
{
    // �������ADֵ�͵�ѹֵ
    uint16_t adc_val; // ADֵ
    float adc_voltage; // ��ѹֵ
    uint16_t adc_max; // ADC���ֵ 
    float vref; // �ο���ѹ
    uint16_t sampling_times; // �������� 
} adc_s;

/* ADC�ṹ�����ݹ������ */
extern adc_s g_adc;

/* �ⲿ�ӿں��� */
void adc1_init(void); /* ��ʼ��ADC1 */
float get_voltage(void); /* ��ȡADC��ѹֵ */
uint16_t get_adc_val(void); /* ��ȡADCֵ */

#endif /* __ADC1_H__ */
