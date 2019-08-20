/*************************************************
��������12864.c

�� �� ������12864Һ��(���ֿ�,���з�ʽ)
Һ��������5VҺ��ʱעע�����5v��ѹ�Ķ˿ڣ�Ϊ����FT�Ķ˿�
��ĻPSB�˿��Ѵ�Һ���ڲ��ӵء���Ĭ�ϴ���
*************************************************/
#include <stm32f10x.h>	   
#include "LCD12864.h"	
#include "delay.h"	
void DisplayOneChar(u8 x,u8 y, u8 p)
{
	 u8 temp;
		if(x>4) {x=1;}
	  if(y>6) {y=0;}
  Display_LCD_Pos(x,y);
  	temp=p;
	Write_LCD_Data(temp);
}
void I_C(u16 x,u8 *p,u8 n)
{
	 u8 i=0,num,j=0;
	u16 x_value;
	u16 temp;
	 x_value=x;
	 num=n;
			
			
			while((x_value>9)&&(i<num))
			{
				*(p+i)=((x_value%10)+'0');
			  x_value=x_value/10;
				i++;
			}
			*(p+i)=x_value+'0';
			i=i+1;
    for(j=0;j<((i)/2);j++)
  		{
				temp=*(p+j);
   		  *(p+j)=*(p+(i-j-1));
				*(p+(i-j-1))=temp;
    	}
		
}
 //	ѡ��PB10:CLK(E),���    PB9R/W(SID),����      PB8:RS/CS,Ƭѡ
void LCD12864_Init(void)
{
	/*----------------------------------------*/	 
	GPIO_InitTypeDef GPIO_InitStructure;		//����GPIO�ṹ����

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //ʹ��AFIOʱ��(��һ����������)
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //����SW-DP(�ر������Կ���IO��)

  RCC_APB2PeriphClockCmd(LCD_RS_RCC, ENABLE);//ʹ��LCD_LCD_RS���ڶ˿�ʱ��
  RCC_APB2PeriphClockCmd(LCD_RW_RCC, ENABLE);//ʹ��LCD_RW���ڶ˿�ʱ��
  RCC_APB2PeriphClockCmd(LCD_EN_RCC, ENABLE);//ʹ��LCD_EN���ڶ˿�ʱ��

  //LCD_RS�˿ڳ�ʼ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
  GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN); //LCD_RS����ߵ�ƽ

  //LCD_RW�˿ڳ�ʼ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
  GPIO_InitStructure.GPIO_Pin = LCD_RW_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_RW_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_RW_PORT,LCD_RW_PIN); //LCD_RW����ߵ�ƽ

  //LCD_EN�˿ڳ�ʼ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
  GPIO_InitStructure.GPIO_Pin = LCD_EN_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_EN_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_EN_PORT,LCD_EN_PIN); //LCD_EN����ߵ�ƽ

/*--------------------LCD����ָ��-----------------------*/
    delay_ms(2);
    Write_LCD_Command(0x30);  //30--����ָ���
    delay_ms(5);
    Write_LCD_Command(0x0c);  //������ƻ��治��
    delay_ms(5);
    Write_LCD_Command(0x01);  //����
    delay_ms(5);              //����ʱ��ϳ�
    Write_LCD_Command(0x06);  //��ʾ�򿪣���꿪�����׹�
    delay_ms(5);

//	Write_LCD_Command(0x34);	// ����ָ��
//    Write_LCD_Command(0x36);	// �򿪻�ͼָ��
	Draw_Clear();					// ����
	//Draw_Update();
}
/*********************************************************
������:Send_Byte()
����ֵ����
��  ��:	д���ݵ�LCD
*********************************************************/
void Send_Byte(u8 zdata)
{
  u16 i;
  for(i=0; i<8; i++)
    {
  
	  if((zdata << i) & 0x80) 
	       SID_H;
	  else   
	       SID_L;
	       SCLK_H;
		   SCLK_L;
	  }
	}
/*********************************************************
������:Receive_Byte()
����ֵ��temp1+temp2
��  ��:	��LCD����
*********************************************************/
u8 Receive_Byte(void)
	{
	   u8 i,temp1,temp2,value;
	   temp1=0;
	   temp2=0;
	   for(i=0;i<8;i++)
	      {
		  	 temp1=temp1<<1;
		  	 SCLK_L;
			 SCLK_H;
		  	 SCLK_L;
			 if(PAout(7)==1)
			   {
			     temp1++;
				}
		  }
	   for(i=0;i<8;i++)
	      {
		  	 temp2=temp2<<1;
		  	 SCLK_L;
			 SCLK_H;
		  	 SCLK_L;
			 if(PAout(7)==1)
			   {
			      temp2++;
			   }
		  }
	      temp1=0xf0&temp1;
		  temp2=0x0f&temp2;
		  value=temp1+temp2;
       return   value ;
	}
/*********************************************************
������:Check_Busy()
����ֵ����
��  ��:	LCDæ���
*********************************************************/
void Check_Busy(void)
{
	do
		Send_Byte(0xfc);		  //11111,RW(1),RS(0),0
	while(0x80&Receive_Byte());
}
/*********************************************************
������: Write_LCD_Command()
����ֵ����
��  ��:	д���LCD
*********************************************************/
void Write_LCD_Command(u8 cmdcode)
{ 
   
   CS_H;
   Check_Busy();
   Send_Byte(0xf8);
   Send_Byte(cmdcode & 0xf0);
   Send_Byte((cmdcode << 4) & 0xf0);
   delay_ms(2);
   CS_L;
}
/*********************************************************
������: Write_LCD_Data()
����ֵ����
��  ��:	д��ʾ���ݵ�LCD
*********************************************************/
void Write_LCD_Data(u8 Dispdata)
{  
  CS_H;
  Check_Busy();
  Send_Byte(0xfa);	  //11111,RW(0),RS(1),0
  Send_Byte(Dispdata & 0xf0);
  Send_Byte((Dispdata << 4) & 0xf0);
  delay_ms(2);
  CS_L;
}
/*********************************************************
������: Read_LCD_Data()
����ֵ��LCD�յ�������
��  ��:	��ȡLCD��ʾ����
*********************************************************/
u8 Read_Data(void)
	{
	  Check_Busy();
	  Send_Byte(0xfe);    //11111��RW(1),RS(1),0 LCD->MCU
	  return  Receive_Byte();
	
	}
/*********************************************************
������: LCD_Clear_Txt
����ֵ����
��  ��:	�ı������
*********************************************************/
void LCD_Clear_TXT( void )
{
     u8 i;
     Write_LCD_Command(0x30);      //8BitMCU,����ָ���
     Write_LCD_Command(0x80);      //AC����ʼλ
     for(i=0;i<64;i++)
	  {
       Write_LCD_Data(0x20);
	  }
}
/*********************************************************
������: LCD_Clear_BMP
����ֵ����
��  ��:	ͼƬ�����
*********************************************************/
void LCD_Clear_BMP( void )
{
     u8 i,j;
     Write_LCD_Command(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     Write_LCD_Command(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           Write_LCD_Command(0x80|i);      //��λ��
           Write_LCD_Command(0x80);      //��λ��
           for(j=0;j<32;j++)            //256/8=32 byte
                Write_LCD_Data(0);
     }
}
/*********************************************************
������: Display_LCD_Pos
����ֵ����
��  ��:������ʾλ��
*********************************************************/
void Display_LCD_Pos(u8 x,u8 y) 
	{
	   u8 pos;
	   switch(x)
	       {
		   	  case 0: x=0x80;break;
			  case 1: x=0x90;break;
		   	  case 2: x=0x88;break;
			  case 3: x=0x98;break;
		   }
	   pos=x+y;
	   Write_LCD_Command(pos);
    }
/******************************************
�������ƣ�Disp_HZ
��    �ܣ�����Һ����ʾ����
��    ����addr--��ʾλ�õ��׵�ַ
          pt--ָ����ʾ���ݵ�ָ��
          num--��ʾ�ַ�����
����ֵ  ����
********************************************/
void Disp_HZ(u8 X,const u8 * pt,u8 num)
{
    u8 i,addr;
		
    if (X==0) {addr=0x80;}
     else if (X==1) {addr=0x90;}
     else if (X==2) {addr=0x88;}
     else if (X==3) {addr=0x98;}
     Write_LCD_Command(addr); 
     for(i = 0;i < (num*2);i++) 
     Write_LCD_Data(*(pt++)); 
} 
/*********************************************************
������:Display_LCD_String()
����ֵ����
��  ��:��ʾ�ַ���
*********************************************************/

void Display_LCD_String(u8 x,u8 *p,u8 time)
	{
	 u8 i,addr,num;
	 num=sizeof p;
	 switch(x)
	     {
		   	 case 0: addr=0x80;break;
		   	 case 1: addr=0x90;break;
		   	 case 2: addr=0x88;break;
		   	 case 3: addr=0x98;break;
		 }
	  Write_LCD_Command(addr);
	  for(i=0;i<(num*4);i++)
	     {
		   Write_LCD_Data(*(p++));
		   delay_ms(time);
		 
		 }

	}
	void LCD_ShowString(u8 x,u8 y,const u8 *p)
{  
	u8 temp;
		if(x>4) {x=1;}
	  if(y>6) {y=0;}
  Display_LCD_Pos(x,y);
  	temp=*p;
	while(temp!='\0')
	{
		Write_LCD_Data(temp);
		temp=*(++p);
	} 
}


/*************************************************************
* ��������    ��PutBMP
**************************************************************/
void PutBMP(u8 *puts)
{
     u16 x=0;
     u8 i,j;
     Write_LCD_Command(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     Write_LCD_Command(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           Write_LCD_Command(0x80|i);      //��λ��
           Write_LCD_Command(0x80);      //��λ��
           for(j=0;j<16;j++)      //256/8=32 byte
           {            //��λ��ÿ���Զ�����
                 Write_LCD_Data(puts[x]);
                 x++;
           }
     }
     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           Write_LCD_Command(0x80|i);      //��λ��
           Write_LCD_Command(0x88);      //��λ��
           for(j=0;j<16;j++)      //256/8=32 byte
           {            //��λ��ÿ���Զ�����
                 Write_LCD_Data(puts[x]);
                 x++;
           }
     }
}

/*-------------------------------------------------
 LCD��ʾ����
--------------------------------------------------*/
u16 LCD12864_Buffer[64][16] = {0};

const u8 m_ch4x5[][6] = 
{
	{0x06,0x09,0x09,0x09,0x06,5},		// 0���ַ����5����
	{0x02,0x06,0x02,0x02,0x07,5},		// 1
	{0x06,0x09,0x02,0x04,0x0F,5},		// 2
	{0x06,0x09,0x02,0x09,0x06,5},		// 3
	{0x02,0x06,0x0A,0x0F,0x02,5},		// 4
	{0x0E,0x08,0x0E,0x01,0x0E,5},		// 5
	{0x06,0x08,0x0E,0x09,0x06,5},		// 6
	{0x0F,0x01,0x02,0x04,0x04,5},		// 7
	{0x06,0x09,0x06,0x09,0x06,5},		// 8
	{0x06,0x09,0x07,0x01,0x06,5},		// 9
	{0x00,0x00,0x00,0x00,0x01,2},		// .
	{0x00,0x01,0x00,0x01,0x00,2},		// :
	{0x00,0x00,0x00,0x00,0x00,5}, 		// �ո�






};
/*------------------------------------------------------------------------------
���ܣ�����
������
	x��������
	y��������
	color����ɫ��0����ʾ��1��ʾ
------------------------------------------------------------------------------*/
const u16 DrawDotsTable[] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
void Draw_Dots(u8 x, u8 y, u8 color)
{
	u8 i0, y0;
	u16 value;

	// �������
	if ( y >= 32 )
	{
		y0 = y-32;
		i0 = x/16+8;
	}
	else
	{
		y0 = y;
		i0 = x/16;
	}
	value = DrawDotsTable[x%16];
	if (color == 0)
	{
		LCD12864_Buffer[y0][i0] &= ~value;
	}
	else
	{
		LCD12864_Buffer[y0][i0] |= value;
	}
}

/*------------------------------------------------------------------------------
���ܣ�ˢ����ʾ
------------------------------------------------------------------------------*/
void Draw_Update(void)
{
	u8 x, y;

	for (y=0; y<64; y++)
	{
		Write_LCD_Command(0x80|y);
		Write_LCD_Command(0x80);
		for (x=0; x<16; x++)
		{
			Write_LCD_Data(LCD12864_Buffer[y][x]>>8);
			Write_LCD_Data(LCD12864_Buffer[y][x]);
		}
	}
}

/*------------------------------------------------------------------------------
���ܣ���ʾ����
------------------------------------------------------------------------------*/
void Draw_Clear(void)
{
	u8 x, y;

	for (y=0; y<64; y++)
	{
		for (x=0; x<16; x++)
		{
			LCD12864_Buffer[y][x] = 0;
		}
	}
}

/*------------------------------------------------------------------------------
���ܣ���ʾ4*5�ַ�
������
	x��д��������
	y��д��������
	value���ַ�
------------------------------------------------------------------------------*/
void Draw_Char4x5(u8 x, u8 y, u8 value)
{
	u8 i, j, p, z, a;
	
	a = m_ch4x5[value][5];	
	for (i=0; i<5; i++)
	{
		p = 0x10>>(5-a);
		for (j=0; j<a; j ++)
		{
			z = m_ch4x5[value][i];
			if ( z & p )
			{
				Draw_Dots(j+x, i+y, 1);
			}
			p >>= 1;
		}
	}
}

/*--------------------------------------------------------
���ܣ���ʾһ��4*5�ַ�
������
	x��д��������
	y��д��������
	value���ַ���
	len������
---------------------------------------------------------*/
void Draw_Char4x5Line(u8 x, u8 y, u8 *value, u8 len)
{
	u8 i;

	for (i=0; i<len; i++)
	{
		Draw_Char4x5(x, y, value[i]);
		x += m_ch4x5[value[i]][5];
	}
}
//////////////////////////
