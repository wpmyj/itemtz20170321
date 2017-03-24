
#define PPP_STRUCT_GLOBAL
#include "tiza_include.h"


void PppInit(void)
{	
	uint8 lcp_data[] = {0x01,0x01,0x00,0x0E,0x02,0x06,0x00,0x00,0x00,0x00,0x07,0x02,0x08,0x02};
	uint8 ncp_data[] = {0x01,0x01,0x00,0x10,0x03,0x06,0x00,0x00,0x00,0x00,0x81,0x06,0x00,0x00,0x00,0x00};

	if(g_ppp_struct.lcp_req_flag)
	{		
		LocalDebug("GSM ppp init...\r\n",StrLen("GSM ppp init...\r\n",0),LOCAL_TEST_DEBUG);
		g_ppp_struct.lcp_req_flag = FALSE;
		g_ppp_struct.lcp_up_ack_data[0] = 0;
		g_ppp_struct.ppp_tx_seq = 1;
		lcp_data[1] = g_ppp_struct.ppp_tx_seq;
		PppLcpReq(lcp_data,sizeof(lcp_data));
	}
	if(g_ppp_struct.pap_req_flag)
	{
		g_ppp_struct.ppp_tx_seq++;
		PppPapReq(g_ppp_struct.ppp_tx_seq);
		GprsDelay(WAIT_1S,TRUE);
	}
	if(g_ppp_struct.ncp_req_flag)
	{
		g_ppp_struct.ppp_tx_seq = 1;
		ncp_data[1] = g_ppp_struct.ppp_tx_seq;
		PppNcpReq(ncp_data,sizeof(ncp_data));
		GprsDelay(WAIT_1S,TRUE);
	}
	if(g_ppp_struct.ppp_tx_seq >= 25)
	{
		gsm_misc_struct.cur_mode = POWER_INIT_MODE;
	}
}
void PppNcpProcess(uint8 data[],uint16 len)
{
	if(data[0] == 0x01)///远端req
	{
		g_ppp_struct.ncp_req_flag = FALSE;///不处理
		data[0] = 0x02;
		PppNcpAck(data,len);
	}
	else if(data[0] == 0x03)///远端nak
	{
		MemCpy(udp_struct.udp_src_ip,data+6,4);
		if(len >= 16)
		{
			MemCpy(g_ppp_struct.dns_ip,data+12,4);
		}
		data[0] = 0x01;
		data[1] = g_ppp_struct.ppp_tx_seq++;
		PppNcpReq(data,len);
	}
	else if(data[0] == 0x02)///远端ack
	{
		if(!g_ppp_struct.ncp_ok_flag)
		{
			gsm_misc_struct.ppp_monitor_falg = FALSE;
			gsm_misc_struct.gsm_mode_exe_flag[2] = AT_EXE_SUCESS;
			gsm_misc_struct.gsm_mode_exe_flag[3] = AT_NOT_EXE;
			g_ppp_struct.ncp_ok_flag = TRUE;
			LocalDebug("GSM ppp success\r\n",StrLen("GSM ppp success\r\n",0),LOCAL_TEST_DEBUG);
			g_ppp_struct.sms_ack_flag[2] = TRUE;
			
			gsm_misc_struct.cur_mode = UDP_MODE;
			
			udp_struct.udp_src_port[0] = 0x1e;///默认7897
			udp_struct.udp_src_port[1] = 0xd9;
			
			MemCpy(udp_struct.udp_dst_port,sys_private_para_struct.port,2);
				
			if(pro_struct.link_center_ip_index == SLAVER_IP_INDEX)
			{
				MemCpy(udp_struct.udp_dst_ip,sys_private_para_struct.slaver_ip_dns+1,4);
				MemCpy(sys_misc_run_struct.link_center_ip,sys_private_para_struct.slaver_ip_dns+1,4);
			}
			else
			{
				pro_struct.link_center_ip_index = MASTER_IP_INDEX;
				MemCpy(udp_struct.udp_dst_ip,sys_private_para_struct.master_ip_dns+1,4);
				MemCpy(sys_misc_run_struct.link_center_ip,sys_private_para_struct.master_ip_dns+1,4);
				pro_struct.link_master_ip_counter = 1;
			}
			
			UdpIpPortInit();	
			ProUpAquireCommPara();
		}
	}
	else if(data[0] == 0x04)///远端nak
	{
		PppNcpNak(g_ppp_struct.ppp_tx_seq);
	}
}
void PppPapProcess(uint8 data[],uint16 len)
{
	if(data[0] == 0x02)///LCP请求
	{
		g_ppp_struct.sms_ack_flag[1] = TRUE;
		g_ppp_struct.pap_req_flag = FALSE;
		g_ppp_struct.ncp_req_flag = TRUE;
	}
}
void PppLcpProcess(uint8 data[],uint16 len)
{
	uint16 i;
	uint8 lcp_data[] = {0x01,0x01,0x00,0x0E,0x02,0x06,0x00,0x00,0x00,0x00,0x07,0x02,0x08,0x02};
	
	if(data[0] == 0x01)///LCP请求
	{
		for(i=5;i<len;i++)
		{
			if((0x03 == data[i])&&(0xC2 == data[i+2])&&(0x23 == data[i+3]))///不支持CHAP认证
			{
				PppLcpNak(data[1]);
				goto RETURN_LAB;
			}
		}
		if(i == len)///上行肯定应答
		{
			if(len < 30 -1)
			{
				g_ppp_struct.lcp_up_ack_data[0] = len;
				MemCpy(g_ppp_struct.lcp_up_ack_data+1,data,len);
			}
			data[0] = 0x02;
			PppLcpAck(data,len);
		}
	}
	else if(data[0] == 0x02)///LCP应答
	{
		g_ppp_struct.pap_req_flag = TRUE;
		g_ppp_struct.sms_ack_flag[0] = TRUE;
		GprsDelay(3*WAIT_1S,TRUE);
	}
	else if(data[0] == 0x03)///否定应答
	{
		g_ppp_struct.ppp_tx_seq++;
		lcp_data[1] = g_ppp_struct.ppp_tx_seq;
		MemCpy(lcp_data+6,data+6,4);
		PppLcpReq(lcp_data,sizeof(lcp_data));
	}
	else if(data[0] == 0x04)///LCP REJECT
	{
		if(g_ppp_struct.lcp_up_ack_data[0] != 0x00)
		{
			g_ppp_struct.lcp_up_ack_data[1] = 0x01;
			g_ppp_struct.ppp_tx_seq++;
			g_ppp_struct.lcp_up_ack_data[2] = g_ppp_struct.ppp_tx_seq;
			PppLcpReq(g_ppp_struct.lcp_up_ack_data+1,g_ppp_struct.lcp_up_ack_data[0]);
		}
		else
		{
			gsm_misc_struct.cur_mode = POWER_INIT_MODE;
		}
	}
	else if(data[0] == 0x05)///LCP链路终止
	{
		data[0] = 0x06;
		PppLcpTerminateAck(data,len);
	}
RETURN_LAB:
	return;
}

uint16 PppPacketDecode(uint8 data[],uint16 len)///ppp包解码
{
	#ifdef PPP_DEBUG
	uint8 str_len;
	char str_ch[20];
    #endif
	u16 i,j;
	
	j = 1;
	for(i=1;i<len-1;i++)
	{
		if(0x7d == data[i])
		{
			i++;
			data[j] = data[i] ^ 0x20;
		}
		else 
		{
			data[j] = data[i];
		}
		j++;
	}
	data[j++] = 0x7e;

	#ifdef PPP_DEBUG
		str_len = sprintf(str_ch,"%s ","下行：");
		LocalDebug((uint8*)str_ch,str_len,LOCAL_PPP_DEBUG);
		for(i=0;i<j;i++)
		{
			str_len = sprintf(str_ch,"%02X ",data[i]);
			LocalDebug((uint8*)str_ch,str_len,LOCAL_PPP_DEBUG);
		}
		LocalDebug("\r\n",2,LOCAL_PPP_DEBUG);
	#endif

	return j;
}

void PppLcpReq(uint8 lcp_data[],uint8 len)
{
	PppTx(lcp_data,len,LCP_PRO_TYPE);
}
void PppLcpNak(uint8 seq)
{
	uint8 lcp_data[] = {0x03,0x01,0x00,0x08,0x03,0x04,0xC0,0x23};
	
	lcp_data[1] = seq;
	PppTx(lcp_data,sizeof(lcp_data),LCP_PRO_TYPE);
}
void PppLcpAck(uint8 data[],uint16 len)
{
	PppTx(data,len,LCP_PRO_TYPE);
}
void PppLcpTerminate(void)
{
	uint8 lcp_data[] = {0x05,0x03,0x00,0x04};
	
	PppTx(lcp_data,sizeof(lcp_data),LCP_PRO_TYPE);
}
void PppLcpTerminateAck(uint8 data[],uint16 len)
{	
	PppTx(data,len,LCP_PRO_TYPE);
}
void PppPapReq(uint8 seq)
{	
	u8 len,len_1,len_2;
	u8 *p,tmp_data[256];
	
	p = tmp_data;
	*p++ = 0x01; 
	*p++ = seq; 
		
	len_1 = StrLen(sys_private_para_struct.gprs_login_user_name,LEN_32);
	len_2 = StrLen(sys_private_para_struct.gprs_login_password,LEN_32);
	
	*p++ = 0x00; ///长度,高字节
	*p++ = len_1+len_2+6; 	
	*p++= len_1;
	MemCpy(p,sys_private_para_struct.gprs_login_user_name,len_1);
	p += len_1;
	*p++= len_2;
	MemCpy(p,sys_private_para_struct.gprs_login_password,len_2);
	p += len_2;
	len = p - tmp_data;
	
	PppTx(tmp_data,len,PAP_PRO_TYPE);
}

void PppNcpReq(uint8 data[],uint8 len)///获取IP地址,DNS地址
{
	PppTx(data,len,NCP_PRO_TYPE);
}
void PppNcpAck(uint8 data[],uint16 len)
{
	PppTx(data,len,NCP_PRO_TYPE);
}
void PppNcpNak(uint8 seq)///获取IP地址
{
	uint8 tmp_data[] = {0x01,0x01,0x00,0x0A,0x03,0x06,0x00,0x00,0x00,0x00};

	tmp_data[1] = seq;
	MemCpy(tmp_data+6,udp_struct.udp_src_ip,4);
	PppTx(tmp_data,sizeof(tmp_data),NCP_PRO_TYPE);
}
void UdpIpPortInit(void)
{
	MemCpy(g_ppp_struct.ppp_src_port,udp_struct.udp_src_port,2);
	MemCpy(g_ppp_struct.ppp_dst_port,udp_struct.udp_dst_port,2);
	MemCpy(g_ppp_struct.ppp_src_ip,udp_struct.udp_src_ip,4);
	MemCpy(g_ppp_struct.ppp_dst_ip,udp_struct.udp_dst_ip,4);
}

uint16 TcpUdpHeadCheck(u8 data[],uint16 len,uint8 frame_type)///TCP,UDP包头校验
{
	uint16 i,check_val;
	uint32 check_sum;
	
	check_sum = 0;
	
	for(i=0;i<2;i++)
	{
		check_val = g_ppp_struct.ppp_src_ip[2*i] << 8;
		check_val += g_ppp_struct.ppp_src_ip[2*i + 1];
		check_sum += check_val;
		check_val = g_ppp_struct.ppp_dst_ip[2*i] << 8;
		check_val += g_ppp_struct.ppp_dst_ip[2*i + 1];
		check_sum += check_val;
	}
	check_sum += frame_type;
	check_sum += len;
	
	for(i=0;i<len/2;i++)
	{
		check_val = data[2*i] << 8;
		check_val += data[2*i + 1];
		check_sum += check_val;
	}
	
	if(len%2)
	{
		check_sum += data[len-1] << 8;
	}
	
	check_val = check_sum >> 16;
	check_val += check_sum;
	check_val ^= 0xffff;

	return check_val;
}
void PppPacketEncode(uint8 tx_data[],uint16 len)///ppp包编码
{
	uint16 i,j=1;
	uint8 *p,tmp_data[2*PRO_MAX_TX_BUF_COUNTER];
	#ifdef PPP_DEBUG
	uint8 str_len;
	char str_ch[20];
	#endif	
	p = tmp_data;
	
	#ifdef PPP_DEBUG

		str_len = sprintf(str_ch,"%s ","上行：");
		LocalDebug((uint8*)str_ch,str_len,LOCAL_PPP_DEBUG);
		
		str_len = sprintf(str_ch,"%2X ",0x7e);
		LocalDebug((uint8*)str_ch,str_len,LOCAL_PPP_DEBUG);
		for(i=0;i<len;i++)
		{
			str_len = sprintf(str_ch,"%02X ",tx_data[i]);
			LocalDebug((uint8*)str_ch,str_len,LOCAL_PPP_DEBUG);
		}
		str_len = sprintf(str_ch,"%2X\r\n",0x7e);
		LocalDebug((uint8*)str_ch,str_len,LOCAL_PPP_DEBUG);
		
		LocalDebug("\r\n",2,LOCAL_PPP_DEBUG);
	#endif
		
	*p++ = 0x7E;
	for(i=0;i<len;i++)
	{
		if ((tx_data[i] == 0x7e)||(tx_data[i]== 0x7d)||(tx_data[i] < 0x20))
		{
			*p++ = 0x7d;
			*p++ = tx_data[i] ^ 0x20;
			j += 2;
		}
		else
		{
			*p++ = tx_data[i];
			j += 1;
		}
	}
	*p = 0x7e;
	j += 1;

	GprsUartFixedLenSend(tmp_data,j);
}
void PppTx(uint8 tx_data[],uint16 len,uint16 pro_type)
{
	uint16 check_val;
	uint8 *p,tmp_data[PRO_MAX_TX_BUF_COUNTER];
	
	p = tmp_data;
	*p++ = 0Xff;
	*p++ = 0X03;
	*p++ = pro_type >> 8;
	*p++ = pro_type;
	memcpy(p,tx_data,len);
	check_val = CrcCheck(tmp_data,len+4);	///加CRC校验,低位在前，高位在后
	p += len;
	*p++ = check_val;				///低字节
	*p++ = check_val >> 8;			///高字节
	
	len = p-tmp_data;
	PppPacketEncode(tmp_data,len);
}
void IpTx(uint8 tx_data[],u16 len,u8 type)///IP组包
{
	static uint16 tx_counter = 0;
	
	u16 i,check_val;
	u8 tmp_data[PRO_MAX_TX_BUF_COUNTER],*p;
	
	p = tmp_data;

	*p++ = 0x45;///其中4代表旧版本号,5表示首部长度
	*p++ = 0x00;///服务类型
	*p++ = (len + 20) >> 8;///包长度,从0x45,至dst_ip尾
	*p++ = len + 20;

	*p++ = tx_counter >> 8;
	*p++ = tx_counter++;

	*p++ = 0;///2字节,3位的标志+13位的片偏移
	*p++ = 0;
	*p++ = 0x80;///TTL
	*p++ = type;///协议类型,0X11表示UDP；0X06表示TCP；0x01,代表ICMP
	
	*p++ = 0;///校验2字节
	*p++ = 0;
	
	for(i=0;i<4;i++)
	{
		*p++ = g_ppp_struct.ppp_src_ip[i];
	}
	
	for(i=0;i<4;i++)
	{
		*p++ = g_ppp_struct.ppp_dst_ip[i];
	}
	
 	check_val = IpHeadCheck(tmp_data,20);
	tmp_data[10] = check_val >> 8;
	tmp_data[11] = check_val;
	
	MemCpy(tmp_data+20,tx_data,len);
	PppTx(tmp_data,len+20,IP_PRO_TYPE);
}
void UdpTx(uint8 tx_data[],u16 tx_len)///UDP组包
{
	uint8 *p,tmp_data[PRO_MAX_TX_BUF_COUNTER];
	u16	check_val;
	
	p = tmp_data;
	*p++ = g_ppp_struct.ppp_src_port[0];
	*p++ = g_ppp_struct.ppp_src_port[1];
	*p++ = g_ppp_struct.ppp_dst_port[0];
	*p++ = g_ppp_struct.ppp_dst_port[1];
	tx_len += 8;
	*p++ = tx_len >> 8;
	*p++ = tx_len;
	*p++ = 0x00;
	*p++ = 0x00;
	memcpy(p,tx_data,tx_len);
	
	check_val = TcpUdpHeadCheck(tmp_data,tx_len,UDP_TYPE);
	
	tmp_data[6] = check_val >> 8;
	tmp_data[7] = check_val;
	
	IpTx(tmp_data,tx_len,UDP_TYPE);
}
uint16 IpHeadCheck(u8 data[],uint16 len)///IP包头校验
{
	uint16 i,check_val;
	uint32 check_sum;
	
	check_sum = 0;
	
	len /= 2;
	
	for(i=0;i<len;i++)
	{
		check_val = data[2*i] << 8;
		check_val += data[2*i + 1];
		check_sum += check_val;
	}
	
	check_val = check_sum >> 16;
	check_val += check_sum;
	check_val ^= 0xffff;

	return check_val;
}
void UdpProcess(uint8 data[],uint16 len)
{
	ProProcess(data,len);
}
void TcpProcess(uint8 data[],uint16 len)
{
}

void PppMainProcess(void)///ppp主要处理
{
	uint8 rx_data[GPRS_UART_BUF_LEN];
	uint8 type_index_bias;
	uint16 rx_len=0;
	uint16 tmp_len,crc_val,check_val;
	uint8 gprs_disconnet[] = {"NO CARRIER"},cpm_len,mat_index;
	
	if(g_gprs_uart_struct.rx_flag != VALID_VAL_AA)
	{
		goto RET_LAB;
	}
	
	rx_len = GetGprsUartRxData(rx_data);
	if(rx_len == 0)
	{
		goto RET_LAB;
	}
	if(rx_len == 2)
	{
		if((rx_data[0] == 0x7e)&&(rx_data[1] == 0x7e))
		{
			GprsUartParaInit();
			goto RET_LAB;
		}
	}
	
	if(rx_data[0] != 0x7e)
	{
		cpm_len = StrLen(gprs_disconnet,0);
		
		if(rx_len >= cpm_len)
		{
			mat_index = SubMatch(gprs_disconnet,cpm_len,rx_data,rx_len);
			if(mat_index > 0)
			{
				gsm_misc_struct.cur_mode = POWER_INIT_MODE;///Gsm模块重启
			}
		}
		goto RET_LAB;
	}
	
	tmp_len = PppPacketDecode(rx_data,rx_len);
		
	if(tmp_len < 4)
	{	
		goto RET_LAB;
	}
	crc_val = rx_data[tmp_len-2];
	crc_val = (crc_val << 8)+rx_data[tmp_len-3];
	check_val = CrcCheck(rx_data+1,tmp_len-4);
	if(crc_val != check_val)
	{
		GprsUartParaInit();
		goto RET_LAB;
	}
	
	if((rx_data[1] == 0xFF)&&(rx_data[2] == 0x03))
	{
		type_index_bias = 0;
	}
	else 
	{
		type_index_bias = 2;
	}
	
	if(rx_data[1] == 0x21)
	{
		type_index_bias = 3;
	}
	
	if((rx_data[PPP_PRO_TYPE_INDEX-type_index_bias] == 0xc0)&&(rx_data[PPP_PRO_TYPE_INDEX+1-type_index_bias] == 0x21))
	{
		if(ftp_struct.ftp_upgrade_flag)
		{
			goto RET_LAB;
		}
		PppLcpProcess(rx_data+PPP_LCP_TYPE_INDEX-type_index_bias,tmp_len-8+type_index_bias);
	}
	else if((rx_data[PPP_PRO_TYPE_INDEX-type_index_bias] == 0xc0)&&(rx_data[PPP_PRO_TYPE_INDEX+1-type_index_bias] == 0x23))
	{
		if(ftp_struct.ftp_upgrade_flag)
		{
			goto RET_LAB;
		}
		PppPapProcess(rx_data+PPP_LCP_TYPE_INDEX-type_index_bias,tmp_len-8+type_index_bias);
	}
	else if((rx_data[PPP_PRO_TYPE_INDEX-type_index_bias] == 0x80)&&(rx_data[PPP_PRO_TYPE_INDEX+1-type_index_bias] == 0x21))
	{
		if(ftp_struct.ftp_upgrade_flag)
		{
			goto RET_LAB;
		}
		PppNcpProcess(rx_data+PPP_LCP_TYPE_INDEX-type_index_bias,tmp_len-8+type_index_bias);
	}
	else if((rx_data[PPP_PRO_TYPE_INDEX+1-type_index_bias] == 0x21)||((rx_data[PPP_PRO_TYPE_INDEX-type_index_bias] == 0x00)&&(rx_data[PPP_PRO_TYPE_INDEX+1-type_index_bias] == 0x21)))
	{
		if((rx_data[TCP_UDP_TYPE_INDEX-type_index_bias] == UDP_TYPE)&&(MemCmp(rx_data+PPP_IP_INDEX-type_index_bias,udp_struct.udp_dst_ip,4)))
		{
			if(ftp_struct.ftp_upgrade_flag)
			{
				goto RET_LAB;
			}
			UdpProcess(rx_data+PRO_IP_TEXT_INDEX-type_index_bias,tmp_len-36+type_index_bias);
		}
		else if(rx_data[TCP_UDP_TYPE_INDEX-type_index_bias] == TCP_TYPE)
		{
			if(MemCmp(rx_data+PPP_IP_INDEX-type_index_bias,tcp_struct.tcp_dst_ip,4))
			{
				TcpProcess(rx_data+TCP_SRC_PORT_INDEX-type_index_bias,tmp_len-28+type_index_bias);
			}
			else
			{	
				FtpProcess(rx_data+TCP_SRC_PORT_INDEX-type_index_bias,tmp_len-28+type_index_bias);
			}
		}
	}
RET_LAB:
	return;
}	
