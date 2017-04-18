#define TEST_STRUCT_GLOBAL

#include "tiza_include.h"

#define FTP_LINK_EXAMP "ftp://Vehicle:Vehicle#*@202.102.090.179:21/TIZA_XGDL_SBJ_V803_170306.bin\x00"

#define TEST_LOCALCMD	
//#define TEST_READLSNAL	
//#define TEST_232_485	
#define TEST_FTP	

//#define TEST_LOGINOUT		1
//#define TEST_CHECKTIME		2
uint8 Test_Step = 0;

uint8 test_loginout_count =0;



//-从信息流中提取出需要的数据
//-例子:
//-ftp://Vehicle:Vehicle#*@202.102.090.179:21/TIZA_XGDL_SBJ_V803_170306.bin
static void TestFTPProcessFun(uint8 data[],uint16 tmp_len)
{
	uint8 mat_index, in_len, res;
	uint8 tmp_data[256],exe_flag[3]={0,0,0};
	uint32 ftpflshadd;
	
	mat_index = SubMatch((uint8*)"ftp://",StrLen((uint8*)"ftp://",0),data,tmp_len);
	if(mat_index > 0){		
		in_len = StrLen(data,0);
		res = FtpAddrAnalysis(data,in_len-1);
		if(res){
			for(ftpflshadd=FTP_BLIND_FLASH_START; ftpflshadd < FTP_BLIND_FLASH_END;){
				FlashErase(ftpflshadd);
				ftpflshadd += 0x800;
			}		
			ftp_struct.ftp_upgrade_flag 						= 1;	
			g_sysmiscrun_struct.ProgramUpgrade_flag = 1;
		}		
	}
	
}


static void test_rs232_485(void)
{
	uint16 count,index;
	uint8  data[256];
	#ifdef TEST_LOCALCMD
	//uart1 本地串口
	if(g_local_uart_struct.rx_head != g_local_uart_struct.rx_tail)
	{
		count = (LOCAL_UART_BUF_LEN + g_local_uart_struct.rx_head - g_local_uart_struct.rx_tail) % LOCAL_UART_BUF_LEN;
		for(index=0; index < count; index++){
			data[index] = g_local_uart_struct.rx_buf[((index+g_local_uart_struct.rx_tail) % LOCAL_UART_BUF_LEN)];
		}
		g_local_uart_struct.rx_tail = (g_local_uart_struct.rx_tail + count)%LOCAL_UART_BUF_LEN;
		
		if(SubMatch((uint8*)"CMD ALARM",9,data,count))						{ //报警
			
//			g_sysalarm_struct.headindex = 0;
//			g_sysalarm_struct.tailindex = 0;
//			g_syslsnal_struct.headindex = 0;
//			g_syslsnal_struct.tailindex = 0;
			g_syslsnal_struct.tailindex = g_syslsnal_struct.headindex;
			ProLsnalHeadTailSave();
			g_sysmiscrun_struct.have_sysAlarm_flag = 1;
		}
		else if(SubMatch((uint8*)"CMD NORMAL",10,data,count))			{	//正常
			if(g_sysmiscrun_struct.have_sysAlarm_flag!=0 && g_sysmiscrun_struct.have_sysAlarm_flag!=3){
				//标志检查有无盲区数据
				g_sysmiscrun_struct.have_sysAlarm_flag = 2;  
			}
			else{
				g_sysmiscrun_struct.have_sysAlarm_flag = 0;				
			}
		}
		else if(SubMatch((uint8*)"CMD REST",8,data,count))				{	//复位终端
			SysReset();
		}
		else if(SubMatch((uint8*)"CMD UPGRADE",11,data,count))		{	//固件审计
			count = StrLen((uint8*)FTP_LINK_EXAMP, 0);
			memcpy(data, (uint8*)FTP_LINK_EXAMP, count);
			TestFTPProcessFun(data, count);
		}
	}
	#endif
	#ifdef TEST_232_485
	//uart4
	if(g_meter_uart_struct.rx_head != g_meter_uart_struct.rx_tail)
	{
		count = (METER_UART_BUF_LEN + g_meter_uart_struct.rx_head - g_meter_uart_struct.rx_tail) % METER_UART_BUF_LEN;
		for(index=0; index < count; index++){
			data[index] = g_meter_uart_struct.rx_buf[((index+g_meter_uart_struct.rx_tail) % METER_UART_BUF_LEN)];
		}
		g_meter_uart_struct.rx_tail = (g_meter_uart_struct.rx_tail + count)%METER_UART_BUF_LEN;
		MeterUartFixedLenSend(data,count);
	}
	//uart5
	if(g_rs485_uart_struct.rx_head != g_rs485_uart_struct.rx_tail)
	{
		count = (RS485_UART_BUF_LEN + g_rs485_uart_struct.rx_head - g_rs485_uart_struct.rx_tail) % RS485_UART_BUF_LEN;
		for(index=0; index < count; index++){
			data[index] = g_rs485_uart_struct.rx_buf[((index+g_rs485_uart_struct.rx_tail) % RS485_UART_BUF_LEN)];
		}
		g_rs485_uart_struct.rx_tail = (g_rs485_uart_struct.rx_tail + count)%RS485_UART_BUF_LEN;
		RS485UartFixedLenSend(data,count);
	}
		#if 0
//		printf("GPRS UART Received data: ");
//		LocalUartFixedLenSend(data, count);
//		printf(" \r\n");	
		#endif	
		
	#endif	

}

void Test_task(void *pdata)
{
	uint8 times=0,i,j;
	uint8 tmpdata[256];
	uint16 addr;
	
	#ifdef TEST_232_485
		UsartInit(METER_USART  ,METER_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);	
		UsartInit(RS485_USART  ,RS485_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	#endif
	
	#ifdef TEST_READLSNAL
		j= (SYS_LSNAL_SPIMAXINDEX+g_syslsnal_struct.headindex-g_syslsnal_struct.tailindex)%SYS_LSNAL_SPIMAXINDEX;
		for(i=0;i<j;i++){
			addr = SYS_LSNAL_SPISTART_ADDR + (g_syslsnal_struct.tailindex+i) * LSNAL_PAGE_SIZE; 
			ExteFlashRead(addr, tmpdata, 250);
			printf("ExteFlashRead 0 is:\r\n");
			for(times=0; times < 250;times++){
				printf("%.2X ",tmpdata[times]);
			}
			printf("\r\n");
		}
	#endif
	
	while(1){
		OSTimeDlyHMSM(0, 0, 4, 0);

		test_rs232_485();
		CanMonitor(2);
		
//	  printf("\r\n g_sysmiscrun_struct.NLogTim_count = %d g_pro_struct.try_login_statu = %d\r\n",g_sysmiscrun_struct.NLogTim_count,g_pro_struct.try_login_statu);
//		printf(" g_sysalarm_struct.headindex = %d   g_sysalarm_struct.tailindex = %d\r\n",g_sysalarm_struct.headindex,g_sysalarm_struct.tailindex);
//		printf(" g_syslsnal_struct.headindex = %d   g_syslsnal_struct.tailindex = %d\r\n",g_syslsnal_struct.headindex,g_syslsnal_struct.tailindex);
//		printf(" g_sysmiscrun_struct.have_sysAlarm_flag = %d \r\n",g_sysmiscrun_struct.have_sysAlarm_flag);
//		printf(" can_struct.rx_data_flag = %d \r\n",can_struct.rx_data_flag);
//		printf("\r\n PWR_C = %d BAT_ADC = %d\r\n",adc_result[0],adc_result[1]);

		
		
		
		switch(Test_Step){
			#ifdef 	TEST_LOGINOUT
			case TEST_LOGINOUT:{
				if((g_pro_struct.try_login_statu  == 3)&&(test_loginout_count < 5)){	//已经登入
					OSTimeDlyHMSM(0, 0, 30, 0);		//延时30s准备登出
					ProUpLogout();	
					g_pro_struct.try_login_statu  = 4;			
					test_loginout_count++;
				}
				if(g_pro_struct.try_login_statu == 5){
					OSTimeDlyHMSM(0, 0, 30, 0);		//延时30s准备登入
					g_pro_struct.try_login_statu  = 0;
					g_sysmiscrun_struct.NLogTim_count = 0xFFFF;
					g_pro_struct.login_center_flag  = FALSE;
				}
				break;
			}
			#endif
			#ifdef TEST_CHECKTIME
			case TEST_CHECKTIME:{
				if(++times >= 60){
					ProUpCheckTime();
					times = 0;
				}				
				break;
			}
			#endif
			case 3:{
				break;
			}
			
			default : break;
		}
		
		
	}
}