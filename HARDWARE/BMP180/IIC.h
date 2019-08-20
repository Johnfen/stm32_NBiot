#ifndef __IIC_H
#define __IIC_H
#include"sys.h"
#include"delay.h"

/*
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)
*/
/****************
SDA----PC11
SCL----PC10
****************/
//��ֲʱҪע���޸�����ӿ�
#define RCC_IIC 	0x00000010//���Ƕ�Ӧ����ı�
#define SDA_IN() 	{GPIOC->CRH &= 0xffff0fff; GPIOC->CRH |= 0x00008000;}
#define SDA_OUT() 	{GPIOC->CRH &= 0xffff0fff; GPIOC->CRH |= 0x00003000;}
#define SCL_OUT() 	{GPIOC->CRH &= 0xfffff0ff; GPIOC->CRH |= 0x00030000;}//{GPIOC->CRH &= 0xfffff0ff; GPIOC->CRH |= 0x00000300;}
#define IIC_SCL 	PCout(10)
#define IIC_SDA 	PCout(11)
#define READ_SDA	PCin(11)



void IIC_Init(void);//��ֲʱҪ��д�������
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8);
u8 IIC_Read_Byte(u8);	//��һ���ֽڣ�ack = 1ʱ�� ����ACK�� ack = 0������nACK

#endif
