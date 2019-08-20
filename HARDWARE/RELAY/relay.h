#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"

#define relay0 PBout(9)// PA8
#define relay1 PBout(10)// PD2	
#define relay2 PBout(11)// PD2	

#define relay0_RCC RCC_APB2Periph_GPIOB  //定义LED0端口时钟
#define relay1_RCC RCC_APB2Periph_GPIOB  //定义LED1端口时钟
#define relay2_RCC RCC_APB2Periph_GPIOB  //定义LED1端口时钟

#define relay0_PIN GPIO_Pin_9 //定义LED0引脚
#define relay1_PIN GPIO_Pin_10 //定义LED1引脚
#define relay2_PIN GPIO_Pin_11 //定义LED1引脚

#define relay0_EN_PORT GPIOB  //定义LED0_EN端口	使能
#define relay1_EN_PORT GPIOB  //定义LED1_EN端口	使能
#define relay2_EN_PORT GPIOB  //定义LED1_EN端口	使能

void relay_Init(void);//初始化

		 				    
#endif
