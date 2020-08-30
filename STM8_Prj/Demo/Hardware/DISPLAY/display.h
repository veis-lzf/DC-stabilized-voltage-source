#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "stm8s_gpio.h"
#include "stm8s_clk.h"

#define A_PORT GPIOA
#define A_PIN GPIO_PIN_1
#define B_PORT GPIOA
#define B_PIN GPIO_PIN_2
#define C_PORT GPIOA
#define C_PIN GPIO_PIN_3
#define D_PORT GPIOC
#define D_PIN GPIO_PIN_3
#define E_PORT GPIOC
#define E_PIN GPIO_PIN_4
#define F_PORT GPIOC
#define F_PIN GPIO_PIN_5
#define G_PORT GPIOC
#define G_PIN GPIO_PIN_6
#define DP_PORT GPIOC
#define DP_PIN GPIO_PIN_7

/* 位选端口 */
#define BIT0_PORT GPIOB
#define BIT0_PIN GPIO_PIN_4
#define BIT1_PORT GPIOB
#define BIT1_PIN GPIO_PIN_5
#define BIT2_PORT GPIOD
#define BIT2_PIN GPIO_PIN_4

enum display_mode_etype
{
    TwoDigitMode,
    OneDigitMode
};

// 数码管位选状态
typedef enum seg_status_etype
{
    DISPLAY,
    NO_DISPLAY
} seg_status;

#define SEG_NUM 3 // 数码管个数

typedef struct _segment_s_
{
    uint8_t display_char[SEG_NUM]; // 每个数码管的显示内容，左边为0号，从左到右依次递增
    uint8_t bits_status[SEG_NUM]; // 数码管位选状态，参考@seg_status_etype
    const uint8_t *p_segtab[SEG_NUM]; // 指向不同的段码表
    uint8_t vanishing_time; // 消影时间，单位ms
    uint8_t display_mode; // 显示模式，2位小数和1位小数
} segment_s;

extern segment_s seg_ctl;

void display_init();
void show_float_num(float val);
void show_int_num(int val);
void show_ch(char ch);
#endif /* __DISPLAY_H__ */