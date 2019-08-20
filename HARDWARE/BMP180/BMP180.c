/*********************
BMP180��������
��Ҫ����ΪBMP180_getdat();
���ֶ�ȡ������
���Ժ���ֱ�����32λ������
Ҳ����ֱ�ӵ���ȫ�ֱ�����bmp180_temp,bmp180_press�ֱ����¶Ⱥ�ѹǿ
˵����
ѹǿ���������λ��С������λ����˹�������ݴ�Լ��10170000
�¶����ݵ�λû���⣬����Ҳ�ò��ϡ�������
�¶Ⱥ�ѹǿ�ֱ�����һ�׺Ͷ��׵�ͨ�˲���ѹǿ�ĵ�ͨ�Ƿֶεĵ�ͨ�����ӽ�Ŀ��ֵʱ�����޽�ֹƵ�ʽ���
�������˲�Ҳ���Խ��˲��Ĳ���ע�͵�
*********************/

#include "bmp180.h"

short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;
s32   bmp180_temp,bmp180_press;

/***********
�������õ�����ʱ��4.5ms��
һ����������
***********/
void BMP180_delay(void)
{
	u32 count=200000;
	while(count--);
}


/****************
IIC��BMP180ͨѶ����
****************/
/*
u8 Read_BMP180(u8 REG_Address)
{   
	u8 REG_data;
    IIC_Start();                          //��ʼ�ź�
    IIC_Send_Byte(BMP180_Address);           //�����豸��ַ+д�ź�
	IIC_Wait_Ack();

    IIC_Send_Byte(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
	IIC_Wait_Ack();

    IIC_Start();                          //��ʼ�ź�
    IIC_Send_Byte(BMP180_Address+1);         //�����豸��ַ+���ź�
	IIC_Wait_Ack();

    REG_data=IIC_Read_Byte(0);              //�����Ĵ�������
	IIC_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
*/
void Multiple_read_BMP180(u8 add,u8*BUF,u16 num)
{
	u8 i;
    IIC_Start();                          //��ʼ�ź�
    IIC_Send_Byte(BMP180_Address);           //�����豸��ַ+д�ź�
	IIC_Wait_Ack();
    IIC_Send_Byte(add);                   //���ʹ洢��Ԫ��ַ
	IIC_Wait_Ack();
    IIC_Start();                          //��ʼ�ź�
    IIC_Send_Byte(BMP180_Address+1);         //�����豸��ַ+���ź�
	IIC_Wait_Ack();
	 for (i=0; i<num; i++)                      //������ȡnum����ַ���ݣ��洢��BUF
    {
        
        if (i == num-1)
        {
           BUF[i] = IIC_Read_Byte(0);          //���һ��������Ҫ��NOACK
        }
        else
        {
          BUF[i] = IIC_Read_Byte(1);          //����ACK
       }
   }
    IIC_Stop();                          //ֹͣ�ź�
}


void Write_BMP180(u8 add, u8 da)
{
    IIC_Start();                  //��ʼ�ź�
    IIC_Send_Byte(BMP180_Address);   //�����豸��ַ+д�ź�
	IIC_Wait_Ack();

    IIC_Send_Byte(add);    //�ڲ��Ĵ�����ַ����ο�����pdf 
	IIC_Wait_Ack();

    IIC_Send_Byte(da);       //�ڲ��Ĵ������ݣ���ο�����pdf
	IIC_Wait_Ack();

    IIC_Stop();                   //����ֹͣ�ź�
}

/*
void Multiple_write_BMP180(u8 add,u8*BUF,u16 num)
{
	while(num--)
	{
		Write_BMP180(add++, *BUF++);
	}
}
*/

/***************
�����Ƕ�ȡ���ݵ�����
���濪ʼ��ȡ����
***************/
/*******************
��ȡ��������
*******************/
void Read_calibration(void)
{
  u8 buf[2];
  Multiple_read_BMP180(0xaa,buf, 2);
  ac1 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xac,buf, 2);
  ac2 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xae,buf, 2);
  ac3 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xb0,buf, 2);
  ac4 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xb2,buf, 2);
  ac5 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xb4,buf, 2);
  ac6 = buf[0] << 8 |buf[1]; 
  Multiple_read_BMP180(0xb6,buf, 2);
  b1 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xb8,buf, 2);
  b2 = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xba,buf, 2);
  mb = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xbc,buf, 2);
  mc = buf[0] << 8 |buf[1];
  Multiple_read_BMP180(0xbe,buf, 2);
  md = buf[0] << 8 |buf[1];
}

/*********************
��ȡѹǿ����
*********************/
void  BMP180_getdat(void)
{
  long x1,x2,x3,b3,b5,b6,b7,UP,press,UT,temp;
  unsigned long b4;
  u8 ReadBuffer[3];
  char oss = 0;  //���ֵ�ڶ���ѹʱ�����ý��Ĵ���
  static float press_fliter=101700,press_q=0.1,temp_fliter=20000,temp_q=0.1;//��ͨ�˲�&��ֵ
 
  //��ȡ��������
  Read_calibration();
  
  //��ȡԭʼ�¶�����
  Write_BMP180(0xf4,0x2e);
  BMP180_delay();						//delay 4.5ms
  Multiple_read_BMP180(0xf6,ReadBuffer,2);
  UT = ReadBuffer[0] << 8 | ReadBuffer[1];
  
  //��ȡԭʼѹǿ����
  Write_BMP180(0xf4,(0x34 +(oss<<6)));
  BMP180_delay();						//delay 4.5ms
  Multiple_read_BMP180(0xf6,ReadBuffer,3);
  UP = ((ReadBuffer[0] << 16)+(ReadBuffer[1] << 8)+ReadBuffer[2]) >> (8 - oss);

  //�¶Ȼ���
  x1 = ((UT - ac6) * ac5) >> 15; 
  x2 = ((long) mc << 11) / (x1 + md);
  b5 = x1 + x2;
  temp = ((b5 + 8)*100) >> 4;//*100��Ϊ��������λ��temp��Ϊ��������
  
  //��ʼ�˲��㷨
  temp_fliter = temp_fliter*(1-temp_q)+(float)temp*temp_q;
  bmp180_temp=temp_fliter;
  
  //ѹǿ����
  b6 = b5 - 4000;
  x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = ((((long)ac1 * 4 + x3) << oss) + 2) / 4;
  x1 = (ac3 * b6) >> 13;
  x2 = (b1 * ((b6 * b6)>> 12)) >> 16;
  x3 = ((x1 + x2 )+ 2) >> 2;
  b4 = (ac4 * (unsigned long)(x3 + 32768)) >> 15;
  b7 = ((unsigned long)UP - b3) * (50000 >> oss);
  if(b7 < 0x80000000)
    press = (b7 * 2) / b4;
  else
    press = (b7 / b4 ) * 2;
  x1 = ((press) >> 8) * ((press) >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * press) >> 16;
  press = press + (((x1 + x2 + 3791)) >> 4);//press��Ϊ����������
  
  //��ʼ�˲��㷨
//  if(fabs(press_fliter-press)<100)press_q=fabs(press_fliter-press)/1000;
//  else press_q=0.1;
//  press_fliter = press_fliter*(1-press_q)+(float)press*press_q;
  bmp180_press  =press*100;// press_fliter*100;
  //return bmp180_press;//bmp180_press;
}




