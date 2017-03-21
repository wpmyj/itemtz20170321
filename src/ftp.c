/****************************************Copyright (c)**************************************************
**                                                                
**--------------File Info-------------------------------------------------------------------------------
** File Name: xg_protocol.c
** Last modified Date:
** Last Version:
** Descriptions: 
*******************************************************************************************************/
#define FTP_GLOBAL

#include "include.h"

#define FTP_FLAG_FIN 	0X01
#define FTP_FLAG_SYN 	0X02
#define FTP_FLAG_RST 	0X04
#define FTP_FLAG_PSH 	0X08
#define FTP_FLAG_ACK 	0X10
#define FTP_FLAG_URG 	0X20
#define FTP_FLAG_MASK 	(FTP_FLAG_FIN|FTP_FLAG_SYN|FTP_FLAG_RST|FTP_FLAG_PSH|FTP_FLAG_ACK|FTP_FLAG_URG)
uint8 FtpTxUserName(void)
{
	uint8 i,tmp_len,res=FALSE;
	uint8 tmp_str[256];
	
	tmp_len = StrLen("USER ",0);
	MemCpy(tmp_str,"USER ",tmp_len);
	MemCpy(tmp_str+tmp_len,ftp_struct.user_name,StrLen(ftp_struct.user_name,0));
	tmp_len += StrLen(ftp_struct.user_name,0);
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
	FtpTx(tmp_str,tmp_len,FALSE);
	FtpDelay(10*WAIT_1S,TRUE);
	
	if(!ftp_struct.rx_ctrl_flag)
	{
		goto RETURN_LAB;
	}
	
	i = SubMatch("331",StrLen("331",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///331 User name okay, need password.
	if(i != 0)
	{
		res = TRUE;	
	}
	
RETURN_LAB:
	return res;
}
uint8 FtpTxPassword(void)
{
	uint8 i,tmp_len,res=FALSE;
	uint8 tmp_str[256];
	
	tmp_len = StrLen("PASS ",0);
	MemCpy(tmp_str,"PASS ",tmp_len);
	MemCpy(tmp_str+tmp_len,ftp_struct.pass_word,StrLen(ftp_struct.pass_word,0));
	tmp_len += StrLen(ftp_struct.pass_word,0);
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
	FtpTx(tmp_str,tmp_len,FALSE);
	FtpDelay(10*WAIT_1S,TRUE);
	
	if(!ftp_struct.rx_ctrl_flag)
	{
		goto RETURN_LAB;
	}
	
	i = SubMatch("230",StrLen("230",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///230 User logged in, proceed.
	if(i != 0)
	{
		res = TRUE;	
	}
	
RETURN_LAB:
	return res;
}
uint8 FtpTxCwd(void)///更改目录
{
	uint8 i,res=FALSE;
	uint8 tmp_len,tmp_str[256];
	
	if(StrLen(ftp_struct.file_path,0) > 0)
	{
		ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
		tmp_len = StrLen("CWD ",0);
		MemCpy(tmp_str,"CWD ",tmp_len);
		MemCpy(tmp_str+tmp_len,ftp_struct.file_path,StrLen(ftp_struct.file_path,0));
		tmp_len += StrLen(ftp_struct.file_path,0);
		FtpTx(tmp_str,tmp_len,FALSE);
		
		FtpDelay(5*WAIT_1S,TRUE);
		
		if(!ftp_struct.rx_ctrl_flag)
		{
			goto RETURN_LAB;
		}
		
		i = SubMatch("250",StrLen("250",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///250更改目录成功
		if(i != 0)
		{
			res = TRUE;	
		}
	}
	else
	{
		res = TRUE;
	}
RETURN_LAB:
	return res;
}
uint8 FtpTxTypeI(void)
{
	uint8 i,res=FALSE;
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
	FtpTx("TYPE I\x0d",StrLen("TYPE I\x0d",0),FALSE);
	FtpDelay(10*WAIT_1S,TRUE);
	
	if(!ftp_struct.rx_ctrl_flag)
	{
		goto RETURN_LAB;
	}
	
	i = SubMatch("200",StrLen("200",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///200 Type set to I.
	if(i != 0)
	{
		res = TRUE;	
	}
	
RETURN_LAB:
	return res;
}
uint8 FtpGetSize(void)
{
	uint8 tmp_len,i,res=FALSE;
	uint8 tmp_str[256];
	
	tmp_len = StrLen("SIZE ",0);
	MemCpy(tmp_str,"SIZE ",tmp_len);
	MemCpy(tmp_str+tmp_len,ftp_struct.file_name,StrLen(ftp_struct.file_name,0));
	tmp_len += StrLen(ftp_struct.file_name,0);
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
	FtpTx(tmp_str,tmp_len,FALSE);
	FtpDelay(10*WAIT_1S,TRUE);
	
	if(!ftp_struct.rx_ctrl_flag)
	{
		goto RETURN_LAB;
	}
	
	i = SubMatch("213 ",StrLen("213",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///213 87360
	if(i != 0)
	{
		ftp_struct.ftp_file_total_size = 0;
		for(;(ftp_struct.ftp_rx_buf[i] != 0x0d)&&(i<ftp_struct.ftp_rx_len);i++) 
		{
			ftp_struct.ftp_file_total_size = ftp_struct.ftp_file_total_size*10 + (ftp_struct.ftp_rx_buf[i]&0x0f);
		}
		if(ftp_struct.ftp_file_total_size <= (FLASH_STORE_END_ADDR - FlASH_STORE_START_ADDR - 0x4000))
		{
			res = TRUE;
		}
	}
	
RETURN_LAB:
	return res;
}

uint8 FtpTxPasv(void)
{
	uint8 i,j,tmp_val,res=FALSE;
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
	FtpTx("PASV\x0d",StrLen("PASV\x0d",0),FALSE);
	FtpDelay(10*WAIT_1S,TRUE);
	
	if(!ftp_struct.rx_ctrl_flag)
	{
		goto RETURN_LAB;
	}
	i = SubMatch("227",StrLen("227",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///227 Entering Passive Mode (202,102,90,166,128,0)
	if(i != 0)
	{
		for(i=4;i<ftp_struct.ftp_rx_len;i++)
		{
			if(ftp_struct.ftp_rx_buf[i] == '(')
				break;
		}
		i++;
		j = 0;
		tmp_val = 0;
		for( ;i<ftp_struct.ftp_rx_len;i++)
		{
			if((ftp_struct.ftp_rx_buf[i]==',')||(ftp_struct.ftp_rx_buf[i]==')'))
			{
				if(j >= 4)
				{
					ftp_struct.ftp_dst_data_port[j-4] = tmp_val;
				}
				else
				{
					ftp_struct.ftp_dst_data_ip[j] = tmp_val;
				}
				j++;
				tmp_val = 0x00;
			}
			else 
			{
				tmp_val = tmp_val*10 + (ftp_struct.ftp_rx_buf[i] & 0x0f);
			}
		}
		res = TRUE;	
	}
	
RETURN_LAB:
	return res;
}

uint8 FtpWaitTransferComplete(void)
{
	uint8 i,j,res=FALSE;
	
	ftp_struct.ftp_data_flag = FALSE;
	for(j=0;j<8;j++)
	{
		FtpDelay(3*WAIT_1S,TRUE);
		
		if(ftp_struct.rx_ctrl_flag)
		{
			i = SubMatch("226 Transfer complete",StrLen("226 Transfer complete",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///213 87360
			if(i != 0)
			{
				res = TRUE;
				break;
			}
		}
	}
	
	return res;
}
uint8 FtpGetFile(void)
{
	uint8 tmp_len,res=FALSE;
	uint8 tmp_str[256];
	
	FtpIpPortInit(FALSE);
	
	tmp_len = StrLen("RETR ",0);
	MemCpy(tmp_str,"RETR ",tmp_len);
	MemCpy(tmp_str+tmp_len,ftp_struct.file_name,StrLen(ftp_struct.file_name,0));
	tmp_len += StrLen(ftp_struct.file_name,0);
	
	ftp_struct.ftp_rx_file_byte_counter = 0;
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_PSH;
	FtpTx(tmp_str,tmp_len,FALSE);

	FtpIpPortInit(TRUE);
	
	while(1)
	{
		ftp_struct.rx_data_flag = FALSE;
		FtpDelay(40*WAIT_1S,TRUE);
		if(!ftp_struct.rx_data_flag)
		{
			break;
		}
		if(ftp_struct.ftp_upgrade_fail_flag)
		{
			break;
		}
		if(ftp_struct.ftp_rx_file_byte_counter >= ftp_struct.ftp_file_total_size)
		{
			if(ftp_struct.ftp_rx_file_byte_counter >= ftp_struct.ftp_file_total_size)
			{
				res = TRUE;
			}
			ftp_struct.rx_data_flag = FALSE;
			break;
		}
	}
	
	return res;
}

/***
ftp://Vehicle:Vehicle#*@202.102.090.166:21/THAG_M2_0H_12V13_131220.bin
***/
uint8 FtpAddrAnalysis(uint8 data[],uint16 len)
{
	uint8 *tem_p,*p_file_name,res = FALSE;
  uint8 i,j,file_len;
	
	tem_p = data;

	if(!MemCmp("ftp://",tem_p,StrLen("ftp://",0)))
	{
		goto RETURN_LAB;
	}
	tem_p += StrLen("ftp://",0);
	len -= StrLen("ftp://",0);
	
	for(i=0;i<len;i++)///取用户名
	{
		if(i >= LEN_16-2)
		{
			goto RETURN_LAB;
		}
		if(tem_p[i] == ':')
		{
			break;
		}
		ftp_struct.user_name[i] = tem_p[i];
	}
	ftp_struct.user_name[i++] = 0x0d;
	ftp_struct.user_name[i+1] = '\0';
	tem_p += i;
	len -= i;
				
	for(i=0;i<len;i++)///指向密码
	{
		if(i >= LEN_16-2)
		{
			goto RETURN_LAB;
		}
		if(tem_p[i] == '@')
		{
			break;
		}
		ftp_struct.pass_word[i] = tem_p[i];
	}
	ftp_struct.pass_word[i++] = 0x0d;
	ftp_struct.pass_word[i+1] = '\0';
	tem_p += i;
	len -= i;
	

	if((tem_p[3] != '.')||(tem_p[7] != '.')||
	   (tem_p[11] != '.')||(tem_p[15] != ':'))
	{
		goto RETURN_LAB;
	}

	for(i=0;i<4;i++) 
	{
		ftp_struct.ftp_dst_ctrl_ip[i] = 0;
		for(j=0;j<3;j++) 
		{

			ftp_struct.ftp_dst_ctrl_ip[i] = ftp_struct.ftp_dst_ctrl_ip[i]*10+(tem_p[i*4+j]&0x0f);
		}
	}
	tem_p += 16;
	len -= 16;
	
	if((tem_p[0] != '2')||(tem_p[1] != '1')||(tem_p[2] != '/'))
	{
		goto RETURN_LAB;
	}
	ftp_struct.ftp_dst_ctrl_port[0] = 0;
	ftp_struct.ftp_dst_ctrl_port[1] = 21;///控制端口都为21
	len = len-3;
	tem_p += 3;
	
	for(i=len-1;i>0;i--)
	{
		if(tem_p[i] == '/')
		{
			file_len = len-1-i;
			p_file_name = tem_p+i+1;
			break;
		}
	}
	
	ftp_struct.file_path[0] = '\0';
	if(i!=0)
	{
		if((i+1) >= (LEN_64-2))
		{
			goto RETURN_LAB;
		}
		
		MemCpy(ftp_struct.file_path,tem_p,i+1);
		ftp_struct.file_path[i+1] = 0x0d;
		ftp_struct.file_path[i+2] = '\0';
	}
	else
	{
		file_len = len;
		p_file_name = tem_p;
	}
	
	if((file_len == 0)||(file_len >= LEN_64-2))
	{
		goto RETURN_LAB;
	}
	MemCpy(ftp_struct.file_name,p_file_name,file_len);
	ftp_struct.file_name[file_len] = 0x0d;
	ftp_struct.file_name[file_len+1] = '\0';
		
	res = MemCmp(ftp_struct.file_name,(uint8*)sys_const_para_struct.software_version,15);

RETURN_LAB:
	return res;
}
void FtpTerminate(void)
{
	FtpIpPortInit(TRUE);///数据端口
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_FIN;
	FtpTx(NULL,0,TRUE);
	FtpDelay(5*WAIT_1S,TRUE);
	
	FtpIpPortInit(FALSE);///控制端口
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK|FTP_FLAG_FIN;
	FtpTx(NULL,0,FALSE);
	FtpDelay(5*WAIT_1S,TRUE);
}
uint8 FtpConnect(uint8 is_data_port_flag)
{
	uint8 i,j,res = FALSE;
	
	for(j=0;j<3;j++)
	{
		FtpIpPortInit(is_data_port_flag);///初始化端口
		
		ftp_struct.tcp_tx_flag_byte = FTP_FLAG_SYN;
		FtpTx(NULL,0,is_data_port_flag);
		
		if(!is_data_port_flag)
		{
			FtpDelay(10*WAIT_1S,TRUE);
			if(!ftp_struct.rx_ctrl_flag)
			{
				continue;
			}
			///220 Serv-U FTP Server v6.3 for WinSock ready...
			i = SubMatch("220",StrLen("220",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);///220 Serv-U FTP Server v6.3 for WinSock ready...
			if(i != 0)
			{
				res = TRUE;
				break;
			}
		}
		else
		{
			FtpDelay(5*WAIT_1S,TRUE);
			res = TRUE;
			break;
		}
	}

	return res;
}
void FtpIpPortInit(uint8 is_data_port_flag)
{
	if(is_data_port_flag)
	{
		
		ftp_struct.ftp_src_data_port[0] = 0x04;
		ftp_struct.ftp_src_data_port[1] = 0x02;
		
		MemCpy(g_ppp_struct.ppp_src_port,ftp_struct.ftp_src_data_port,2);
		MemCpy(g_ppp_struct.ppp_dst_port,ftp_struct.ftp_dst_data_port,2);
		MemCpy(g_ppp_struct.ppp_dst_ip,ftp_struct.ftp_dst_data_ip,4);
	}
	else
	{
		ftp_struct.ftp_src_ctrl_port[0] = 0x04;
		ftp_struct.ftp_src_ctrl_port[1] = 0x01;	
		
		MemCpy(g_ppp_struct.ppp_src_port,ftp_struct.ftp_src_ctrl_port,2);
		MemCpy(g_ppp_struct.ppp_dst_port,ftp_struct.ftp_dst_ctrl_port,2);
		MemCpy(g_ppp_struct.ppp_dst_ip,ftp_struct.ftp_dst_ctrl_ip,4);
	}
}
void FtpDelay(uint16 delay_sec,uint8 wait_ack_flag)
{
	uint8 past_sec = 0;
	uint16 rtc_cur_val,rtc_past_val;
	
	rtc_past_val = sys_misc_run_struct.sys_rtc_sec_counter;
	while((past_sec <= delay_sec))
	{
		DriverMain();
		FeedWtd();
		rtc_cur_val = sys_misc_run_struct.sys_rtc_sec_counter;
		past_sec = (rtc_cur_val + (65535 - rtc_past_val)) % 65535;
		
		if(wait_ack_flag)///	是否等待响应
		{
			if(g_gprs_uart_struct.rx_flag == VALID_VAL_AA)
			{
				PppMainProcess();
			}
			if(((ftp_struct.ftp_data_flag)&&(ftp_struct.rx_data_flag))||
			   ((!ftp_struct.ftp_data_flag)&&(ftp_struct.rx_ctrl_flag)))
			{
				break;
			}
		}
	}
}
void FtpTx(u8 data[],u16 len,uint8 is_data_port_flag)///FTP
{
	uint8 *p,tmp_data[256];
	uint16	i,check_val;

	ftp_struct.ftp_rx_len = 0x00;
	ftp_struct.tcp_rx_flag_byte = 0x00;
	ftp_struct.rx_data_flag = FALSE;
	ftp_struct.rx_ctrl_flag = FALSE;
	p = tmp_data;

	*p++ = g_ppp_struct.ppp_src_port[0];
	*p++ = g_ppp_struct.ppp_src_port[1];
	*p++ = g_ppp_struct.ppp_dst_port[0];
	*p++ = g_ppp_struct.ppp_dst_port[1];
	
	if(is_data_port_flag)
	{
		Uint32ToU8(p,ftp_struct.ftp_dst_data_next_rx_seq);
		Uint32ToU8(p+4,ftp_struct.ftp_dst_data_cur_tx_seq);
	}
	else
	{
		Uint32ToU8(p,ftp_struct.ftp_dst_ctrl_next_rx_seq);
		Uint32ToU8(p+4,ftp_struct.ftp_dst_ctrl_cur_tx_seq);
	}
	p += 8;
	
	if(ftp_struct.tcp_tx_flag_byte == 0x02)
	{
		*p++ = 0x60;
	}
	else
	{
		*p++ = 0x50;
	}
	
	*p++ = ftp_struct.tcp_tx_flag_byte;
	
	*p++ = (GPRS_UART_BUF_LEN >> 1) >> 8;///窗口大小
	*p++ = (GPRS_UART_BUF_LEN >> 1) & 0xff;
	
	*p++ = 0x00;///校验和
	*p++ = 0x00;				
	*p++ = 0x00;///紧急指针
	*p++ = 0x00;
	
	if(ftp_struct.tcp_tx_flag_byte == 0x02)
	{
		*p++ = 0x02;
		*p++ = 0x04;
		
		*p++ = (GPRS_UART_BUF_LEN >> 3) >> 8;///最大接收数据单元
		*p++ = (GPRS_UART_BUF_LEN >> 3) & 0xff;
	}
	
	i = p - tmp_data;
	
	memcpy(tmp_data+i,data,len);
	len += i;
	
	check_val = TcpUdpHeadCheck(tmp_data,len,TCP_TYPE);
	tmp_data[16] = check_val >> 8;
	tmp_data[17] = check_val;
	
	IpTx(tmp_data,len,TCP_TYPE);
}
void FtpFileSave(void)
{
	uint8 res;
	
	if((ftp_struct.ftp_rx_file_byte_counter + ftp_struct.ftp_rx_len) < ftp_struct.ftp_file_total_size)
	{
		if(ftp_struct.ftp_rx_len % 2)
		{
			ftp_struct.ftp_rx_len = ftp_struct.ftp_rx_len -1;
			ftp_struct.ftp_rx_file_odd_byte_flag = TRUE;
		}
		else
		{
			ftp_struct.ftp_rx_file_odd_byte_flag = FALSE;
		}
	}
	res = FlashWrite(FlASH_STORE_START_ADDR+ftp_struct.ftp_rx_file_byte_counter,ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);
	
	if(!res)
	{
		ftp_struct.ftp_upgrade_fail_flag = TRUE;
	}
	
	if(ftp_struct.ftp_rx_file_odd_byte_flag)
	{
		ftp_struct.ftp_rx_buf[0] = ftp_struct.ftp_rx_buf[ftp_struct.ftp_rx_len];///保存剩余字节
	}
	
	ftp_struct.ftp_rx_file_byte_counter += ftp_struct.ftp_rx_len;
	
	LocalCommFtpState(0,ftp_struct.ftp_file_total_size,ftp_struct.ftp_rx_file_byte_counter);
}
void FtpProcess(uint8 data[],uint16 len)
{
	uint8 h_len,rx_data_flag = FALSE,not_ack_flag = FALSE;
	uint8 bias_addr;
	uint16 rx_len;

	if(!ftp_struct.ftp_upgrade_flag)
	{
		goto RETURN_LAB;
	}

	if(MemCmp(data+2,ftp_struct.ftp_src_data_port,2))
	{
		rx_data_flag = TRUE;
	}
	else if(MemCmp(data+2,ftp_struct.ftp_src_ctrl_port,2))
	{
	
	}
	else
	{
		goto RETURN_LAB;
	}
	
	h_len = data[12] >> 2;
	ftp_struct.tcp_rx_flag_byte = data[13] & FTP_FLAG_MASK;
	if(rx_data_flag)
	{
		ftp_struct.ftp_dst_data_cur_tx_seq = U8ToUint32(data+4);
		ftp_struct.ftp_dst_data_next_rx_seq = U8ToUint32(data+8);
	}
	else
	{
		ftp_struct.ftp_dst_ctrl_cur_tx_seq = U8ToUint32(data+4);
		ftp_struct.ftp_dst_ctrl_next_rx_seq = U8ToUint32(data+8);
	}
	
	rx_len = len - h_len;
	if((ftp_struct.tcp_rx_flag_byte & (FTP_FLAG_SYN|FTP_FLAG_PSH|FTP_FLAG_FIN))||
	  (rx_len >0))
	{
		if(rx_data_flag)
		{
			if(rx_len == 0)
			{
				ftp_struct.ftp_dst_data_cur_tx_seq += 1;
			}
			else
			{
				if(ftp_struct.ftp_rx_file_byte_counter == 0)
				{
					ftp_struct.ftp_dst_data_last_tx_seq = ftp_struct.ftp_dst_data_cur_tx_seq;
				}
				ftp_struct.ftp_dst_data_cur_tx_seq += rx_len;
			}
		}
		else
		{
		
			if(rx_len == 0)
			{
				ftp_struct.ftp_dst_ctrl_cur_tx_seq += 1;
			}
			else
			{
				ftp_struct.ftp_dst_ctrl_cur_tx_seq += rx_len;
			}
		}
	}

	if(rx_len > 0)
	{
		bias_addr = 0;
		if(ftp_struct.ftp_rx_file_odd_byte_flag)
		{
			bias_addr = 1;
			ftp_struct.ftp_rx_len = rx_len+1;
		}
		else
		{
			ftp_struct.ftp_rx_len = rx_len;
		}
		MemCpy(ftp_struct.ftp_rx_buf+bias_addr,data + h_len,rx_len);

		if(rx_data_flag)
		{
			ftp_struct.rx_data_flag = TRUE;
			if(ftp_struct.ftp_rx_file_byte_counter < ftp_struct.ftp_file_total_size)
			{
				if(ftp_struct.ftp_dst_data_last_tx_seq + rx_len == ftp_struct.ftp_dst_data_cur_tx_seq)
				{
					if(rx_len != (GPRS_UART_BUF_LEN >> 3))
					{
						if(ftp_struct.ftp_rx_file_byte_counter + rx_len  < ftp_struct.ftp_file_total_size)
						{
							not_ack_flag = TRUE;
						}
					}
					
					if(!not_ack_flag)
					{
						FtpFileSave();///保存升级文件
						ftp_struct.ftp_dst_data_last_tx_seq = ftp_struct.ftp_dst_data_cur_tx_seq;
					}
				}
				else if(ftp_struct.ftp_dst_data_last_tx_seq + rx_len >= ftp_struct.ftp_dst_data_cur_tx_seq)
				{
				
				}
				else
				{
					not_ack_flag = TRUE;
				}
			}
		}
		else
		{
			ftp_struct.rx_ctrl_flag = TRUE;
		}
	}

	if(not_ack_flag)
	{
		goto RETURN_LAB;
	}
	
	if(rx_data_flag)
	{
		FtpIpPortInit(TRUE);
	}
	else
	{
		FtpIpPortInit(FALSE);
		
	}
	
	ftp_struct.tcp_tx_flag_byte = FTP_FLAG_ACK;
	FtpTx(NULL,0,rx_data_flag);

	if(rx_len > 0)
	{
		if(rx_data_flag)
		{
			ftp_struct.rx_data_flag = TRUE;
		}
		else
		{
			ftp_struct.rx_ctrl_flag = TRUE;
			ftp_struct.ftp_rx_len = rx_len;
		}
	}
	
RETURN_LAB:
	return;
}
typedef union
{
	uint8 buff[4];
	uint16 u16_data;
	uint32 u32_data;
} LONG_TypeDef;

void ThagFtpFun(void)///兼容天昊的BOOT远程升级应用程序
{
	uint8 j,tmp_data[50];
	uint16 crc_chk,len,page_addr;
	uint32 addr_counter,rest_len;
	LONG_TypeDef long_structure;
	
	addr_counter = FlASH_STORE_START_ADDR;
	rest_len = ftp_struct.ftp_file_total_size;
	crc_chk = 0x0000;
	while(rest_len)
	{
		long_structure.u32_data = *(u32*)addr_counter;
		if(rest_len < 4)
		{
			j = (u8)rest_len;
		}
		else 
		{
			j = 4;
		}
		
		crc_chk = Crc16Verify(crc_chk,long_structure.buff,j);
	
		if(j != 4)
		{
			break;
		}
		addr_counter += 4;
		rest_len -= 4;
	}

	tmp_data[0] = 0xAA;
	tmp_data[1] = 0x55;
	tmp_data[2] = 0xA5;
	page_addr = (ftp_struct.ftp_file_total_size >> 10);
	page_addr++;
	len = ftp_struct.ftp_file_total_size % 1024;
	tmp_data[3] = (uint8)(page_addr >> 8);
	tmp_data[4] = (uint8)page_addr;
	tmp_data[5] = (uint8)(len >> 8);
	tmp_data[6] = (uint8)(len);
	tmp_data[7] = (uint8)(crc_chk >> 8);
	tmp_data[8] = (uint8)(crc_chk);
	SpiFramWrite(2049,tmp_data,9);
	tmp_data[0] = 0x11;
	tmp_data[1] = 0x22;
	SpiFramWrite(2047,tmp_data,2);
	
}
uint8 FtpFun(uint8 pfun(void))
{
	uint8 i,res,ret_res = FALSE;
	
	for(i=0;i<3;i++)
	{
		res = pfun();
		if(res)
		{
			ret_res = TRUE;
			break;
		}
	}

	return ret_res;	
}
uint8 FtpMain(void)
{
	uint8 i,res = FALSE;
	uint8 total_page;
	uint16 bias_addr;
	
	OFF_CAN_PWR();
	
	LocalCommFtpState(0,0,0);
	
	sys_work_para_struct.lsnal_head_page = 0x00;///清盲区
	sys_work_para_struct.lsnal_tail_page = 0x00;
	
	bias_addr = ((uint8*)&sys_work_para_struct.lsnal_head_page) - ((uint8*)sys_work_para_struct.acc_on_sec_statistic_counter);
	SpiFramWrite(SYS_WORK_PARA_STRUCT_START_ADDR+bias_addr,(uint8*)&sys_work_para_struct.lsnal_head_page,2);

	
	NVIC_DISABLE();
	total_page = FLASH_LSNAL_MAX_PAGES/4;
	for(i=0;i<total_page;i++)///擦除文件存储区
	{
		FeedWtd();
		if(!FlashErase(FlASH_STORE_START_ADDR+i*ST32_PAGE_SIZE))
		{
			break;
		}
	}
	NVIC_ENABLE();
	
	if(i != total_page)
	{
		goto RETURN_LAB;
	}
	
	ftp_struct.ftp_upgrade_fail_flag = FALSE;
	ftp_struct.ftp_rx_file_byte_counter = 0;
	ftp_struct.ftp_upgrade_flag = TRUE;
	ftp_struct.ftp_data_flag = FALSE;
	ftp_struct.ftp_dst_ctrl_next_rx_seq = 0x00;
	ftp_struct.ftp_dst_data_next_rx_seq = 0x00;
	
	res = FtpConnect(FALSE);///建立TCP控制连接
	if(!res)
	{
		goto RETURN_LAB;
	}

	res = FtpFun(FtpTxUserName);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	res = FtpFun(FtpTxPassword);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	res = FtpFun(FtpTxCwd);
	if(!res)
	{
		goto RETURN_LAB;
	}
	res = FtpFun(FtpTxTypeI);
	if(!res)
	{
		goto RETURN_LAB;
	}
	res = FtpFun(FtpGetSize);
	if(!res)
	{
		goto RETURN_LAB;
	}
	res = FtpFun(FtpTxPasv);
	if(!res)
	{
		goto RETURN_LAB;
	}
	ftp_struct.ftp_data_flag = TRUE;
	ftp_struct.ftp_rx_file_odd_byte_flag = FALSE;
	res = FtpConnect(TRUE);///建立TCP数据连接
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	res = FtpGetFile();
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	sys_boot_para_struct.program_update_flag = VALID_VAL_DWORD_AA;
	sys_boot_para_struct.sys_para_init_flag = VALID_VAL_DWORD_AA;
	sys_boot_para_struct.program_total_size = ftp_struct.ftp_file_total_size;
	res = SysBootParaWrite();
	
RETURN_LAB:
	ON_CAN_PWR();
	if(res)
	{
		ThagFtpFun();
	}
	FtpTerminate();
	ftp_struct.ftp_upgrade_flag = FALSE;
	
	return res;
}

