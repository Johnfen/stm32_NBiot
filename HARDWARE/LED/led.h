#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(15)// PA8
#define LED1 PDout(2)// PD2	
#define LED0_RCC RCC_APB2Periph_GPIOC  //����LED0�˿�ʱ��
#define LED1_RCC RCC_APB2Periph_GPIOD  //����LED1�˿�ʱ��
#define LED0_PIN GPIO_Pin_15 //����LED0����
#define LED1_PIN GPIO_Pin_2 //����LED1����
#define LED0_EN_PORT GPIOC  //����LED0_EN�˿�	ʹ��
#define LED1_EN_PORT GPIOD  //����LED1_EN�˿�	ʹ��
void LED_Init(void);//��ʼ��

		 				    
#endif
