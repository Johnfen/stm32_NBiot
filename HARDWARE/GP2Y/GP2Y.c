#include "GPY.h"  
 
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void GPLED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(GP_LED, ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GP_LED_PIN;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GP_EN_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(GP_EN_PORT,GP_LED_PIN);						 
 						 
}
