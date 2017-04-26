#ifndef __SYS_PARA_H
#define __SYS_PARA_H

#include "stdint.h"

	#ifdef SYS_PARA_STRUCT_GLOBAL
		#define EXTERN_SYS_PARA
	#else
		#define EXTERN_SYS_PARA extern
	#endif
	
	
		
	#define SYSM_CANRX_TIMEOUT   60		//60������(1s���þ�60s)	
	typedef struct{//ϵͳ����鳬ʱ��ʱ
		uint16 canrx_count;
	}SYSM_TIMEOUT_STRUCT;
	
	#define SYSM_ON							1
	#define SYSM_OFF						0
	typedef struct{//ϵͳ������Ƿ����� ����
		uint16 canrx_switch;						//�����ж�
		uint16 GPRSPacketTx_switch;						//GPRS����Ӧ��ʽ���� �� ��������ڷ���
		uint16 GPRSPeriodTx_switch;						//GPRS��ҪӦ��ʽ����
	}SYSM_ON_OFF_STRUCT;
	
	EXTERN_SYS_PARA SYSM_TIMEOUT_STRUCT sysm_timeout_struct;
	EXTERN_SYS_PARA SYSM_ON_OFF_STRUCT 	sysm_on_off_struct;
	

	


#endif
