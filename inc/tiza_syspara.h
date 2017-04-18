#ifndef __SYS_PARA_H
#define __SYS_PARA_H

#include "stdint.h"

	#ifdef SYS_PARA_STRUCT_GLOBAL
		#define EXTERN_SYS_PARA
	#else
		#define EXTERN_SYS_PARA extern
	#endif
	
	

	
	
	

	
	
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
