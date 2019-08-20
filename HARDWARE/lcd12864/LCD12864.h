#ifndef __LCD12864_H
#define __LCD12864_H
#include "sys.h"
/*         */
#define CS_H PBout(6)=1      //P10 CS RS
#define CS_L PBout(6)=0
#define SID_H PBout(7)=1     //P12 SID RW
#define SID_L PBout(7)=0
#define SCLK_H PBout(8)=1   //P11 SCLK E
#define SCLK_L PBout(8)=0
//#define PSB_L PEout(11)=0	 //PE11 PSB
/*----------------------------------*/
#define LCD_RS_PORT GPIOB  //定义LCD_RS端口  数据命令
#define LCD_RW_PORT GPIOB  //定义LCD_RW端口	读写
#define LCD_EN_PORT GPIOB  //定义LCD_EN端口	使能
#define LCD_RS_RCC RCC_APB2Periph_GPIOB  //定义LCD_RS端口时钟
#define LCD_RW_RCC RCC_APB2Periph_GPIOB  //定义LCD_RW端口时钟
#define LCD_EN_RCC RCC_APB2Periph_GPIOB  //定义LCD_EN端口时钟
#define LCD_RS_PIN GPIO_Pin_6 //定义LCD_RS引脚
#define LCD_RW_PIN GPIO_Pin_7 //定义LCD_RW引脚
#define LCD_EN_PIN GPIO_Pin_8  //定义LCD_EN引脚

//#define LCD_RS PEout(2)  //定义LCD_RS位带操作地址
//#define LCD_RW PEout(3)  //定义LCD_RW位带操作地址
//#define LCD_EN PEout(4)  //定义LCD_EN位带操作地址
/*-----------------------------------*/

//函数声明
void LCD12864_Init(void);
void Send_Byte(u8 zdata);
u8 Receive_Byte(void);
void Check_Busy(void);
void Write_LCD_Command(u8 cmdcode);
void Write_LCD_Data(u8 Dispdata);
u8 Read_Data(void);
void LCD_Clear_TXT( void );
void LCD_Clear_BMP( void );
void Display_LCD_Pos(u8 x,u8 y) ;
void Draw_dots(u8 x,u8 y);
void Disp_HZ(u8 X,const u8 * pt,u8 num);
void Display_LCD_String(u8 x,u8 *p,u8 time);	 
void PutBMP(u8 *puts);
void Draw_Dots(u8 x, u8 y, u8 color);
void Draw_Update(void);
void Draw_Clear(void);
void Draw_Char4x5(u8 x, u8 y, u8 value);
void Draw_Char4x5Line(u8 x, u8 y, u8 *value, u8 len);
void LCD_ShowString(u8 x,u8 y,const u8 *p);
void I_C(u16 x,u8 *p,u8 n);
void DisplayOneChar(u8 x,u8 y, u8 p);

#endif  
	 
	 



