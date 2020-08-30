#include "display.h"

/* �ⲿms��ʱ��������ֲ������ƽ̨��Ҫ����delay_ms������� */
extern void delay_ms(const uint32_t nms);
#define DELAY_MS delay_ms

/* ��������ܳ�Ա */
segment_s seg_ctl;

/* ��������ܶ�ѡ�����С���㣬0~F */
static const unsigned char seg_tab[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
	0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, 0xff
};

/* ��������ܶ�ѡ�����С���㣬0~F */
static const unsigned char seg_tab_p[] = {
	0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78,
	0x00, 0x10, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0e, 0xff
};

/**
 * @brief ��ʼ������ܶ�����ص�IO��
 * 
 */
static void gpio_config()
{
    /* ��ѡ�˿ڳ�ʼ�� */
    GPIO_Init(A_PORT, A_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_PORT, B_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_PORT, C_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(D_PORT, D_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(E_PORT, E_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(F_PORT, F_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(G_PORT, G_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(DP_PORT, DP_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    
    /* λѡ�˿ڳ�ʼ�� */
    GPIO_Init(BIT0_PORT, BIT0_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BIT1_PORT, BIT1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BIT2_PORT, BIT2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
}

/**
 * @brief �޸��������ʾģʽ��һλС��/��λС������
 * 
 * @param mode ��ʾģʽ��ȡֵ��ο�@display_mode_etype
 */
static void change_mode(const uint8_t mode)
{
    seg_ctl.display_mode = mode;
}

/**
 * @brief ��ȡ��ǰ��ʾģʽ
 * 
 * @return uint8_t ������ʾģʽ
 */
static uint8_t read_mode()
{
    return seg_ctl.display_mode;
}

/**
 * @brief λѡѡ��
 * 
 * @return ��
 */
static void select_bit(uint8_t num)
{
    GPIO_Init(BIT0_PORT, BIT0_PIN, (num == 0) ? (GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(BIT1_PORT, BIT1_PIN, (num == 1) ? (GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(BIT2_PORT, BIT2_PIN, (num == 2) ? (GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
}

/**
 * @brief �������ʾ����
 * 
 */
static void display()
{
    int i = 0;
    while(i < SEG_NUM)
    {
        select_bit(0XFF);
        show_ch(seg_ctl.p_segtab[i][seg_ctl.display_char[i]]);
        select_bit(i);
        /* ��ʱ��Ӱ */
        DELAY_MS(seg_ctl.vanishing_time);
        ++i;
    }
}


void show_ch(char ch)
{
    GPIO_Init(A_PORT, A_PIN, (ch & 0x01)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(B_PORT, B_PIN, (ch & 0x02)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(C_PORT, C_PIN, (ch & 0x04)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(D_PORT, D_PIN, (ch & 0x08)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(E_PORT, E_PIN, (ch & 0x10)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(F_PORT, F_PIN, (ch & 0x20)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(G_PORT, G_PIN, (ch & 0x40)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(DP_PORT, DP_PIN, (ch & 0x80)?(GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
}
/**
 * @brief ��ʼ���ṹ���Ա�Լ�IO����ʼ�������Ĭ��״̬
 * 
 */
void display_init()
{
    int i = 0;
    
    /* ��ʼ��GPIO */
    gpio_config();
    /* ������ʾģʽ */
    change_mode(TwoDigitMode);
    /* ��ʼ���ṹ���Ա */
    for(i = 0; i < SEG_NUM; ++i) {  
        seg_ctl.display_char[i] = 0;
        seg_ctl.bits_status[i] = DISPLAY;
        if(i < SEG_NUM - 1)
            seg_ctl.p_segtab[i+1] = seg_tab;
        seg_ctl.bits_status[i] = i;
    }
    /* Ĭ����λС����ʾģʽ */
    seg_ctl.p_segtab[0] = seg_tab_p;
    /* ������Ӱʱ�� */
    seg_ctl.vanishing_time = 5;
}

/**
 * @brief ��ʾ����
 * 
 * @param val ��Ҫ��ʾ�ĵ�ѹֵ
 */
void show_float_num(float val)
{
    int i = 0;
    int tmp = 0;
    uint8_t mode = read_mode();
    
    tmp = (int)(val);
    if(tmp > 9) mode = OneDigitMode; // �Զ��л�Ϊ��λС��ģʽ
    
    if(mode == OneDigitMode)
        tmp = (int)(val * 10);
    else
        tmp = (int)(val * 100);
    
    seg_ctl.display_char[0] = tmp / 100; 
    seg_ctl.display_char[1] = tmp % 100 / 10;
    seg_ctl.display_char[2] = tmp % 100 % 10;
    
    /* һλС����ʾģʽ */
    if(mode == OneDigitMode) {
        seg_ctl.p_segtab[OneDigitMode] = seg_tab_p; // ��2������ܴ�С����
        while(i < SEG_NUM)
        {
            if(i != OneDigitMode)
                seg_ctl.p_segtab[i] = seg_tab;;
            ++i;
        }
    }
    /* ��λС����ʾģʽ */
    else {
        seg_ctl.p_segtab[TwoDigitMode] = seg_tab_p; // ��2������ܴ�С����
        while(i < SEG_NUM)
        {
            if(i != TwoDigitMode)
                seg_ctl.p_segtab[i] = seg_tab;;
            ++i;
        }
    }
    display();
}

void show_int_num(int val)
{
    int i = 0;
    seg_ctl.display_char[0] = val / 100; 
    seg_ctl.display_char[1] = val % 100 / 10;
    seg_ctl.display_char[2] = val % 100 % 10;
    
    while(i < SEG_NUM)
    {
        seg_ctl.p_segtab[i] = seg_tab;;
        ++i;
    }
    display();
}
