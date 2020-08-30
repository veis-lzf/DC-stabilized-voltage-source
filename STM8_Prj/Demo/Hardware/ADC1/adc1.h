#ifndef __ADC1_H__
#define __ADC1_H__

#include "stm8s_adc1.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"

/* ADC参数定义 */
#define ADC_MAX 1024 // ADC精度
#define VREF 3.33f // 参考电压
#define TIMES 10 // 采样次数
#define RATIO 4 // 采样电阻分压比例因子
#define OFFSET 0.025

/* ADC相关IO定义 */
#define ADC_GPIO_PORT GPIOD
#define ADC_PIN GPIO_PIN_2

/* ADC采样数据类型 */
typedef struct _adc_s_
{
    // 采样结果AD值和电压值
    uint16_t adc_val; // AD值
    float adc_voltage; // 电压值
    uint16_t adc_max; // ADC最大值 
    float vref; // 参考电压
    uint16_t sampling_times; // 采样次数 
} adc_s;

/* ADC结构体数据管理变量 */
extern adc_s g_adc;

/* 外部接口函数 */
void adc1_init(void); /* 初始化ADC1 */
float get_voltage(void); /* 获取ADC电压值 */
uint16_t get_adc_val(void); /* 获取ADC值 */

#endif /* __ADC1_H__ */
