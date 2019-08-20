#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"

#define relay0 PBout(9)// PA8
#define relay1 PBout(10)// PD2	
#define relay2 PBout(11)// PD2	

#define relay0_RCC RCC_APB2Periph_GPIOB  //����LED0�˿�ʱ��
#define relay1_RCC RCC_APB2Periph_GPIOB  //����LED1�˿�ʱ��
#define relay2_RCC RCC_APB2Periph_GPIOB  //����LED1�˿�ʱ��

#define relay0_PIN GPIO_Pin_9 //����LED0����
#define relay1_PIN GPIO_Pin_10 //����LED1����
#define relay2_PIN GPIO_Pin_11 //����LED1����

#define relay0_EN_PORT GPIOB  //����LED0_EN�˿�	ʹ��
#define relay1_EN_PORT GPIOB  //����LED1_EN�˿�	ʹ��
#define relay2_EN_PORT GPIOB  //����LED1_EN�˿�	ʹ��

void relay_Init(void);//��ʼ��

		 				    
#endif
