#define TEST_STRUCT_GLOBAL

#include "tiza_include.h"



//#define TEST_LOGINOUT	
#define TEST_FTP	

uint8 test_loginout_count =0;
uint8 test_ProgramUpgrade_flag = 0;


uint8 TestFTPProgramUpgrade(uint8 p_in[])
{
	uint8 in_len,res;
	uint8 ack_flag = 'N';
	
	in_len = StrLen(p_in,0);
	res = FtpAddrAnalysis(p_in+1,in_len-2);
	if(res)
	{
		ack_flag = 'Y';
	}
	//-SmsAckPacket(p_in,p_out,ack_flag);
	return res;
}

//-从信息流中提取出需要的数据
//-例子:
//-ftp://Vehicle:Vehicle#*@202.102.090.179:21/TIZA_XGDL_SBJ_V803_170306.bin
void TestFTPProcessFun(uint8 data[],uint16 tmp_len)
{
	uint8 res,mat_index = 0,tel_index = 0,run = 1;
	uint8 tmp_data[256],exe_flag[3]={0,0,0};
	
	
	mat_index = SubMatch("*ftp://",StrLen("*ftp://",0),data,tmp_len);
	if(mat_index > 0)
	{
		exe_flag[2] = TestFTPProgramUpgrade(data);

	}
	
	if(exe_flag[2])
	{
		ftp_struct.ftp_upgrade_flag = 1;
		test_ProgramUpgrade_flag = 1;
		//-while(run)
		//-{
		//-	OSTimeDlyHMSM(0, 0, 0, 2);
		//-	//-FtpMain();
		//-	if(ftp_struct.ftp_upgrade_fail_flag == 1)
		//-		run = 0;
		//-}
	}		
}

/****************接收来自debug口信息*******************
//
******************************************************/
void SearchDataFromDebug(void)
{
	uint8 recdata[GPS_UART_BUF_LEN];
	uint16 reclen,index,mat_index;
	
	if(g_local_uart_struct.rx_head != g_local_uart_struct.rx_tail)	//-判断是否有新数据接收到
	{//有数据
		reclen = (GPS_UART_BUF_LEN + g_local_uart_struct.rx_head - g_local_uart_struct.rx_tail) % GPS_UART_BUF_LEN;
		if(reclen>GPS_UART_BUF_LEN){//至少400字节
		return;
		}
		for(index=0;index < reclen;index++){
			g_local_uart_struct.rx_tail = ((++g_local_uart_struct.rx_tail) % GPS_UART_BUF_LEN);
			recdata[index] = g_local_uart_struct.rx_buf[((g_local_uart_struct.rx_tail) % GPS_UART_BUF_LEN)];
		}		
		#ifdef GPS_DEBUG  //打印GPS所有信息
			LocalUartFixedLenSend(recdata, reclen);
		#endif		
		
		mat_index = SubMatch((uint8*)"CMGL:",StrLen((uint8*)"CMGL:",0),recdata,reclen);
		if(mat_index > 0){
			//有GNRMC数据	
			TestFTPProcessFun(recdata+mat_index,reclen-mat_index);
		}
	
	}
}



void Test_task(void *pdata)
{
	while(1){
		OSTimeDlyHMSM(0, 0, 1, 0);
		
	#ifdef 	TEST_LOGINOUT
		if((g_pro_struct.try_login_statu  == 3)&&(test_loginout_count < 5)){	//已经登入
			OSTimeDlyHMSM(0, 0, 30, 0);		//延时30s准备登出
			ProUpLogout();	
			g_pro_struct.try_login_statu  = 4;			
			test_loginout_count++;
		}
		if(g_pro_struct.try_login_statu == 5){
			OSTimeDlyHMSM(0, 0, 30, 0);		//延时30s准备登入
			g_pro_struct.try_login_statu  = 0;
			g_pro_struct.over3_relogin_time = 0xFFFF;
			g_pro_struct.login_center_flag  = FALSE;
		}
	#endif

	#ifdef 	TEST_FTP
	SearchDataFromDebug();
	#endif
		
	}
}