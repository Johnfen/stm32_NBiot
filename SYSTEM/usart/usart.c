#include "sys.h"
#include "usart.h"	
		
////////////////////////////////////////////////////////////////////////////////// 	 
u8 trans_buff1[USART1_REC_LEN];  							//����1���ջ�����

u8 receive_count1;                       											//����1�������ݼ���ֵ
u8 checksum_and1;                       											//����1У���
u8 rece_buff1[USART1_REC_LEN];
//u8 Mk_Usart1Al3; 																	//����1����һ����ɵı�־λ
u16 trans_end1,trans_begin1; 														//����1���Ϳ�ʼ���ݺͽ�������
//u16 DataLength3;																	//����1�Ľ������ݳ���	
u8 Scan_Wtime = 0;//����ɨ����Ҫ��ʱ��
/////////////////////////////////////////////////////////////////////
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USAR1T_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}


  	 
void USART1_IRQHandler(void)
{
	u8 res;	    
if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART1);				 
		if(USART1_RX_STA<USART1_REC_LEN)		//�����Խ�������
		{      
			 TIM_SetCounter(TIM4,0);                 //���������      
			if(USART1_RX_STA==0)TIM4_Set(1);	 	//ʹ�ܶ�ʱ��4���ж� 
			USART1_RX_BUF[USART1_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART1_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  											 
}   
//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 
	static u8 i=0;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־    
		if(Scan_Wtime!=0)//����ɨ��ģʽ
		{
		   i++;
		   if(i==Scan_Wtime)
		   {
			  i=0;
			  Scan_Wtime = 0;
			  USART1_RX_STA|=1<<15;//ֱ�ӱ�ǽ��ճɹ�
			  TIM4_Set(0);	
			  TIM4_SetARR(99);     //��������Ϊ10ms�ж�
		   }
		}
		else//������ɨ��ģʽ
		{  
			 USART1_RX_STA|=1<<15; //��ǽ������
		     TIM4_Set(0);		   //�ر�TIM4  
	  }
	}   
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM4,0);  //���������
		TIM_Cmd(TIM4, ENABLE);   //ʹ��TIMx	
	}else TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��4
}
//����TIM4Ԥװ������ֵ
void TIM4_SetARR(u16 period)
{
     TIM_SetCounter(TIM4,0); //���������
	 TIM4->ARR&=0x00;        //����Ԥװ������ֵΪ0
	 TIM4->ARR|= period;     //����Ԥװ������ֵ
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Init(u16 arr,u16 psc)
{	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��//TIM4ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
/***************************************************************************/
//*******��������USART1_IRQHandler()/
//*******˵  ��������1���жϷ�����/
//*******��  ��������1/
//*******����ֵ���޷���/
//***************************************************************************/ 
//void USART1_IRQHandler(void)
//{
//	u8 sbuf1=0;	    
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)                              	     //���յ�����
//	{	 

//		sbuf1 =USART_ReceiveData(USART1); 	                                                 //��ȡ���յ�������

//	} 
// 	
//	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                                     //�����ж�	
//	{
//		if(trans_end1 != trans_begin1)                							             //�жϷ��Ϳ�ʼ�뷢�ͽ����Ƿ���ͬһ��ַ
//		{
//		    USART_SendData(USART1,trans_buff1[trans_end1]);                                 //�жϷ�������
//		    trans_end1++;                                                                    //��ַ����
//		    if(trans_end1 == USART_REC_LEN)                                       		     //�ж���û�г�������
//		    {
//		        trans_end1 = 0;                                                   		     //��λ
//		    }
//		}
//		else  				                            				                     //�������
//		{
//			USART_ITConfig(USART1,USART_IT_TXE,DISABLE);                       		         //��ֹ�����жϱ�־λ

//		}
//	}		
//	
//}   
void UsartSend_Bit1(u8 Rdata,u8 whe)
{
	checksum_and1 += Rdata;                 									    //����У���              
	trans_buff1[trans_begin1] = Rdata;                   						    //�����ݷ��뻺����
	trans_begin1++;
	if(trans_begin1 == USART1_REC_LEN)                 								//�ж���û�г�������
	{
		trans_begin1=0;                               								//�����
	}
	if(whe)
	{
	    USART_ITConfig(USART1,USART_IT_TXE,ENABLE);                       		    //ʹ�ܴ���1�����ж�,�����ж�
	}

} 

#endif	
