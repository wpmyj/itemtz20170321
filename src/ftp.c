/****************************************Copyright (c)**************************************************
**                                                                
**--------------File Info-------------------------------------------------------------------------------
** File Name: xg_protocol.c
** Last modified Date:
** Last Version:
** Descriptions: 
*******************************************************************************************************/
#define FTP_GLOBAL

#include "tiza_include.h"

#define FTP_FLAG_FIN 	0X01
#define FTP_FLAG_SYN 	0X02
#define FTP_FLAG_RST 	0X04
#define FTP_FLAG_PSH 	0X08
#define FTP_FLAG_ACK 	0X10
#define FTP_FLAG_URG 	0X20
#define FTP_FLAG_MASK 	(FTP_FLAG_FIN|FTP_FLAG_SYN|FTP_FLAG_RST|FTP_FLAG_PSH|FTP_FLAG_ACK|FTP_FLAG_URG)

FTP_STRUCT	ftp_struct;
FTP_TX_STEP	ftp_txstep = e_ftpstart;
uint8 ftp_wait_flag = FALSE;

uint8 Ftp_USER[SMS_FTPNAME_LEN+1] 				;//= {"Vehicle"};
uint8 Ftp_PSW[SMS_FTPPSW_LEN+1] 					;//= {"Vehicle#*"};
uint8 Ftp_IP[SMS_FTPIP_MAXLEN+1] 					;//= {"202.102.90.179"};
uint8 Ftp_File_Path[SMS_FTPPATH_MAXLEN+1] ;
//={"/THAG_WL_V1_35_HW130107.bin"};
//= {"/THAG-GXLG01-0E-13-140710.bin"};
	
uint8 AT_CFTPSERV[] 		= {"AT+CFTPSERV=\""};						///IP
uint8 AT_CFTPUN[] 			= {"AT+CFTPUN=\""};							///USER
uint8 AT_CFTPPW[] 			= {"AT+CFTPPW=\""};							///PASSWORD
uint8 AT_CFTPGETFILE[] 	= {"AT+CFTPGETFILE=\""};				///获取文件到EFS +""
uint8 AT_CFTRANTX[] 		= {"AT+CFTRANTX=\"c:"};					///获取EFS文件到串口 +"c:"

uint8 AT_CFTPPORT[] = {"AT+CFTPPORT=21\x0d\x0a"};				///服务器端口
uint8 AT_CFTPMODE[] = {"AT+CFTPMODE=1\x0d\x0a"};				///模式1/0=passive/proactive
uint8 AT_CFTPTYPE[] = {"AT+CFTPTYPE=I\x0d\x0a"};				///类型A/I=ASCII/binary
uint8 AT_CATR[] 		= {"AT+CATR=1,0\x0d\x0a"};					///配置资源中心接口
uint8 AT_FSCD[] 		= {"AT+FSCD=C:\x0d\x0a"};						///设置EFS当前目录
uint8 AT_FSMEM[] 		= {"AT+FSMEM\x0d\x0a"};							///查询EFS容量
uint8 AT_FSDEL_ALL[]= {"AT+FSDEL=*.*\x0d\x0a"};					///删除EFS下所有文件

//===========================================================================================================



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
	
	for(i=len-1;i>0;i--)	//-查找可能有的路劲
	{
		if(tem_p[i] == '/')
		{
			file_len = len-1-i;
			p_file_name = tem_p+i+1;	//-最后一个/后面是文件名称
			break;
		}
	}
	
	ftp_struct.file_path[0] = '\0';
	if(i!=0)	//-判断是否存在路劲,如果i=0说明不存在路劲
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
	MemCpy(ftp_struct.file_name,p_file_name,file_len);	//-获取的文件名记录下来
	ftp_struct.file_name[file_len] = 0x0d;
	ftp_struct.file_name[file_len+1] = '\0';
		
	//-res = MemCmp(ftp_struct.file_name,(uint8*)sys_const_para_struct.software_version,15);
	res = TRUE;	//-目前只要正确指向就进行升级
RETURN_LAB:
	return res;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static void FtpTx(u8 data[],u16 len)///FTP
{
	ftp_struct.ftp_receive_ack_flag = 0;
	
	ftp_struct.ftp_rx_len = 0;
	ftp_struct.ftp_rx_file_byte_counter = 0;
	
	g_gsm_struct.rx_counter = 0;
	
	GprsUartFixedLenSend(data,len);	
	#ifdef FTP_DEBUG
		printf("SEND-->");
		*(data+len) = '\0';
		printf((const char*)data);
	#endif
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void FtpProcess(void)
{	
	//-ClearEWatchDog();		//external
	//-ClearWatchDog();		//inside
	
	if(ftp_struct.ftp_request_file_flag == 0)						//获取ACK
	{	
		if(ftp_struct.ftp_receive_ack_flag == 0)
		{
			if((g_gsm_struct.rx_buf[g_gsm_struct.rx_counter-1]==0x0a)&&
				 (g_gsm_struct.rx_buf[g_gsm_struct.rx_counter-2]==0x0d))
			{
				ftp_struct.ftp_rx_len = g_gsm_struct.rx_counter;			
				MemCpy(ftp_struct.ftp_rx_buf,g_gsm_struct.rx_buf,ftp_struct.ftp_rx_len);

				ftp_struct.ftp_receive_ack_flag = 1;
			}
			
			#ifdef FTP_DEBUG
			if(ftp_struct.ftp_rx_len)
				printf("FTP_AT_ACK_LEN = %d \r\n" ,ftp_struct.ftp_rx_len);
			#endif
		}
	}
	else if(ftp_struct.ftp_request_file_flag==1)				//获取文件到串口
	{
		if(ftp_struct.ftp_rx_len)
		{
			CpuFlashWrite(FTP_BLIND_FLASH_START+ftp_struct.ftp_rx_file_byte_counter,ftp_struct.ftp_rx_buf, ftp_struct.ftp_rx_len);

			ftp_struct.ftp_rx_file_lastfram_len = ftp_struct.ftp_rx_len;
			ftp_struct.ftp_rx_file_byte_counter += ftp_struct.ftp_rx_len;
			ftp_struct.ftp_rx_len = 0;
			
			#ifdef FTP_DEBUG
//				printf("ftp_rx_file_lastfram_len = %d \r\n" ,ftp_struct.ftp_rx_file_lastfram_len);
//				printf("ftp_rx_file_byte_counter = %d \r\n" ,ftp_struct.ftp_rx_file_byte_counter);
			#endif
		}
	}
	
}

void FtpDelay(void)
{
	uint16 delaytime;
	static uint16 old_delaytime = TIME7_MAX_COUNTER;
	
	if(ftp_struct.ftp_delaywait_time)											//需要延时
	{
		delaytime = Time7_10ms_counter;
		if(old_delaytime == TIME7_MAX_COUNTER)							
		{
			old_delaytime = delaytime;
		}
		delaytime = (delaytime + TIME7_MAX_COUNTER - old_delaytime)%TIME7_MAX_COUNTER;
		
		if(delaytime >= ftp_struct.ftp_delaywait_time*T7_WAIT_1S)	//延时时间到
		{
			old_delaytime = TIME7_MAX_COUNTER;
			ftp_struct.ftp_delaywait_time = 0;	
		}
				
		FtpProcess();
	}		
}
*/

///////////////////////////////////////////////////////////////////////////////
static uint8 FtpTxDelFile(void)
{
	uint8 i,res=RES_WAIT;
	
	if(ftp_wait_flag == FALSE)
	{		
		FtpTx(AT_FSDEL_ALL,StrLen(AT_FSDEL_ALL,0));
		ftp_wait_flag = TRUE;
		ftp_struct.ftp_delaywait_time = 5;
	}
	else
	{
		if(ftp_struct.ftp_delaywait_time == 0)
		{
			i = SubMatch((uint8*)"OK",StrLen((uint8*)"OK",0),ftp_struct.ftp_rx_buf,ftp_struct.ftp_rx_len);
			if(i != 0)
			{
				res = RES_TRUE;	
				ftp_wait_flag = FALSE;
				#ifdef FTP_DEBUG
				printf("DELFILE_TRUE\r\n=======================\r\n");
				#endif
			}
			else	res = RES_FALSE;
		}
	}
	
	return res;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


uint8 FtpMain(void)
{
	uint8 res = RES_WAIT;	
	
	if(ftp_struct.ftp_upgrade_flag == 0)
	{
		res = RES_NOFTPUP;
		goto RETURN_LAB;	
	}		
	
	//-FtpDelay();	//-内部有写flash
	
	switch(ftp_txstep)
	{
		case e_ftpstart:		//升级前准备
		{		
			ftp_txstep = e_efsdelfile;
			ftp_struct.ftp_upgrade_fail_flag=0;
			ftp_struct.ftp_delaywait_time 	= 0;
		}
		break;
				
		case e_efsdelfile:
		{
			res = FtpTxDelFile();
			if(res == RES_TRUE)
				ftp_txstep = e_setip;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		
		case e_setip:
		{
			res = FtpSetIp();
			if(res == RES_TRUE)
				ftp_txstep = e_setport;
			else if(res == RES_FALSE) 	//不考虑重发
			{
				goto RETURN_LAB;	
			}
		}
		break;
		case e_setport:
 		{
			res = FtpSetPort();
			if(res == RES_TRUE)
				ftp_txstep = e_setmode;
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		case e_setmode:
		{
			res = FtpTxPasv();
			if(res == RES_TRUE)
				ftp_txstep = e_settype;
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		case e_settype: 
		{
			res = FtpTxTypeI();
			if(res == RES_TRUE)
				ftp_txstep = e_setuser;
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		case e_setuser:
		{
			res = FtpTxUserName();
			if(res == RES_TRUE)
				ftp_txstep = e_setpassword;
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		case e_setpassword:
		{
			res = FtpTxPassword();
			if(res == RES_TRUE)
				ftp_txstep = e_efscatr;
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		///
		case e_efscatr:
		{
			res = FtpTxSetURC();
			if(res == RES_TRUE)
			{
				ftp_txstep = e_efsmem;
				ftp_struct.ftp_readefs_usedsize_times = 1;
			}
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		case e_efsmem:
		{
			res = FtpTxEFSMEM();
			if(res == RES_TRUE)
			{
				if(ftp_struct.ftp_readefs_usedsize_times == 2)//second read
					ftp_txstep = e_efscd;
				else if(ftp_struct.ftp_readefs_usedsize_times == 1)//first read
					ftp_txstep = e_getfile2efs;
			}
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		case e_getfile2efs:
		{
			res = FtpTxGetFile();
			if(res == RES_TRUE)
			{
				if(ftp_struct.ftp_readefs_usedsize_times == 1)
					ftp_struct.ftp_readefs_usedsize_times = 2;
				ftp_txstep = e_efsmem;
			}
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;
		}
		break;
		case e_efscd:
		{
			res = FtpTxSetCurList();
			if(res == RES_TRUE)
				ftp_txstep = e_getfile2uart;
			else if(res == RES_FALSE) 	
			{
				goto RETURN_LAB;	
			}
		}
		break;
		
		case e_getfile2uart:
		{
			res = FtpGetFileToUART();
			if(res == RES_TRUE)
			{
				WriteVersion();
				ftp_txstep = e_ftpend;
			}
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		
		case e_ftpend:		//恢复初始状态
		{
				ftp_txstep = e_ftpstart;
				ftp_struct.ftp_upgrade_flag = 0;
		}
		break;
		
		default:	break;		
	}
	
RETURN_LAB:	
	if(res == RES_FALSE) 
	{
		ftp_txstep = e_ftpend;
		ftp_struct.ftp_upgrade_fail_flag=1;		
		#ifdef FTP_DEBUG
			printf("!!!warn ftp_upgrade_fail\r\n");
		#endif
	}
		
	return res;
}

