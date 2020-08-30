#include "display.h"

/* 外部ms延时函数，移植到其他平台需要定义delay_ms这个函数 */
extern void delay_ms(const uint32_t nms);
#define DELAY_MS delay_ms

/* 管理数码管成员 */
segment_s seg_ctl;

/* 共阳数码管段选码表，无小数点，0~F */
static const unsigned char seg_tab[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
	0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, 0xff
};

/* 共阳数码管段选码表，有小数点，0~F */
static const unsigned char seg_tab_p[] = {
	0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78,
	0x00, 0x10, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0e, 0xff
};

/**
 * @brief 初始化数码管段码相关的IO口
 * 
 */
static void gpio_config()
{
    /* 段选端口初始化 */
    GPIO_Init(A_PORT, A_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_PORT, B_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_PORT, C_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(D_PORT, D_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(E_PORT, E_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(F_PORT, F_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(G_PORT, G_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(DP_PORT, DP_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    
    /* 位选端口初始化 */
    GPIO_Init(BIT0_PORT, BIT0_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BIT1_PORT, BIT1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BIT2_PORT, BIT2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
}

/**
 * @brief 修改数码管显示模式，一位小数/两位小数两种
 * 
 * @param mode 显示模式，取值请参考@display_mode_etype
 */
static void change_mode(const uint8_t mode)
{
    seg_ctl.display_mode = mode;
}

/**
 * @brief 读取当前显示模式
 * 
 * @return uint8_t 返回显示模式
 */
static uint8_t read_mode()
{
    return seg_ctl.display_mode;
}

/**
 * @brief 位选选择
 * 
 * @return 无
 */
static void select_bit(uint8_t num)
{
    GPIO_Init(BIT0_PORT, BIT0_PIN, (num == 0) ? (GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(BIT1_PORT, BIT1_PIN, (num == 1) ? (GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
    GPIO_Init(BIT2_PORT, BIT2_PIN, (num == 2) ? (GPIO_MODE_OUT_PP_HIGH_SLOW):(GPIO_MODE_OUT_PP_LOW_SLOW));
}

/**
 * @brief 数码管显示函数
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
        /* 延时消影 */
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
 * @brief 初始化结构体成员以及IO，初始化数码管默认状态
 * 
 */
void display_init()
{
    int i = 0;
    
    /* 初始化GPIO */
    gpio_config();
    /* 设置显示模式 */
    change_mode(TwoDigitMode);
    /* 初始化结构体成员 */
    for(i = 0; i < SEG_NUM; ++i) {  
        seg_ctl.display_char[i] = 0;
        seg_ctl.bits_status[i] = DISPLAY;
        if(i < SEG_NUM - 1)
            seg_ctl.p_segtab[i+1] = seg_tab;
        seg_ctl.bits_status[i] = i;
    }
    /* 默认两位小数显示模式 */
    seg_ctl.p_segtab[0] = seg_tab_p;
    /* 设置消影时间 */
    seg_ctl.vanishing_time = 5;
}

/**
 * @brief 显示数字
 * 
 * @param val 需要显示的电压值
 */
void show_float_num(float val)
{
    int i = 0;
    int tmp = 0;
    uint8_t mode = read_mode();
    
    tmp = (int)(val);
    if(tmp > 9) mode = OneDigitMode; // 自动切换为两位小数模式
    
    if(mode == OneDigitMode)
        tmp = (int)(val * 10);
    else
        tmp = (int)(val * 100);
    
    seg_ctl.display_char[0] = tmp / 100; 
    seg_ctl.display_char[1] = tmp % 100 / 10;
    seg_ctl.display_char[2] = tmp % 100 % 10;
    
    /* 一位小数显示模式 */
    if(mode == OneDigitMode) {
        seg_ctl.p_segtab[OneDigitMode] = seg_tab_p; // 第2个数码管带小数点
        while(i < SEG_NUM)
        {
            if(i != OneDigitMode)
                seg_ctl.p_segtab[i] = seg_tab;;
            ++i;
        }
    }
    /* 两位小数显示模式 */
    else {
        seg_ctl.p_segtab[TwoDigitMode] = seg_tab_p; // 第2个数码管带小数点
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
