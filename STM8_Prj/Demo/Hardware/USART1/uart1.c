/*
 * 程序说明：串口接收缓冲区大小为128个字节，缓冲区满后自动从起始地址开始重新覆盖，
 * 不会主动清空缓冲区，故使用缓冲区前建议先使用clear_recvbuf函数清空缓冲区再进行使用。
 */

#include "uart1.h"
#include <string.h>
#include <stdarg.h>

/* 串口接收缓冲区变量 */
uart_rxbuf  g_uart_rxbuf;

/*******************************************************************************
                                    静态函数区域
*******************************************************************************/
/*
  功能：将int型数据转为2，8，10，16进制字符串
  参数：value --- 输入的int整型数
        str --- 存储转换的字符串
        radix --- 进制类型选择
  注意：8位单片机int字节只占2个字节
*/
static char *sky_itoa(int value, char *str, unsigned int radix)
{
	char list[] = "0123456789ABCDEF";
	unsigned int tmp_value;
	int i = 0, j, k = 0;
	char tmp;

	if (NULL == str)
		return NULL;

	if (2 != radix && 8 != radix && 10 != radix && 16 != radix)
		return NULL;

	if (radix == 10 && value < 0) {
		// 十进制且为负数
		tmp_value = (unsigned int)(0 - value);
		str[i++] = '-';
		k = 1;
	}
	else
		tmp_value = (unsigned int)value;

	// 数据转换为字符串，逆序存储
	do
	{
		str[i ++] = list[tmp_value % radix];
		tmp_value /= radix;
	} while(tmp_value);
	str[i] = '\0';

	// 将逆序字符串转换为正序
	for (j = k; j < (i + k) / 2; j++)
	{
		tmp = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = tmp;
	}

	return str;
}

/*
  功能：将double型数据转为字符串
  参数：value --- 输入的double浮点数
        str --- 存储转换的字符串
        eps --- 保留小数位选择，至少保留一个小数位,至多保留4个小数位
  注意：8位单片机int字节只占2个字节
*/
static void sky_ftoa(double value, char *str, unsigned int eps)
{
	unsigned int integer;
	double decimal;
	char list[] = "0123456789";
	int i = 0, j, k = 0;

	//将整数及小数部分提取出来
	if (value < 0)
	{
		decimal = (double)(((int)value) - value);
		integer = (unsigned int)(0 - value);
		str[i ++] = '-';
		k = 1;
	}
	else
	{
		integer = (unsigned int)(value);
		decimal = (double)(value - integer);
	}

	//整数部分数据转换为字符串，逆序存储
	do
	{
		str[i ++] = list[integer % 10];
		integer /= 10;
	} while(integer);

	str[i] = '\0';
	//将逆序字符串转换为正序
	char tmp;

	for (j = k; j < (i + k) / 2; j++)
	{
		tmp = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = tmp;
	}

	// 处理小数部分
	if (eps < 1 || eps > 4)
		eps = 4;

	// 精度问题，防止输入1.2输出1.19等情况
	double pp = 0.1;

	for (j = 0; j <= eps; j++) {
		pp *= 0.1;
	}

	decimal += pp;
	while (eps)
	{
		decimal *= 10;
		eps --;
	}

	int tmp_decimal = (int)decimal;
	str[i ++] = '.';
	k = i;

	// 整数部分数据转换为字符串，逆序存储
	do
	{
		str[i ++] = list[tmp_decimal % 10];
		tmp_decimal /= 10;
	} while(tmp_decimal);
	str[i] = '\0';

	// 将逆序字符串转换为正序
	for (j = k; j < (i + k) / 2; j++)
	{
		tmp = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = tmp;
	}
	str[i] = '\0';
}


/*******************************************************************************
                                    外部函数区域
*******************************************************************************/

/**
 * @brief 串口1初始化函数
 *
 * @param bound [I] 波特率值，常用9600,115200
 */
void uart1_init(const uint32_t bound)
{
	/* 初始化接收缓冲区 */
	clear_recvbuf();
	/* 设置接收缓冲区状态为空 */
	g_uart_rxbuf.status = EMPTY;
	UART1_DeInit();
	/* 初始化串口 */
	UART1_Init(bound,
	    UART1_WORDLENGTH_8D,
	    UART1_STOPBITS_1,
	    UART1_PARITY_NO,
	    UART1_SYNCMODE_CLOCK_DISABLE,
	    UART1_MODE_TX_ENABLE);
//	/* 使能接收中断 */
//	UART1_ITConfig(UART1_IT_RXNE, ENABLE);
	/* 清除接收完成标志位 */
//	UART1_ClearFlag(UART1_FLAG_TC);
}

/**
 * @brief 发送一个字节
 *
 * @param ch [I] 待发送的字符
 */
void send_byte(const uint8_t ch)
{
    /* 等待发送缓冲区为空 */
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
	/* 发送一个字节 */
	UART1_SendData8(ch);
}

/**
 * @brief 发送一个字符串
 *
 * @param cpstr [I] 存储待发送字符首地址的指针变量
 */
void send_string(const uint8_t *cpstr)
{
	if(cpstr == NULL)
		return;

	while(*cpstr != '\0')
	{
		send_byte(*cpstr);
		++cpstr;
	}
}

/**
 * @brief 清除接收缓冲区
 *
 */
void clear_recvbuf()
{
	memset((void *)g_uart_rxbuf.buf, 0, SIZE);
}

/**
 * @brief 接收中断服务函数
 *
 */
#pragma vector = UART1_R_RXNE_VECTOR  /* Symbol from I/O */
__interrupt void MyInterruptRoutine(void)
{
	g_uart_rxbuf.bbuf = UART1_ReceiveData8();

	if(g_uart_rxbuf.len >= 2)
	{
		/* 接收缓冲区满，则清空接收缓冲区 */
		if(g_uart_rxbuf.status == FULL)
		{
			clear_recvbuf();
			g_uart_rxbuf.status = EMPTY;
			g_uart_rxbuf.len = 0;
		}

		else if((g_uart_rxbuf.buf[g_uart_rxbuf.len - 2] == '\r') &&
		    (g_uart_rxbuf.buf[g_uart_rxbuf.len - 1] == '\n'))
			g_uart_rxbuf.len = 0;
	}

	g_uart_rxbuf.buf[g_uart_rxbuf.len++] = g_uart_rxbuf.bbuf;

	if(g_uart_rxbuf.len < SIZE)
		g_uart_rxbuf.status = NOT_EMPTY;

	else if(g_uart_rxbuf.len >= SIZE)
		g_uart_rxbuf.status = FULL;

	UART1_ClearFlag(UART1_FLAG_RXNE);
}

/**
 * @brief 自己实现类printf格式化输出函数
 *
 * @param pdata 数据指针
 * @param ... 可变参数列表
 */
void uart_printf(const char *pdata, ...)
{
	const char *s;
	int d;
	char buf[16];
	uint8_t txdata;
	va_list ap;
	va_start(ap, pdata);

	while (*pdata != 0)
	{
		if (*pdata == 0x5c)
		{
			switch (*++pdata )
			{
            case 'r':
            {
                txdata = 0x0d;
                send_byte(txdata);
                pdata++;
                break;
            }
            case 'n':
            {
                txdata = 0x0a;
                send_byte(txdata);
                pdata++;
                break;
            }
            default:
                pdata++;
                break;
			}
		}
		else if (*pdata == '%')
		{
			switch (*++pdata)
			{
            case 's':
            {
                s = va_arg(ap, const char *);
                for ( ; *s; s++) {
                    send_byte(*((uint8_t *)s)); 
                }
                pdata++;
                break;
            }
            case 'd':
            {
                d = va_arg(ap, int);
                sky_itoa(d, buf, 10);
                for (s = buf; *s; s++) {
                    send_byte(*((uint8_t *)s));
                }
                pdata++;
                break;
            }
            case 'x':
            {
                d = va_arg(ap, int);
                sky_itoa(d, buf, 16);
                for (s = buf; *s; s++) {
                    send_byte(*((uint8_t *)s));
                }
                pdata++;
                break;
            }
            case 'f':
            {
                double num = va_arg(ap, double);
                sky_ftoa(num, buf, 4);
                for (s = buf; *s; s++) {
                    send_byte(*((uint8_t *)s));
                }
                pdata++;
                break;
            }
            default:
                pdata++;
                break;
			}
		}
		else
		{
			send_byte(*((uint8_t *)pdata));
			pdata++;
		}
	}
}
