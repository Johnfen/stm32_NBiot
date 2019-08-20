#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void Temper_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
static char *itoa(int value, char *string, int radix);
void UsartSend_Bit1(u8 Rdata,u8 whe);                                   	//串口1发送函数
void TIM4_Set(u8 sta);
void TIM4_SetARR(u16 period);
void TIM4_Init(u16 arr,u16 psc);

#endif
