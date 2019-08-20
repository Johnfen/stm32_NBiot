#ifndef _GPY_H
#define _GPY_H	 
#include "sys.h"

#define GP_LED PAout(3)// PA8
#define  GP_LED_RCC RCC_APB2Periph_GPIOA  //定义LED0端口时钟
#define GP_LED_PIN GPIO_Pin_3 //定义LED0引脚
#define GP_EN_PORT GPIOA  //定义LED0_EN端口	使能
void GPLED_Init(void);//初始化
#endif
