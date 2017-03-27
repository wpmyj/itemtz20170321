#ifndef __TIZA_L218_H
#define __TIZA_L218_H
	
#include <stdint.h>

		#ifdef L218_GLOBAL
			#define L218_EXTERN
		#else
			#define L218_EXTERN extern
		#endif
				
		#define WAIT_100MS 100	///延时
		#define SEND_1T 1				///发送1次
				
		#define EXE_NO	 	0X00 ///未执行
		#define EXE_OK	 	0X01
		#define EXE_FAIL 	0X02
		#define EXE_GPS_ON 		0X03
		#define EXE_GPS_OFF 	0X04
				
		#define L218_MAX_BUF_LEN 		GPRS_UART_BUF_LEN
		
		typedef struct
		{///AT命令结构体
			uint8 *cmd_text;																/// 指令内容
			uint16 max_ms_wait;															///最长等待n个100毫秒
			uint8 tx_max_count;															///最大发送次数
			uint8 exe_flag;																	///执行结果
			void (*fun)(uint8 *data,uint16 len,uint8 flag);	///函数处理
		}AT_CMD_STRUCT;

		typedef enum 
		{///指令序列
			AT_INDEX = 0, 
			AT_E0_INDEX,
			AT_IPR_INDEX,
			AT_CIMI_INDEX,
			AT_CNUM_INDEX,
			AT_CPBS_ON_INDEX,
			AT_CPBW_INDEX,
			AT_IMEI_INDEX,
			AT_CSQ_INDEX,
			AT_CREG_INDEX,
			AT_CGATT_INDEX,
			AT_CMGF_INDEX,
			AT_CMGL_INDEX,
			AT_CMGS_INDEX,
			AT_CMGD_INDEX,
			AT_ATH_INDEX,
			AT_SLEEP_INDEX,
		//	AT_WAKE_INDEX,
			AT_NULL_INDEX,
			
			AT_CIPMUX_INDEX,
			AT_CSTT_INDEX,
			AT_NETOPEN_INDEX,
			AT_NETCLOSE_INDEX,
			AT_CIFSR_INDEX,
			AT_CIPOPEN_TCP_INDEX,
			AT_CIPSEND_TCP_INDEX,
			AT_CIPCLOSE_INDEX,
			AT_CIPRXGET_INDEX,
			AT_CIPSRIP_INDEX,
			
			AT_GETGPS_INDEX,
			AT_EGPSC_INDEX,
			AT_PMTK314_INDEX,

      AT_CPIN_INDEX,
      AT_SAPBR_INDEX,
			
		}AT_CMD_INDEX;
	///AT指令处理函数---start
		void AtFun(uint8 *data,uint16 len,uint8 flag);
		void AtE0Fun(uint8 *data,uint16 len,uint8 flag);
		void AtIprFun(uint8 *data,uint16 len,uint8 flag);
		void AtCimiFun(uint8 *data,uint16 len,uint8 flag);
		void AtCnumFun(uint8 *data,uint16 len,uint8 flag);
		void AtCpbsOnFun(uint8 *data,uint16 len,uint8 flag);
		void AtCpbwFun(uint8 *data,uint16 len,uint8 flag);
		void AtImeiFun(uint8 *data,uint16 len,uint8 flag);
		void AtCsqFun(uint8 *data,uint16 len,uint8 flag);
		void AtCregFun(uint8 *data,uint16 len,uint8 flag);
		void AtCgattFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmgfFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmglFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmgsFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmgdFun(uint8 *data,uint16 len,uint8 flag);
		void AtAthFun(uint8 *data,uint16 len,uint8 flag);
		void AtSleepFun(uint8 *data,uint16 len,uint8 flag);
//		void AtWakeFun(uint8 *data,uint16 len,uint8 flag);
		void AtNullFun(uint8 *data,uint16 len,uint8 flag);      
		// 网络服务相关
    void AtCipmuxFun(uint8 *data,uint16 len,uint8 flag);
		void AtCsttFun(uint8 *data,uint16 len,uint8 flag);
		void AtNetOpenFun(uint8 *data,uint16 len,uint8 flag);
		void AtNetCloseFun(uint8 *data,uint16 len,uint8 flag);
		void AtCifsrFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipOpenTcpFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipSendTcpFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipCloseFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipRxGetFun(uint8 *data,uint16 len,uint8 flag);
		void AtCipSripFun(uint8 *data,uint16 len,uint8 flag);
		
		void AtGetgpsFun(uint8 *data,uint16 len,uint8 flag);
		void AtEgpscFun(uint8 *data,uint16 len,uint8 flag);
		void AtPmtk314Fun(uint8 *data,uint16 len,uint8 flag);
		void AtCPINFun(uint8 *data,uint16 len,uint8 flag);
		void AtSAPBRFun(uint8 *data,uint16 len,uint8 flag);
		
	///AT指令处理函数---end
//		void L218SendAtCmd(uint8 cmd_index,uint8 app_data[],uint8 app_len,uint8 mat_data[],uint8 mat_len);
		uint16 L218UartIsRxDone(uint8 data[],uint16 len);
		
		void L218PowerInit(void);
		void L218Reset(void);
		

//=============================================================GPRS=================GPRS=//
	#ifdef GPRS_GLOBAL
		#define GPRS_EXTERN
	#else
		#define GPRS_EXTERN extern
	#endif
	
//	#define TERMINAL_ID_LEN				5			///终端ID长度
	#define SIM_CARD_IMSI_LEN			15		///SIM卡IMSI长度
	#define GSM_IMEI_LEN					15		
//	#define GPRS_SEND_BUF_LEN 	512 		///<1460

	typedef struct
	{///3G模块控制结构体
		uint8   business;  	    //业务
		uint8   ope;    		    //服务
	}GPRS_CTR_STRUCT;

	typedef struct
	{///3G模块数据结构体
		uint8 initStatus;       //初始化状态
		uint8 netLinkStatus;    //网络连接状态
		uint8 MserLinkStatus;		//主IP连接状态
		uint8 sendDataStatus;   //数据发送状态
		uint8 GpsOpenStatus;   	//GPS打开状态
		
		uint8  IP[4];						//IP地址
		uint16 Port;						//端口
		uint8  csq_val;
		uint8  imei[SIM_CARD_IMSI_LEN];	
		uint8* SendData;												//要发送的数据
		uint16 SendDataLen; 	 									//要发送的数据的长度
	}GPRS_DATA_STRUCT;

//typedef enum
//{
//	TYPE_TCP,
//	TYPE_UDP
//}SOCKET_TYPE;
//{///GPRS参数结构体
//	SOCKET_TYPE net_type;   // 连接类型
//	char *ip;           		// IP地址
//	uint16 port;            // 端口
//}SERVER_ADDR;

///状态枚举
typedef enum
{
  GPRS_NULL = 0,
  GPRS_INIT,
  GPRS_CONNECT_NET,
  GPRS_CONNECT_SERVER,
  GPRS_DISCONNECT_SERVER,
	GPRS_DISCONNECT_NET,
	GPRS_SEND_DATA,
  GPRS_RESET,
	
	GPS_OPEN_NET,
	GPS_CLOSE_NET,
}GPRS_BUSINESS;

//3G模块初始化状态
typedef enum{
	GPRS_INIT_OK = 0,
	GPRS_INIT_NO,
	GPRS_INIT_ING,
}GPRS_INIT_STATUS;
//3G模块网络连接状态
typedef enum{
	GPRS_NETLINK_OK = 0,
	GPRS_NETLINK_NO,
	GPRS_NETLINK_ING,
	GPRS_NETLINK_ERR,		//关闭失败
}GPRS_NETLINK_STATUS;
//服务器连接状态
typedef enum{
	GPRS_SERLINK_OK = 0,
	GPRS_SERLINK_NO,
	GPRS_SERLINK_ING,
	GPRS_SERLINK_ERR,		//关闭失败
}GPRS_SERLINK_STATUS;
//数据发送状态
typedef enum{
	GPRS_SENDDATA_ZERO = 0,	//初始状态
	GPRS_SENDDATA_BUSY,			//忙,正在发送
	GPRS_SENDDATA_IDLE,    	//成功就空闲
	GPRS_SENDDATA_OUT,      //要发送
	GPRS_SENDDATA_ERR,     
}GPRS_SENDDATA_STATUS;

//GPS模块打开状态
typedef enum{
	GPS_OPEN_OK = 0,
	GPS_OPEN_NO,
	GPS_OPEN_ING,
	GPS_OPEN_ERR,		//关闭失败
}GPS_OPEN_STATUS;

///变量
GPRS_EXTERN GPRS_CTR_STRUCT g_gprs_ctr_struct;
GPRS_EXTERN GPRS_DATA_STRUCT g_gprs_data_struct;

GPRS_EXTERN void ModlueCalledProcess(void);

//=============================================================GPS===================GPS=//
	#ifdef GPS_GLOBAL
		#define GPS_EXTERN
	#else
		#define GPS_EXTERN extern
	#endif
	
	#define RMC_TYPE 		1
	#define GGA_TYPE 		2
	
	#define LAT_TYPE 		0
	#define LONG_TYPE 	1
	#define SPEED_TYPE 	2
	#define DIR_TYPE 		3
	#define AMP_TYPE 		4
	
 	#define LAT_INDEX							0			///纬度地址，百分秒，4
	#define LONG_INDEX						4			///经度地址，百分秒，4
	#define SPEED_INDEX						8			///GPS速度1km/h 1
	#define DIR_INDEX							9			///方向1
	#define AMP_INDEX							10			///海拔
	#define STATUS_INDEX					12			///状态4
	#define DATE_INDEX						16			///日期起始地址
	#define TIME_INDEX						19			///时间起始地址
	#define GPS_INFO_LEN					(TIME_INDEX+3)///上行协议GPS信息长度
	#define SAT_INDEX						GPS_INFO_LEN  ///卫星数量

	#define GPS_INFORM_LEN			71	 		///GPS截取信息长度
	
	typedef union 
	{///GPS截取信息
		uint8 array[GPS_INFORM_LEN];
		struct 
		{
			uint32 latitude;
			uint32 longitude;
			uint16 high;
			uint8  speedk;
			uint8  direction;
			uint8  year;
			uint8  month;
			uint8  day;
			uint8  hour;
			uint8  minute;
			uint8  second;
			uint8  starnum;
			union08 statu;			//B0=0定位有效 B1=0北纬 B2=0东经
			
		}subitem;
	}GPS_INFORM_UNION;
	
	typedef struct
	{
		GPS_INFORM_UNION gpsinform;
	}GPS_STRUCT;

GPS_EXTERN 	GPS_STRUCT g_gps_struct;





//#define L218_DEBUG_qlj


GPS_EXTERN 	const uint8 RECV_IPD_ACK[];       		// RECV DATA LEN, ADD "+IPD" HEADER
//const uint8 RECV_FROM_ACK[] = "RECV FROM:";       // RECV FORM: STRING IF SET SHOW "RECV FROM" HEADER
GPS_EXTERN 	const uint8 OK_ACK[];
GPS_EXTERN 	const uint8 CRLF_ACK[];
//const uint8 CIPOPEN_EXTRA_OK[]	= "CONNECT OK";		// IP IS OPENED SUCCESS
GPS_EXTERN 	const uint8 CIPSEND_EXTRA_OK[];			// SEND SUCCESS
GPS_EXTERN 	const uint8 CIPCLOSE_EXTRA_OK[];			// IP IS CLOSED SUCCESS
GPS_EXTERN 	const uint8 GPRS_HAVE_RX_DATA[];		// ?????

GPS_EXTERN 	const uint8 FTP_TYPEOFINTERCONNECT[];
GPS_EXTERN 	const uint8 FTP_SETAPN[];
GPS_EXTERN 	const uint8 FTP_OPENBEARER[];





GPS_EXTERN 	AT_CMD_STRUCT g_at_cmd_struct[];
		
#endif













