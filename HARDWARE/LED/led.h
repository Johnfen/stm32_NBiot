#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PCout(15)// PA8
#define LED1 PDout(2)// PD2	
#define LED0_RCC RCC_APB2Periph_GPIOC  //定义LED0端口时钟
#define LED1_RCC RCC_APB2Periph_GPIOD  //定义LED1端口时钟
#define LED0_PIN GPIO_Pin_15 //定义LED0引脚
#define LED1_PIN GPIO_Pin_2 //定义LED1引脚
#define LED0_EN_PORT GPIOC  //定义LED0_EN端口	使能
#define LED1_EN_PORT GPIOD  //定义LED1_EN端口	使能
void LED_Init(void);//初始化

		 				    
#endif
