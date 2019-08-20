#include "sys.h"
#include "usart.h"	
		
////////////////////////////////////////////////////////////////////////////////// 	 
u8 trans_buff1[USART1_REC_LEN];  							//串口1接收缓存区

u8 receive_count1;                       											//串口1接收数据计数值
u8 checksum_and1;                       											//串口1校验和
u8 rece_buff1[USART1_REC_LEN];
//u8 Mk_Usart1Al3; 																	//串口1接收一组完成的标志位
u16 trans_end1,trans_begin1; 														//串口1发送开始数据和结束数据
//u16 DataLength3;																	//串口1的接收数据长度	
u8 Scan_Wtime = 0;//保存扫描需要的时间
/////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USAR1T_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}


  	 
void USART1_IRQHandler(void)
{
	u8 res;	    
if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART1);				 
		if(USART1_RX_STA<USART1_REC_LEN)		//还可以接收数据
		{      
			 TIM_SetCounter(TIM4,0);                 //计数器清空      
			if(USART1_RX_STA==0)TIM4_Set(1);	 	//使能定时器4的中断 
			USART1_RX_BUF[USART1_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			USART1_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}  											 
}   
//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 
	static u8 i=0;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{	 			   

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
		if(Scan_Wtime!=0)//蓝牙扫描模式
		{
		   i++;
		   if(i==Scan_Wtime)
		   {
			  i=0;
			  Scan_Wtime = 0;
			  USART1_RX_STA|=1<<15;//直接标记接收成功
			  TIM4_Set(0);	
			  TIM4_SetARR(99);     //重新设置为10ms中断
		   }
		}
		else//非蓝牙扫描模式
		{  
			 USART1_RX_STA|=1<<15; //标记接收完成
		     TIM4_Set(0);		   //关闭TIM4  
	  }
	}   
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM4,0);  //计数器清空
		TIM_Cmd(TIM4, ENABLE);   //使能TIMx	
	}else TIM_Cmd(TIM4, DISABLE);//关闭定时器4
}
//配置TIM4预装载周期值
void TIM4_SetARR(u16 period)
{
     TIM_SetCounter(TIM4,0); //计数器清空
	 TIM4->ARR&=0x00;        //先清预装载周期值为0
	 TIM4->ARR|= period;     //更新预装载周期值
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能//TIM4时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}
/***************************************************************************/
//*******函数名：USART1_IRQHandler()/
//*******说  明：串口1的中断服务函数/
//*******参  数：串口1/
//*******返回值：无返回/
//***************************************************************************/ 
//void USART1_IRQHandler(void)
//{
//	u8 sbuf1=0;	    
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)                              	     //接收到数据
//	{	 

//		sbuf1 =USART_ReceiveData(USART1); 	                                                 //读取接收到的数据

//	} 
// 	
//	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                                     //发送中断	
//	{
//		if(trans_end1 != trans_begin1)                							             //判断发送开始与发送结束是否在同一地址
//		{
//		    USART_SendData(USART1,trans_buff1[trans_end1]);                                 //中断发送数据
//		    trans_end1++;                                                                    //地址自增
//		    if(trans_end1 == USART_REC_LEN)                                       		     //判断有没有超出上限
//		    {
//		        trans_end1 = 0;                                                   		     //复位
//		    }
//		}
//		else  				                            				                     //发送完成
//		{
//			USART_ITConfig(USART1,USART_IT_TXE,DISABLE);                       		         //禁止发送中断标志位

//		}
//	}		
//	
//}   
void UsartSend_Bit1(u8 Rdata,u8 whe)
{
	checksum_and1 += Rdata;                 									    //计算校验和              
	trans_buff1[trans_begin1] = Rdata;                   						    //将数据放入缓冲区
	trans_begin1++;
	if(trans_begin1 == USART1_REC_LEN)                 								//判断有没有超出上限
	{
		trans_begin1=0;                               								//清计数
	}
	if(whe)
	{
	    USART_ITConfig(USART1,USART_IT_TXE,ENABLE);                       		    //使能串口1接收中断,发送中断
	}

} 

#endif	
