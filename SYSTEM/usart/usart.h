#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#define USART1_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void Temper_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
static char *itoa(int value, char *string, int radix);
void UsartSend_Bit1(u8 Rdata,u8 whe);                                   	//����1���ͺ���
void TIM4_Set(u8 sta);
void TIM4_SetARR(u16 period);
void TIM4_Init(u16 arr,u16 psc);

#endif
