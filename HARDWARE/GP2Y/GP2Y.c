#include "GPY.h"  
 
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void GPLED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(GP_LED, ENABLE);
	
 GPIO_InitStructure.GPIO_Pin = GP_LED_PIN;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GP_EN_PORT, &GPIO_InitStructure);					 
 GPIO_SetBits(GP_EN_PORT,GP_LED_PIN);						 
 						 
}
