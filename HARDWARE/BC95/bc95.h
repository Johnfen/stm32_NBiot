#ifndef __BC95_H
#define __BC95_H
#include "sys.h"
 


#define BC95_PWR_ON_TEST
//#undef  BC95_PWR_ON_TEST

#define UE_LOCAL_UDP_PORT        "3005"
#define SERVER_HOST_UDP_IP       "123.57.41.13"
#define SERVER_HOST_UDP_PORT     "5566"

//Ä¬ÈÏÆµ¶Î
#define SET_UE_DEFAULT_BAND      "AT+NBAND=5"         
#define UE_DEFAULT_BAND          "+NBAND:5"


#define SET_UE_REBOOT            "AT+NRB"

#define QUERY_UE_BAND            "AT+NBAND?"
#define SET_UE_BAND_5            "AT+NBAND=5"
#define SET_UE_BAND_8            "AT+NBAND=8"

#define QUERY_UE_CONNECT_MODE    "AT+NCONFIG?"
#define SET_UE_AUTOCONNECT       "AT+NCONFIG=AUTOCONNECT,TRUE"
#define SET_UE_MANUALCONNECT     "AT+NCONFIG=AUTOCONNECT,FALSE"

#define QUERY_UE_FUNC            "AT+CFUN?"
#define SET_UE_FUNC_0            "AT+CFUN=0"
#define SET_UE_FUNC_1            "AT+CFUN=1"

#define QUERY_UE_SIGNAL_QTY      "AT+CSQ"

#define QUERY_UE_ATTACH_STATS    "AT+CGATT?"
#define UE_ATTACHED_STATS        "+CGATT:1"
#define SET_UE_ATTACH            "AT+CGATT=1"

#define QUERY_UE_EREG_STATS      "AT+CEREG?"
#define UE_EREGISTERING_STATS    "+CEREG:0,2"
#define UE_EREGISTERED_STATS     "+CEREG:0,1"
#define SET_UE_EREG              "AT+CEREG=1"

#define QUERY_UE_SCCON_STATS     "AT+CSCON?"
#define SET_UE_SCCON             "AT+CSCON=1"
#define UE_CONNECTED             "+CSCON:0,1" 
typedef struct
{
	char manufacture_id[12];
	char device_module[18];
	char firmware_version[30];
	char frequency_band[10];
} BC95_UE_INFO_typedef;
extern  u8 chuan_flag1;
extern u8 chuan_flag2;

//void BC95_power_on(void);
u8* BC95_check_ack( char *str);
//u8  BC95_send_cmd(char *cmd,char *ack,uint16_t waittime);
//u8 creat_UDP_socket(char* local_port);
//u8  send_UDP_msg(char *socket,char *hostIP,char *port,char *dataLen,char *data);
char* receive_udp(char *socket,char *dataLen);
void get_str_data(char* des,char pos,char len);
//u8 query_net_status(void);
void HexToStr(char *pbDest, char *pbSrc, int nLen);
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
void BC95_Test_Demo(void);
u8 BC95_send_cmd(char  *cmd,char *Re1);
u8  BC95_init(u8 bu);
u8 send_UDP_msg(char *dataLen,char *data);
u8* BC95_check_ack2( char *str);
int hex_to_str2(const char *bufin, int len, u8 *bufout);
#endif

