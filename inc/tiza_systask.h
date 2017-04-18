#ifndef __SYS_TASK_H
#define __SYS_TASK_H

#include "stdint.h"

	#ifdef SYS_TASK_STRUCT_GLOBAL
		#define EXTERN_SYS_TASK
	#else
		#define EXTERN_SYS_TASK extern
	#endif

	
	#define SYS_ALARM_MAXINDEX						48
	#define ALARM_PAGE_SIZE 							0x200						//512byte
	#define SYS_ALARM_START_ADDR				0x20000	//0x08020000			//64PAGE
	#define SYS_ALARM_END_ADDR					0x267FF	//0X080263FF
	#define SYS_ALARM_INDEXPAGE					0x26800	//0X08026800			//77PAGE
	#define SYS_SAVEPARA_INDEXPAGE			0x27000	//0X08027000			//78PAGE
	#define SYS_SAVEVIN_INDEXPAGE				0x27800	//0X08027800			//79PAGE   
	
	#define SYS_SAVEALARM_NUMB					3				//系统未出现报警时 保留INTFLAH的记录条数  
	#define SYS_HAVEALARM_COUNT					40			//出现报警最大倒计时时间 s
	typedef struct
	{
		uint32 headindex;							  //包号(每块16段，每段4Kbety)
		uint32 tailindex;								//包号(每块16段，每段4Kbety)
	}SYS_ALARM_STRUCT;
		
			
	typedef struct
	{
		uint16 time3_200ms_count;						///系统毫秒计数器
		
		uint16 SavePeri_count;							///周期保存计时
		uint16 NorUpPeri_count;							///正常周期上传信息计时
		uint16 AlrUpPeri_count;							///报警上传间隔计时
		uint16 TAckTim_count;								///终端应答超时时间					FF无效		
		uint16 PAckTim_count;								///平台应答超时时间					FF无效
		uint16 NLogTim_count;								///三次登录失败后重新登录时间间隔	
		uint8  upheart_count;								///系统上传心跳计时
			
		uint8  need_check_time_flag;						///需要校验系统日期时间     01从GPS   02从平台  00无效
		uint8  have_sysAlarm_flag;							///系统出现报警             01有报警	02检测有无报警盲区 03无报警盲区	00无报警
		uint8  save_sysAlarm_numb;							///系统未出现报警时 保留INTFLAH的记录条数  
//		uint16 happend_Alarm_head;							///系统出现报警时记录 保存到INTFLAH的headindex   FF无效
//		uint16 happend_Alarm_tail;							///系统报警时超时记录 保存到INTFLAH的headindex   FF无效
		uint8  have_sysAlarm_count;							///系统出现报警倒计时    

		uint8  main_power_statu;						///主电源状态     01上电1次  02断电  03再上电
		uint8  assist_gps_flag;							///辅助定位标志		02初始     00之前定位有效   01之前定位无效
		
		uint8  ProgramUpgrade_flag;					///固件升级标志		00初始		 01执行固件下载   02写BOOT标志  03
	}SYS_MISC_RUN_STRUCT;///系统运行参数

	
	EXTERN_SYS_TASK SYS_ALARM_STRUCT 				g_sysalarm_struct;
	EXTERN_SYS_TASK SYS_MISC_RUN_STRUCT 		g_sysmiscrun_struct;		
			
			
			
	EXTERN_SYS_TASK void SetPara2FactoryReset(void);
	EXTERN_SYS_TASK void ProWrite_SysPara(void);
	EXTERN_SYS_TASK void SetGPRSNetPara(void);
	EXTERN_SYS_TASK void ProPutIntoAlarm(uint8 data[], uint16 len, uint8 cmd);
	
#endif
