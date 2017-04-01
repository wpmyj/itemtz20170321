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

uint8 ftp_wait_flag = FALSE;
/*
uint8 Ftp_USER[SMS_FTPNAME_LEN+1] 				;//= {"Vehicle"};
uint8 Ftp_PSW[SMS_FTPPSW_LEN+1] 					;//= {"Vehicle#*"};
uint8 Ftp_IP[SMS_FTPIP_MAXLEN+1] 					;//= {"202.102.90.179"};
uint8 Ftp_File_Path[SMS_FTPPATH_MAXLEN+1] ;
//={"/THAG_WL_V1_35_HW130107.bin"};
//= {"/THAG-GXLG01-0E-13-140710.bin"};
	
uint8 AT_CFTPSERV[] 		= {"AT+CFTPSERV=\""};						///IP
uint8 AT_CFTPUN[] 			= {"AT+CFTPUN=\""};							///USER
uint8 AT_CFTPPW[] 			= {"AT+CFTPPW=\""};							///PASSWORD
uint8 AT_CFTPGETFILE[] 	= {"AT+CFTPGETFILE=\""};				///»ñÈ¡ÎÄ¼þµ½EFS +""
uint8 AT_CFTRANTX[] 		= {"AT+CFTRANTX=\"c:"};					///»ñÈ¡EFSÎÄ¼þµ½´®¿Ú +"c:"

uint8 AT_CFTPPORT[] = {"AT+CFTPPORT=21\x0d\x0a"};				///·þÎñÆ÷¶Ë¿Ú
uint8 AT_CFTPMODE[] = {"AT+CFTPMODE=1\x0d\x0a"};				///Ä£Ê½1/0=passive/proactive
uint8 AT_CFTPTYPE[] = {"AT+CFTPTYPE=I\x0d\x0a"};				///ÀàÐÍA/I=ASCII/binary
uint8 AT_CATR[] 		= {"AT+CATR=1,0\x0d\x0a"};					///ÅäÖÃ×ÊÔ´ÖÐÐÄ½Ó¿Ú
uint8 AT_FSCD[] 		= {"AT+FSCD=C:\x0d\x0a"};						///ÉèÖÃEFSµ±Ç°Ä¿Â¼
uint8 AT_FSMEM[] 		= {"AT+FSMEM\x0d\x0a"};							///²éÑ¯EFSÈÝÁ¿
uint8 AT_FSDEL_ALL[]= {"AT+FSDEL=*.*\x0d\x0a"};					///É¾³ýEFSÏÂËùÓÐÎÄ¼þ
*/
//===========================================================================================================








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
	
	for(i=0;i<len;i++)///È¡ÓÃ»§Ãû
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
				
	for(i=0;i<len;i++)///Ö¸ÏòÃÜÂë
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
	ftp_struct.ftp_dst_ctrl_port[1] = 21;///¿ØÖÆ¶Ë¿Ú¶¼Îª21
	len = len-3;
	tem_p += 3;
	
	for(i=len-1;i>0;i--)	//-²éÕÒ¿ÉÄÜÓÐµÄÂ·¾¢
	{
		if(tem_p[i] == '/')
		{
			file_len = len-1-i;
			p_file_name = tem_p+i+1;	//-×îºóÒ»¸ö/ºóÃæÊÇÎÄ¼þÃû³Æ
			break;
		}
	}
	
	ftp_struct.file_path[0] = '\0';
	if(i!=0)	//-ÅÐ¶ÏÊÇ·ñ´æÔÚÂ·¾¢,Èç¹ûi=0ËµÃ÷²»´æÔÚÂ·¾¢
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
	MemCpy(ftp_struct.file_name,p_file_name,file_len);	//-»ñÈ¡µÄÎÄ¼þÃû¼ÇÂ¼ÏÂÀ´
	ftp_struct.file_name[file_len] = 0x0d;
	ftp_struct.file_name[file_len+1] = '\0';
		
	//-res = MemCmp(ftp_struct.file_name,(uint8*)sys_const_para_struct.software_version,15);
	res = TRUE;	//-Ä¿Ç°Ö»ÒªÕýÈ·Ö¸Ïò¾Í½øÐÐÉý¼¶
RETURN_LAB:
	return res;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
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
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
void FtpProcess(void)
{	
	//-ClearEWatchDog();		//external
	//-ClearWatchDog();		//inside
	
	if(ftp_struct.ftp_request_file_flag == 0)						//»ñÈ¡ACK
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
	else if(ftp_struct.ftp_request_file_flag==1)				//»ñÈ¡ÎÄ¼þµ½´®¿Ú
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
	
	if(ftp_struct.ftp_delaywait_time)											//ÐèÒªÑÓÊ±
	{
		delaytime = Time7_10ms_counter;
		if(old_delaytime == TIME7_MAX_COUNTER)							
		{
			old_delaytime = delaytime;
		}
		delaytime = (delaytime + TIME7_MAX_COUNTER - old_delaytime)%TIME7_MAX_COUNTER;
		
		if(delaytime >= ftp_struct.ftp_delaywait_time*T7_WAIT_1S)	//ÑÓÊ±Ê±¼äµ½
		{
			old_delaytime = TIME7_MAX_COUNTER;
			ftp_struct.ftp_delaywait_time = 0;	
		}
				
		FtpProcess();
	}		
}
*/

///////////////////////////////////////////////////////////////////////////////
/*
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
*/

///////////////////////////////////////////////////////////////////////////////

static uint8 FtpCheckSIM(void)
{
	uint8 i,res=RES_WAIT;
	uint8 tmp_str[100];
	uint8 tmp_len;
	
		
	L218SendAtCmd(AT_CPIN_INDEX,NULL,0,(uint8 *)OK_ACK,2);//-¼ì²éSIM
		
	if(g_at_cmd_struct[AT_CPIN_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
	
	return res;
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
	//-ÔÝÊ±²»¿¼ÂÇÓ¦´ðå,ÊÇ·ñÓÐÓ¦´ð´ýÑéÖ¤å?å?
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
	
	L218SendAtCmd(AT_FTPSIZE_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	
	if(g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		res = RES_FALSE;	
		
	return res;
}

static uint8 FtpSETDLAFile(void)	//-¼òµ¥ÅÜÁ÷³Ì,×éÖ¯ÊÇÓÐÎÊÌâµÄå?å?å?
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

	res=RES_FALSE;
	L218SendAtCmd(AT_FTPSIZE_INDEX,NULL,0,(uint8 *)OK_ACK,2);
	if(g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag == EXE_OK)
		res = RES_TRUE;
	else
		return res;

	res=RES_FALSE;
	L218SendAtCmd(AT_FTPGET1_INDEX,NULL,0,(uint8 *)FTPGET_OK_ACK,12);
	
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
	uint8 TEMP_OK_ACK[32] = "\x0d\x0a\x0d\x0a+FTPGET: 1,";
	

	L218SendAtCmd(AT_FTPGET2_INDEX,TEMP_file_size,7,(uint8 *)TEMP_OK_ACK,15);
	
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

//-½ÓÊÕµ½FTPÉý¼¶ÃüÁîÖ®ºó½øÈëÉý¼¶Á÷³Ì
uint8 FtpMain(void)
{
	uint8 res = RES_WAIT;	
	
	if(ftp_struct.ftp_upgrade_flag == 0)
	{
		res = RES_NOFTPUP;
		goto RETURN_LAB;	
	}		
	
	//-FtpDelay();	//-ÄÚ²¿ÓÐÐ´flash
	
	switch(ftp_struct.ftp_txstep)
	{
		case e_ftpstart:		//Éý¼¶Ç°×¼±¸,½øÐÐ±ØÒªµÄ³õÊ¼»¯
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

		case e_FTPSIZE:	//-»ñÈ¡ÎÄ¼þ´óÐ¡
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
		
		
		case e_ftpend:		//»Ö¸´³õÊ¼×´Ì¬
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

