#ifndef __SYS_PARA_H
#define __SYS_PARA_H

#include "stdint.h"

	#ifdef SYS_PARA_STRUCT_GLOBAL
		#define EXTERN_SYS_PARA
	#else
		#define EXTERN_SYS_PARA extern
	#endif
	
	
	#define VALID_VAL_DWORD_AA		0xAAAAAAAA
	#define INVALID_VAL_DWORD_55	0x55555555
	#define VALID_VAL_AA					0xAA
	#define VALID_VAL_BB					0xBB
	#define INVALID_VAL_55				0x55	
	#define VALID_VAL_2A					0x2A
	#define INVALID_VAL_FF				0xFF
		

	#define LSNAL_PAGE_SIZE 		0x200///盲区数据512字节/页
	typedef struct 
	{
		char sms_city_center_num[LEN_32];///SIM卡所在城市短信中心
		char second_sms_center_num[LEN_32];///部门内部短信中心2
		char third_sms_cetner_num[LEN_32];///公司默认短信中心3
		char software_version[LEN_32];///TIZA_开头，车机电压24V，版本号1.0,年月日
	}SYS_CONST_PARA_STRUCT;///系统启动参数

	typedef struct
	{
		uint32 program_update_flag;///程序更新标志
		uint32 program_total_size;///字节总数
		uint32 sys_para_init_flag;///参数重新初始化标志
	}SYS_BOOT_PARA_STRUCT;///系统启动参数
	
	
	typedef struct
	{
		uint16 sys_time3_200ms_count;						///系统毫秒计数器
		
		uint8  sys_upheart_count;								///系统上传心跳计时
	}SYS_MISC_RUN_STRUCT;///系统运行参数

	
	EXTERN_SYS_PARA SYS_MISC_RUN_STRUCT 		g_sysmiscrun_struct;
	

	
	
	// void SysCommParaInit(void);
	// uint8 SysSerialRead(void);
	// uint8 SysSerialWrite(void);
	// void SysPrivateParaInit(void);
	// void SysWorkParaInit(void);
	// void SysBootParaRead(void);
	// uint8 SysBootParaWrite(void);
	// uint8 SysPrivateParaRead(void);
	// uint8 SysPrivateParaWrite(void);
	// uint8 SysWorkParaRead(void);
	// uint8 SysWorkParaWrite(void);
	// void SysReset(void);
	// void TermReset(void);
	// void TermReset2(void);
	// void TermRingReset(void);
	// void SysKeyDataSave(void);
	// void SysVaryInit(void);
	// void SysParaRead(void);
	// uint16 ParaCorrespond(uint16 cmd);
	// void LockRecord(uint8 lock_method,uint8 level,uint8 reason);
	// void UnLockRecord(void);
	// uint16 QuerySysPrivatePara(uint8 dst[],uint8 src[],uint16 len,uint8 is_local_flag);
	// uint8 SetSysPrivatePara(uint8 data[],uint16 len,uint8 is_local_flag);
	
	// uint8 GetSmsAlarmCenterNum(uint8 data[]);
	// uint8 GetApn(uint8 data[]);
	// uint8 GetMasterIp(uint8 data[]);
	// uint8 GetSlaverIp(uint8 data[]);
	// uint8 GetPort(uint8 data[]);
	// uint8 GetTerminalId(uint8 data[]);
	// uint8 GetTerminalPassword(uint8 data[]);
	// uint8 GetTerminalSerialNum(uint8 data[]);
	// uint8 GetHardwareVersion(uint8 data[]);
	// uint8 GetSleepTimer(uint8 data[]);
	// uint8 GetAccAtatistic(uint8 data[]);
	// uint8 GetMeterErrTimer(uint8 data[]);
	// uint8 GetOverSpeed(uint8 data[]);
	// uint8 GetLowVoltageAlarm(uint8 data[]);
	// uint8 GetBindImsi(uint8 data[]);
	
	// uint8 SetSmsAlarmCenterNum(uint8 data[],uint8 len);
	// uint8 SetApn(uint8 data[],uint8 len);
	// uint8 SetMasterIp(uint8 data[],uint8 len);
	// uint8 SetSlaverIp(uint8 data[],uint8 len);
	// uint8 SetPort(uint8 data[],uint8 len);
	// uint8 SetTerminalId(uint8 data[],uint8 len);
	// uint8 SetTerminalPassword(uint8 data[],uint8 len);
	// uint8 ParaRecover(uint8 data[],uint8 len);
	// uint8 SetTerminalSerialNum(uint8 data[],uint8 len);
	// uint8 SetHardwareVersion(uint8 data[],uint8 len);
	// uint8 SetSleepTimer(uint8 data[],uint8 len);
	// uint8 SetAccStatistic(uint8 data[],uint8 len);
	// uint8 SetMeterErrTimer(uint8 data[],uint8 len);
	// uint8 SetOverSpeed(uint8 data[],uint8 len);
	// uint8 SetLowVoltageAlarm(uint8 data[],uint8 len);
	// uint8 SetBindImsi(uint8 data[],uint8 len);
	// uint8 SetWorkPara(uint8 data[],uint8 len);

#endif
