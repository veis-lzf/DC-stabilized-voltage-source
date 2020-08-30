#ifndef __USART1_H__
#define __USART1_H__

#include "stm8s_clk.h"
#include "stm8s_uart1.h"
#include "stm8s_gpio.h"

#define UART1_R_RXNE_VECTOR (unsigned char)(18+2) // �ж�������ַ
#define SIZE 128 // ��������С����λ�ֽ�

/* ���ڻ�����״̬���� */
enum {
  FULL, 
  EMPTY, 
  NOT_EMPTY
};

/*����һ���ṹ�壬��Ŵ�����ر���*/
typedef struct _uart_rxbuf_
{
  uint8_t bbuf; // һ��byte�Ļ���������
  uint8_t buf[SIZE]; // ���ڽ������ݵ�buffer
  uint8_t status;
  uint32_t len; // ���ݵĳ���
} uart_rxbuf;

/* ����I/O��غ궨�� */
#define UART1_PORT GPIOD
#define UART1_TX GPIO_PIN_5
#define UART1_RX GPIO_PIN_6

/* �ⲿ�ӿ����� */
void uart1_init(const uint32_t bound);
void send_byte(const uint8_t ch);
void send_string(const uint8_t *cpstr);
void clear_recvbuf();
void uart_printf(const char * pdata, ... );

extern uart_rxbuf g_uart_rxbuf;

#endif /* __USART1_H__ */