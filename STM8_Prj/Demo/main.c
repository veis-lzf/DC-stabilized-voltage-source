/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "uart1.h"
#include "adc1.h"
#include "display.h"
/* Private defines -----------------------------------------------------------*/
#undef DEBUG
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {   
    nCount--;
  }
}

/* ms延时函数 */
void delay_ms(uint32_t ms)
{
    unsigned char i;
    while(ms != 0)
    {
        for(i = 0; i < 250; i++)
        {}
        for(i = 0; i < 75; i++)
        {}
        ms--;
    }
}

void main(void)
{
    float f_voltage;
    uint16_t count = 0;
#ifdef DEBUG
    
    uint16_t u16_adc1_value;
    uart1_init(115200); // 初始化调试串口
#endif
    adc1_init();
    display_init();
    f_voltage = get_voltage();
    while (1)
    {
        show_float_num(f_voltage);
        count++;
        if(count == 10)
        {
            count = 0;
            f_voltage = get_voltage();
        }
#ifdef DEBUG
        u16_adc1_value = get_adc_val();
        count++;
        if(count == 100)
        {
            count = 0;
            uart_printf("ADC Value：%d, Voltage：%f\r\n", u16_adc1_value, f_voltage);
        }
#endif
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
