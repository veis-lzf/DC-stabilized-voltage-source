#ifndef __USART1_H__
#define __USART1_H__

#include "stm8s_clk.h"
#include "stm8s_uart1.h"
#include "stm8s_gpio.h"

#define UART1_R_RXNE_VECTOR (unsigned char)(18+2) // 中断向量地址
#define SIZE 128 // 缓冲区大小，单位字节

/* 串口缓冲区状态类型 */
enum {
  FULL, 
  EMPTY, 
  NOT_EMPTY
};

/*定义一个结构体，存放串口相关变量*/
typedef struct _uart_rxbuf_
{
  uint8_t bbuf; // 一个byte的缓冲区变量
  uint8_t buf[SIZE]; // 串口接收数据的buffer
  uint8_t status;
  uint32_t len; // 数据的长度
} uart_rxbuf;

/* 串口I/O相关宏定义 */
#define UART1_PORT GPIOD
#define UART1_TX GPIO_PIN_5
#define UART1_RX GPIO_PIN_6

/* 外部接口区域 */
void uart1_init(const uint32_t bound);
void send_byte(const uint8_t ch);
void send_string(const uint8_t *cpstr);
void clear_recvbuf();
void uart_printf(const char * pdata, ... );

extern uart_rxbuf g_uart_rxbuf;

#endif /* __USART1_H__ */