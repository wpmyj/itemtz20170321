#ifndef _FTP_H 
#define _FTP_H

		#ifdef  FTP_GLOBAL
			#define EXTERN_FTP_GLOBAL
		#else
			#define EXTERN_FTP_GLOBAL extern
		#endif

		typedef struct
		{
			uint8 ftp_upgrade_flag;///FTP升级标志
			uint32 ftp_file_total_size;
			uint32 ftp_rx_file_byte_counter;///接收文件字节计数
			
			uint8 user_name[LEN_16];
			uint8 pass_word[LEN_16];
			uint8 file_name[LEN_64];
			uint8 file_path[LEN_64];
			
			uint8 tcp_tx_flag_byte;///FTP中TCP发送标志(应答，同步等)
			uint8 tcp_rx_flag_byte;
			
			uint8 ftp_data_flag;///FTP数据端口连接标志
			uint32 ftp_dst_ctrl_cur_tx_seq;///接收FTP控制端口返回帧，源当前发送的序列号
			uint32 ftp_dst_ctrl_next_rx_seq;///接收FTP控制端口返回帧，目标应该发送的序列号
			
			uint32 ftp_dst_data_cur_tx_seq;///接收FTP数据端口返回帧，源当前发送的序列号
			uint32 ftp_dst_data_last_tx_seq;
			uint32 ftp_dst_data_next_rx_seq;///接收FTP数据端口返回帧，目标应该发送的序列号
			
			uint8 rx_ctrl_flag;
			uint8 rx_data_flag;
			uint8 ftp_upgrade_fail_flag;///升级失败标志
			uint8 ftp_rx_file_odd_byte_flag;///接收奇字节标志
			
			uint16 ftp_rx_len;///数据区长度
			uint8 ftp_rx_buf[(GPRS_UART_BUF_LEN >> 3) + 1];///数据区指针,加1目的,STM32FLASH只支持2字节对齐
			
			
			uint8 ftp_src_ctrl_port[2];
			uint8 ftp_dst_ctrl_port[2];
			uint8 ftp_dst_ctrl_ip[4];
			
			
			uint8 ftp_src_data_port[2];
			uint8 ftp_dst_data_port[2];
			uint8 ftp_dst_data_ip[4];
			
			uint8  ftp_receive_ack_flag;							//??ACK??
			uint16 ftp_delaywait_time;								//??????? ???
			
			uint32 ftp_readefs_usedsize;
			uint8 ftp_readefs_usedsize_times;

		}FTP_STRUCT;
		
		EXTERN_FTP_GLOBAL FTP_STRUCT	ftp_struct;
		
			typedef enum 
			{
				e_ftpstart=0,
				e_setip = 1,
				e_setport,
				e_setmode,
				e_settype,
				e_setuser,
				e_setpassword,
				
				e_efscatr,
				e_efsmem,
				e_getfile2efs,
				e_efscd,
				e_efsdelfile,
				
				e_getfile2uart,
				
				
				
				e_ftpend,
			}FTP_TX_STEP;
		
		
		uint8 FtpTxUserName(void);
		uint8 FtpTxPassword(void);
		uint8 FtpTxCwd(void);
		uint8 FtpTxTypeI(void);
		uint8 FtpGetSize(void);
		uint8 FtpTxPasv(void);
		uint8 FtpGetFile(void);
		uint8 FtpAddrAnalysis(uint8 data[],uint16 len);
		void FtpIpPortInit(uint8 data_port_flag);
		void FtpTerminate(void);
		uint8 FtpConnect(uint8 data_port_flag);
		void FtpDelay(uint16 delay_sec,uint8 wait_ack_flag);
		void FtpTx(u8 data[],u16 len,uint8 is_data_port_flag);
		void FtpProcess(uint8 data[],uint16 len);
		uint8 FtpMain(void);
#endif

