/*
 * ����˵�������ڽ��ջ�������СΪ128���ֽڣ������������Զ�����ʼ��ַ��ʼ���¸��ǣ�
 * ����������ջ���������ʹ�û�����ǰ������ʹ��clear_recvbuf������ջ������ٽ���ʹ�á�
 */

#include "uart1.h"
#include <string.h>
#include <stdarg.h>

/* ���ڽ��ջ��������� */
uart_rxbuf  g_uart_rxbuf;

/*******************************************************************************
                                    ��̬��������
*******************************************************************************/
/*
  ���ܣ���int������תΪ2��8��10��16�����ַ���
  ������value --- �����int������
        str --- �洢ת�����ַ���
        radix --- ��������ѡ��
  ע�⣺8λ��Ƭ��int�ֽ�ֻռ2���ֽ�
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
		// ʮ������Ϊ����
		tmp_value = (unsigned int)(0 - value);
		str[i++] = '-';
		k = 1;
	}
	else
		tmp_value = (unsigned int)value;

	// ����ת��Ϊ�ַ���������洢
	do
	{
		str[i ++] = list[tmp_value % radix];
		tmp_value /= radix;
	} while(tmp_value);
	str[i] = '\0';

	// �������ַ���ת��Ϊ����
	for (j = k; j < (i + k) / 2; j++)
	{
		tmp = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = tmp;
	}

	return str;
}

/*
  ���ܣ���double������תΪ�ַ���
  ������value --- �����double������
        str --- �洢ת�����ַ���
        eps --- ����С��λѡ�����ٱ���һ��С��λ,���ౣ��4��С��λ
  ע�⣺8λ��Ƭ��int�ֽ�ֻռ2���ֽ�
*/
static void sky_ftoa(double value, char *str, unsigned int eps)
{
	unsigned int integer;
	double decimal;
	char list[] = "0123456789";
	int i = 0, j, k = 0;

	//��������С��������ȡ����
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

	//������������ת��Ϊ�ַ���������洢
	do
	{
		str[i ++] = list[integer % 10];
		integer /= 10;
	} while(integer);

	str[i] = '\0';
	//�������ַ���ת��Ϊ����
	char tmp;

	for (j = k; j < (i + k) / 2; j++)
	{
		tmp = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = tmp;
	}

	// ����С������
	if (eps < 1 || eps > 4)
		eps = 4;

	// �������⣬��ֹ����1.2���1.19�����
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

	// ������������ת��Ϊ�ַ���������洢
	do
	{
		str[i ++] = list[tmp_decimal % 10];
		tmp_decimal /= 10;
	} while(tmp_decimal);
	str[i] = '\0';

	// �������ַ���ת��Ϊ����
	for (j = k; j < (i + k) / 2; j++)
	{
		tmp = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = tmp;
	}
	str[i] = '\0';
}


/*******************************************************************************
                                    �ⲿ��������
*******************************************************************************/

/**
 * @brief ����1��ʼ������
 *
 * @param bound [I] ������ֵ������9600,115200
 */
void uart1_init(const uint32_t bound)
{
	/* ��ʼ�����ջ����� */
	clear_recvbuf();
	/* ���ý��ջ�����״̬Ϊ�� */
	g_uart_rxbuf.status = EMPTY;
	UART1_DeInit();
	/* ��ʼ������ */
	UART1_Init(bound,
	    UART1_WORDLENGTH_8D,
	    UART1_STOPBITS_1,
	    UART1_PARITY_NO,
	    UART1_SYNCMODE_CLOCK_DISABLE,
	    UART1_MODE_TX_ENABLE);
//	/* ʹ�ܽ����ж� */
//	UART1_ITConfig(UART1_IT_RXNE, ENABLE);
	/* ���������ɱ�־λ */
//	UART1_ClearFlag(UART1_FLAG_TC);
}

/**
 * @brief ����һ���ֽ�
 *
 * @param ch [I] �����͵��ַ�
 */
void send_byte(const uint8_t ch)
{
    /* �ȴ����ͻ�����Ϊ�� */
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
	/* ����һ���ֽ� */
	UART1_SendData8(ch);
}

/**
 * @brief ����һ���ַ���
 *
 * @param cpstr [I] �洢�������ַ��׵�ַ��ָ�����
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
 * @brief ������ջ�����
 *
 */
void clear_recvbuf()
{
	memset((void *)g_uart_rxbuf.buf, 0, SIZE);
}

/**
 * @brief �����жϷ�����
 *
 */
#pragma vector = UART1_R_RXNE_VECTOR  /* Symbol from I/O */
__interrupt void MyInterruptRoutine(void)
{
	g_uart_rxbuf.bbuf = UART1_ReceiveData8();

	if(g_uart_rxbuf.len >= 2)
	{
		/* ���ջ�������������ս��ջ����� */
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
 * @brief �Լ�ʵ����printf��ʽ���������
 *
 * @param pdata ����ָ��
 * @param ... �ɱ�����б�
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
