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
	
	#define SYS_SAVEALARM_NUMB					3				//ϵͳδ���ֱ���ʱ ����INTFLAH�ļ�¼����  
	#define SYS_HAVEALARM_COUNT					40			//���ֱ�����󵹼�ʱʱ�� s
	typedef struct
	{
		uint32 headindex;							  //����(ÿ��16�Σ�ÿ��4Kbety)
		uint32 tailindex;								//����(ÿ��16�Σ�ÿ��4Kbety)
	}SYS_ALARM_STRUCT;
		
			
	typedef struct
	{
		uint16 time3_200ms_count;						///ϵͳ���������
		
		uint16 SavePeri_count;							///���ڱ����ʱ
		uint16 NorUpPeri_count;							///���������ϴ���Ϣ��ʱ
		uint16 AlrUpPeri_count;							///�����ϴ������ʱ
		uint16 TAckTim_count;								///�ն�Ӧ��ʱʱ��					FF��Ч		
		uint16 PAckTim_count;								///ƽ̨Ӧ��ʱʱ��					FF��Ч
		uint16 NLogTim_count;								///���ε�¼ʧ�ܺ����µ�¼ʱ����	
		uint8  upheart_count;								///ϵͳ�ϴ�������ʱ
			
		uint8  need_check_time_flag;						///��ҪУ��ϵͳ����ʱ��     01��GPS   02��ƽ̨  00��Ч
		uint8  have_sysAlarm_flag;							///ϵͳ���ֱ���             01�б���	02������ޱ���ä�� 03�ޱ���ä��	00�ޱ���
		uint8  save_sysAlarm_numb;							///ϵͳδ���ֱ���ʱ ����INTFLAH�ļ�¼����  
//		uint16 happend_Alarm_head;							///ϵͳ���ֱ���ʱ��¼ ���浽INTFLAH��headindex   FF��Ч
//		uint16 happend_Alarm_tail;							///ϵͳ����ʱ��ʱ��¼ ���浽INTFLAH��headindex   FF��Ч
		uint8  have_sysAlarm_count;							///ϵͳ���ֱ�������ʱ    

		uint8  main_power_statu;						///����Դ״̬     01�ϵ�1��  02�ϵ�  03���ϵ�
		uint8  assist_gps_flag;							///������λ��־		02��ʼ     00֮ǰ��λ��Ч   01֮ǰ��λ��Ч
		
		uint8  ProgramUpgrade_flag;					///�̼�������־		00��ʼ		 01ִ�й̼�����   02дBOOT��־  03
	}SYS_MISC_RUN_STRUCT;///ϵͳ���в���

	
	EXTERN_SYS_TASK SYS_ALARM_STRUCT 				g_sysalarm_struct;
	EXTERN_SYS_TASK SYS_MISC_RUN_STRUCT 		g_sysmiscrun_struct;		
			
			
			
	EXTERN_SYS_TASK void SetPara2FactoryReset(void);
	EXTERN_SYS_TASK void ProWrite_SysPara(void);
	EXTERN_SYS_TASK void SetGPRSNetPara(void);
	EXTERN_SYS_TASK void ProPutIntoAlarm(uint8 data[], uint16 len, uint8 cmd);
	
#endif
