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

/* λѡ�˿� */
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

// �����λѡ״̬
typedef enum seg_status_etype
{
    DISPLAY,
    NO_DISPLAY
} seg_status;

#define SEG_NUM 3 // ����ܸ���

typedef struct _segment_s_
{
    uint8_t display_char[SEG_NUM]; // ÿ������ܵ���ʾ���ݣ����Ϊ0�ţ����������ε���
    uint8_t bits_status[SEG_NUM]; // �����λѡ״̬���ο�@seg_status_etype
    const uint8_t *p_segtab[SEG_NUM]; // ָ��ͬ�Ķ����
    uint8_t vanishing_time; // ��Ӱʱ�䣬��λms
    uint8_t display_mode; // ��ʾģʽ��2λС����1λС��
} segment_s;

extern segment_s seg_ctl;

void display_init();
void show_float_num(float val);
void show_int_num(int val);
void show_ch(char ch);
#endif /* __DISPLAY_H__ */