#ifndef __GPRS_H
#define __GPRS_H
	
		#ifdef GPRS_GLOBAL
			#define GPRS_EXTERN
		#else
			#define GPRS_EXTERN extern
		#endif
	
		#define WAIT_1S 1	///��ʱ1��
		#define SEND_1T 1	///����1��
		
		
		
		#define AT_NOT_EXE 		0X00
		#define AT_EXE_SUCESS 	0X01
		#define AT_EXE_FAIL 	0X02
		
		
		enum GSM_CUR_MODE
		{
			POWER_INIT_MODE = 0, 
			AT_INIT_MODE,
		///	GPRS_DATA_MODE,
			PPP_MODE,
			UDP_MODE,
			SLEEP_MODE,
		};
		
		enum AT_CMD_INDEX
		{
			AT_INDEX = 0, 
			AT_E0_INDEX,
			AT_IPR_INDEX,
			AT_CIMI_INDEX,
			AT_CPBS_ON_INDEX,
			AT_CPBW_INDEX,
			AT_IMEI_INDEX,
			AT_CSQ_INDEX,
			AT_CREG_INDEX,
			AT_CNUM_INDEX,
			AT_CGATT_INDEX,
			AT_CGATT_1_INDEX,
			AT_CGATT_0_INDEX,
			AT_CGDCONT_INDEX,
			AT_ATD_INDEX,
			AT_CMD_MODE_INDEX,
			AT_GPRS_DATA_MODE_INDEX,
			AT_CMGF_INDEX,
			AT_CMGL_INDEX,
			AT_CMGS_INDEX,
			AT_CMGD_INDEX,
			AT_ATH_INDEX,
			AT_SLEEP_INDEX,
			AT_WAKE_INDEX,
			AT_NULL_INDEX
		};
		
		typedef struct
		{
			uint8 *cmd_text;/// ָ������
			uint8 sec_wait;///ִ�и�������ȴ�����
			uint8 tx_max_count;///����ʹ���
		}AT_CMD_STRUCT;///AT����ṹ��
		
		#define MAX_STATE_SIZE (AT_NULL_INDEX+1)///���״̬��
		
		typedef struct
		{
			enum GSM_CUR_MODE cur_mode;///��ǰ������ģʽ
			uint8 gsm_module_reset_counter;///GSMģ����������
			uint8 gsm_mode_exe_flag[4];///������ģʽִ��״̬,����,AT,PPP,GPRSͨѶ
			uint8 gsm_state_array[MAX_STATE_SIZE];
			uint8 ring_low_counter;///ring�ŵͼ���
			uint8 ring_low_sec_counter;///ring�ŵ������
			uint8 gsm_rx_ring_flag;///�绰�����־
			uint8 sleep_rx_ring_falg;///˯��ʱ�յ��绰��־
			uint8 gsm_rx_sms_flag;///���ն��ű�־
			
			uint8 gsm_ring_low_flag;///RING�ܽŵ͵�ƽ��־
			uint16 gsm_ring_low_ms_counter;///RING�ܽŵ͵�ƽ�������
			
			uint8 first_deal_sms_flag;///�ϵ��1��ִ�ж���
			uint8 first_deal_sms_sec_counter;///�ϵ��1��ִ�ж������ʱ
			
			uint8 ppp_monitor_falg;///PPP�κż��
			uint8 ppp_sec_counter;///PPP�κ����ʱ
		}GSM_MISC_STRUCT;///GSM���ݽṹ��
		
		GPRS_EXTERN GSM_MISC_STRUCT gsm_misc_struct;
		typedef void (*AT_CMD_PROCESS_FUN_PTR)(uint8 *data,uint16 len,uint8 flag);
		
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
		void AtCgatt1Fun(uint8 *data,uint16 len,uint8 flag);
		void AtCgatt0Fun(uint8 *data,uint16 len,uint8 flag);
		void AtCgdcontFun(uint8 *data,uint16 len,uint8 flag);
		void AtAdtFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmdModeFun(uint8 *data,uint16 len,uint8 flag);
		void AtGprsDataModeFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmgfFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmglFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmgsFun(uint8 *data,uint16 len,uint8 flag);
		void AtCmgdFun(uint8 *data,uint16 len,uint8 flag);
		void AtAthFun(uint8 *data,uint16 len,uint8 flag);
		void AtSleepFun(uint8 *data,uint16 len,uint8 flag);
		void AtWakeFun(uint8 *data,uint16 len,uint8 flag);
		void AtNullFun(uint8 *data,uint16 len,uint8 flag);
		
		uint8 GprsAutoBpr(void);
		void GprsExceptMain(uint16 delay_sec);
		void GprsDelay(uint16 delay_sec,uint8 wait_ack_flag);
		
		uint8 GprsSwitchAtMode(uint8 cur_mode);
		uint8 GprsSwitchGprsDataMode(uint8 cur_mode);
		
		void GprsSendAtCmd(uint8 cmd_index,uint8 app_data[],uint8 app_len,uint8 mat_data[],uint8 mat_len);
		void GprsPowerInit(void);
		void GprsAtInit(void);
		void GprsPppInit(void);
		void GprsParaInit(void);
		void GsmVaryInit(void);
		void GsmTelSmsProcess(void);
		void GsmDisLcpAndIntoSleep(void);
		void GprsMain(void);
#endif
