#ifndef _GPY_H
#define _GPY_H	 
#include "sys.h"

#define GP_LED PAout(3)// PA8
#define  GP_LED_RCC RCC_APB2Periph_GPIOA  //����LED0�˿�ʱ��
#define GP_LED_PIN GPIO_Pin_3 //����LED0����
#define GP_EN_PORT GPIOA  //����LED0_EN�˿�	ʹ��
void GPLED_Init(void);//��ʼ��
#endif
