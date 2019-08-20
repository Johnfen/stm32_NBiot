#include "bc95.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"


/*
  *驱动主要基于BC95_check_ack，BC95_send_cmd这两个函数，移植这两个函数，自己可以根据需要实现更多功能
  *
  ******************************************************************************
  */



uint8_t check_ack_timeout = 10;
uint8_t ue_exist_flag = 0;
uint8_t ue_need_reboot_flag = 0;

//usmart支持部分 
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;
void sim_at_response(u8 mode)
{

	if(USART1_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART1_RX_BUF);	//发送到串口
		if(mode)USART1_RX_STA=0;
	} 
}
//检查返回的响应是否符合预期
//传入参数为预期返回的字符串
//返回0，为检测到预期值
//其他值，预期字符所在的位置
//usmart支持部分 
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;
u8* BC95_check_ack( char *str)
{
	 char *strx=0;
	if(USART1_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
		USART1_RX_STA=0;
	} 
	
	return (u8*)strx;
}

u8* BC95_check_ack2( char *str)
{
	 char *strx=0;
	if(USART1_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
		USART1_RX_STA=0;
		
	} 
	
	return (u8*)strx;
}







////读取数据，截取接收缓存中所需的数据保存到des,pos为起始地址，len为截取长度
//void get_str_data(char* des,char pos,char len)
//{
// 	memcpy(des,usart2_rcvd_buf+pos,len);
//}	

//创建UDP链接，传入本地UDP端口号，返回0-6的socket id号，
u8 creat_UDP_socket(char* local_port)
{
	//char data[10]="";
	//uint8_t socket_id = 7;
	char temp[64]="AT+NSOCR=DGRAM,17,";
	strcat(temp,local_port);
	strcat(temp,",1");
	if(chuan_flag1==0)
	{
	 printf("%s\r\n",temp);
	 chuan_flag1=1;
	}
	
	return 0;
}
//发送数据函数，传入socket,主机IP，远程主机端口，数据长度，数据
//这里暂时使用字符串参数
//返回值1，发送成功（鉴于UDP为报文传输，数据主机是否接收到模块是无法确认的）
//返回值0，发送失败
u8 send_UDP_msg(char *dataLen,char *data)
{
	char ptr[600]="AT+NMGS=";
	strcat(ptr,dataLen);
	strcat(ptr,",");
	strcat(ptr,data);
	BC95_send_cmd(ptr,"OK");
//	{
//	//USARTx_printf(USART3,"发送数据--> %s！\r\n",ptr);	
//		return 1;
//	}
	return 1;
}

////将1个字符转换为16进制数字
////chr:字符,0~9/A~F/a~F
////返回值:chr对应的16进制数值
//u8 BC95B5_chr2hex(u8 chr)
//{
//	if(chr>='0'&&chr<='9')return chr-'0';
//	if(chr>='A'&&chr<='F')return (chr-'A'+10);
//	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
//	return 0;
//}
////将1个16进制数字转换为字符
////hex:16进制数字,0~15;
////返回值:字符
//u8 BC95B5_hex2chr(u8 hex)
//{
//	if(hex<=9)return hex+'0';
//	if(hex>=10&&hex<=15)return (hex-10+'A'); 
//	return '0';
//}
int hex_to_str2(const char *bufin, int len, u8 *bufout)
{
    int i = 0;
    unsigned char tmp2 = 0x0;
    unsigned int tmp = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout)
    {
        return -1;
    }
    for(i = 0; i < len; i = i+2)
    {
        #if 1
        tmp2 =  bufin[i];
        tmp2 =  tmp2 <= '9'?tmp2-0x30:tmp2-0x37;
        tmp =  bufin[i+1];
        tmp =  tmp <= '9'?tmp-0x30:tmp-0x37;
        bufout[i/2] =(tmp2<<4)|(tmp&0x0F);
        #else
        sscanf(bufin+i, "%02x", &tmp);
        bufout[i/2] = tmp;
        #endif
    }
    return 0; 
}

void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	char ddl,ddh;
	int i;
	
	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}
	pbDest[nLen*2] = '\0';
}
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
    
    for (i = 0; i < sourceLen; i++)  
    {  
        highByte = source[i] >> 4;  
        lowByte = source[i] & 0x0f ;  
  
        highByte += 0x30;  
  
        if (highByte > 0x39)  
                dest[i * 2] = highByte + 0x07;  
        else  
                dest[i * 2] = highByte;  
  
        lowByte += 0x30;  
        if (lowByte > 0x39)  
            dest[i * 2 + 1] = lowByte + 0x07;  
        else  
            dest[i * 2 + 1] = lowByte;  
    }  
    
}



//注网附网成功之后循环发送数据。

	 
  

////////////////////////////////////////////////////////
//////////////////////////////////////////////
/***************************************************************/
//向BC95B5发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 BC95_send_cmd(char  *cmd,char *Re1)
{
	
	
	 u8 res=0;
	if(chuan_flag1==0)
	{
	 printf("%s\r\n",cmd);
	 chuan_flag1=1;
		//chuan_flag2=0;
		res=0;
	}
	else
	{		
		 if(BC95_check_ack(Re1))
			{
				 res=1;
				chuan_flag1=0;
				chuan_flag2=0;
			}
			else if(chuan_flag2==1)
			{
				 chuan_flag1=0;
				 chuan_flag2=0;
				 res=0;
			}
	 }
		
	 return res;
} 
///////////////////////////////////////////////////////
/////////////////////
//NB初始化
/////////////////////
u8  BC95_init(u8 bu)
{
	
  switch (bu)
	{	
		case 0:	
	  if(BC95_send_cmd("AT","OK"))//检测是否应答AT指令
    {			
	    
			 bu=1;
		}
	  break;
   
    case 1:		
	  if(BC95_send_cmd("AT+CMEE=1\r\n","OK"))//err|=1<<1;	
    {
			bu=2;
		  
		}
		break;
		 case 2:		
	  if(BC95_send_cmd("AT+NBAND=5\r\n","OK"))//err|=1<<1;	
    {
			bu=3;
		  
		}
		break;
		case 3:
	  if(BC95_send_cmd("AT+NCONFIG?\r\n","OK"))
		{
			bu=4;
		 
		}
		break;
		case 4:
		if(	BC95_send_cmd("AT+CFUN=1\r\n","OK"))
		{
			bu=5;
			
		}
		break;
		
		case 5:
		if(	BC95_send_cmd("AT+CGATT=1\r\n","OK"))
		{
			bu=6;
		 }
	  break;
	 case 6:
	if(	BC95_send_cmd("AT+CSQ\r\n","+CSQ:99,99"))
		{
			bu=7;
		 }
	else 
		 bu=8;
	  break;
	
		case 8:
	 if	(BC95_send_cmd("AT+NSMI=1\r\n","OK"))
		{
			bu=9;
		  
		}
		break;
		case 9:
	 if(BC95_send_cmd("AT+NNMI=2\r\n","OK"))
		{
			bu=10;
		  
		}
		break;
		case 10:
	 if(BC95_send_cmd("AT+NCDP=139.159.140.34,5683\r\n","OK"))
		{
			bu=11;
		  
		}
		break;
		case 11:
	 if(BC95_send_cmd("AT+CFUN=1\r\n","OK"))
		{
			bu=12;
		  
		}
		case 12:
		if(BC95_send_cmd("AT+NNMI=1\r\n","OK"))
		{
			bu=13;
		  
		}
		default:
		USART1_RX_STA=0;
		break;
 }
		
			return bu;
		
} 

