#ifndef __PPP_H
#define __PPP_H

	#ifdef PPP_STRUCT_GLOBAL
		#define EXTERN_PPP
	#else
		#define EXTERN_PPP extern
	#endif
	
	#define PPP_PRO_TYPE_INDEX 3
	#define PPP_LCP_TYPE_INDEX 5///LCP类型下标
	#define PPP_LCP_TEXT_INDEX 9///LCP内容下标
	#define PPP_IP_INDEX 	   17///DST IP下标
	#define TCP_UDP_TYPE_INDEX 14///tcp,udp下标
	#define TCP_SRC_PORT_INDEX 25///tcp源端口号下标
	#define PRO_IP_TEXT_INDEX  33///上行协议内容下标
	
	#define LCP_PRO_TYPE 0xc021
	#define PAP_PRO_TYPE 0xc023
	#define NCP_PRO_TYPE 0x8021
	#define IP_PRO_TYPE  0x0021
	
	#define UDP_TYPE  0x11
	#define TCP_TYPE  0x06
	#define ICMP_TYPE 0x01
	

	typedef struct
	{
		uint8 sms_ack_flag[3];///不上线短信诊断回复
		uint8 lcp_req_flag;
		uint8 pap_req_flag;
		uint8 ncp_req_flag;
		uint8 ncp_ok_flag;
		
		uint8 ppp_tx_seq;
		uint8 ppp_tx_ncp_counter;
		
		uint8 lcp_up_ack_data[30];
		
		uint8 dns_ip[4];
		
		uint8 ppp_src_ip[4];
		uint8 ppp_dst_ip[4];
		uint8 ppp_src_port[2];
		uint8 ppp_dst_port[2];

		uint32 tcp_tx_seq;
		uint32 tcp_rx_seq;
		uint8 tcp_src_ip[4];
		uint8 tcp_dst_ip[4];
		uint8 tcp_src_port[2];
		uint8 tcp_dst_port[2];
			
	}PPP_STRUCT;
	
	typedef struct
	{
		uint8 udp_src_ip[4];
		uint8 udp_dst_ip[4];
		uint8 udp_src_port[2];
		uint8 udp_dst_port[2];
	}UDP_STRUCT;
	typedef struct
	{
		uint8 tcp_src_ip[4];
		uint8 tcp_dst_ip[4];
		uint8 tcp_src_port[2];
		uint8 tcp_dst_port[2];
	}TCP_STRUCT;
	void PppPacketEncode(uint8 data[],uint16 len);
	uint16 PppPacketDecode(uint8 data[],uint16 len);
	uint16 PppPacketLen(uint8 data[]);
	void PppTx(uint8 data[],uint16 len,uint16 pro_type);
	
	void PppLcpReq(uint8 lcp_data[],uint8 len);
	void PppLcpNak(uint8 seq);
	void PppLcpAck(uint8 data[],uint16 len);
	void PppLcpTerminate(void);
	void PppLcpTerminateAck(uint8 data[],uint16 len);
	void PppPapReq(uint8 seq);
	void PppNcpReq(uint8 data[],uint8 len);
	void PppNcpAck(uint8 data[],uint16 len);
	void PppNcpNak(uint8 seq);
	
	void PppMainProcess(void);
	void PppInit(void);
	void PppPapProcess(uint8 data[],uint16 len);
	void PppLcpProcess(uint8 data[],uint16 len);
	void PppNcpProcess(uint8 data[],uint16 len);
	
	uint16 TcpUdpHeadCheck(u8 data[],uint16 len,uint8 frame_type);
	void UdpIpPortInit(void);
	void UdpTx(uint8 tx_data[],u16 tx_len);
	void IpTx(uint8 tx_data[],u16 len,u8 type);
	uint16 IpHeadCheck(u8 data[],uint16 len);
	
	EXTERN_PPP PPP_STRUCT g_ppp_struct;
	EXTERN_PPP UDP_STRUCT	udp_struct;
	EXTERN_PPP TCP_STRUCT	tcp_struct;
#endif
