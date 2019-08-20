#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LCD12864.h"
#include "IIC.h"
#include "bmp180.h"
#include "MAX6675.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "GPY.h" 
#include "db_amme.h"
#include "bc95.h"
#include "key.h"
#include "relay.h"
#define time  50					   //宏定义，字符显示的时间差

//////////////////////////////////
u8 zhuan_su2[3];

////////////////////////
u8 wen_yuz[2]="0";
u8 wen_yu=35;
double qi_yu=101.30;

u8  p_value[20];
u8  t_value[20];
u8  ad_value[20];

int fen_chens=1;

u8  dis_VolA[20];
u8  dis_CurrentA[20];
u8  dis_PowerA[20];
u8 chuan11[30];
char chuan12[30];
//////////////////////////
u8 xian_ch[10]={'C',':','0' ,'0'};
u8 xian_ch2[10]={'J',':','0','0'};
u8 clear[8]={' ',' ',' ',' ',' ',' ',' '};
u8 clear_1[8]={'U',':','0','V',' ',' ',' '};
u8 clear_2[8]="I:0.00A";
u8 clear_3[8]={'P',':','0','W',' ',' ',' '};
u8 xian_qi[8];
////////////////////////////////
//u8 jd_flag=0;
/////////////////////
long  int ji_1s=99999;
long  int ji_0_5s=49999;
//long  int ji_50ms=2000;
u8 ji2_flag;
unsigned char ji_5s=0;
u8 ji_3s=3;
u8 ji3_flag=0;
u8 ji40_flag=0;
u8 ji_40s=10;
u8 ji1_flag=0;
u8 chuan3_flag=0;
////////////////////////////
unsigned char du_dian_flag=0;
unsigned char du_wen_flag=0;
unsigned char du_qi_flag=0;
float du=0;
//////////////////////
unsigned int t,i;
unsigned char c;
unsigned char flag;
float temprature;
//////////////////////
s32  press_temp;
s32 temp=0;
u8 ge,shi,bai,qian,wan,shiwan; 
double pree_tmp,temp_tmp;
/////////////////////////////
long int zhou_qi=999;
u8 pwmval=32;
//////////////////////////////
u16 adcx;
float dian_ya;
u8 ad_flag=0;
////////////////////////////////
u8 m1='1';
u8 m2;
u8 m3,m4,m5;
u8 bu_liu=0;
//////////////////////////////////
u8 bu=0; 
u8 chuan_flag1;
u8 chuan_flag2; 
u8 shuju=0;
//////////////////////////////////
u16 USART1_RX_STA;
u16 pluse_cnt=0;
u16 pluse_cnt2=0;
u16 zhuan_su=0;
u8 su_value[6]={'N',':'};

u8 su_wei;
///////////////////////////////////
char buf1[100];
u8 buf2[24];   //23;
int du_z;
int pan_dian_chu[3];

///////////////////////////////////
//typedef struct _she_zhi_
//{
//    u8 set;
//    double qi_ya;
//	  u16 wen_du;
//	  u8 nong_du;
//	  u16 dian_ya;
//	  u16 dian_liu;
//	  u16 gong_lv;
//}she_zhi;
//she_zhi she_zhi1;
////////////////////////////////////
void conversion_p(u32 temp_data);
//void conversion_t2(u32 temp_data);
void conversion_ad(float temp_data);                            
void du_wendu(void);
void xian_qiya(void);
void conversion_t(float temp_data);
void xian_ad(void);
void conversion_va(u32 temp_data,u32 temp_data2,u32 temp_data3);
void shu_chu1(void);
void shu_chu2(void);
void ji_kong(void);
void conversion_su(u16 su);
//void copystr(u8 *p1,char *p2,int m,int m2); //截取数据
//void she_chu(void);

int main()
{	                    
		SystemInit();                                          /* 配置系统时钟为72M */
	
		delay_init();
	 delay_ms(200);
	 InitAmmeter();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		LCD12864_Init();
		IIC_Init();
		/* MAX6675 SPI 接口初始化 */
		SPI_MAX6675_Init();
		//Read_calibration();
	   GPLED_Init();         //GPLED端口初始化
		 LED_Init();			     //LED端口初始化
	   relay_Init();
	   KEY2_Init();
	  TIM3_Int_Init(9,71);// 10us计数//10Khz的计数频率，计数到5000为500ms,20为2ms,9为1ms
	//////////////////////////////////////////
    TIM4_Init(99,7199);		//10ms中断
	  TIM4_Set(0);			//关闭定时器4
	////////////////////////////////////////////////////////////
	   Adc_Init();
		Write_LCD_Command(0x30);		 //再次打开基本指令，后面用到
	 uart_init(9600);  
	 TIM2_CAP_Init();               //0XFFFF,72-1
	
	
   	while(1)													 
	{ 
	
	//////////////////////////////////////
		ji_kong();
		///////////////////////////
			if(ad_flag)//&&(ji40_flag==0))
				{
					adcx=Get_Adc_Average(ADC_Channel_1,10);
					ad_flag=0;
				}
//////////////////////////////////////
			
				if(du_wen_flag==1)
		{
      du_wendu();	
			
			xian_qiya();
			xian_ad();
		}
//			if(du_qi_flag==1)
//		{
//      du_qi_flag=0;
//			xian_qiya();
//		
//		}
		
		
		
		if(du_dian_flag==1)
		{
//			if((pan_dian_chu[0]!=relay0)||(pan_dian_chu[0]!=relay1)||(pan_dian_chu[0]!=relay2))
//			{
//				pan_dian_chu[0]=relay0;
//				pan_dian_chu[1]=relay1;
//				pan_dian_chu[2]=relay2;
//				InitAmmeter();
//			}
//			else{
			ReadAmmeterData();//Up_CurrentA
			conversion_va(Up_VolA,Up_CurrentA,Up_PA_Power);
			//}
			//printf("A相电压：%4d,电流：%d,有功功率:%d.\n",Up_VolA,Up_CurrentA,Up_PA_Power);
     
		  LCD_ShowString(0,0,clear);
      LCD_ShowString(1,0,clear);	
			LCD_ShowString(0,0,dis_VolA);
			LCD_ShowString(0,4,dis_CurrentA);
	    //LCD_ShowString(1,0,clear);
			LCD_ShowString(1,0,dis_PowerA);

			du_dian_flag=0;
			
		}   
/////////////////////////////////////////
			
			if(bu!=13)
			{
			  if(ji1_flag==1)
				{
					ji1_flag=0;

				 //  printf("%c\r\n",pluse_cnt2);
				  bu=BC95_init(bu);
					ji40_flag=0;
				}
				// xian_ch[2]= bu/10+0x30;
				 //xian_ch[3]= bu%10+0x30;
				 //LCD_ShowString(2,5,xian_ch);
				 
   	
			} 
			else 
			{
				 if(ji1_flag==1)//(ji40_flag==0)&&(ji1_flag==1))
		{
			ji1_flag=0;
			
			/////////////////////////
			/*转速测量*/
			if(pluse_cnt2>70)
			{
			if((((pluse_cnt2/7)+7)>zhuan_su)||(((pluse_cnt2/7)-7)<zhuan_su))
			{
			 zhuan_su=(zhuan_su+pluse_cnt2/7)/2;
			}
			else if(zhuan_su==0)
			{
				zhuan_su=pluse_cnt2/7;
			}
		 }
			else if(pluse_cnt2<70)
			{
				pluse_cnt2=0;
				pluse_cnt=0;
				zhuan_su=0;
			}
//			if((relay0==1)&&(relay1==1)&&(relay2==1))
//			{
//				zhuan_su=0;
//			}
			//pluse_cnt2=0;
		
			//TIM2CH1_CAPTURE_STA=0;
			//su_wei=ji_shu_c(zhuan_su);
			//itoa(zhuan_su, zhuan_su2, 10);
		  conversion_su(zhuan_su);
			LCD_ShowString(3,6,su_value);
			////////////////////////
			
			}
				
				if(ji40_flag==1)
				{
					//USART1_RX_STA=0;
					shu_chu1();
					ByteToHexStr(chuan11, chuan12, 24);
					
				  if(send_UDP_msg("24",chuan12))
					 {
						 shuju++;

					 }
					 
				  ji40_flag=0;
				
				}
			
			 
			 if(USART1_RX_STA&0X8000)		//接收到一次数据了
			{ 
				USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
				m2=USART1_RX_BUF[12];
				//DisplayOneChar(2,5, m2);
				if((m2=='0')||(m2=='1')||(m2=='2')||(m2=='3'))
				{
				m1=m2;	
				qi_yu=((USART1_RX_BUF[14]-'0')*100+(USART1_RX_BUF[16]-'0')*10+(USART1_RX_BUF[18]-'0')+(double)(USART1_RX_BUF[22]-'0')/10+(double)(USART1_RX_BUF[24]-'0'+0.5)/100);
				//wen_yu=wen_yu*10+(USART1_RX_BUF[22]-'0');
					
				wen_yu=USART1_RX_BUF[26]-'0';
				wen_yu=wen_yu*10+USART1_RX_BUF[28]-'0';
			  wen_yu=wen_yu*10+USART1_RX_BUF[30]-'0';
				fen_chens=USART1_RX_BUF[32]-'0';
				fen_chens=fen_chens*10+USART1_RX_BUF[36]-'0';
//			xian_qi[0]= (int)(qi_yu/100)%10+0x30;
//		  xian_qi[1]= (int)(qi_yu/10)%10+0x30;
//		  xian_qi[2]= (int)qi_yu%10+0x30;
//			xian_qi[3]= (int)((qi_yu)*10)%10+0x30;
//			xian_qi[4]= (int)((qi_yu)*100)%10+0x30;
									
			 
				
					
			  //copystr(USART1_RX_BUF,buf1,12, 56);
				//LCD_ShowString(2,5, xian_qi);
         // printf("%s\r\n",buf1);
        //du_z=hex_to_str2(buf1,56,buf2);
				//she_chu();
					
				
				} 
				USART1_RX_STA=0;
		  }
	}
			 
}
	}
//////////////////////////////
//定时器5中断服务程序     

	void TIM2_IRQHandler(void)
{ 
    

	
    if(TIM_GetFlagStatus(TIM2, TIM_FLAG_CC1) != RESET)  // 边沿捕获触发标志触发
    {
			  if(pluse_cnt>800)
				{
					pluse_cnt=0;
				}
        pluse_cnt ++;   
    }
    TIM_ClearFlag(TIM2, TIM_FLAG_CC1);                  // 清除状态标志
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);         // 清除中断标志位  
}
//定时器3中断服务程序
void TIM3_IRQHandler(void) //TIM3中断
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除TIM3更新中断标志
/////////////////////////////

	 	if(zhou_qi==0)
		{
			zhou_qi=999;
			pwmval=32;
		}
		else
		{
			if(pwmval==0)
			{
				//pwmval=32;
				//LED0=1;
				GP_LED=1;
			}
			else
			{
				GP_LED=0;
				//LED0=0;
				pwmval--;
				if(pwmval==3)
				{
					ad_flag=1;
				}
				
			}
			zhou_qi--; 
		}
	
//    if(ji_50ms--==0)
//		{
//			ji_50ms=5000;
//			du_qi_flag=1;
//		}
////////////////////////////////////
		if(ji_1s--==0)
		{

			ji1_flag=1;
			ji_1s = 99999;
			pluse_cnt2=pluse_cnt;
			pluse_cnt=0;
			du_wen_flag=1;
			LED1=!LED1;
			
			if(ji_5s==4)
			{
				 ji_5s=0;
				 du_dian_flag=1;
			}
			else
				ji_5s++;
//////////////////////
//计40s
//////////////////////////
			if(ji40_flag==0)
			{
			if(ji_40s--==0)
			{
				ji40_flag=1;
				ji_40s=10;
				
			}
		  }
	 
	  
//////////////////////////////
			if(ji_3s==0)
			{
				 ji_3s=3;
				if(chuan_flag1==1)
		   {
        chuan_flag2=1;
		   }
			}
			else
				ji_3s--;
	}
}
}
void shu_chu1()
{
	u16  i,j;
	for(i=0;i<24;i++)
	{
		if(i<=5)
		{
			   if(((p_value[i+2]>='0')&&(p_value[i+2]<='9'))||(p_value[i+2]=='.'))
		     chuan11[i]=p_value[i+2];
				 else
					 chuan11[i]='0';
		    
		}
		else if(i<=8)
		{
			  if(i==6)
				{
					 j=0;
			    if(temprature<100)
				 {
					  chuan11[i]='0';
					  i=i+1;
					 
				 }
				
			 }
				 
           chuan11[i]=t_value[j+2];

					 j++;
				
		}			
		     
    else if(i<=11)
		{ 
			  if(i==9)
				{
					j=0;
				}
			 
         chuan11[i]=ad_value[j+3]; 
				 j++;
		}
		else if(i<=14)
		{
			 if(i==12)
				{
					j=0;
				 if(Up_VolA<10)
				 {
					  chuan11[i]='0';
					  chuan11[i+1]='0';
					  i=i+2;
					  
				 }
				 else if(Up_VolA<100)
				 {
					  chuan11[i]='0';
					  i=i+1;
					  
				 }
			 }				 
         chuan11[i]=dis_VolA[j+2]; 
			   j++;
		}
		else if(i<=17)
		{
			  if(i==15)
				{
					j=0;
				}
				if(j<2)
			   chuan11[i]=dis_CurrentA[j+4];
				else 
					 chuan11[i] = bu_liu%10+'0';
				
				j++;
		}
		else if(i<=21)
		{
			 
			  if(i==18)
				{
					j=0;
			 if(Up_PA_Power<10)
				 {
					  chuan11[i]='0';
					  chuan11[i+1]='0';
					  chuan11[i+2]='0';
					  i=i+3;
					  
				 }
				
				 else if(Up_PA_Power<100)
				 {
					  chuan11[i]='0';
					  chuan11[i+1]='0';
					  i=i+2;
					 
				 }
				
			 }				 
         chuan11[i]=dis_PowerA[j+2]; 
			 j++;
		}
		else if(i<=23)
		{
			 
			  if(i==22)
				{
					j=0;
				}
			 if(zhuan_su<100)
				 {
					 
					 chuan11[i]=su_value[j+2]; 
					 if(zhuan_su==0)
					 {
						 chuan11[i]='0';
						 
					 }
					  
				 }
				else
				{
					chuan11[i]='0';
				}
		 
         
			 j++;
		}
		
  }
}
void ji_kong()
{
//	  if(m1=='0')
//			{
//				if(LED0==1)
//				LED0=0;
//				
//			}
	
//		{
//			if(temprature<(wen_yu))   //)(/*(du>2)&&*/
//			{
//				if(LED0==0)
//				LED0=1;		
//			}
//			else if(temprature>(wen_yu+5))//)/*(du<0.8)||*/
//			{
//				if(LED0==1)
//				LED0=0;	
//			}
				if(temprature<(wen_yu))   //)(/*(du>2)&&*/
			{
				m3=1;
			}
			else if(temprature>(wen_yu+5))//)/*(du<0.8)||*/
			{
				m3=0;	
			}
			if(pree_tmp<qi_yu-0.04)
			{
				m4=2;
			}
			else if(pree_tmp>qi_yu+0.04)
			{
				m4=0;
			}
			else if((pree_tmp>qi_yu-0.01)&&(pree_tmp<qi_yu+0.01))
			{
				m4=1;
			}
			if(du*10>fen_chens)
			{
				m5=1;
			}
			else 
			{
				m5=0;
			}
	 if((m3==1)&&(m5==1))
	 {
	 if(m1=='0')
	 {
		 relay0=1;
	   relay1=1;
	   relay2=1;
		 //printf("%d\r\n",she_zhi1.set);
		 //she_zhi1.set=4;
		 //m2='4';
		 //m1='4';
	 }
	 
	  else if(m1=='1')//||(m4==0))
		{
	   relay0=1;
	   relay1=1;
	   relay2=0;
			//printf("%d\r\n",she_zhi1.set);
		 m1='4';
			
			//m2='4';
   }
		 else if(m1=='2')//||(m4==2))
		{
			  relay0=0;
	   relay1=1;
	   relay2=1;
			///printf("%d\r\n",she_zhi1.set);
			 m1='4';
			//m2='4';
		}
		else if(m1=='3')//||(m4==3))
		{
			relay0=1;
	   relay1=0;
	   relay2=1;
			
			//printf("%d\r\n",she_zhi1.set);
			 
			 m1='4';
		}
		if(m1!='0')
		{
		if(m4==0)
		{
			relay0=1;
	   relay1=1;
	   relay2=0;
			
		}
		else if(m4==1)
		{
			relay0=0;
	    relay1=1;
	    relay2=1;
		}
		else if(m4==2)
		{
			relay0=1;
	   relay1=0;
	   relay2=1;
		}
	
	}
}
	else
	 {
		  relay0=1;
	   relay1=1;
	   relay2=1;
	 }
		
	
	//}
}
void conversion_p(u32 temp_data)  
{  
		u32 temp;
		u8 wei=1;
		u8 i;
		temp = temp_data/1000;

		while((temp/10)>0)
		{
			temp=temp/10;
			wei++;
		}
		temp = temp_data/1000;	
		p_value[0]='P';
		p_value[1]=':';
		p_value[wei+3]='K';
		p_value[wei+4]='p';
		
		for(i=wei+2;i>1;i--)
		{
			if(i==wei)
			{
			  p_value[i]='.';
			}
			else
			{
				p_value[i]=temp%10+0x30;
				temp=temp/10;
			}
		}
    	
}
  
void conversion_ad(float temp_data)
{
	u32 temp;
	 u8 wei=1;
	 u8 i;
	
 	 temp=(int)(temp_data*100);
	 while((temp/10)>0)
	 {
		 temp=temp/10;
		 wei++;
	 } 
	 ad_value[0]='P';
	 ad_value[1]='M';
	 ad_value[2]=':';
	 
	 if(wei==2)
	 {
		 wei=wei+1;
	 }
	 if(wei==1)
	 {
		 wei=wei+2;
	 }
	 ad_value[wei+3]='V';
	 temp=(int)(temp_data*100);
	 for(i=wei+3;i>2;i--)
	 {
		 if(i==wei+1)
		 {
			 ad_value[i]='.';
		 }
		 else
		 {
			 ad_value[i]=temp%10+0x30;
			 temp=temp/10;
		 }
	 }
}
void conversion_va(u32 temp_data1,u32 temp_data2,u32 temp_data3)
{
	u32 temp1,temp2,temp3;
	 u8 wei1=1,wei2=1,wei3=1;
	 u8 i;
	
 	 temp1=temp_data1;
	 temp2=temp_data2/10;
	 bu_liu=temp_data2%10;
	 temp3=temp_data3;
	 while((temp1/10)>0)
	 {
		 temp1=temp1/10;
		 wei1++;
	 }
   while((temp2/10)>0)
	 {
		 temp2=temp2/10;
		 wei2++;
	 } 
    while((temp3/10)>0)
	 {
		 temp3=temp3/10;
		 wei3++;
	 } 
   temp1=temp_data1;
	 
	 temp2=temp_data2/10;
	 
	 temp3=temp_data3;	 
//////////////////////////////////	 
	 dis_VolA[0]='U';
	 dis_VolA[1]=':';
	 dis_VolA[wei1+2]='V';
	 dis_VolA[wei1+3]='\0';
	  for(i=wei1+1;i>1;i--)
	 {
		 dis_VolA[i]=temp1%10+0x30;
			 temp1=temp1/10;
	 }
///////////////////////////////////////
	 
	
	 if(wei2==2)
	 {
		 wei2=wei2+1;
	 }
	 if(wei2==1)
	 {
		 wei2=wei2+2;
	 }
	  dis_CurrentA[0]='I';
	  dis_CurrentA[1]=':';
	  dis_CurrentA[wei2+3]='A';
	  dis_CurrentA[wei2+4]='\0';
	  for(i=wei2+2;i>1;i--)
	 {
		 if(i==wei2)
		 {
			 dis_CurrentA[i]='.';
		 }
		 else 
		 {
		   dis_CurrentA[i]=temp2%10+0x30;
			 temp2=temp2/10;
		 }
	 }
/////////////////////////////////////
	  dis_PowerA[0]='P';
	 dis_PowerA[1]=':';
	 dis_PowerA[wei3+2]='W';
	 dis_PowerA[wei3+3]='\0';
	  for(i=wei3+1;i>1;i--)
	 {
		 dis_PowerA[i]=temp3%10+0x30;
			 temp3=temp3/10;
	 }
}

void conversion_t(float temp_data)
{
	 u32 temp;
	 u8 wei=1;
	 u8 i;
	
 	 temp=(int)(temp_data*100);
	 while((temp/10)>0)
	 {
		 temp=temp/10;
		 wei++;
	 } 
	 t_value[0]='T';
	 t_value[1]=':';
	  t_value[wei+3]=' ';
	 t_value[wei+4]=0XA1;
	  t_value[wei+5]=0xE6;
	 temp=(int)(temp_data*100);
	 for(i=wei+2;i>1;i--)
	 {
		 if(i==wei)
		 {
			 t_value[i]='.';
		 }
		 else
		 {
			 t_value[i]=temp%10+0x30;
			 temp=temp/10;
		 }
	 }
	  
}
void conversion_su(u16 su)
{
	 u16 temp;
		u8 wei=1;
		u8 i;
		temp = su;

		while((temp/10)>0)
		{
			temp=temp/10;
			wei++;
		}
		
		temp = su;
		
		for(i=wei+1;i>1;i--)
		{
			
				su_value[i]=temp%10+0x30;
				temp=temp/10;
			
		}
		temp = su;
		if(temp==0)
		{
			su_value[2]='0';
			su_value[3]='0';
		}
}
void du_wendu()
{

			MAX6675_CSL();
			c = MAX6675_ReadByte();
			i = c;
			i = i<<8;
			c = MAX6675_ReadByte();
			MAX6675_CSH();

			i = i|((unsigned int)c);			//i是读出来的原始数据
			flag = i&0x04;						//flag保存了热电偶的连接状态
			t = i<<1;
			t = t>>4;
			temprature = t*0.25;
			du_wen_flag=0;
	  if(i!=0)							//max6675有数据返回
		{
			if(flag==0)						//热电偶已连接
			{
				//printf("原始数据是：%04X,  当前温度是：%4.2f 'C\r\n",i,temprature);
				//printf("                \r\n");
				conversion_t(temprature);
				LCD_ShowString(2,0,t_value);
			}	
//			else							//热电偶掉线
//			{
//				//printf("未检测到热电偶，请检查。\r\n");
//			}
//		
		}
//		else								//max6675没有数据返回
//		{
//			//printf("max6675没有数据返回，请检查max6675连接。\r\n");
//		}
		//for(i=0;i<0x2fffff;i++);			//max6675的转换时间是0.2秒左右，所以两次转换间隔不要太近
}
void xian_qiya()
{
	  
		BMP180_getdat();
	  pree_tmp=(double)bmp180_press/100000;
	  temp_tmp=(double)bmp180_temp/1000; 
		delay_ms(500);	
		conversion_p(bmp180_press);
	 shu_chu1();
	  LCD_ShowString(3,0,p_value);
}
void xian_ad()
{
	  
		dian_ya=(float)adcx*(3.3/4096);
	  if(dian_ya<0.08)
	  du = 0;
		else
			du=((dian_ya-0.08)/0.2);
//		if(du>6)
//			du=0.05;
    conversion_ad(du);
	  LCD_ShowString(1,4,ad_value);
}

//void copystr(u8 *p1,char *p2,int m,int m2)
//{
//	int n;
//	n = 0;
//	while(n < m - 1)
//	{
//		n++;
//		p1++;
//	}
//	while((*p1 != '\0')&&(n<=m2))
//	{
//		*p2 = *p1;
//		p1++;
//		p2++;
//		n++;
//	}
//	*p2 = '\0';
//}
//void she_chu()
//{
//	
//	she_zhi1.set=buf2[0]-'0';
//	she_zhi1.qi_ya=((buf2[1]-'0')*100+(buf2[2]-'0')*10+(buf2[3]-'0')+(double )(buf2[5]-'0')*0.1+(double )(buf2[6]-'0')*0.01);
//	she_zhi1.qi_ya=((int)(she_zhi1.qi_ya=she_zhi1.qi_ya*100+0.5))/100.0;
//	she_zhi1.wen_du=((buf2[7]-'0')*100+(buf2[8]-'0')*10+(buf2[9]-'0'));
//	she_zhi1.nong_du=((buf2[10]-'0')*10+(buf2[12]-'0'));
//	she_zhi1.dian_ya=((buf2[13]-'0')*100+(buf2[14]-'0')*10+(buf2[15]-'0'));
//	she_zhi1.dian_liu=((buf2[16]-'0')*100+(buf2[17]-'0')*10+(buf2[18]-'0'));
//	she_zhi1.gong_lv=((buf2[21]-'0')*10+(buf2[22]-'0'));
// }

