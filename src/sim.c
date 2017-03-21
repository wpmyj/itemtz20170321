#include "include.h"

uint8 SimNumToTermId23(uint8 sim_num[],uint8 term_id[])
{
	uint8 i,res = FALSE;
	uint32 u32_val;

	if(sim_num[0] != '1')
	{
		goto RETURN_LAB;
	}
	if(sim_num[1] == '5')
	{
		sim_num[1] = '1';
	}
	else if(sim_num[1] == '8')
	{
		sim_num[1] = '0';
	}	
	else if(sim_num[1] == '4')
	{
		sim_num[1] = '2';
	}	
	else if(sim_num[1] == '3')
	{
		sim_num[1] = '3';
	}
	else
	{
		goto RETURN_LAB;
	}
	
	u32_val = 0;
	for(i=0;i<10;i++)
	{
		u32_val = u32_val*10 + (sim_num[i+1]&0x0f);
	}
	
	term_id[0] = 0x23;///ÌìÔó
	term_id[1] = u32_val >> 24;
	term_id[2] = u32_val >> 16;
	term_id[3] = u32_val >> 8;
	term_id[4] = u32_val;
	
	res = TRUE;
RETURN_LAB:
	return res;
}

void SimTermId23ToAsciiNum(uint8 term_id[],uint8 sim_num[])
{
	uint32 u32_val;
	uint8 str_len;
	char str_ch[20];

	sim_num[0] = '1';
	u32_val  = term_id[1] << 24;
	u32_val += term_id[2] << 16;
	u32_val += term_id[3] << 8;
	u32_val += term_id[4];
	str_len = sprintf(str_ch,"%.10u",u32_val);
	MemCpy(sim_num+1,(uint8*)str_ch,str_len);
	
	if(sim_num[1] == '1')
	{
		sim_num[1] = '5';
	}
	else if(sim_num[1] == '0')
	{
		sim_num[1] = '8';
	}	
	else if(sim_num[1] == '2')
	{
		sim_num[1] = '4';
	}	
}
uint8 SimNumToTermId(uint8 sim_num[],uint8 term_id[])
{
	uint8 i,res = FALSE;
	uint64 u64_val;

	if(sim_num[0] != '1')
	{
		goto RETURN_LAB;
	}
	if((sim_num[1] != '3')&&(sim_num[1] != '4')&&
	   (sim_num[1] != '5')&&(sim_num[1] != '8'))
	{
		goto RETURN_LAB;
	}

	u64_val = 0;
	for(i=0;i<11;i++)
	{
		u64_val = u64_val*10 + (sim_num[i]&0x0f);
	}
	
	term_id[0] = u64_val >> 32;
	term_id[1] = u64_val >> 24;
	term_id[2] = u64_val >> 16;
	term_id[3] = u64_val >> 8;
	term_id[4] = u64_val;
	
	res = TRUE;
RETURN_LAB:
	return res;
}
void SimTermIdToAsciiNum(uint8 term_id[],uint8 sim_num[])
{
	uint64 u64_val = 0;
	uint32 u32_val = 0;
	uint8 str_len;
	char str_ch[20];

	u64_val  = term_id[0];
	u64_val  = u64_val << 32;
	
	u32_val = term_id[1] << 24;
	u32_val += term_id[2] << 16;
	u32_val += term_id[3] << 8;
	u32_val += term_id[4];
	
	u64_val += u32_val;
	
	str_len = sprintf(str_ch,"%llu",u64_val);
	MemCpy(sim_num,(uint8*)str_ch,str_len);
}
uint8 SimSetSimNum(uint8 sim_num[])
{
	uint8 tmp_data[14],ok_ack[] = {"OK"},exe_res,res = FALSE;
	
	enum GSM_CUR_MODE tmp_mode = gsm_misc_struct.cur_mode;
	
	exe_res = GprsSwitchAtMode(tmp_mode);
	
	if(exe_res)
	{
		tmp_data[0] = '"';
		MemCpy(tmp_data+1,sim_num,11);
		tmp_data[12] = '"';
		tmp_data[13] = 0x0d;
		
		GprsSendAtCmd(AT_CPBS_ON_INDEX,NULL,0,ok_ack,2);
		if(gsm_misc_struct.gsm_state_array[AT_CPBS_ON_INDEX] == AT_EXE_SUCESS)
		{
			GprsSendAtCmd(AT_CPBW_INDEX,tmp_data,14,ok_ack,2);
			if(gsm_misc_struct.gsm_state_array[AT_CPBW_INDEX] == AT_EXE_SUCESS)
			{
				res = TRUE;
			}
		}
		
		GprsSwitchGprsDataMode(tmp_mode);
	}	
	
	return res;
}
