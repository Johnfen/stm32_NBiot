#include "relay.h"  

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void relay_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(relay0_RCC|relay1_RCC|relay2_RCC, ENABLE);
	
			 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
 GPIO_InitStructure.GPIO_Pin = relay0_PIN;	
 GPIO_Init(relay0_EN_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(relay0_EN_PORT,relay0_PIN);						 

 GPIO_InitStructure.GPIO_Pin = relay1_PIN;	    		 
 GPIO_Init(relay1_EN_PORT, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(relay1_EN_PORT,relay1_PIN); 		

 GPIO_InitStructure.GPIO_Pin = relay2_PIN;	    		 
 GPIO_Init(relay2_EN_PORT, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(relay2_EN_PORT,relay2_PIN); 		
}
