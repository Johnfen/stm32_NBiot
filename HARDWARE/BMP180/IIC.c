/***********************
模拟IIC驱动

***********************/

#include "IIC.h"

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;                            //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
  GPIO_SetBits(GPIOC,GPIO_Pin_11|GPIO_Pin_10); 	//PC11,PC10 输出高
}

void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1;
	delay_us(4);
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;

	SDA_IN();
	IIC_SDA = 1;
	delay_us(1);
	IIC_SCL = 1;
	delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;	  //应答失败
		}
	}
	IIC_SCL = 0;
	return 0;	//应答成功
}

void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}

void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}

void IIC_Send_Byte(u8 txd)
{
	u8 t;

	SDA_OUT();
	IIC_SCL = 0;
	for (t = 0; t < 8; t++)
	{
		IIC_SDA = (txd&0x80) >> 7;
		txd <<= 1;
		delay_us(2);
		IIC_SCL = 1;
		delay_us(2);
		IIC_SCL = 0;
	}
	//IIC_Wait_Ack();
}

//读一个字节，ack = 1时， 发送ACK， ack = 0，发送nACK
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, receive = 0;

	SDA_IN();
	for (i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		delay_us(2);
		IIC_SCL = 1;
		receive <<= 1;
		if(READ_SDA) receive++;
		delay_us(1);
	}
	if (!ack)	IIC_NAck();
	else IIC_Ack();
	return receive;
}
