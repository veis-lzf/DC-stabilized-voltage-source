/*
 * ����˵����ADC1���õ���ת��ģʽ��ʹ�ò�ѯ��ʽ���ж�ȡת����ADֵ��
 * �ʵ���ת��ʱ��ÿ�ζ���Ҫ��������ת����
 */

#include "adc1.h"

adc_s g_adc;

/**
 * @brief ��ȡһ��ADֵ
 * 
 */
static uint16_t get_adc1_val()
{  
    /* ת����ɱ�־λ */
    uint16_t flag_status = RESET; 
    /* ����ת�� */
    ADC1_StartConversion();
    /* �ȴ�ת����� */   
    flag_status = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
    while(flag_status == RESET);
    
    /* ����ADֵ */
    return ADC1_GetConversionValue();
}

/**
 * @brief ADC1��ʼ��
 * 
 */
void adc1_init()
{   
    /* ��ʼ���ṹ���Ա */
    g_adc.adc_voltage = 0.0f;
    g_adc.sampling_times = TIMES;
    g_adc.vref = VREF;
    g_adc.adc_max = ADC_MAX;

    /* ��λADC�Ĵ�����ֵ */
    ADC1_DeInit();
    /* ��ʼ��ADC */
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
              ADC1_CHANNEL_3,
              ADC1_PRESSEL_FCPU_D2, 
              ADC1_EXTTRIG_TIM, 
              DISABLE, 
              ADC1_ALIGN_RIGHT, 
              ADC1_SCHMITTTRIG_CHANNEL3, 
              ENABLE);
    /* ����ADC */
    ADC1_Cmd(ENABLE);
}

/**
 * @brief ��ȡƽ��ADCֵ����Χ0~ADC_MAX-1
 * 
 * @return uint16_t [O] ���ػ�ȡ����ADֵ
 */
uint16_t get_adc_val()
{
    int i = 0;
    
    g_adc.adc_val = 0;
    while(i++ < g_adc.sampling_times)
    {
        g_adc.adc_val = g_adc.adc_val + get_adc1_val();
    }
    g_adc.adc_val = g_adc.adc_val / g_adc.sampling_times;
    
    return g_adc.adc_val;
}

/**
 * @brief ��ȡ������ĵ�ѹֵ
 * 
 * @return float [O] ���ػ�ȡ���ĵ�ѹ
 */
float get_voltage()
{
    get_adc_val();
    g_adc.adc_voltage = (float)g_adc.adc_val / g_adc.adc_max * g_adc.vref * RATIO + OFFSET;
    
    return g_adc.adc_voltage;
}