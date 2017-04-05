#define GPRS_GLOBAL		//����ҵ���ʹ��
#define GPRS_DEBUG
#define GPS_GLOBAL		//����ҵ���ʹ��
#define GPS_DEBUG

#define L218_GLOBAL
#define BOOT_GLOBAL
#define L218_DEBUG
#include "tiza_include.h"

//�鿴�汾��AT+CGMR;+ISLKVRSCAN
//GTPOS �߰汾����ʹ�ô�������ͬ��AGPS����Ҫ��
///AT ָ��
const uint8 AT[] 					= {"AT\x0d"};								///AT����
const uint8 AT_E0[] 			= {"ATE0\x0d"};							///�رջ���
const uint8 AT_IPR[] 			= {"AT+IPR=38400;&w\x0d"};	///���ò�����Ϊ38400
const uint8 AT_CIMI[] 		= {"AT+CIMI\x0d\x0a"};			///��ȡSIM��IMSI
const uint8 AT_CNUM[] 		= {"AT+CNUM\x0d"};					///��ѯ��������
const uint8 AT_CPBS_ON[] 	= {"AT+CPBS=\"ON\"\x0d"};		///���ñ�����ʹ��
const uint8 AT_CPBW[] 		= {"AT+CPBW=1,"};						///д�뱾���� +����,����,����
const uint8 AT_IMEI[] 		= {"AT+EGMR=0,7\x0d\x0a"};	///��ѯGSMģ���IMEI
const uint8 AT_CSQ[] 			= {"AT+CSQ\x0d"};						///��ѯ�ź�ǿ��
const uint8 AT_CREG[] 		= {"AT+CREG?\x0d"};					///��ѯע�����
const uint8 AT_CGATT[] 		= {"AT+CGATT?\x0d"};				///GPRS�������

const uint8 AT_CMGF[] 		= {"AT+CMGF=0\x0d"};				///��PDU��ʽ�������Ϣ
const uint8 AT_CMGL[] 		= {"AT+CMGL=0\x0d"};				///0δ������,4ȫ������
const uint8 AT_CMGS[] 		= {"AT+CMGS="};							///���Ͷ���
const uint8 AT_CMGD[] 		= {"AT+CMGD=1,4\x0d"};			///ɾ����

const uint8 AT_ATH[] 			= {"ATH\x0d"};							///�Ҷϵ绰
const uint8 AT_SLEEP[]		= {"AT+ESLP=1\x0d\x0a"};		///����
//����const uint8 AT_WAKE[]  		= {"AT+CFUN=1\x0d\x0a"};		///Ҫ����
const uint8 AT_NULL[]			= {""};											///��,�ȴ�����

// �����������
//������ ��������
const uint8 AT_CIPMUX[] 			= {"AT+CIPMUX=0\x0d\x0a"};           								///������ģʽ
const uint8 AT_CSTT[] 				= {"AT+CSTT=\"CMNET\"\x0d\x0a"};           						///��������
const uint8 AT_NETOPEN[] 			= {"AT+CIICR\x0d\x0a"};                           		///������
const uint8 AT_NETCLOSE[] 		= {"AT+CIPSHUT\x0d\x0a"};                         		///�ر�����
const uint8 AT_CIFSR[] 				= {"AT+CIFSR\x0d\x0a"};           										///��ȡ����IP
//			uint8 AT_CIPOPEN_TCP[] 	= {"AT+CIPSTART=\"UDP\",\"%s\",\"%s\"\x0d\x0a"};   		///��UDP����
//			uint8 AT_CIPOPEN_TCP[] 	= {"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\x0d\x0a"};   		///��TCP����
			uint8 AT_CIPOPEN_TCP[] 	= {"AT+CIPSTART=\"TCP\",\"%u.%u.%u.%u\",\"%u\"\x0d\x0a"};   		///��TCP����
			uint8 AT_CIPSEND_TCP[] 	= {"AT+CIPSEND=%u\x0d\x0a"};           								///����TCP����
const	uint8 AT_CIPCLOSE[] 		= {"AT+CIPCLOSE\x0d\x0a"};               	   					///�ر�����
const	uint8 AT_CIPRXGET[] 		= {"AT+CIPRXGET=0\x0d\x0a"};               	   				///�Զ�����TCP����
const	uint8 AT_CIPSRIP[] 			= {"AT+CIPSRIP=1\x0d\x0a"};               	   				///��������ǰ��IP��ַ
// GPS											///����GPS������GPSģ�鲨����
const uint8 AT_GETGPS[]				={"AT+GETGPS=1\x0d\x0a"};															///�رմ�GSM���ڴ�ӡGPS��Ϣ																///�رմ�GSM���ڴ�ӡGPS��Ϣ
      uint8 AT_EGPSC[]				={"AT+EGPSC%s\x0d\x0a"};															///1/0  ����/�ر�GPSģ��
const	uint8 AT_PMTK314[] 			={"AT+EGPSS=\"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\"\x0d\x0a"}; ///����GPS

const uint8 AT_CPIN[] 			= {"AT+CPIN?\x0d"};	//-���SIM
const uint8 AT_SAPBR[] 			= {"AT+SAPBR="};	//-����FTP�Ļ�������������






const uint8 RECV_IPD_ACK[] = "+RECEIVE,";       		// RECV DATA LEN, ADD "+IPD" HEADER
//const uint8 RECV_FROM_ACK[] = "RECV FROM:";       // RECV FORM: STRING IF SET SHOW "RECV FROM" HEADER
const uint8 OK_ACK[] 						= "OK";
const uint8 CRLF_ACK[] 					= "\x0d\x0a";
//const uint8 CIPOPEN_EXTRA_OK[]	= "CONNECT OK";		// IP IS OPENED SUCCESS
const uint8 CIPSEND_EXTRA_OK[] 	= "SEND OK";			// SEND SUCCESS
const uint8 CIPCLOSE_EXTRA_OK[] = "CLOSE OK";			// IP IS CLOSED SUCCESS
const uint8 GPRS_HAVE_RX_DATA[] = "+RECEIVE,";		// �н�������
const uint8 CONST_DATA_1[] = "1\x0d";		// �н�������

const uint8 FTPGET_OK_ACK[] = "+FTPGET: 1,1";		// Open FTP session SUCCESS
const uint8 FTPGET_FINISH_ACK[] = "+FTPGET: 1,0";		// means finish read DATA




const uint8 FTP_TYPEOFINTERCONNECT[] ={"3,1,\"CONTYPE\",\"GPRS\"\x0d"};
const uint8 FTP_SETAPN[] ={"3,1,\"APN\",\"CMNET\"\x0d"};
const uint8 FTP_OPENBEARER[] ={"1,1\x0d"};
const uint8 AT_FTPTYPE[]		= {"AT+FTPTYPE=\"I\"\x0d"};	//-Set the type of data:��A�� for FTP ASCII sessions,��I�� for FTP Binary sessions
const uint8 AT_FTPSERV[] 		= {"AT+FTPSERV=\"202.102.090.179\"\x0d"};						///IP
const uint8 AT_FTPPORT[]        = {"AT+FTPPORT=21\x0d"};				///�������˿�
const uint8 AT_FTPUN[] 			= {"AT+FTPUN=\"Vehicle\"\x0d"};							///USER
const uint8 AT_FTPPW[] 			= {"AT+FTPPW=\"Vehicle#*\"\x0d"};							///PASSWORD
const uint8 AT_FTPGETNAME[] 	= {"AT+FTPGETNAME=\"TIZA_FTP.bin\"\x0d"};	//-Set the file name in FTP server
//-const uint8 AT_FTPGETNAME[] 	= {"AT+FTPGETNAME=\"TIZA_FTP_test.bin\"\x0d"};
//-const uint8 AT_FTPGETNAME[] 	= {"AT+FTPGETNAME=\"TIZA_XGDL_SBJ_V803_170306.bin\"\x0d"};	//-Set the file name in FTP server
const uint8 AT_FTPGETPATH[] 	= {"AT+FTPGETPATH=\"/\"\x0d"};	//-Set the path of file
const uint8 AT_FTPGET1[] 		= {"AT+FTPGET=1\x0d"};	//-
const uint8 AT_FTPGET2[] 		= {"AT+FTPGET="};	//-

const uint8 AT_FTPQUIT[] 		= {"AT+FTPQUIT\x0d"};	//-Quit FTP connection
const uint8 AT_FTPSIZE[] 		= {"AT+FTPSIZE\x0d"};	//-Get the size of file.




uint8 TEMPF_DATA_FROM_FTP[4096];
uint16 FLASH_DATA_PT = 0;






///ָ��ṹ��ʼ��
AT_CMD_STRUCT g_at_cmd_struct[] = 
{
		{(uint8 *)AT, 								1,	10*SEND_1T,	EXE_NO,	AtFun},
		{(uint8 *)AT_E0,							1,	10*SEND_1T,	EXE_NO,	AtE0Fun},
		{(uint8 *)AT_IPR,							1,	 5*SEND_1T,	EXE_NO,	AtIprFun},
		{(uint8 *)AT_CIMI,						1,	50*SEND_1T,	EXE_NO,	AtCimiFun},
		{(uint8 *)AT_CNUM,				   50,	50*SEND_1T,	EXE_NO,	AtCnumFun},
		{(uint8 *)AT_CPBS_ON,					1,	10*SEND_1T,	EXE_NO,	AtCpbsOnFun},
		{(uint8 *)AT_CPBW,						1,	10*SEND_1T,	EXE_NO,	AtCpbwFun},
		{(uint8 *)AT_IMEI,						1,	 3*SEND_1T,	EXE_NO,	AtImeiFun},
		{(uint8 *)AT_CSQ,							1,	 3*SEND_1T,	EXE_NO,	AtCsqFun},
		{(uint8 *)AT_CREG,						1,	 3*SEND_1T,	EXE_NO,	AtCregFun},
		{(uint8 *)AT_CGATT,						1,	 3*SEND_1T,	EXE_NO,	AtCgattFun},

		{(uint8 *)AT_CMGF,						1,	 3*SEND_1T,	EXE_NO,	AtCmgfFun},
		{(uint8 *)AT_CMGL,				  100,   1*SEND_1T,	EXE_NO,	AtCmglFun},
		{(uint8 *)AT_CMGS,						1,   1*SEND_1T,	EXE_NO,	AtCmgsFun},
		{(uint8 *)AT_CMGD,						1,	 3*SEND_1T,	EXE_NO,	AtCmgdFun},

		{(uint8 *)AT_ATH,							1,	 5*SEND_1T,	EXE_NO,	AtAthFun},
		{(uint8 *)AT_SLEEP,						1,	 5*SEND_1T,	EXE_NO,	AtSleepFun},
//		{(uint8 *)AT_WAKE,						1,	 3*SEND_1T,	EXE_NO,	AtWakeFun},
		{(uint8 *)AT_NULL,						1,	 1*SEND_1T,	EXE_NO,	AtNullFun},

		{(uint8 *)AT_CIPMUX,	    		3,	 1*SEND_1T,	EXE_NO,	AtCipmuxFun},
		{(uint8 *)AT_CSTT,	    			3,	 1*SEND_1T,	EXE_NO,	AtCsttFun},
		{(uint8 *)AT_NETOPEN,	   	 	 30,	 1*SEND_1T,	EXE_NO,	AtNetOpenFun},
		{(uint8 *)AT_NETCLOSE,				3,	 1*SEND_1T,	EXE_NO,	AtNetCloseFun},
		{(uint8 *)AT_CIFSR,	         	3,	 1*SEND_1T,	EXE_NO,	AtCifsrFun},
		{(uint8 *)AT_CIPOPEN_TCP,	  100,	 3*SEND_1T,	EXE_NO,	AtCipOpenTcpFun},
		{(uint8 *)AT_CIPSEND_TCP,    	3,	 1*SEND_1T,	EXE_NO,	AtCipSendTcpFun},
		{(uint8 *)AT_CIPCLOSE,      	3,	 1*SEND_1T,	EXE_NO,	AtCipCloseFun},
		{(uint8 *)AT_CIPRXGET,	    	3,	 1*SEND_1T,	EXE_NO,	AtCipRxGetFun},
		{(uint8 *)AT_CIPSRIP,	       	3,	 1*SEND_1T,	EXE_NO,	AtCipSripFun},

		{(uint8 *)AT_GETGPS,	       	3,	 1*SEND_1T,	EXE_NO,	AtGetgpsFun},
		{(uint8 *)AT_EGPSC,	       		3,	 1*SEND_1T,	EXE_NO,	AtEgpscFun},
		{(uint8 *)AT_PMTK314,	       	3,	 1*SEND_1T,	EXE_NO,	AtPmtk314Fun},
		
		{(uint8 *)AT_CPIN,	    	   	3,	 1*SEND_1T,	EXE_NO,	AtCPINFun},
		{(uint8 *)AT_SAPBR,	    	   	30,	 1*SEND_1T,	EXE_NO,	AtSAPBRFun},
		{(uint8 *)AT_FTPTYPE,    	   	30,	 1*SEND_1T,	EXE_NO,	AtFTPTYPEFun},
		{(uint8 *)AT_FTPSERV,    	   	3,	 1*SEND_1T,	EXE_NO,	AtFTPSERVFun},
		{(uint8 *)AT_FTPPORT,    	   	3,	 1*SEND_1T,	EXE_NO,	AtFTPPORTFun},
		{(uint8 *)AT_FTPUN,    		   	3,	 1*SEND_1T,	EXE_NO,	AtFTPUNFun},
		{(uint8 *)AT_FTPPW,    		   	3,	 1*SEND_1T,	EXE_NO,	AtFTPPWFun},
		{(uint8 *)AT_FTPGETNAME,    	3,	 1*SEND_1T,	EXE_NO,	AtFTPGETNAMEFun},
		{(uint8 *)AT_FTPGETPATH,    	3,	 1*SEND_1T,	EXE_NO,	AtFTPGETPATHFun},
		{(uint8 *)AT_FTPGET1,    	  100,	 1*SEND_1T,	EXE_NO,	AtFTPGET1Fun},
		{(uint8 *)AT_FTPGET2,    	  100,	 10*SEND_1T,	EXE_NO,	AtFTPGET2Fun},
		{(uint8 *)AT_FTPQUIT,    	    3,	 1*SEND_1T,	EXE_NO,	AtFTPQUITFun},
		{(uint8 *)AT_FTPSIZE,    	    100,	 1*SEND_1T,	EXE_NO,	AtFTPSIZEFun},
};
static void ReadOverTailIndex(uint16 len)
{
	g_gprs_uart_struct.rx_tail = (len+g_gprs_uart_struct.rx_tail) % GPRS_UART_BUF_LEN;
}
///ATָ�����---start
void AtNullFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_NULL_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_NULL_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}							
void AtFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtE0Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_E0_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_E0_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);	
}
void AtIprFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag){
		g_at_cmd_struct[AT_IPR_INDEX].exe_flag = EXE_OK;
	}
	else{
		g_at_cmd_struct[AT_IPR_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}							
void AtCimiFun(uint8 *data,uint16 len,uint8 flag)
{	
	if(flag) {
		g_at_cmd_struct[AT_CIMI_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIMI_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtCnumFun(uint8 *data,uint16 len,uint8 flag)
{	
	if(flag) {
		g_at_cmd_struct[AT_CNUM_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CNUM_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtCpbsOnFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CPBS_ON_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CPBS_ON_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtCpbwFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CPBW_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CPBW_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}
void AtImeiFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 res,mat_index;
	uint8 cmp_data[] = {"IMEI: "};///+CNUM: ,"13828756403"
	
	g_at_cmd_struct[AT_IMEI_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		if(mat_index > 0) {
			res = IsValidNum(data+mat_index,GSM_IMEI_LEN);
			if(res) {
				MemCpy(g_gprs_data_struct.imei,data+mat_index,GSM_IMEI_LEN);
				g_at_cmd_struct[AT_IMEI_INDEX].exe_flag = EXE_OK;
			}
		}	
	}
	ReadOverTailIndex(len);
}
void AtCsqFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 res,mat_index,tmp_val;
	uint8 cmp_data[] = {"+CSQ: "};
	
	g_at_cmd_struct[AT_CSQ_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		if(mat_index > 0) {
			res = FALSE;
			if(data[mat_index+1] == ',') {
				if(IsValidNum(data+mat_index,1))
				{
					tmp_val = data[mat_index] - '0';
					res = TRUE;
				}
			}
			else {
				if(IsValidNum(data+mat_index,2))
				{
					tmp_val = data[mat_index] - '0';
					tmp_val *= 10;
					tmp_val += data[mat_index+1] - '0';
					res = TRUE;
				}
			}
			
			if(res) {
				if(tmp_val <= 31) {
					g_gprs_data_struct.csq_val = tmp_val;
				}
			}
			g_at_cmd_struct[AT_CSQ_INDEX].exe_flag = EXE_OK;
		}	
	}
	ReadOverTailIndex(len);
}
void AtCregFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+CREG: "};
	
	g_at_cmd_struct[AT_CREG_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			if((data[mat_index+2] == '1')||(data[mat_index+2] == '5')) {
				g_at_cmd_struct[AT_CREG_INDEX].exe_flag = EXE_OK;
			}
		}
	}	
	ReadOverTailIndex(len);
}
void AtCgattFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index;
	uint8 cmp_data[] = {"+CGATT: "};
	
	g_at_cmd_struct[AT_CGATT_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			if(data[mat_index]=='1')//|| data[mat_index]=='0')
			{
				g_at_cmd_struct[AT_CGATT_INDEX].exe_flag = EXE_OK;
			}
		}
	}	
	ReadOverTailIndex(len);
}
void AtCmgfFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CMGF_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CMGF_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCmglFun(uint8 *data,uint16 len,uint8 flag)	//-���յ���AT����,Ȼ������������Ӧ����
{
	uint8 mat_index;
	uint8 cmp_data[] = {"CMGL: "};
	
	if(flag) {
		g_at_cmd_struct[AT_CMGL_INDEX].exe_flag = EXE_OK;
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {//-����0˵������������Ҫ����
	//		SmsProcessFun(data+mat_index,len-mat_index);	//-��ʣ�����ݼ�������
		}
	}
	ReadOverTailIndex(len);
}
void AtCmgsFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CMGS_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CMGS_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCmgdFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CMGD_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CMGD_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtAthFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_ATH_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_ATH_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}								
void AtSleepFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_SLEEP_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_SLEEP_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}																
/* void AtWakeFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_WAKE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_WAKE_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}	 */

/////���������غ���
void AtCipmuxFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCsttFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CSTT_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CSTT_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtNetOpenFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_NETOPEN_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_NETOPEN_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtNetCloseFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCifsrFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIFSR_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIFSR_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipOpenTcpFun(uint8 *data,uint16 len,uint8 flag)
{
	//�ɴ�data��һ��У��CONNECT OK  �� ALREADY CONNECT
	char Aleaddy_open[] = {"ALREADY"};
	
	if(flag) {
		g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag = EXE_OK;
	}
	else {
		if(SubMatch((uint8 *)Aleaddy_open,7,data,len) > 0){
			g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag = EXE_OK;
		}
		g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipSendTcpFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipCloseFun(uint8 *data, uint16 len, uint8 flag)
{    
	if(flag) {
		g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag = EXE_FAIL;
	}	    
	ReadOverTailIndex(len);
}
void AtCipRxGetFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIPRXGET_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPRXGET_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtCipSripFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CIPSRIP_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CIPSRIP_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtGetgpsFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}
void AtEgpscFun(uint8 *data,uint16 len,uint8 flag)
{
	char temp_ack[8]={"+EGPSP: "};
	uint16 mat_index;
	
	mat_index = SubMatch((uint8 *)temp_ack,8,data,len);
	if(mat_index > 0){//  ?
		if(flag){
			if(data[mat_index] == '1'){
				g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_GPS_ON;
			}
			else if(data[mat_index] == '0'){
				g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_GPS_OFF;
			}
		}
		else{
			g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_FAIL;
		}
	}
	else{//   1/0
		if(flag) {
			g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_OK;
		}
		else {
			g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag = EXE_FAIL;
		}	
	}
	ReadOverTailIndex(len);
}
void AtPmtk314Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtCPINFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_CPIN_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_CPIN_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtSAPBRFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_SAPBR_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPTYPEFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPTYPE_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPTYPE_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPSERVFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPSERV_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPSERV_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPPORTFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPPORT_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPPORT_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPUNFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPUN_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPUN_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPPWFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPPW_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPPW_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPGETNAMEFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPGETNAME_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPGETNAME_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPGETPATHFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPGETPATH_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPGETPATH_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}

void AtFTPGET1Fun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPGET1_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPGET1_INDEX].exe_flag = EXE_FAIL;
	}	
	ReadOverTailIndex(len);
}


void AtFTPGET2Fun(uint8 *data,uint16 len,uint8 flag)
{
    uint8 i = 0,j = 0,cmp_data[] = {"+FTPGET: 2,"},cmp_data3[] = {"+FTPGET: 1,1"};
	uint16 flash_len = 0,mat_index = 0,rx_len = 0,source_head = 0,end_head = 0,time;
	uint8 ack_flag = FALSE,rrx_data[L218_MAX_BUF_LEN];

	
	g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag = EXE_FAIL;

	mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);	//-��ȡ�ֽڳ�������,���ұ����и��Ӵ���
	if(mat_index > 0)
	{
		while((data[mat_index + i] >= 0x30)&&(data[mat_index + i] <= 0x39))
			i++;
		while(j < i)
		{
			flash_len *= 10;
			flash_len += (data[mat_index + j] - 0x30);
			j++;
		}
		source_head = mat_index + i + 4;	//-ָ�����ݿ�ʼ�ĵط�
		end_head = source_head + flash_len + 2 + 8;	//-ָ����+FTPGET:1,�Ŀ�ʼλ��
		if((end_head > L218_MAX_BUF_LEN) || (flash_len > 1024))
		{
			ReadOverTailIndex(len);
			return;	
		}	
			
		if(len < end_head + 11)	//-˵��û�н���ȫ��Ҫ����ֱ�����ս���
		{
			for(time=0;time<g_at_cmd_struct[AT_FTPGET2_INDEX].max_ms_wait;time++)
			{
				OSTimeDlyHMSM(0, 0, 0, 100);
				rx_len = L218UartIsRxDone(rrx_data,L218_MAX_BUF_LEN);
				if(rx_len > end_head + 11)
				{
					ack_flag = TRUE;
					data = rrx_data;
					break;
				}
			}
			if(!ack_flag)
			{
				ReadOverTailIndex(rx_len);
				return;
			}
		}

		MemCpy(TEMPF_DATA_FROM_FTP + ftp_struct.ftp_rx_len,data + source_head,flash_len);
		ftp_struct.ftp_rx_file_byte_counter += flash_len;
		ftp_struct.ftp_rx_len += flash_len;

		if(ftp_struct.ftp_rx_len >= 2048)
		{
			CpuFlashWrite(FTP_BLIND_FLASH_START+((ftp_struct.ftp_rx_file_byte_counter / 2048) - 1) * 2048,TEMPF_DATA_FROM_FTP, 2048);
			ftp_struct.ftp_rx_len -= 2048;
			if(ftp_struct.ftp_rx_len)
				MemCpy(TEMPF_DATA_FROM_FTP,TEMPF_DATA_FROM_FTP + ftp_struct.ftp_rx_len,ftp_struct.ftp_rx_len);
		}

		if(ftp_struct.ftp_rx_file_byte_counter >= boot_struct.program_total_size)	//-�ж��Ǽ������ǽ���
		{//-�������ӽ������ж�
			g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag = EXE_OK;
			if(ftp_struct.ftp_rx_len)
			{
				CpuFlashWrite(FTP_BLIND_FLASH_START+(ftp_struct.ftp_rx_file_byte_counter / 2048) * 2048,TEMPF_DATA_FROM_FTP, ftp_struct.ftp_rx_len);
				ftp_struct.ftp_rx_len = 0;
				ftp_struct.ftp_rx_file_byte_counter = 0;
			}
		}
		else
		{
			mat_index = SubMatch(cmp_data3,StrLen(cmp_data3,0),data + end_head,len - end_head);	//-��֤��β�����������ݲ���
			if(mat_index > 0)
				g_at_cmd_struct[AT_FTPGET2_INDEX].exe_flag = EXE_CONTINUE;	//-��Ҫ���ӳ�ʱ�˳�
		}
		
	}
	
	ReadOverTailIndex(len);
}

void AtFTPQUITFun(uint8 *data,uint16 len,uint8 flag)
{
	if(flag) {
		g_at_cmd_struct[AT_FTPQUIT_INDEX].exe_flag = EXE_OK;
	}
	else {
		g_at_cmd_struct[AT_FTPQUIT_INDEX].exe_flag = EXE_FAIL;
	}
	ReadOverTailIndex(len);
}

void AtFTPSIZEFun(uint8 *data,uint16 len,uint8 flag)
{
	uint8 mat_index,i = 0,j = 0;
	uint8 cmp_data[] = {"+FTPSIZE: 1,0,"};
	uint32 temp_len = 0;
	
	g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag = EXE_FAIL;
	
	if(flag) {
		mat_index = SubMatch(cmp_data,StrLen(cmp_data,0),data,len);
		
		if(mat_index > 0) {
			while((data[mat_index + i] >= 0x30)&&(data[mat_index + i] <= 0x39))
				i++;
			while(j < i)
			{
				temp_len *= 10;
				temp_len += (data[mat_index + j] - 0x30);
				j++;
			}
			boot_struct.program_total_size = temp_len;
			g_at_cmd_struct[AT_FTPSIZE_INDEX].exe_flag = EXE_OK;
		}
	}	
	ReadOverTailIndex(len);
}












///ATָ�����---end

/******************************************************
//ATָ���
******************************************************/
//                    ATָ������      ��������      �������ݳ���     ƥ������      ƥ�����ݳ���
void L218SendAtCmd(uint8 cmd_index,uint8 app_data[],uint8 app_len,uint8 mat_data[],uint8 mat_len)
{
	uint8 count,ack_flag = FALSE,rx_data[L218_MAX_BUF_LEN];
	uint16 rx_len = 0,rx_pt = 0,time,mat_index = 0;
	
	g_at_cmd_struct[cmd_index].exe_flag = EXE_NO;
	//ָ��ʹ���
	for(count=0; count<g_at_cmd_struct[cmd_index].tx_max_count; count++)
	{
		//����ָ��
		GprsUartFixedLenSend(g_at_cmd_struct[cmd_index].cmd_text,StrLen(g_at_cmd_struct[cmd_index].cmd_text,0));
		if(app_len > 0){
			GprsUartFixedLenSend(app_data,app_len);
		}		
		#ifdef L218_DEBUG
			LocalUartFixedLenSend((uint8*)"\r\nSEND AT...\r\n",StrLen((uint8*)"\r\nSEND AT...\r\n",0));
			LocalUartFixedLenSend(g_at_cmd_struct[cmd_index].cmd_text,StrLen(g_at_cmd_struct[cmd_index].cmd_text,0));
			if(app_len > 0){
				LocalUartFixedLenSend(app_data,app_len);
			}
			LocalUartFixedLenSend((uint8*)"\r\nSEND AT END...\r\n",StrLen((uint8*)"\r\nSEND AT END...\r\n",0));
		#endif
		//��ʱ�ȴ�Ӧ��
		rx_pt = 0;
		for(time=0;time<g_at_cmd_struct[cmd_index].max_ms_wait;time++)
		{
			OSTimeDlyHMSM(0, 0, 0, WAIT_100MS);
			//-OSTimeDlyHMSM(0, 0, 0, 200);
			rx_len = L218UartIsRxDone(rx_data,L218_MAX_BUF_LEN);
			if(rx_len > 0)
			{
				#ifdef L218_DEBUG
					//-LocalUartFixedLenSend(rx_data + rx_pt,rx_len - rx_pt);
					//-rx_pt = rx_len;
				#endif
				mat_index = SubMatch(mat_data,mat_len,rx_data,rx_len);
				if(mat_index > 0){
					ack_flag = TRUE;
					break;
				}
			}
		}
		if(ack_flag)
		{
			break;
		}
	}
	
	(g_at_cmd_struct[cmd_index].fun)(rx_data,rx_len,ack_flag);
}

uint16 L218UartIsRxDone(uint8 data[],uint16 len)	//-������ѯ�Ƿ�����Ҫ������
{
	uint16 count=0,index;
	static uint16 rx_head_pt = 0;
	
	if(g_gprs_uart_struct.rx_head != g_gprs_uart_struct.rx_tail)
	{//������
		count = (GPRS_UART_BUF_LEN + g_gprs_uart_struct.rx_head - g_gprs_uart_struct.rx_tail) % GPRS_UART_BUF_LEN;
		if(count > len){//����ָ��Խ��
			return 0;
		}
		for(index=0; index < count; index++){
			data[index] = g_gprs_uart_struct.rx_buf[((index+g_gprs_uart_struct.rx_tail) % GPRS_UART_BUF_LEN)];
		}
		#ifdef L218_DEBUG_qlj
//		printf("GPRS UART Received data: ");
//		LocalUartFixedLenSend(data, count);
//		printf(" \r\n");	
		while(rx_head_pt != g_gprs_uart_struct.rx_head)
		{
			LocalUartFixedLenSend(&g_gprs_uart_struct.rx_buf[rx_head_pt], 1);
			rx_head_pt = (rx_head_pt + 1) % GPRS_UART_BUF_LEN;
		}
		#endif		
		
	}
	return count;
}
/******************************************************
//ģ����I/O��ʼ��
******************************************************/
void L218PowerInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; ///POWER_ON/OFF,4V��Դ������� 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; ///POWER_ON/OFF,4V��Դ������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;///POWERKEY_CONNECT��ģ��ػ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	L218Reset();
}
/******************************************************
//ģ�鸴λ&������
******************************************************/
void L218Reset(void)
{
	uint8 i;
	uint32 bpr[3] = {38400,9600,115200};
		
	HIGH_GPRS_IGT();
	OFF_GPRS_PWR();///�ȹػ�
	OSTimeDlyHMSM(0, 0,5, 0);
	ON_GPRS_PWR(); ///�ٿ���
	OSTimeDlyHMSM(0, 0,1, 0);
	LOW_GPRS_IGT();
	OSTimeDlyHMSM(0, 0,5, 0);
	HIGH_GPRS_IGT();
	OSTimeDlyHMSM(0, 0,1, 0);
	
//ģ���Զ���ȡ������	
	for(i=0;i<3;i++){
		UsartInit(GPRS_USART, bpr[i], USART_DATA_8B, USART_STOPBITS_1, USART_PARITY_NO);
		L218SendAtCmd(AT_INDEX,NULL,0,(uint8 *)OK_ACK,2);
		if(g_at_cmd_struct[AT_INDEX].exe_flag == EXE_OK){
			break;
		}
	}
	
	if(i != 0){
		L218SendAtCmd(AT_IPR_INDEX,NULL,0,(uint8 *)OK_ACK,2);///����ģ��Ĳ�����Ϊ38400
		UsartInit(GPRS_USART, 38400, USART_DATA_8B, USART_STOPBITS_1, USART_PARITY_NO);
	}
}
///****************���繤����������***********************
////
////����ֵ:0:�ɹ�; 1:ʧ��; 2:����ִ��;
//******************************************************/
//static uint8 GprsConfigNetwork(void)
//{ 
//	//��������
//  L218SendAtCmd(AT_CIPMUX_INDEX, NULL, 0, (uint8 *)OK_ACK, 2);
//	if(g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag == EXE_FAIL){
//		return 1;
//	}
//	else if(g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag == EXE_NO){
//		return 2;
//	}
//    
//  L218SendAtCmd(AT_CSTT_INDEX, NULL, 0, (uint8 *)OK_ACK, 2);
//	if(g_at_cmd_struct[AT_CSTT_INDEX].exe_flag == EXE_FAIL){
//		return 1;
//	}
//	else if(g_at_cmd_struct[AT_CSTT_INDEX].exe_flag == EXE_NO){
//		return 2;
//	}

//  return 0;
//}



/****************����������***************************
//
//����ֵ:0:�ɹ�; 1:ʧ��;2:����ִ�� 
******************************************************/
static uint8 GprsConnectNetProcess(void)
{
	uint8 res = 2;
	
	switch(g_gprs_ctr_struct.ope)
	{
		//��������
		case AT_NETCLOSE_INDEX:{
			L218SendAtCmd(AT_NETCLOSE_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_OK)
			{//�ɹ�
				g_gprs_ctr_struct.ope = AT_CIPMUX_INDEX;
			}
			else if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CIPMUX_INDEX:{
			L218SendAtCmd(AT_CIPMUX_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CSTT_INDEX;
			}
			else if(g_at_cmd_struct[AT_CIPMUX_INDEX].exe_flag == EXE_FAIL){
				//g_gprs_ctr_struct.ope = AT_NETCLOSE_INDEX;
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CSTT_INDEX:{
			L218SendAtCmd(AT_CSTT_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CSTT_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_NETOPEN_INDEX;
			}
			else if(g_at_cmd_struct[AT_CSTT_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		//����NET
		case AT_NETOPEN_INDEX:{
			L218SendAtCmd(AT_NETOPEN_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_NETOPEN_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CIFSR_INDEX;
			}
			else if(g_at_cmd_struct[AT_NETOPEN_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CIFSR_INDEX:{
			L218SendAtCmd(AT_CIFSR_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CIFSR_INDEX].exe_flag == EXE_OK){
				res = 0;
				break;
			}
			else if(g_at_cmd_struct[AT_CIFSR_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		default: 	res = 1;   break;
	}
		
  return res;	
}
/****************�Ͽ���������***************************
//
//����ֵ:0:�ɹ�; 1:ʧ��;2:����ִ�� 
******************************************************/
static uint8 GprsDisonnectNetProcess(void)
{
  L218SendAtCmd(AT_NETCLOSE_INDEX, NULL, 0, (uint8 *)OK_ACK, 2);
	if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}
	else if(g_at_cmd_struct[AT_NETCLOSE_INDEX].exe_flag == EXE_NO){
		return 2;
	}

  return 0;	
}

/****************�򿪷���������*************************
//
//����ֵ:0:�ɹ�; 1:ʧ��;2:����ִ�� 
******************************************************/
static uint8 GprsConnectSerProcess(void)
{
///	char ip[] = {"218.94.153.146"};///{"192.168.10.22"};
///	char port[] = {"9903"};
  char temp_cmd[45];

	sprintf(temp_cmd, (const char*)AT_CIPOPEN_TCP, g_gprs_data_struct.IP[0],g_gprs_data_struct.IP[1],g_gprs_data_struct.IP[2],g_gprs_data_struct.IP[3], g_gprs_data_struct.Port);
///	sprintf(temp_cmd, (const char*)AT_CIPOPEN_TCP, ip, port);
	g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].cmd_text = (uint8 *)temp_cmd;
 	L218SendAtCmd(AT_CIPOPEN_TCP_INDEX, NULL, 0, (uint8 *)OK_ACK, 2);
	if(g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}
	else if(g_at_cmd_struct[AT_CIPOPEN_TCP_INDEX].exe_flag == EXE_NO){
		return 2;
	} 

  return 0;
}
/****************�Ͽ�����������*************************
//
//����ֵ:0:�ɹ�; 1:ʧ��;2:����ִ�� 
******************************************************/
static uint8 GprsDisconnectSerProcess(void)
{
	L218SendAtCmd(AT_CIPCLOSE_INDEX, NULL, 0, (uint8 *)OK_ACK, 2);
	if(g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}
	else if(g_at_cmd_struct[AT_CIPCLOSE_INDEX].exe_flag == EXE_NO){
		return 2;
	}

  return 0;
}
/****************���緢������**************************
//
//����ֵ:0:�ɹ�; 1:ʧ��; 
******************************************************/
//                         ��������    �������ݳ���
static uint8 GprsSendDataProcess(uint8 *data, uint16 len)
{
	uint16 temp_len;
	uint8  temp_data[40];
	char   temp_ack[] = ">";
	#ifdef GPRS_DEBUG
	uint16 i;
	#endif	

	sprintf((char *)temp_data, (const char*)AT_CIPSEND_TCP, len);
	g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].cmd_text = (uint8 *)temp_data;
	L218SendAtCmd(AT_CIPSEND_TCP_INDEX, NULL, 0, (uint8 *)temp_ack, StrLen((const uint8 *)temp_ack, 0));
	if(g_at_cmd_struct[AT_CIPSEND_TCP_INDEX].exe_flag == EXE_FAIL){
		#ifdef GPRS_DEBUG
		printf("ack error \r\n");	
		#endif	 	  
		return 1;
	}
	
	#ifdef GPRS_DEBUG
		printf("\r\n GPRS send data is:\r\n");
		for(i=0;i<len;i++){
			printf("%.2X ",data[i]);
		}	
		printf("\r\n");		
	#endif
	OSTimeDlyHMSM(0, 0, 0, 100);   
	if(len > 0){
		GprsUartFixedLenSend(data, len);
		GprsUartFixedLenSend((uint8 *)"\x0d\x0a", 2);
		
		OSTimeDlyHMSM(0, 0, 0, 100);
		temp_len = L218UartIsRxDone(temp_data,40);
		if(temp_len > 0){
			ReadOverTailIndex(temp_len);
			#ifdef GPRS_DEBUG
				LocalUartFixedLenSend(temp_data,temp_len);		
			#endif
			if(SubMatch((uint8 *)CIPSEND_EXTRA_OK,7,temp_data,temp_len) == 0){
			 //û��CIPSEND_EXTRA_OK��Ӧ
			 return 1;
			}	
		}
	}

	return 0;
}

uint8 GprsRecvDataCmd(uint8 *data, uint16 len)
{
	L218SendAtCmd(AT_CIPRXGET_INDEX, NULL, 0, (uint8 *)OK_ACK, StrLen((const uint8 *)OK_ACK, 0));
	if(g_at_cmd_struct[AT_CIPRXGET_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}

// qlj �������ݴ�����
/* 	if(g_at_resp_data_len > 0){
		g_at_resp_data_len = g_at_resp_data_len > len ? len : g_at_resp_data_len;
		MemCpy(data, g_at_resp_data, g_at_resp_data_len);
	}
	GenRecvData(id, data, g_at_resp_data_len); */
    
   return 0;
}
/****************ģ���ʼ������***********************
//��ʼ����������
//����ֵ:0����ʼ���ɹ�;1:��ʼ��ʧ��;2:���ڳ�ʼ��
******************************************************/
static uint8 GprsInitProcess(void)
{
	uint8 res = 2;
	
	switch(g_gprs_ctr_struct.ope)
	{
		case AT_INDEX:{
			L218SendAtCmd(AT_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_INDEX].exe_flag == EXE_OK)
			{//�ɹ�
				g_gprs_ctr_struct.ope = AT_E0_INDEX;
			}
			else if(g_at_cmd_struct[AT_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_E0_INDEX:{
			L218SendAtCmd(AT_E0_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_E0_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CIMI_INDEX;
			}
			else if(g_at_cmd_struct[AT_E0_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CIMI_INDEX:{
			L218SendAtCmd(AT_CIMI_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CIMI_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CSQ_INDEX;
			}
			else if(g_at_cmd_struct[AT_CIMI_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CSQ_INDEX:{
			OSTimeDlyHMSM(0, 0, 2, 0);
			L218SendAtCmd(AT_CSQ_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CSQ_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CREG_INDEX;
			}
			else if(g_at_cmd_struct[AT_CSQ_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CREG_INDEX:{
			L218SendAtCmd(AT_CREG_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CREG_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CGATT_INDEX;
			}
			else if(g_at_cmd_struct[AT_CREG_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CGATT_INDEX:{
			L218SendAtCmd(AT_CGATT_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CGATT_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CMGL_INDEX;
			}
			else if(g_at_cmd_struct[AT_CGATT_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CMGL_INDEX:{
			L218SendAtCmd(AT_CMGL_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CMGL_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_CNUM_INDEX;
			}
			else if(g_at_cmd_struct[AT_CMGL_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		case AT_CNUM_INDEX:{
			L218SendAtCmd(AT_CNUM_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_CNUM_INDEX].exe_flag == EXE_OK){
				res = 0;
				break;
			}
			else if(g_at_cmd_struct[AT_CNUM_INDEX].exe_flag == EXE_FAIL){
				res = 1;
				break;
			}
			else{
			  break;  
			}
		}
		default: 	res = 1;   break;
	}
	
	return res; 
}

/****************��GPSģ��****************************
//��ʼ����������
//����ֵ:0����ʼ���ɹ�;1:��ʼ��ʧ��;2:���ڳ�ʼ��
******************************************************/
static uint8 GpsOpenProcess(void)
{
	uint8 res = 2;
	char temp_cmd[15];
	
	switch(g_gprs_ctr_struct.ope)
	{
		case AT_GETGPS_INDEX:{
			L218SendAtCmd(AT_GETGPS_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_EGPSC_INDEX;
			}
			else if(g_at_cmd_struct[AT_GETGPS_INDEX].exe_flag == EXE_FAIL){
				return 1;
			}
			else{
			  break;  
			}
		}
		case AT_EGPSC_INDEX:{
			sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "?");
			g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
			L218SendAtCmd(AT_EGPSC_INDEX,NULL,0,(uint8 *)OK_ACK,2);	
			if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_GPS_ON){
				g_gprs_ctr_struct.ope = AT_PMTK314_INDEX;
				goto RETURN_LAB;
			//	return 0;
			}
			else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
				return 1;
			}
			
			sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "=1");	
			g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
			L218SendAtCmd(AT_EGPSC_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_OK){
				g_gprs_ctr_struct.ope = AT_PMTK314_INDEX;
				goto RETURN_LAB;
			//	res = 0;
			}
			else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
				return 1;
			}
			break;  
		}
		
		
		
		case AT_PMTK314_INDEX:{
		RETURN_LAB:
			OSTimeDlyHMSM(0, 0, 5, 0); //>2S�ȴ�GPS��Ϣͬ�����
			L218SendAtCmd(AT_PMTK314_INDEX,NULL,0,(uint8 *)OK_ACK,2);			
			if(g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag == EXE_OK){
				res = 0;
			}
			else if(g_at_cmd_struct[AT_PMTK314_INDEX].exe_flag == EXE_FAIL){
				res = 1;
			}
			break; 
		}
		
		default: 	res = 1;   break;
	}
	
	return res; 
}

/****************�ر�GPSģ��****************************
//
//����ֵ:0:�ɹ�; 1:ʧ��;2:����ִ�� 
******************************************************/
static uint8 GpsCloseProcess(void)
{
	char temp_cmd[15];
	
	sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "?");
	g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
	L218SendAtCmd(AT_EGPSC_INDEX,NULL,0,(uint8 *)OK_ACK,2);	
	if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_GPS_OFF){
		return 0;
	}
	else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}
	
	sprintf((char *)temp_cmd, (const char*)AT_EGPSC, "=0");
	g_at_cmd_struct[AT_EGPSC_INDEX].cmd_text = (uint8 *)temp_cmd;
	L218SendAtCmd(AT_EGPSC_INDEX, NULL, 0, (uint8 *)OK_ACK, 2);
	if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_FAIL){
		return 1;
	}
	else if(g_at_cmd_struct[AT_EGPSC_INDEX].exe_flag == EXE_NO){
		return 2;
	}

  return 0;
}	



/****************��ȡGPSʱ�䣬����**********************
//
//����ֵ:0:ʧ��; 1:�ɹ�; 
******************************************************/
static uint8 GpsGetTime(uint8 utc_time[],uint8 hex_time[])
{
	uint8 i,res,u8_val;
	
	res = IsValidNum(utc_time,6);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	for(i=0;i<3;i++)
	{
		u8_val = (utc_time[i*2] - '0')*10;
		u8_val += utc_time[i*2+1] - '0';
		hex_time[i] = u8_val;
	}
	
	if((hex_time[0] >= 24)||(hex_time[1] >= 60)||(hex_time[2] >= 60))
	{
		res = FALSE;
	}
	else
	{
		res = TRUE;
	}
RETURN_LAB:
	return res;
}
static uint8 GpsGetDate(uint8 utc_date[],uint8 hex_date[])
{
	uint8 i,u8_val,res;
	
	res = IsValidNum(utc_date,6);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	for(i=0;i<3;i++)
	{
		u8_val = (utc_date[i*2] - '0')*10;
		u8_val += utc_date[i*2+1] - '0';
		hex_date[2-i] = u8_val;
	}
	
	if((hex_date[0] < 14 )||
	   (hex_date[0] >= 99)||
	   (hex_date[1] > 12 )||
	   (hex_date[1] < 1 )||
	   (hex_date[2] > 31)||
	   (hex_date[2] < 1))///�꣬�£���
	{
		res = FALSE;
	}
	else
	{
		res = TRUE;
	}
RETURN_LAB:
	return res;
}

/****************GPS UTCʱ��ת����8��******************
//
******************************************************/
static void GpsAdd8Hour(uint8 d_t[])
{
	if(d_t[3] < 16)
	{
		d_t[3] += 8;
	}
	else
	{
		d_t[3] += 8;
		d_t[3] = d_t[3] % 24;
		
		d_t[2] += 1;
		if(d_t[2] > 28)
		{
			switch(d_t[1])
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				{
					if(d_t[2] > 31)
					{
						d_t[2] = 1;
						d_t[1] += 1;
					}
					break;
				}
				case 2:
				{
					if((d_t[0] % 4 == 0)&&(d_t[0] % 100 != 0))
					{
						if(d_t[2] > 29)
						{
							d_t[2] = 1;
							d_t[1] += 1;
						}
					}
					else
					{
						if(d_t[2] > 28)
						{
							d_t[2] = 1;
							d_t[1] += 1;
						}
					}
					break;
				}
				case 4:
				case 6:
				case 9:
				case 11:
				{
					if(d_t[2] > 30)
					{
						d_t[2] = 1;
						d_t[1] += 1;
					}
					break;
				}
				case 12:
				{
					if(d_t[2] > 31)
					{
						d_t[2] = 1;
						d_t[1] = 1;
						d_t[0] += 1;
					}
					break;
				}
			}
		}
	}
}

/****************GPS�ַ�������תʮ������***************
//
//����ֵ:0:ʧ��; 1:�ɹ�; 
******************************************************/
static uint8 GpsCharFloatDataToHex(uint8 gps_data[],uint8 len,uint8 type,uint32 *r_val)
{
	uint8 i,j,k,m,res;
	uint32 u32_val = 0,u32_val_div = 0;
	
	res = IsValidCharFloatNum(gps_data,len);
	if(!res)
	{
		goto RETURN_LAB;
	}
	
	for(i=0;i<len;i++)
	{
		if(gps_data[i] != '.')
		{
			u32_val *= 10;
			u32_val += gps_data[i] - 0x30;
			
		}
		else
		{
			j = len - i - 1;///С������λ��
			break;
		}
	}
	
	i++;
	if((type == LAT_TYPE)||(type == LONG_TYPE))///��γ��ȡС�����5λ
	{
		k = 0;
		if(j > 5)
		{
			j = 5;
		}
		else
		{
			if(j < 5)
			{
				k = 5 - j;///����5λ����0����
			}
		}
	}
	
	if((type == LAT_TYPE)||(type == LONG_TYPE))
	{
		u32_val_div = u32_val % 100;
		u32_val /= 100;
		u32_val *= 100;
		for(m=0;m<j;m++)///С��
		{
			u32_val *= 10;
			u32_val_div *= 10;
			u32_val_div += gps_data[i++] - 0x30;
		}
		
		for(m=0;m<k;m++)
		{
			u32_val *= 10;
			u32_val_div *= 10;
		}
		u32_val_div += 5;///l��������
		
		u32_val_div = u32_val_div * 5;
		u32_val_div /= 3;
	}
	else
	{
		for(m=0;m<j;m++)///С��
		{
			u32_val *= 10;
			u32_val += gps_data[i++] - 0x30;
		}
	}

	if(type == LAT_TYPE)
	{
		u32_val += u32_val_div;
		u32_val /= 10;///���ȣ�С�����4λ
		if(u32_val > 90000000)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == LONG_TYPE)
	{
		u32_val += u32_val_div;
		
		u32_val /= 10;
		if(u32_val > 180000000)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == SPEED_TYPE)
	{		
		for(i=0;i<j;i++)///ȡ��
		{
			u32_val /= 10;
		}
		
		u32_val *= 1852;
		u32_val /= 1000;
		
		if(u32_val > 255)///��תKM/H
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == DIR_TYPE)
	{
		for(i=0;i<j;i++)///ȡ��
		{
			u32_val /= 10;
		}
		
		u32_val /= 2;///1����2��
		
		if(u32_val > 180)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else if(type == AMP_TYPE)
	{
		for(i=0;i<j;i++)///ȡ��
		{
			u32_val /= 10;
		}
		
		if(u32_val > 9999)
		{
			res = FALSE;
			goto RETURN_LAB;
		}
	}
	else
	{
		res = FALSE;
		goto RETURN_LAB;
	}
	
	*r_val = u32_val;
	res = TRUE;
RETURN_LAB:
	return res;
}

/****************��ȡGPRMC\GNGGA����*******************
//
******************************************************/
static void GpsGetGprmcGpggaInfo(uint8 gps_data[],uint16 len,uint8 type)
{
	uint8 i,j,dot_index[14],t_d[6],res,res_1,res_2;
	uint8 lat_len,long_len,speed_len,dir_len,sat_len,amp_len;
	uint16 tmp_amp_val;
	uint32 lat_val,long_val,speed_val,dir_val,sat_val,amp_val;
	
	#ifdef GPS_DEBUG
		char str_ch[256];
		uint8 str_len;
	#endif
	///Ѱ�Ҷ���
	if(len>100)len=100;
	j=0;
	for(i=0; i<len; i++)
	{
		if(gps_data[i] == ','){
			dot_index[j++] = i;
		}
		if(j>=13){
			break;
		}
	}
	if(j==11 || j==13){}//GGA_j=13,RMC_j=11�ڶ�����������
	else{
//		#ifdef GPS_DEBUG
//			str_len = sprintf(str_ch,"\r\n********************%d--------------j=%d\r\n",type,j);
//			LocalUartFixedLenSend((uint8*)str_ch,str_len);
//		#endif
		return;
	}
	
	if(type == RMC_TYPE)
	{
		if((gps_data[0] != ',')&&(dot_index[0] >= 6)&&
		   (gps_data[dot_index[7]+1] != ',')&&((dot_index[8]-dot_index[7]) >= 6))
		{///��ȡ����ʱ��
			res_1 = GpsGetTime(gps_data,t_d+3);
			res_2 = GpsGetDate(gps_data+dot_index[7]+1,t_d);
			
			if(res_1 && res_2)
			{
				GpsAdd8Hour(t_d);				
				MemCpy(&g_gps_struct.gpsinform.subitem.year,t_d,6);				
//				if(sys_misc_run_struct.gps_need_datetime_check_flag)
//				{	
//					sys_misc_run_struct.gps_need_datetime_check_flag = FALSE;
//					RtcSetCalendarTime();
//				}
				#ifdef GPS_DEBUG
					str_len = sprintf(str_ch,"\r\n%02d��%02d��%02d�� %02dʱ%02d��%02d��\t",t_d[0],t_d[1],t_d[2],t_d[3],t_d[4],t_d[5]);
					LocalUartFixedLenSend((uint8*)str_ch,str_len);
				#endif
			}
		}
		
		if(gps_data[dot_index[0]+1] != 'A'){//����λ
			g_gps_struct.gpsinform.subitem.statu.bit.B0 = 1;
//		OFF_GPS_LED();
		}
		else{
			g_gps_struct.gpsinform.subitem.statu.bit.B0 = 0;
//		ON_GPS_LED();
		}
		
		lat_len = dot_index[2]-dot_index[1]-1;
		long_len = dot_index[4]-dot_index[3]-1;
		if((gps_data[dot_index[1]+1] != ',')&&(lat_len > 0)&&
		   (gps_data[dot_index[3]+1] != ',')&&(long_len > 0))
		{///��ȡγ���뾭��
			if(((gps_data[dot_index[2]+1] == 'N')||(gps_data[dot_index[1]+1] == 'S'))&&
			   ((gps_data[dot_index[4]+1] == 'E')||(gps_data[dot_index[3]+1] == 'W')))
			{
				res_1 = GpsCharFloatDataToHex(gps_data+dot_index[1]+1,lat_len,LAT_TYPE,&lat_val);
				res_2 = GpsCharFloatDataToHex(gps_data+dot_index[3]+1,long_len,LONG_TYPE,&long_val);
				
				if(res_1 && res_2)
				{
					if(gps_data[dot_index[2]+1] == 'N'){		
						g_gps_struct.gpsinform.subitem.statu.bit.B1 = 0;					
					}
					else{					
						g_gps_struct.gpsinform.subitem.statu.bit.B1 = 1;	
					}
					if(gps_data[dot_index[4]+1] == 'E'){				
						g_gps_struct.gpsinform.subitem.statu.bit.B2 = 0;					
					}
					else{					
						g_gps_struct.gpsinform.subitem.statu.bit.B2 = 1;	
					}
					
					g_gps_struct.gpsinform.subitem.latitude = lat_val;
					g_gps_struct.gpsinform.subitem.longitude = long_val;
					#ifdef GPS_DEBUG
						str_len = sprintf(str_ch,"%ld%c %ld%c\t",lat_val,gps_data[dot_index[2]+1],long_val,gps_data[dot_index[4]+1]);
						LocalUartFixedLenSend((uint8*)str_ch,str_len);
					#endif					
				}
			}
		}
		
		speed_len = dot_index[6]-dot_index[5]-1;
		if((gps_data[dot_index[5]+1] != ',')&&(speed_len > 0))
		{///��ȡ�ٶ�
			res = GpsCharFloatDataToHex(gps_data+dot_index[5]+1,speed_len,SPEED_TYPE,&speed_val);
			if(res)
			{
				g_gps_struct.gpsinform.subitem.speedk = speed_val;
				#ifdef GPS_DEBUG
					str_len = sprintf(str_ch,"�ٶȣ�%03d\t",speed_val);
					LocalUartFixedLenSend((uint8*)str_ch,str_len);
				#endif
			}
		}
		
		dir_len = dot_index[7]-dot_index[6]-1;
		if((gps_data[dot_index[6]+1] != ',')&&(dir_len > 0))
		{///��ȡ����
			res = GpsCharFloatDataToHex(gps_data+dot_index[6]+1,dir_len,DIR_TYPE,&dir_val);
			if(res)
			{
				g_gps_struct.gpsinform.subitem.direction = dir_val;
				#ifdef GPS_DEBUG
					str_len = sprintf(str_ch,"����%03d ��\r\n",dir_val*2);
					LocalUartFixedLenSend((uint8*)str_ch,str_len);
				#endif
			}
		}
	}
	else if(type == GGA_TYPE)
	{
		sat_len = dot_index[6]-dot_index[5]-1;
		if((gps_data[dot_index[5]+1] != ',')&&(sat_len > 0)&&(sat_len <= 2))
		{///��ȡ���Ǹ���
			res = IsValidNum(gps_data+dot_index[5]+1,sat_len);
			if(res)
			{
				sat_val = 0;
				for(i=0;i<sat_len;i++)
				{
					sat_val *= 10;
					sat_val += gps_data[dot_index[5]+1+i] - 0x30;
				}
				g_gps_struct.gpsinform.subitem.starnum = sat_val;
				#ifdef GPS_DEBUG
					str_len = sprintf(str_ch,"����������: %02d\t",sat_val);
					LocalUartFixedLenSend((uint8*)str_ch,str_len);
				#endif
			}
		}
		else
		{
			g_gps_struct.gpsinform.subitem.starnum  = 0;
		}
		
		amp_len = dot_index[10]-dot_index[9]-1;
		if((gps_data[dot_index[9]+1] != ',')&&(amp_len > 0))
		{///��ȡ����
			tmp_amp_val = 0x0000;
			i = 1;
			if(gps_data[dot_index[9]+1] == '-')
			{
				tmp_amp_val = 0x8000;
				i = 2;
				amp_len -= 1;
			}
			res = GpsCharFloatDataToHex(gps_data+dot_index[9]+i,amp_len,AMP_TYPE,&amp_val);
			if(res)
			{
				amp_val = tmp_amp_val | amp_val;
				g_gps_struct.gpsinform.subitem.high = amp_val;
				#ifdef GPS_DEBUG
					str_len = sprintf(str_ch,"����: %d\r\n",amp_val);
					LocalUartFixedLenSend((uint8*)str_ch,str_len);
				#endif
			}
		}
	}

	return;
}



/****************GPSģ�����ݲ�׽***********************
//
******************************************************/
void SearchDataFromGPS(void)
{
	uint8 recdata[GPS_UART_BUF_LEN];
	uint16 reclen,index,mat_index;
	
	if(g_gps_uart_struct.rx_head != g_gps_uart_struct.rx_tail)	//-�ж��Ƿ��������ݽ��յ�
	{//������
		reclen = (GPS_UART_BUF_LEN + g_gps_uart_struct.rx_head - g_gps_uart_struct.rx_tail) % GPS_UART_BUF_LEN;
		if(reclen<(GPS_UART_BUF_LEN/2) || reclen>GPS_UART_BUF_LEN){//����400�ֽ�
		return;
		}
		for(index=0;index < reclen;index++){
			g_gps_uart_struct.rx_tail = ((++g_gps_uart_struct.rx_tail) % GPS_UART_BUF_LEN);
			recdata[index] = g_gps_uart_struct.rx_buf[((g_gps_uart_struct.rx_tail) % GPS_UART_BUF_LEN)];
		}		
		#ifdef GPS_DEBUG  //��ӡGPS������Ϣ
			LocalUartFixedLenSend(recdata, reclen);
		#endif		
		
		mat_index = SubMatch((uint8*)"$GNRMC,",StrLen((uint8*)"$GNRMC,",0),recdata,reclen);
		if(mat_index > 0){
			//��GNRMC����	
			GpsGetGprmcGpggaInfo(recdata+mat_index, reclen-mat_index, RMC_TYPE);
		}
		mat_index = SubMatch((uint8*)"$GNGGA,",StrLen((uint8*)"$GNGGA,",0),recdata,reclen);
		if(mat_index > 0){
			//��GNGGA����
			GpsGetGprmcGpggaInfo(recdata+mat_index, reclen-mat_index, GGA_TYPE);
		} 
	
	}
}

/****************3Gģ�����ݲ�׽************************
//�ӽ��ջ����в��������ڷ�����������

******************************************************/
void SearchDataFromSer(void)
{
	uint8 recdata[L218_MAX_BUF_LEN],comma=0,colon;
	uint16 reclen,datanum=0;	
	#ifdef GPRS_DEBUG
	uint16 i;
	#endif		
	
	reclen = L218UartIsRxDone(recdata,L218_MAX_BUF_LEN);
	if(reclen < 15){//"+RECEIVE,x,x:\r\n"����15�ֽ�
		return;
	}
	ReadOverTailIndex(reclen);	//ʲôʱ��ִ�У�����
	if(SubMatch((uint8 *)GPRS_HAVE_RX_DATA,9,recdata,reclen) > 0){
		for(colon=9; colon < 20; colon++){	//��9~20�ֶ��Ҷ�����ð��
			if(recdata[colon]==','){
				comma = colon;
			}
			else if(recdata[colon]==':'){
				break;
			}
		}
		for(++comma;comma < colon;comma++){	//��Ч���ݳ���
			datanum = datanum * 10 +recdata[comma];
		}
		colon += 3;	//�±굽��Ч���ݿ�ͷ
		//�н�������
		#ifdef GPRS_DEBUG
			printf("datanum=%d  colon=%d\r\n",datanum,colon);
			LocalUartFixedLenSend(recdata, colon);//reclen); 
			for(i=colon;i<(reclen-1);i++){
				printf("%.2X ",recdata[i]);
			}	
			printf("\r\n");
		#endif		
			
		ProProcess(recdata+colon,reclen-colon);	
	}	
}

/******************************************************
//L218ģ�鶨ʱ���Ⱥ���
//����GPRS\GPS���ݵȽ���
******************************************************/
void ModlueCalledProcess(void)
{
	uint8 res=2,run=0;

	switch(g_gprs_ctr_struct.business)
	{
		case GPRS_INIT:{
			res = GprsInitProcess();
			if(res == 0){
				g_gprs_data_struct.initStatus = GPRS_INIT_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.initStatus = GPRS_INIT_ING;
			}
			else{
				g_gprs_data_struct.initStatus = GPRS_INIT_NO;
			}
			break;
		}
	  case GPRS_CONNECT_NET:{
			res = GprsConnectNetProcess();
			if(res == 0){
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_ING;				
			}
			else{
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;		
			}
			break;
		}
	  case GPRS_CONNECT_SERVER:{
			res = GprsConnectSerProcess();
			if(res == 0){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_OK;
			}
			else if(res == 2){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_ING;				
			}
			else{
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_NO;		
			}
			break;
		}
		case GPRS_DISCONNECT_SERVER:{
			res = GprsDisconnectSerProcess();
			if(res == 0){
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;
			}
			else if(res == 2){
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_ING;				
			}
			else{
				g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_ERR;
			}
			break;
		}
    case GPRS_SEND_DATA:{
			if(g_gprs_data_struct.SendDataLen==0 || g_gprs_data_struct.SendData==NULL){
				#ifdef GPRS_DEBUG
				printf("g_gprs_data_struct.SendData==NULL \r\n");	
				#endif	 	 
				return;
			}
			res = GprsSendDataProcess(g_gprs_data_struct.SendData,g_gprs_data_struct.SendDataLen);//(NULL,0);//
			if(res == 0){
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
			}
			else{
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_BUSY;
			}
			break;
		}
		case GPRS_DISCONNECT_NET:{
			res = GprsDisonnectNetProcess();
			if(res == 0){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_NO;
			}
			else if(res == 2){
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_ING;				
			}
			else{
				g_gprs_data_struct.MserLinkStatus 	= GPRS_SERLINK_ERR;		
			}
			break;
		}
		case GPRS_RESET:
			L218Reset();
			break; 
			
			
		case GPS_OPEN_NET:{
			res = GpsOpenProcess();
			if(res == 0){
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_OK;
				g_gprs_ctr_struct.business = GPRS_NULL; 
			}
			else if(res == 2){
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_ING;				
			}
			else{
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;		
			}
			break;
		}
		case GPS_CLOSE_NET:{
			res = GpsCloseProcess();
			if(res == 0){
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;
			}
			else if(res == 2){
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_ING;				
			}
			else{
				g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_ERR;		
			}
			break;
		}
		default:	break;
	}
	if(g_gprs_ctr_struct.business!=GPRS_INIT && (res==0 || res==1)){
		g_gprs_ctr_struct.business = GPRS_NULL;
	}
	SearchDataFromGPS();		//Ѱ��GPS��Ϣ
	SearchDataFromSer();		//Ѱ�ҷ�������������
	if(test_ProgramUpgrade_flag)
	{
		run = 1;
		ftp_struct.ftp_txstep = e_ftpstart;
		while(run)
		{
			OSTimeDlyHMSM(0, 0, 0, 2);
			FtpMain();
			if((ftp_struct.ftp_upgrade_fail_flag == 1) || (ftp_struct.ftp_upgrade_success_flag == 1))
			{	
				test_ProgramUpgrade_flag = 0;
				run = 0;
			}
		}
	}	

	if(ftp_struct.ftp_upgrade_success_flag == 1)
	{//-�ɹ���ȡ���´���,�ñ�־λ,��boot����
		res = FlashErase(BOOT_PARA_START_ADDR);
		if(res)
		{
			boot_struct.program_update_flag = VALID_VAL_DWORD_AA;
			boot_struct.sys_para_init_flag = INVALID_VAL_DWORD_55;
			res = FlashWrite(BOOT_PARA_START_ADDR,(uint8*)&boot_struct.program_update_flag,12);
			if(res)
			{
				ftp_struct.ftp_upgrade_success_flag = 0;
				LocalUartFixedLenSend((uint8*)"Device upgrade success\r\n",StrLen((uint8*)"Device upgrade success\r\n",0));
			}
			else
				LocalUartFixedLenSend((uint8*)"Device upgrade fail\r\n",StrLen((uint8*)"Device upgrade fail\r\n",0));
		}

	}
} 

















