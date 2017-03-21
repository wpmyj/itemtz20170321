#ifndef __SYS_TASK_H
#define __SYS_TASK_H

	#ifdef SYS_TASK_STRUCT_GLOBAL
		#define EXTERN_SYS_TASK
	#else
		#define EXTERN_SYS_TASK extern
	#endif

	
			
	void SysTaskMain(void);
	void TermEnterSleep(void);
	void DayChangeMonitor(uint8 date_time[]);
	void AccOnOffStatisticDataCheck(uint8 date[]);
	void AccOnOffStatisticDataSave(uint16 len);
	void AccOnOffDataInit(uint8 date[]);
	void AccOnOffDataAppend(uint8 time[],uint8 is_acc_on_falg);
	void SysNoTaskDelay(uint8 delay_sec);
	void SysDelay(uint8 delay_sec);
	
#endif
