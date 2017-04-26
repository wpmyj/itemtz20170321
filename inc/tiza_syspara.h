#ifndef __SYS_PARA_H
#define __SYS_PARA_H

#include "stdint.h"

	#ifdef SYS_PARA_STRUCT_GLOBAL
		#define EXTERN_SYS_PARA
	#else
		#define EXTERN_SYS_PARA extern
	#endif
	
	
		
	#define SYSM_CANRX_TIMEOUT   60		//60个计数(1s调用就60s)	
	typedef struct{//系统程序块超时计时
		uint16 canrx_count;
	}SYSM_TIMEOUT_STRUCT;
	
	#define SYSM_ON							1
	#define SYSM_OFF						0
	typedef struct{//系统程序块是否启用 开关
		uint16 canrx_switch;						//接收中断
		uint16 GPRSPacketTx_switch;						//GPRS无需应答式发送 或 打包给周期发送
		uint16 GPRSPeriodTx_switch;						//GPRS需要应答式发送
	}SYSM_ON_OFF_STRUCT;
	
	EXTERN_SYS_PARA SYSM_TIMEOUT_STRUCT sysm_timeout_struct;
	EXTERN_SYS_PARA SYSM_ON_OFF_STRUCT 	sysm_on_off_struct;
	

	


#endif
