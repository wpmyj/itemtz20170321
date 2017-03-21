#ifndef __GPRS_H
#define __GPRS_H
	
		#ifdef GPRS_GLOBAL
			#define GPRS_EXTERN
		#else
			#define GPRS_EXTERN extern
		#endif
	
		#define WAIT_1S 1	///延时1秒
		#define SEND_1T 1	///发送1次
		
		
		
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
			uint8 *cmd_text;/// 指令内容
			uint8 sec_wait;///执行该命令最长等待秒数
			uint8 tx_max_count;///最大发送次数
		}AT_CMD_STRUCT;///AT命令结构体
		
		#define MAX_STATE_SIZE (AT_NULL_INDEX+1)///最大状态数
		
		typedef struct
		{
			enum GSM_CUR_MODE cur_mode;///当前所处的模式
			uint8 gsm_module_reset_counter;///GSM模块重启计数
			uint8 gsm_mode_exe_flag[4];///所处的模式执行状态,开机,AT,PPP,GPRS通讯
			uint8 gsm_state_array[MAX_STATE_SIZE];
			uint8 ring_low_counter;///ring脚低计数
			uint8 ring_low_sec_counter;///ring脚低秒计数
			uint8 gsm_rx_ring_flag;///电话呼入标志
			uint8 sleep_rx_ring_falg;///睡眠时收到电话标志
			uint8 gsm_rx_sms_flag;///接收短信标志
			
			uint8 gsm_ring_low_flag;///RING管脚低电平标志
			uint16 gsm_ring_low_ms_counter;///RING管脚低电平毫秒计数
			
			uint8 first_deal_sms_flag;///上电第1次执行短信
			uint8 first_deal_sms_sec_counter;///上电第1次执行短信秒计时
			
			uint8 ppp_monitor_falg;///PPP拔号监测
			uint8 ppp_sec_counter;///PPP拔号秒计时
		}GSM_MISC_STRUCT;///GSM数据结构体
		
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
