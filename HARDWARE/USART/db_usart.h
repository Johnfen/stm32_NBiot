#ifndef __DB_USART_H
#define __DB_USART_H
#include "sys.h"
#include "stm32f10x.h"
#define MAX_TRAN_BUF2 1200													//���崮�ڷ�����󻺴�
#define MAX_RECE_BUF2 1200													//���崮�ڽ�����󻺴�

#define MAX_TRAN_BUF5 200
#define MAX_TRAN_BUF3 200

#define EnableUsart2Tx USART_ITConfig(USART2,USART_IT_TXE,ENABLE)           //ʹ�ܴ���2�����ж�,�����ж�
extern u8 checksum_and2;                       								//����2У���
extern u8 rece_buff2[MAX_TRAN_BUF2],trans_buff2[MAX_TRAN_BUF2];  			//����2���ջ�����
extern u8 Mk_Usart1Al2; 													//����2����һ����ɵı�־λ
extern u16 trans_end2,trans_begin2; 										//����2���Ϳ�ʼ���ݺͽ�������
extern u16 DataLength2;				
extern u16 UCollectSite2;													//����2�Ľ������ݳ���


extern u8 checksum_and3;                       								//����2У���
extern u8 rece_buff3[MAX_TRAN_BUF3],trans_buff3[MAX_TRAN_BUF3];  			//����2���ջ�����
extern u8 Mk_Usart1Al3; 													//����2����һ����ɵı�־λ
extern u16 trans_end3,trans_begin3; 										//����2���Ϳ�ʼ���ݺͽ�������
extern u16 DataLength3;				
extern u16 UCollectSite3;													//����2�Ľ������ݳ���

//extern u16 RecepAdd5;                                                   	//����5�Ľ��յ�ַ
//extern u16 trans_end5,trans_begin5; 										//����5���Ϳ�ʼ���ݺͽ�������
//extern u8 rece_buff5[MAX_TRAN_BUF5],trans_buff5[MAX_TRAN_BUF5],DataLength5; //����5���ķ��ͻ���ͽ��ջ���
//extern OS_EVENT * sem_usa5;													//����5�ź���ָ��

extern u16 checksum_and5;                     //����5CRCУ���
extern u8  Receive_Data_OK_flag;								//���յ�һ�����ݱ�־

#define  U2Baud	57600                                                    	//����2������

void Usart_Init(void);		   												//����2��ʼ��
void UsartRece_Data5(void);                                             	//����5��������
void UsartSend_Bit2(u8 Rdata,u8 whe);                                   	//����2���ͺ���
void UsartSend_Bit3(u8 Rdata,u8 whe);                                   	//����3���ͺ���
void UsartSend_Bit5(u8 Rdata,u8 whe);                                   	//����5���ͺ���

unsigned int CRC16(unsigned char *puchMsg,unsigned char usDataLen);		//crc���㺯��
void SendData5(unsigned char nums,unsigned char Buff[20]);				//��������
#define USART_Gallery_Num 		      
                                    
                                     

                                     


#endif
