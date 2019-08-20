#ifndef __DB_USART_H
#define __DB_USART_H
#include "sys.h"
#include "stm32f10x.h"
#define MAX_TRAN_BUF2 1200													//定义串口发送最大缓存
#define MAX_RECE_BUF2 1200													//定义串口接收最大缓存

#define MAX_TRAN_BUF5 200
#define MAX_TRAN_BUF3 200

#define EnableUsart2Tx USART_ITConfig(USART2,USART_IT_TXE,ENABLE)           //使能串口2接收中断,发送中断
extern u8 checksum_and2;                       								//串口2校验和
extern u8 rece_buff2[MAX_TRAN_BUF2],trans_buff2[MAX_TRAN_BUF2];  			//串口2接收缓存区
extern u8 Mk_Usart1Al2; 													//串口2接收一组完成的标志位
extern u16 trans_end2,trans_begin2; 										//串口2发送开始数据和结束数据
extern u16 DataLength2;				
extern u16 UCollectSite2;													//串口2的接收数据长度


extern u8 checksum_and3;                       								//串口2校验和
extern u8 rece_buff3[MAX_TRAN_BUF3],trans_buff3[MAX_TRAN_BUF3];  			//串口2接收缓存区
extern u8 Mk_Usart1Al3; 													//串口2接收一组完成的标志位
extern u16 trans_end3,trans_begin3; 										//串口2发送开始数据和结束数据
extern u16 DataLength3;				
extern u16 UCollectSite3;													//串口2的接收数据长度

//extern u16 RecepAdd5;                                                   	//串口5的接收地址
//extern u16 trans_end5,trans_begin5; 										//串口5发送开始数据和结束数据
//extern u8 rece_buff5[MAX_TRAN_BUF5],trans_buff5[MAX_TRAN_BUF5],DataLength5; //串口5发的发送缓存和接收缓存
//extern OS_EVENT * sem_usa5;													//串口5信号量指针

extern u16 checksum_and5;                     //串口5CRC校验和
extern u8  Receive_Data_OK_flag;								//接收到一组数据标志

#define  U2Baud	57600                                                    	//串口2波特率

void Usart_Init(void);		   												//串口2初始化
void UsartRece_Data5(void);                                             	//串口5处理数据
void UsartSend_Bit2(u8 Rdata,u8 whe);                                   	//串口2发送函数
void UsartSend_Bit3(u8 Rdata,u8 whe);                                   	//串口3发送函数
void UsartSend_Bit5(u8 Rdata,u8 whe);                                   	//串口5发送函数

unsigned int CRC16(unsigned char *puchMsg,unsigned char usDataLen);		//crc计算函数
void SendData5(unsigned char nums,unsigned char Buff[20]);				//发送数据
#define USART_Gallery_Num 		      
                                    
                                     

                                     


#endif
