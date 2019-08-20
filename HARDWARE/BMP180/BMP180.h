#ifndef _BMP180_H_
#define _BMP180_H_

#include "IIC.h"
#include "math.h"
#define BMP180_Address 0xee

extern s32   bmp180_temp,bmp180_press;
void BMP180_getdat(void);
void Read_calibration(void);
void Write_BMP180(u8 add, u8 da);
void BMP180_delay(void);
void Multiple_read_BMP180(u8 add,u8*BUF,u16 num);
void Write_BMP180(u8 add, u8 da);


#endif



