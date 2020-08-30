/*
 * 程序说明：ADC1采用单次转换模式，使用查询方式进行读取转换的AD值，
 * 故调用转换时，每次都需要重新启动转换。
 */

#include "adc1.h"

adc_s g_adc;

/**
 * @brief 获取一次AD值
 * 
 */
static uint16_t get_adc1_val()
{  
    /* 转换完成标志位 */
    uint16_t flag_status = RESET; 
    /* 启动转换 */
    ADC1_StartConversion();
    /* 等待转换完成 */   
    flag_status = ADC1_GetFlagStatus(ADC1_FLAG_EOC);
    while(flag_status == RESET);
    
    /* 返回AD值 */
    return ADC1_GetConversionValue();
}

/**
 * @brief ADC1初始化
 * 
 */
void adc1_init()
{   
    /* 初始化结构体成员 */
    g_adc.adc_voltage = 0.0f;
    g_adc.sampling_times = TIMES;
    g_adc.vref = VREF;
    g_adc.adc_max = ADC_MAX;

    /* 复位ADC寄存器的值 */
    ADC1_DeInit();
    /* 初始化ADC */
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,
              ADC1_CHANNEL_3,
              ADC1_PRESSEL_FCPU_D2, 
              ADC1_EXTTRIG_TIM, 
              DISABLE, 
              ADC1_ALIGN_RIGHT, 
              ADC1_SCHMITTTRIG_CHANNEL3, 
              ENABLE);
    /* 开启ADC */
    ADC1_Cmd(ENABLE);
}

/**
 * @brief 获取平均ADC值，范围0~ADC_MAX-1
 * 
 * @return uint16_t [O] 返回获取到的AD值
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
 * @brief 获取采样后的电压值
 * 
 * @return float [O] 返回获取到的电压
 */
float get_voltage()
{
    get_adc_val();
    g_adc.adc_voltage = (float)g_adc.adc_val / g_adc.adc_max * g_adc.vref * RATIO + OFFSET;
    
    return g_adc.adc_voltage;
}