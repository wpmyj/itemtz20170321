#define GPRS_GLOBAL
#include "include.h"


uint8 AT[] = {"AT\x0d"};///AT命令
uint8 AT_E0[] = {"ATE0\x0d"};///关闭回显
uint8 AT_IPR[] = {"AT+IPR=38400;&w\x0d"};///设置波特率为38400
uint8 AT_CIMI[] = {"AT+CIMI\x0d\x0a"};///读取SIM卡IMSI
uint8 AT_CNUM[] = {"AT+CNUM\x0d"};///查询本机号码
uint8 AT_CPBS_ON[] = {"AT+CPBS=\"ON\"\x0d"};///设置本机号使能
uint8 AT_CPBW[] = {"AT+CPBW=1,"};///写入本机号
uint8 AT_IMEI[] = {"ATD*#06#\x0d\x0a"};///查询GSM模块的IMEI
uint8 AT_CSQ[] = {"AT+CSQ\x0d"};///查询信号强度
uint8 AT_CREG[] = {"AT+CREG?\x0d"};///查询注册情况

uint8 AT_CGATT[] = {"AT+CGATT?\r"};///查询PDP上下文状态
uint8 AT_CGATT_1[] = {"AT+CGATT=1\r"};///激活PDP上下文
uint8 AT_CGATT_0[] = {"AT+CGATT=0\r"};///关闭PDP上下文
uint8 AT_CGDCONT[] = {"AT+CGDCONT=1,\"IP\","};
uint8 AT_ATD[] = {"ATD*99***1#\r"};///拨*99***1#

uint8 AT_CMD_MODE[] = {"+++"};///AT命令模式
uint8 AT_GPRS_DATA_MODE[] = {"ATO\r"};///GPRS数据模式

uint8 AT_CMGF[] = {"AT+CMGF=0\x0d"};///以PDU形式处理短消息
uint8 AT_CMGL[] = {"AT+CMGL=0\x0d"};///0未读短信,4全部短信
uint8 AT_CMGS[] = {"AT+CMGS="};///发送短信
uint8 AT_CMGD[] = {"AT+CMGD=1,4\x0d"};///删短信
uint8 AT_ATH[] = {"ATH\x0d"};///挂断电话
uint8 AT_SLEEP[] = {"AT+CFUN=5\x0d\x0a"};///休眠
uint8 AT_WAKE[]  = {"AT+CFUN=1\x0d\x0a"};///要唤醒
uint8 AT_NULL[] = {""};///空,等待接收




uint8 OK_ACK[] = "OK";
uint8 CONNECT[] = "CONNECT";
uint8 CRLF_ACK[] = "\x0d\x0a";


AT_CMD_STRUCT at_cmd_struct[] = 
                            {
								{AT, 		2*WAIT_1S,10*SEND_1T},
								{AT_E0,		2*WAIT_1S,10*SEND_1T},
								{AT_IPR,	2*WAIT_1S,5*SEND_1T},
								{AT_CIMI,	2*WAIT_1S,50*SEND_1T},
								{AT_CPBS_ON,5*WAIT_1S,10*SEND_1T},
								{AT_CPBW,	5*WAIT_1S,10*SEND_1T},
								{AT_IMEI,	2*WAIT_1S,3*SEND_1T},
								{AT_CSQ,	2*WAIT_1S,3*SEND_1T},
								{AT_CREG,	2*WAIT_1S,3*SEND_1T},
								{AT_CNUM,	5*WAIT_1S,50*SEND_1T},
								{AT_CGATT,	2*WAIT_1S,10*SEND_1T},
								{AT_CGATT_1,4*WAIT_1S,3*SEND_1T},
								{AT_CGATT_0,4*WAIT_1S,3*SEND_1T},
								{AT_CGDCONT,2*WAIT_1S,10*SEND_1T},
								{AT_ATD,	2*WAIT_1S,10*SEND_1T},
								
								{AT_CMD_MODE,		3*WAIT_1S,3*SEND_1T},
								{AT_GPRS_DATA_MODE,	3*WAIT_1S,3*SEND_1T},
								
								{AT_CMGF,	2*WAIT_1S,3*SEND_1T},
								{AT_CMGL,	10*WAIT_1S, SEND_1T},
								{AT_CMGS,	3*WAIT_1S, SEND_1T},
								{AT_CMGD,	2*WAIT_1S,3*SEND_1T},
								
								{AT_ATH,	3*WAIT_1S,5*SEND_1T},
								{AT_SLEEP,	3*WAIT_1S,5*SEND_1T},
								{AT_WAKE,	3*WAIT_1S,3*SEND_1T},
								{AT_NULL,	15*WAIT_1S, SEND_1T},
							};
							
AT_CMD_PROCESS_FUN_PTR at_cmd_process_fun_array[] = 
							{
								AtFun,
								AtE0Fun,
								AtIprFun,
								AtCimiFun,
								AtCpbsOnFun,
								AtCpbwFun,
								AtImeiFun,
								AtCsqFun,
								AtCregFun,
								AtCnumFun,
								AtCgattFun,
								AtCgatt1Fun,
								AtCgatt0Fun,
								AtCgdcontFun,
								AtAdtFun,								
								AtCmdModeFun,
								AtGprsDataModeFun,
								AtCmgfFun,
								AtCmglFun,
								AtCmgsFun,
								AtCmgdFun,
								AtAthFun,
								AtSleepFun,
								AtWakeFun,
								AtNullFun,
							};

void GprsSendAtCmd(uint8 cmd_index,uint8 app_data[],uint8 app_len,uint8 mat_data[],uint8 mat_len)///附加的数据及长度,匹配的数据及长度
{
	uint8 i,ack_flag = FALSE,rx_data[GPRS_UART_BUF_LEN];
	uint16 rx_len,mat_index = 0;
	
	gsm_misc_struct.gsm_state_array[cmd_index] = AT_NOT_EXE;
	
	for(i=0;i<at_cmd_struct[cmd_index].tx_max_count;i++)
	{
		GprsUartParaInit();
		GprsUartFixedLenSend(at_cmd_struct[cmd_index].cmd_text,StrLen(at_cmd_struct[cmd_index].cmd_text,0));
		if(app_len > 0)
		{
			GprsUartFixedLenSend(app_data,app_len);
		}
		
		#ifdef GPRS_DEBUG
			LocalDebug(at_cmd_struct[cmd_index].cmd_text,StrLen(at_cmd_struct[cmd_index].cmd_text,0),LOCAL_PRO_DEBUG);
			if(app_len > 0)
			{
				LocalDebug(app_data,app_len,LOCAL_PRO_DEBUG);
			}
		#endif
		
		GprsDelay(at_cmd_struct[cmd_index].sec_wait,TRUE);
		
		if(g_gprs_uart_struct.rx_flag == VALID_VAL_AA)
		{
			rx_len = GetGprsUartRxData(rx_data);
			#ifdef GPRS_DEBUG
				LocalDebug(rx_data,rx_len,LOCAL_PRO_DEBUG);
			#endif
			mat_index = SubMatch(mat_data,mat_len,rx_data,rx_len);
			if(mat_index > 0)
			{
				ack_flag = TRUE;
				break;
			}
		}
	}
	
	at_cmd_process_fun_array[cmd_index](rx_data,rx_len,ack_flag);
	
	if(!ack_flag)
	{
		LocalDebug(at_cmd_struct[cmd_index].cmd_text,StrLen(at_cmd_struct[cmd_index].cmd_text,0),LOCAL_TEST_DEBUG);
		LocalDebug("exe_fail\r\n",StrLen("exe_fail\r\n",0),LOCAL_TEST_DEBUG);
	}
}
void AtNullFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_NULL_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_NULL_INDEX] = AT_EXE_FAIL;
	}
}							
void AtFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_INDEX] = AT_EXE_FAIL;
	}
}
void AtE0Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_E0_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_E0_INDEX] = AT_EXE_FAIL;
	}
}
void AtIprFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_IPR_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_IPR_INDEX] = AT_EXE_FAIL;
	}
	
}							
void AtCimiFun(uint8 *data,uint16 len,uint8 flag)///这其中可以实现无卡，换卡报警功能
{
	uint8 res,mat_index;
	uint8 cmp_code[] = {"CME ERROR: 10"};
	
	gsm_misc_struct.gsm_state_array[AT_CIMI_INDEX] = AT_EXE_FAIL;
	
	if(flag)
	{
		sys_work_para_struct.no_simcard_falg = FALSE;
		mat_index = SubMatch(CRLF_ACK,StrLen(CRLF_ACK,0),data,len);
		if(mat_index > 0)
		{
			res = IsValidNum(data+mat_index,SIM_CARD_IMSI_LEN);
			if(res)
			{
				if(sys_private_para_struct.sim_card_imsi[0] == INVALID_VAL_FF)
				{
					MemCpy(sys_private_para_struct.sim_card_imsi,data+mat_index,SIM_CARD_IMSI_LEN);
					res = SysPrivateParaWrite();
					if(!res)
					{
						LocalDebug("IMSI set fail\r\n",StrLen("IMSI set fail\r\n",0),LOCAL_TEST_DEBUG);
					}
				}
				else
				{
					res = MemCmp(sys_private_para_struct.sim_card_imsi,data+mat_index,SIM_CARD_IMSI_LEN);
					if(res)
					{
						if(sys_work_para_struct.term_run_status_word & STATE_SIMCARD_CHANGE)
						{
							sys_work_para_struct.term_run_status_word &= ~STATE_SIMCARD_CHANGE;///解除报警
							ProUpAlarm(ALRM_FLAG_CLEAR_BYTE|ALRM_FLAG_SIMCARD_CHANGE);
						}
					}
					else
					{
						LocalDebug("simcard changed\r\n",StrLen("simcard changed\r\n",0),LOCAL_TEST_DEBUG);
						if(!(sys_work_para_struct.term_run_status_word & STATE_SIMCARD_CHANGE))
						{
							sys_work_para_struct.term_run_status_word |= STATE_SIMCARD_CHANGE;///报警
							ProUpAlarm(ALRM_FLAG_SIMCARD_CHANGE);
						}
						
					}
				}
				gsm_misc_struct.gsm_state_array[AT_CIMI_INDEX] = AT_EXE_SUCESS;
			}
		}	
	}
	else
	{
		mat_index = SubMatch(cmp_code,StrLen(cmp_code,0),data,len);
		if(mat_index > 0)
		{
			sys_work_para_struct.no_simcard_falg = TRUE;
		}
		else
		{
			sys_work_para_struct.no_simcard_falg = FALSE;
		}
	}
	SysWorkParaWrite();
}

void AtCnumFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 cnum_ack[] = {"+CNUM:"};///+CNUM: ,"13828756403"
	uint8 res,mat_index,cmp_code[]={",\""};
	uint8 tmp_terminal_id[TERMINAL_ID_LEN];
	
	gsm_misc_struct.gsm_state_array[AT_CNUM_INDEX] = AT_EXE_FAIL;
	
	if(flag)
	{
		mat_index = SubMatch(cnum_ack,StrLen(cnum_ack,0),data,len);
		if(mat_index > 0)
		{
			mat_index = SubMatch(cmp_code,StrLen(cmp_code,0),data,len);
			if(mat_index > 0)
			{
				res = IsValidNum(data+mat_index,11);
				if(res)
				{
					SimNumToTermId(data+mat_index,tmp_terminal_id);
					res = MemCmp(sys_private_para_struct.terminal_id,tmp_terminal_id,TERMINAL_ID_LEN);
					if(!res)
					{
						MemCpy(sys_private_para_struct.terminal_id,tmp_terminal_id,TERMINAL_ID_LEN);
						SysPrivateParaWrite();
					}
					
					gsm_misc_struct.gsm_state_array[AT_CNUM_INDEX] = AT_EXE_SUCESS;
				}
			}
		}	
	}
}
void AtCpbsOnFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CPBS_ON_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CPBS_ON_INDEX] = AT_EXE_FAIL;
	}
}
void AtCpbwFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CPBW_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CPBW_INDEX] = AT_EXE_FAIL;
	}
}
void AtImeiFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 res,mat_index;
	uint8 cmp_data[] = {"IMEI: "};///+CNUM: ,"13828756403"
	
	gsm_misc_struct.gsm_state_array[AT_IMEI_INDEX] = AT_EXE_FAIL;
	
	if(flag)
	{
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		if(mat_index > 0)
		{
			res = IsValidNum(data+mat_index,GSM_IMEI_LEN);
			if(res)
			{
				MemCpy(sys_private_para_struct.gsm_imei,data+mat_index,GSM_IMEI_LEN);
				gsm_misc_struct.gsm_state_array[AT_IMEI_INDEX] = AT_EXE_SUCESS;
			}
		}	
	}
}
void AtCsqFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 res,mat_index,tmp_val;
	uint8 cmp_data[] = {"+CSQ: "};
	
	gsm_misc_struct.gsm_state_array[AT_CSQ_INDEX] = AT_EXE_FAIL;
	
	if(flag)
	{
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		if(mat_index > 0)
		{
			res = FALSE;
			if(data[mat_index+1] == ',')
			{
				if(IsValidNum(data+mat_index,1))
				{
					tmp_val = data[mat_index] - '0';
					res = TRUE;
				}
			}
			else
			{
				if(IsValidNum(data+mat_index,2))
				{
					tmp_val = data[mat_index] - '0';
					tmp_val *= 10;
					tmp_val += data[mat_index+1] - '0';
					res = TRUE;
				}
			}
			
			if(res)
			{
				if(tmp_val <= 31)
				{
					sys_misc_run_struct.gsm_csq_val = tmp_val;
				}
			}
			gsm_misc_struct.gsm_state_array[AT_CSQ_INDEX] = AT_EXE_SUCESS;
		}	
	}
}
void AtCregFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+CREG: "};
	
	gsm_misc_struct.gsm_state_array[AT_CREG_INDEX] = AT_EXE_FAIL;
	
	if(flag)
	{
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0)
		{
			if((data[mat_index+2] == '1')||(data[mat_index+2] == '5'))
			{
				gsm_misc_struct.gsm_state_array[AT_CREG_INDEX] = AT_EXE_SUCESS;
			}
		}
	}	
}
void AtCgattFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+CGATT: "};
	
	gsm_misc_struct.gsm_state_array[AT_CGATT_INDEX] = AT_EXE_FAIL;
	
	if(flag)
	{
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0)
		{
			if(data[mat_index] == '1')
			{
				gsm_misc_struct.gsm_state_array[AT_CGATT_INDEX] = AT_EXE_SUCESS;
			}
		}
	}	
}
void AtCgatt1Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CGATT_1_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CGATT_1_INDEX] = AT_EXE_FAIL;
	}
}	
void AtCgatt0Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CGATT_0_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CGATT_0_INDEX] = AT_EXE_FAIL;
	}
}
void AtCgdcontFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CGDCONT_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CGDCONT_INDEX] = AT_EXE_FAIL;
	}
}	
void AtAdtFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_ATD_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_ATD_INDEX] = AT_EXE_FAIL;
	}	
}

void AtCmdModeFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CMD_MODE_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CMD_MODE_INDEX] = AT_EXE_FAIL;
	}	
}
void AtGprsDataModeFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_GPRS_DATA_MODE_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_GPRS_DATA_MODE_INDEX] = AT_EXE_FAIL;
	}	
}
void AtCmgfFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CMGF_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CMGF_INDEX] = AT_EXE_FAIL;
	}	
}
void AtCmgsFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CMGS_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CMGS_INDEX] = AT_EXE_FAIL;
	}	
}
void AtCmglFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"CMGL: "};
	
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CMGL_INDEX] = AT_EXE_SUCESS;
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0)
		{
			SmsProcessFun(data+mat_index,len-mat_index);
		}
	}
}
void AtCmgdFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_CMGD_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_CMGD_INDEX] = AT_EXE_FAIL;
	}	
}
void AtAthFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_ATH_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_ATH_INDEX] = AT_EXE_FAIL;
	}	
}								
void AtSleepFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_SLEEP_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_SLEEP_INDEX] = AT_EXE_FAIL;
	}	
}																
void AtWakeFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag)
	{
		gsm_misc_struct.gsm_state_array[AT_WAKE_INDEX] = AT_EXE_SUCESS;
	}
	else
	{
		gsm_misc_struct.gsm_state_array[AT_WAKE_INDEX] = AT_EXE_FAIL;
	}	
}	

uint8 GprsAutoBpr(void)///波特率
{
	uint8 i,res=FALSE;
	uint32 bpr[3] = {38400,9600,115200};
	
	for(i=0;i<3;i++)
	{
		UsartInit(GPRS_USART,bpr[i],USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);

		GprsSendAtCmd(AT_INDEX,NULL,0,OK_ACK,2);

		if(gsm_misc_struct.gsm_state_array[AT_INDEX] == AT_EXE_SUCESS)
		{
			break;
		}
	}
	
	if(i != 0)
	{
		GprsSendAtCmd(AT_IPR_INDEX,NULL,0,OK_ACK,2);///设置模块的波特率为38400
		UsartInit(GPRS_USART,GPRS_USART_BPR,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	}
	
	return res;
}

void GprsDelay(uint16 delay_sec,uint8 wait_ack_flag)///除去GPRS之处的MAIN运行
{
	uint8 past_sec = 0;
	uint16 rtc_cur_val,rtc_past_val;
	
	sys_misc_run_struct.gprs_delay_flag = TRUE;
	rtc_past_val = sys_misc_run_struct.sys_rtc_sec_counter;
	while((past_sec <= delay_sec))
	{
		FeedWtd();
		rtc_cur_val = sys_misc_run_struct.sys_rtc_sec_counter;
		past_sec = (rtc_cur_val + (65535 - rtc_past_val)) % 65535;
		
		DriverMain();
		GpsMain();
		if(!sys_misc_run_struct.systask_delay_flag)
		{
			SysTaskMain();
		}
		if(!sys_misc_run_struct.localcomm_delay_flag)
		{
			LocalCommMain();
		}
		Rs232Main();
		if(wait_ack_flag)
		{
			if(g_gprs_uart_struct.rx_flag == VALID_VAL_AA)
			{
				break;
			}
		}		
	}
	sys_misc_run_struct.gprs_delay_flag = FALSE;
}
void GprsPowerInit(void)
{
	HIGH_GPRS_IGT();
	
	OFF_GPRS_PWR();///关机
	GprsDelay(3*WAIT_1S,FALSE);
	ON_GPRS_PWR();///开机
	
	GprsDelay(WAIT_1S,FALSE);
	
	LOW_GPRS_IGT();
	GprsDelay(WAIT_1S,FALSE);
	HIGH_GPRS_IGT();
	
	GprsDelay(3*WAIT_1S,FALSE);
	
	gsm_misc_struct.cur_mode = AT_INIT_MODE;///转向AT_MODE
	GprsAutoBpr();
}
void GprsAtInit(void)
{
	uint8 i;
	uint8 str_len;
	char str_ch[256];
	
	gsm_misc_struct.first_deal_sms_flag = TRUE;
	gsm_misc_struct.first_deal_sms_sec_counter = 0;
	
	GprsSendAtCmd(AT_INDEX,NULL,0,OK_ACK,2);
	if(gsm_misc_struct.gsm_state_array[AT_INDEX] == AT_EXE_FAIL)
	{
		gsm_misc_struct.gsm_mode_exe_flag[0] = AT_EXE_FAIL;
		goto RET_LAB;
	}
	else
	{
		gsm_misc_struct.gsm_mode_exe_flag[0] = AT_EXE_SUCESS;
	}

	GprsSendAtCmd(AT_E0_INDEX,NULL,0,OK_ACK,2);
	if(gsm_misc_struct.gsm_state_array[AT_E0_INDEX] == AT_EXE_FAIL)
	{
		goto RET_LAB;
	}
	GprsSendAtCmd(AT_CIMI_INDEX,NULL,0,OK_ACK,2);
	if(gsm_misc_struct.gsm_state_array[AT_CIMI_INDEX] == AT_EXE_FAIL)
	{
		if(sys_work_para_struct.no_simcard_falg)
		{
			goto RET_LAB;
		}
	}
	
	GprsSendAtCmd(AT_CMGF_INDEX,NULL,0,OK_ACK,2);
	
	GprsSendAtCmd(AT_CSQ_INDEX,NULL,0,OK_ACK,2);
	
	for(i=0;i<120;i++)
	{
		GprsSendAtCmd(AT_CREG_INDEX,NULL,0,OK_ACK,2);
		if(gsm_misc_struct.gsm_state_array[AT_CREG_INDEX] == AT_EXE_SUCESS)
		{
			break;
		}
	}
	if(i == 120)
	{
		goto RET_LAB;
	}
	
	GprsSendAtCmd(AT_CMGL_INDEX,NULL,0,OK_ACK,2);
	
	#ifndef SYS_PARA_INIT
	GprsSendAtCmd(AT_CNUM_INDEX,NULL,0,OK_ACK,2);
	if(gsm_misc_struct.gsm_state_array[AT_CNUM_INDEX] == AT_EXE_FAIL)
	{
		if((sys_work_para_struct.no_simcard_falg)||(sys_private_para_struct.terminal_id[0] == INVALID_VAL_FF))
		{
			goto RET_LAB;
		}
	}
	#endif
	for(i=0;i<50;i++)
	{
		GprsSendAtCmd(AT_CGATT_INDEX,NULL,0,OK_ACK,2);
		if(gsm_misc_struct.gsm_state_array[AT_CGATT_INDEX] == AT_EXE_SUCESS)
		{
			break;
		}
		else
		{
			GprsSendAtCmd(AT_CGATT_1_INDEX,NULL,0,OK_ACK,2);
		}
	}
	if(i == 50)
	{
		goto RET_LAB;
	}
	
	GprsSendAtCmd(AT_CSQ_INDEX,NULL,0,OK_ACK,2);
	
	str_len = sprintf(str_ch,"\"%s\"\r",sys_private_para_struct.apn);
	GprsSendAtCmd(AT_CGDCONT_INDEX,(uint8*)str_ch,str_len,OK_ACK,2);
	if(gsm_misc_struct.gsm_state_array[AT_CGDCONT_INDEX] == AT_EXE_FAIL)
	{
		goto RET_LAB;
	}
	
	GprsSendAtCmd(AT_ATD_INDEX,NULL,0,CONNECT,7);
	if(gsm_misc_struct.gsm_state_array[AT_ATD_INDEX] == AT_EXE_FAIL)
	{
		goto RET_LAB;
	}
	else
	{
		gsm_misc_struct.cur_mode = PPP_MODE;
		gsm_misc_struct.gsm_mode_exe_flag[2] = AT_NOT_EXE;
		g_ppp_struct.lcp_req_flag = TRUE;
	}
	gsm_misc_struct.gsm_mode_exe_flag[1] = AT_EXE_SUCESS;
	gsm_misc_struct.ppp_monitor_falg = TRUE;
	gsm_misc_struct.ppp_sec_counter = 0X00;
	return;
RET_LAB:
	gsm_misc_struct.cur_mode = POWER_INIT_MODE;
	gsm_misc_struct.gsm_mode_exe_flag[1] = AT_EXE_FAIL;
}
uint8 GprsSwitchAtMode(uint8 cur_mode)
{
	uint8 OK_ACK[] = "OK",res = TRUE;
	
	if(cur_mode == POWER_INIT_MODE)
	{
		res = FALSE;
	}
	else if(cur_mode != AT_INIT_MODE)
	{
		gsm_misc_struct.cur_mode = AT_INIT_MODE;
		GprsDelay(2*WAIT_1S,FALSE);
		GprsSendAtCmd(AT_CMD_MODE_INDEX,NULL,0,OK_ACK,2);
		if(gsm_misc_struct.gsm_state_array[AT_CMD_MODE_INDEX] != AT_EXE_SUCESS)
		{
			res = FALSE;
		}
	}
	
	if(res)
	{
		gsm_misc_struct.cur_mode = AT_INIT_MODE;
	}
	else
	{
		gsm_misc_struct.cur_mode = (enum GSM_CUR_MODE)cur_mode;
		LocalDebug("data-->at mode fail\r\n",StrLen("data-->at mode fail\r\n",0),LOCAL_TEST_DEBUG);
	}
	
	return res;
}
uint8 GprsSwitchGprsDataMode(uint8 cur_mode)
{
	uint8 CONNECT[] = "CONNECT",res = TRUE;
	
	if(cur_mode == POWER_INIT_MODE)
	{
		res = FALSE;
	}
	else if(cur_mode != AT_INIT_MODE)
	{
		GprsDelay(2*WAIT_1S,FALSE);
		GprsSendAtCmd(AT_GPRS_DATA_MODE_INDEX,NULL,0,CONNECT,7);
		if(gsm_misc_struct.gsm_state_array[AT_GPRS_DATA_MODE_INDEX] != AT_EXE_SUCESS)
		{
			res = FALSE;
		}
	}
	if(res)
	{
		gsm_misc_struct.cur_mode = (enum GSM_CUR_MODE)cur_mode;
	}
	else
	{
		gsm_misc_struct.cur_mode = POWER_INIT_MODE;
		LocalDebug("at-->data mode fail\r\n",StrLen("at-->data mode fail\r\n",0),LOCAL_TEST_DEBUG);
	}
	
	return res;
}
void GsmVaryInit(void)
{
	g_ppp_struct.ppp_tx_ncp_counter = 0;
	gsm_misc_struct.ring_low_counter = 0;
	gsm_misc_struct.ring_low_sec_counter = 0;
	g_ppp_struct.ncp_ok_flag = FALSE;
	gsm_misc_struct.gsm_rx_sms_flag = FALSE;
	gsm_misc_struct.gsm_rx_ring_flag = FALSE;
	RamZero(g_ppp_struct.sms_ack_flag,3);
	RamZero(udp_struct.udp_src_ip,4);
	RamZero(g_ppp_struct.dns_ip,4);
	ProParaInit();
}
void GprsParaInit(void)
{
	RamZero(gsm_misc_struct.gsm_mode_exe_flag,0);
	gsm_misc_struct.cur_mode = POWER_INIT_MODE;
	gsm_misc_struct.gsm_state_array[AT_CREG_INDEX] = AT_NOT_EXE;
	sys_misc_run_struct.para_change_re_login_flag = FALSE;
	GsmVaryInit();
}
void GsmTelSmsProcess(void)///电话，短信处理
{
	uint8 exe_res;
	enum GSM_CUR_MODE tmp_mode = gsm_misc_struct.cur_mode;
	
	exe_res = GprsSwitchAtMode(tmp_mode);
	
	if(exe_res)
	{
		if(gsm_misc_struct.gsm_rx_ring_flag)
		{
			gsm_misc_struct.gsm_rx_ring_flag = FALSE;
			LocalDebug("rx ring\r\n",StrLen("rx ring\r\n",0),LOCAL_TEST_DEBUG);
			GprsSendAtCmd(AT_ATH_INDEX,NULL,0,OK_ACK,2);
			
			LocalDebug("rx ring,term reset...\r\n",StrLen("rx ring,term reset...\r\n",0),LOCAL_TEST_DEBUG);
			TermReset2();
		}
		else if(gsm_misc_struct.gsm_rx_sms_flag)
		{
			gsm_misc_struct.gsm_rx_sms_flag = FALSE;
			LocalDebug("deal sms\r\n",StrLen("deal sms\r\n",0),LOCAL_TEST_DEBUG);
			GprsSendAtCmd(AT_CMGL_INDEX,NULL,0,OK_ACK,2);
		}
		
		GprsSwitchGprsDataMode(tmp_mode);
	}
	else
	{
		if(gsm_misc_struct.gsm_rx_ring_flag)
		{
			gsm_misc_struct.gsm_rx_ring_flag = FALSE;
			LocalDebug("ath fail,term reset...\r\n",StrLen("ath fail,term reset...\r\n",0),LOCAL_TEST_DEBUG);
			if(!gsm_misc_struct.sleep_rx_ring_falg)
			{
				TermReset2();
			}
		}
		else
		{
			LocalDebug("sms exe fail\r\n",StrLen("sms exe fail\r\n",0),LOCAL_TEST_DEBUG);
		}
	}
}
void GsmCsqCheck(void)///GSM状态监测,csq,imsi,处理短信，来电
{
	uint8 exe_res;
	enum GSM_CUR_MODE tmp_mode = gsm_misc_struct.cur_mode;
	
	exe_res = GprsSwitchAtMode(tmp_mode);
	
	if(exe_res)
	{
		GprsSendAtCmd(AT_CSQ_INDEX,NULL,0,OK_ACK,2);
		GprsSwitchGprsDataMode(tmp_mode);
	}	
}
void GsmDisLcpAndIntoSleep(void)///断开LCP链接,解除附着，进入低功模式
{
	if(gsm_misc_struct.cur_mode == UDP_MODE)
	{
		ProUpSleep();///上行省电休眠
		GprsDelay(2,FALSE);
	}
/**	if(gsm_misc_struct.cur_mode >= PPP_MODE)
	{
		PppLcpTerminate();
		GprsDelay(2,FALSE);
	}
**/
	GprsSwitchAtMode(gsm_misc_struct.cur_mode);
	gsm_misc_struct.cur_mode = AT_INIT_MODE;
	GprsSendAtCmd(AT_ATH_INDEX,NULL,0,OK_ACK,2);
	GprsSendAtCmd(AT_SLEEP_INDEX,NULL,0,OK_ACK,2);
	if(gsm_misc_struct.gsm_state_array[AT_SLEEP_INDEX] != AT_EXE_SUCESS)
	{
		GprsPowerInit();
		GprsSendAtCmd(AT_SLEEP_INDEX,NULL,0,OK_ACK,2);
	}
}

void GprsMain(void)
{	
	uint8 i;
	
	if(gsm_misc_struct.cur_mode == POWER_INIT_MODE)
	{
		gsm_misc_struct.gsm_module_reset_counter++;
		if(gsm_misc_struct.gsm_module_reset_counter > 5)
		{
			TermReset();
		}
		LocalDebug("GSM module restart...\r\n",StrLen("GSM module restart...\r\n",0),LOCAL_TEST_DEBUG);
		GprsParaInit();
		GprsPowerInit();
	}
	else if(gsm_misc_struct.cur_mode == AT_INIT_MODE)
	{
		gsm_misc_struct.gsm_mode_exe_flag[1] = AT_NOT_EXE;
		LocalDebug("GSM AT init...\r\n",StrLen("GSM AT init...\r\n",0),LOCAL_TEST_DEBUG);
		GsmVaryInit();
		GprsAtInit();
	}
	else if(gsm_misc_struct.cur_mode == PPP_MODE)
	{
		gsm_misc_struct.gsm_module_reset_counter = 0;
		PppInit();
	}

	if(sys_misc_run_struct.gsm_csq_check_flag)
	{
		GsmCsqCheck();///CSQ检测
		sys_misc_run_struct.gsm_csq_check_flag = FALSE;
	}
	if(gsm_misc_struct.gsm_rx_ring_flag||gsm_misc_struct.gsm_rx_sms_flag)
	{
		if(gsm_misc_struct.gsm_ring_low_ms_counter >= 50)
		{
			GsmTelSmsProcess();///来电短信处理
		}
		
		gsm_misc_struct.gsm_ring_low_flag = FALSE;
		gsm_misc_struct.gsm_ring_low_ms_counter = 0;
		
		gsm_misc_struct.ring_low_counter = 0;
		gsm_misc_struct.ring_low_sec_counter = 0;
		gsm_misc_struct.gsm_rx_ring_flag = FALSE;
		gsm_misc_struct.gsm_rx_sms_flag = FALSE;
	}
	
	if(sys_misc_run_struct.term_enter_sleep_flag)
	{
		TermEnterSleep();
		gsm_misc_struct.cur_mode = AT_INIT_MODE;
		if(gsm_misc_struct.sleep_rx_ring_falg)
		{
			gsm_misc_struct.sleep_rx_ring_falg = FALSE;
			GprsDelay(1,FALSE);
			GprsDelay(5,FALSE);
			GprsSendAtCmd(AT_ATH_INDEX,NULL,0,OK_ACK,2);
		}
		else
		{
			for(i=0;i<5;i++)
			{
				GprsUartFixedLenSend(at_cmd_struct[AT_INDEX].cmd_text,StrLen(at_cmd_struct[AT_INDEX].cmd_text,0));
				LongTimeDly(200);
			}
			GprsSendAtCmd(AT_INDEX,NULL,0,OK_ACK,2);
			GprsSendAtCmd(AT_WAKE_INDEX,NULL,0,OK_ACK,2);
		
			if(gsm_misc_struct.gsm_state_array[AT_WAKE_INDEX] != AT_EXE_SUCESS)
			{
				gsm_misc_struct.cur_mode = POWER_INIT_MODE;
			}
		}
		sys_misc_run_struct.term_enter_sleep_flag = FALSE;
	}
	
	if(sys_misc_run_struct.para_change_re_login_flag)
	{
		GprsDelay(3,FALSE);
		sys_misc_run_struct.para_change_re_login_flag = FALSE;
		gsm_misc_struct.cur_mode = POWER_INIT_MODE;
	}
	
	PppMainProcess();
}
