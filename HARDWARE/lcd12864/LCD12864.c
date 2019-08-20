/*************************************************
函数名：12864.c

功 能 ：驱动12864液晶(带字库,串行方式)
液晶屏采用5V液晶时注注意接耐5v电压的端口，为带有FT的端口
屏幕PSB端口已从液晶内部接地。即默认串口
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
 //	选择PB10:CLK(E),输出    PB9R/W(SID),数据      PB8:RS/CS,片选
void LCD12864_Init(void)
{
	/*----------------------------------------*/	 
	GPIO_InitTypeDef GPIO_InitStructure;		//定义GPIO结构变量

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //使能AFIO时钟(下一操作所必需)
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //启用SW-DP(关闭其他以开放IO口)

  RCC_APB2PeriphClockCmd(LCD_RS_RCC, ENABLE);//使能LCD_LCD_RS所在端口时钟
  RCC_APB2PeriphClockCmd(LCD_RW_RCC, ENABLE);//使能LCD_RW所在端口时钟
  RCC_APB2PeriphClockCmd(LCD_EN_RCC, ENABLE);//使能LCD_EN所在端口时钟

  //LCD_RS端口初始化
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
  GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_RS_PORT,LCD_RS_PIN); //LCD_RS输出高电平

  //LCD_RW端口初始化
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
  GPIO_InitStructure.GPIO_Pin = LCD_RW_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_RW_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_RW_PORT,LCD_RW_PIN); //LCD_RW输出高电平

  //LCD_EN端口初始化
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
  GPIO_InitStructure.GPIO_Pin = LCD_EN_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_EN_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_EN_PORT,LCD_EN_PIN); //LCD_EN输出高电平

/*--------------------LCD基本指令-----------------------*/
    delay_ms(2);
    Write_LCD_Command(0x30);  //30--基本指令动作
    delay_ms(5);
    Write_LCD_Command(0x0c);  //光标右移画面不动
    delay_ms(5);
    Write_LCD_Command(0x01);  //清屏
    delay_ms(5);              //清屏时间较长
    Write_LCD_Command(0x06);  //显示打开，光标开，反白关
    delay_ms(5);

//	Write_LCD_Command(0x34);	// 扩充指令
//    Write_LCD_Command(0x36);	// 打开绘图指令
	Draw_Clear();					// 清屏
	//Draw_Update();
}
/*********************************************************
函数名:Send_Byte()
返回值：无
功  能:	写数据到LCD
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
函数名:Receive_Byte()
返回值：temp1+temp2
功  能:	读LCD数据
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
函数名:Check_Busy()
返回值：无
功  能:	LCD忙检查
*********************************************************/
void Check_Busy(void)
{
	do
		Send_Byte(0xfc);		  //11111,RW(1),RS(0),0
	while(0x80&Receive_Byte());
}
/*********************************************************
函数名: Write_LCD_Command()
返回值：无
功  能:	写命令到LCD
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
函数名: Write_LCD_Data()
返回值：无
功  能:	写显示内容到LCD
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
函数名: Read_LCD_Data()
返回值：LCD收到的数据
功  能:	读取LCD显示内容
*********************************************************/
u8 Read_Data(void)
	{
	  Check_Busy();
	  Send_Byte(0xfe);    //11111，RW(1),RS(1),0 LCD->MCU
	  return  Receive_Byte();
	
	}
/*********************************************************
函数名: LCD_Clear_Txt
返回值：无
功  能:	文本区清除
*********************************************************/
void LCD_Clear_TXT( void )
{
     u8 i;
     Write_LCD_Command(0x30);      //8BitMCU,基本指令集合
     Write_LCD_Command(0x80);      //AC归起始位
     for(i=0;i<64;i++)
	  {
       Write_LCD_Data(0x20);
	  }
}
/*********************************************************
函数名: LCD_Clear_BMP
返回值：无
功  能:	图片区清除
*********************************************************/
void LCD_Clear_BMP( void )
{
     u8 i,j;
     Write_LCD_Command(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     Write_LCD_Command(0x36);      //绘图ON,基本指令集里面36H不能开绘图
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           Write_LCD_Command(0x80|i);      //行位置
           Write_LCD_Command(0x80);      //列位置
           for(j=0;j<32;j++)            //256/8=32 byte
                Write_LCD_Data(0);
     }
}
/*********************************************************
函数名: Display_LCD_Pos
返回值：无
功  能:设置显示位置
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
函数名称：Disp_HZ
功    能：控制液晶显示汉字
参    数：addr--显示位置的首地址
          pt--指向显示数据的指针
          num--显示字符个数
返回值  ：无
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
函数名:Display_LCD_String()
返回值：无
功  能:显示字符串
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
* 函数名称    ：PutBMP
**************************************************************/
void PutBMP(u8 *puts)
{
     u16 x=0;
     u8 i,j;
     Write_LCD_Command(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     Write_LCD_Command(0x36);      //绘图ON,基本指令集里面36H不能开绘图
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           Write_LCD_Command(0x80|i);      //行位置
           Write_LCD_Command(0x80);      //列位置
           for(j=0;j<16;j++)      //256/8=32 byte
           {            //列位置每行自动增加
                 Write_LCD_Data(puts[x]);
                 x++;
           }
     }
     for(i=0;i<32;i++)            //12864实际为256x32
     {
           Write_LCD_Command(0x80|i);      //行位置
           Write_LCD_Command(0x88);      //列位置
           for(j=0;j<16;j++)      //256/8=32 byte
           {            //列位置每行自动增加
                 Write_LCD_Data(puts[x]);
                 x++;
           }
     }
}

/*-------------------------------------------------
 LCD显示缓存
--------------------------------------------------*/
u16 LCD12864_Buffer[64][16] = {0};

const u8 m_ch4x5[][6] = 
{
	{0x06,0x09,0x09,0x09,0x06,5},		// 0，字符宽度5点阵
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
	{0x00,0x00,0x00,0x00,0x00,5}, 		// 空格






};
/*------------------------------------------------------------------------------
功能：画点
参数：
	x：横坐标
	y：纵坐标
	color：颜色，0不显示，1显示
------------------------------------------------------------------------------*/
const u16 DrawDotsTable[] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
void Draw_Dots(u8 x, u8 y, u8 color)
{
	u8 i0, y0;
	u16 value;

	// 获得坐标
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
功能：刷新显示
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
功能：显示清屏
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
功能：显示4*5字符
参数：
	x：写出横坐标
	y：写出纵坐标
	value：字符
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
功能：显示一行4*5字符
参数：
	x：写出横坐标
	y：写出纵坐标
	value：字符串
	len：长度
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
