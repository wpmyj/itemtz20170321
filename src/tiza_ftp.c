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

uint8 ftp_wait_flag = FALSE;

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

static uint8 FtpCheckSIM(void)
{
//	uint8 i,res=RES_WAIT;
//	uint8 tmp_str[100];
//	uint8 tmp_len;
//	
//		
//	L218SendAtCmd(AT_CPIN_INDEX,NULL,0,(uint8 *)OK_ACK,2);//-检查SIM
//		
//	if(g_at_cmd_struct[AT_CPIN_INDEX].exe_flag == EXE_OK)
//		res = RES_TRUE;
//	else
//		res = RES_FALSE;	
//	
//	return res;
	
	return RES_TRUE;
}

static uint8 FtpCheckNET(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_CREG_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
	
	if(g_at_cmd_struct[AT_CREG_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpCGATT(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_CGATT_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_CGATT_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpSAPBR(void)
{
	uint8 res=RES_FALSE;
	
	//-1.Set the type of internet connection for FTP
	L218SendAtCmd(AT_SAPBR_INDEX,FTP_TYPEOFINTERCONNECT,21,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;
	//-2.Set APN for FTP	
	L218SendAtCmd(AT_SAPBR_INDEX,FTP_SETAPN,18,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;	
	//-3.Open bearer	
	L218SendAtCmd(AT_SAPBR_INDEX,FTP_OPENBEARER,4,(uint8 *)OK_ACK,2);
	//-暂时不考虑应答?是否有应答待验证??
	if(g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;		
		
	return res;
}

static uint8 FtpFTPTYPE(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_FTPTYPE_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPTYPE_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpFTPSERV(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_FTPSERV_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPSERV_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpFTPPORT(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_FTPPORT_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPPORT_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpFTPUN(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_FTPUN_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPUN_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpFTPPW(void)
{
	uint8 res=RES_WAIT;
	
	L218SendAtCmd(AT_FTPPW_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPPW_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpFTPSIZE(void)
{
	uint8 res=RES_WAIT;
	uint8 ack[] = {"+FTPSIZE: "};
	
	L218SendAtCmd(AT_FTPSIZE_INDEX,NULL,0,(uint8 *)ack,10);
	
	if(g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpSETDLAFile(void)	//-简单跑流程,组织是有问题的???
{
	uint8 res=RES_FALSE;
	uint8 TEMP_file_size[32] = "2,1024\x0d";
	
	L218SendAtCmd(AT_FTPGETNAME_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPGETNAME_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;

	res=RES_FALSE;
	L218SendAtCmd(AT_FTPGETPATH_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPGETPATH_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;

	res = FtpFTPSIZE();
//	res=RES_FALSE;
//	L218SendAtCmd(AT_FTPSIZE_INDEX,NULL,0,(uint8 *)OK_ACK,2);
//	if(g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag == EXE_OK)
//		res = RES_TRUE;
//	else
//		return res;

	res=RES_FALSE;
	L218SendAtCmd(AT_FTPGET1_INDEX,NULL,0,(uint8 *)FTPGET_OK_ACK,11);
	
	if(g_at_cmd_struct[AT_FTPGET1_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;

	//-L218SendAtCmd(AT_FTPGET_INDEX,TEMP_file_size,7,(uint8 *)OK_ACK,2);
	//-
	//-if(g_at_cmd_struct[AT_FTPGET_INDEX].exe_flag == EXE_OK)
	//-	res = RES_TRUE;
	//-else
	//-	res = RES_FALSE;
	
		
	return res;
}




static uint8 FtpDLAFile(void)
{
	uint8 res=RES_WAIT;
	uint8 TEMP_file_size[32] = "2,1024\x0d";
//	uint8 TEMP_OK_ACK[32];
	uint8 TEMP_OK_ACK[32] = "+FTPGET:1,";

//	TEMP_OK_ACK[0] = '\x0d';
//	TEMP_OK_ACK[1] = '\x0a';

//	TEMP_OK_ACK[2] = 'O';
//	TEMP_OK_ACK[3] = 'K';

//	TEMP_OK_ACK[4] = '\x0d';
//	TEMP_OK_ACK[5] = '\x0a';
//	TEMP_OK_ACK[6] = '\x0d';
//	TEMP_OK_ACK[7] = '\x0a';
//	TEMP_OK_ACK[8] = '\x0d';
//	TEMP_OK_ACK[9] = '\x0a';

//	TEMP_OK_ACK[10] = '+';
//	TEMP_OK_ACK[11] = 'F';
//	TEMP_OK_ACK[12] = 'T';
//	TEMP_OK_ACK[13] = 'P';
//	TEMP_OK_ACK[14] = 'G';
//	TEMP_OK_ACK[15] = 'E';
//	TEMP_OK_ACK[16] = 'T';
//	TEMP_OK_ACK[17] = ':';
////	TEMP_OK_ACK[18] = ' ';
////	TEMP_OK_ACK[19] = '1';
////	TEMP_OK_ACK[20] = ',';
////	TEMP_OK_ACK[21] = '\0';
//	TEMP_OK_ACK[18] = '1';
//	TEMP_OK_ACK[19] = ',';
//	TEMP_OK_ACK[20] = '\0';

	L218SendAtCmd(AT_FTPGET2_INDEX,TEMP_file_size,7,(uint8 *)TEMP_OK_ACK,StrLen(TEMP_OK_ACK,0));
	
	if(g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else if(g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag == EXE_FAIL)
		res = RES_FALSE;
		
	return res;
}

static uint8 FtpFTPQUIT(void)
{
	uint8 res=RES_WAIT;
	//-uint8 cmp_data[] = {"+FTPGET: 1,86"};
	
	L218SendAtCmd(AT_FTPQUIT_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPQUIT_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static void FtpCBearer(void)
{
	uint8 temp_data[] = {"0,1\x0d"};
		
	//-1.Close bearer
	L218SendAtCmd(AT_SAPBR_INDEX,temp_data,StrLen(temp_data,0),(uint8 *)OK_ACK,2);	
}





///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-接收到FTP升级命令之后进入升级流程
uint8 FtpMain(void)
{
	uint8 res = RES_WAIT;	
	
	if(ftp_struct.ftp_upgrade_flag == 0)
	{
		res = RES_NOFTPUP;
		goto RETURN_LAB;	
	}		
	
	//-FtpDelay();	//-内部有写flash
	
	switch(ftp_struct.ftp_txstep)
	{
		case e_ftpstart:		//升级前准备,进行必要的初始化
		{		
			//-ftp_txstep = e_efsdelfile;
			ftp_struct.ftp_txstep = e_CheckSIM;
			ftp_struct.ftp_upgrade_fail_flag=0;
			ftp_struct.ftp_delaywait_time 	= 0;
			ftp_struct.ftp_rx_len = 0;
			ftp_struct.ftp_rx_file_byte_counter = 0;
			ftp_struct.ftp_upgrade_success_flag = 0;
			boot_struct.program_total_size = 0;
		}
		break;
		
		case e_CheckSIM:
		{
			res = FtpCheckSIM();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_CheckNET;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		
		case e_CheckNET:
		{
			res = FtpCheckNET();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_CGATT;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		
		case e_CGATT:
		{
			res = FtpCGATT();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_SAPBR;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		
		case e_SAPBR:
		{
			res = FtpSAPBR();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_FTPTYPE;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPTYPE:
		{
			res = FtpFTPTYPE();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_FTPSERV;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPSERV:
		{
			res = FtpFTPSERV();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_FTPPORT;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPPORT:
		{
			res = FtpFTPPORT();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_FTPUN;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPUN:
		{
			res = FtpFTPUN();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_FTPPW;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPPW:
		{
			res = FtpFTPPW();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_SETDLAFile;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_SETDLAFile:
		{
			res = FtpSETDLAFile();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_DLAFile;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPSIZE:	//-获取文件大小
		{
			res = FtpFTPSIZE();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_DLAFile;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_DLAFile:
		{
			res = FtpDLAFile();
			if(res == RES_TRUE)
				ftp_struct.ftp_txstep = e_FTPQUIT;
			else if(res == RES_WAIT)
				ftp_struct.ftp_txstep = e_DLAFile;
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;

		case e_FTPQUIT:
		{
			res = FtpFTPQUIT();
			if(res == RES_TRUE)
			{
				ftp_struct.ftp_txstep = e_ftpend;
				ftp_struct.ftp_upgrade_success_flag = 1;
			}
			else if(res == RES_FALSE) 	
				goto RETURN_LAB;	
		}
		break;
		
		
		case e_ftpend:		//恢复初始状态
		{
			FtpCBearer();
			ftp_struct.ftp_txstep = e_ftpstart;
			ftp_struct.ftp_upgrade_flag = 0;
		}
		break;
		
		default:	break;		
	}
	
RETURN_LAB:	
	if(res == RES_FALSE) 
	{
		ftp_struct.ftp_txstep = e_ftpend;
		ftp_struct.ftp_upgrade_fail_flag=1;		
		#ifdef FTP_DEBUG
			printf("!!!warn ftp_upgrade_fail\r\n");
		#endif
	}
		
	return res;
}

