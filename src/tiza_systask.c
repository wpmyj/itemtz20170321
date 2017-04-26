
#define SYS_TASK_STRUCT_GLOBAL
#define SYS_TASK_DEBUG

#include "tiza_include.h"


void SetPara2FactoryReset(void)
{
	uint8 i ;
		
	g_propara_union.Item.SavePeri	 = 10000;	//10000ms
	g_propara_union.Item.NorUpPeri = 30;		//1~30s 周期上传信息间隔
	g_propara_union.Item.AlrUpPeri = 1000;	//1000ms
	g_propara_union.Item.HeartPeri = 30;		//1~240s
	g_propara_union.Item.PAckTim	 = 30;		//平台超时
	g_propara_union.Item.NLogTim	 = 10;		//三次失败重登录时间间隔min
	
	g_propara_union.Item.DomainLen = 4;										//平台域名长度									 										
	g_propara_union.Item.Domain[0] = 218;									//平台域名	
	g_propara_union.Item.Domain[1] = 94;
	g_propara_union.Item.Domain[2] = 153;
	g_propara_union.Item.Domain[3] = 146;	
	g_propara_union.Item.Port			 = 9903;//27055;//			//平台端口	
	g_propara_union.Item.TAckTim	 = 0xFFFF;							//终端应答超时时间
	g_propara_union.Item.PDomainLen = 4;									//平台域名长度									 										
	g_propara_union.Item.PDomain[0] = 218;								//公共平台域名	
	g_propara_union.Item.PDomain[1] = 94;
	g_propara_union.Item.PDomain[2] = 153;
	g_propara_union.Item.PDomain[3] = 146;	
	g_propara_union.Item.PPort			= 20002;//27055;//		//公共平台端口	
	g_propara_union.Item.Monitor		= 0xFF;							//是否抽样检测中	
	
	for(i=4; i<PRO_DOMAINLEN_MAX;i++){
		g_propara_union.Item.Domain[i]  = 0x00;
		g_propara_union.Item.PDomain[i] = 0x00;
	}	
//	memcpy(g_propara_union.Item.g_para_HDVers,"Dh744",5);	
//	memcpy(g_propara_union.Item.g_para_SFVers,"Ds744",5);	
	memcpy(g_propara_union.Item.g_para_HDVers,"Dh700",5);	
	memcpy(g_propara_union.Item.g_para_SFVers,"Ds724",5);	
		
}
void SetGPRSNetPara(void){
//Port = 20000;//9903;//27055;//
	
	g_gprs_data_struct.SendDataLen=0;
  g_gprs_data_struct.SendData=(uint8 *)GPRStestdata;
//	//企业平台 9903
//	g_gprs_data_struct.IP[0] = g_propara_union.Item.Domain[0];
//	g_gprs_data_struct.IP[1] = g_propara_union.Item.Domain[1];
//	g_gprs_data_struct.IP[2] = g_propara_union.Item.Domain[2];
//	g_gprs_data_struct.IP[3] = g_propara_union.Item.Domain[3];
//	g_gprs_data_struct.Port = g_propara_union.Item.Port;
	//公共平台 20000
	g_gprs_data_struct.IP[0] = g_propara_union.Item.PDomain[0];
	g_gprs_data_struct.IP[1] = g_propara_union.Item.PDomain[1];
	g_gprs_data_struct.IP[2] = g_propara_union.Item.PDomain[2];
	g_gprs_data_struct.IP[3] = g_propara_union.Item.PDomain[3];
	g_gprs_data_struct.Port = g_propara_union.Item.PPort;
	
	g_gprs_data_struct.setnetparaok_flag = 1;
}

static void ProAlarmHeadTailSave(void)///保存页码
{
	uint8  data[9],res;
	
	if(g_sysmiscrun_struct.ProgramUpgrade_flag != 0){	
//		printf("正在升级,不保存页码\n");
		return ;
	}

	data[0] = (uint8)(g_sysalarm_struct.headindex >> 24) & 0xFF;
	data[1] = (uint8)(g_sysalarm_struct.headindex >> 16) & 0xFF;
	data[2] = (uint8)(g_sysalarm_struct.headindex >>  8) & 0xFF;
	data[3] = (uint8) g_sysalarm_struct.headindex        & 0xFF;
	data[4] = (uint8)(g_sysalarm_struct.tailindex >> 24) & 0xFF;
	data[5] = (uint8)(g_sysalarm_struct.tailindex >> 16) & 0xFF;
	data[6] = (uint8)(g_sysalarm_struct.tailindex >>  8) & 0xFF;
	data[7] = (uint8) g_sysalarm_struct.tailindex        & 0xFF;
	
	data[8] = U8SumCheck(data,8);
	res = FlashErase(SYS_ALARM_INDEXPAGE);	
	if(!res){// 失败
		return;
	}
	FlashWrite(SYS_ALARM_INDEXPAGE, data, 9);
}
static void ProAlarmPageSave(uint8 data[],uint16 len)
{
	uint8  res;
	uint16 page;
	uint32 addr;
	
	if(g_sysmiscrun_struct.ProgramUpgrade_flag != 0){	
//		printf("正在升级,不写页保存\n");
		return ;
	}
	
	//满一页
	if((g_sysalarm_struct.headindex&0x03)==0){	//FLASH擦除 2K字节/段 即512*4
		addr = SYS_ALARM_START_ADDR + g_sysalarm_struct.headindex*ALARM_PAGE_SIZE;
		res = FlashErase(addr);
		if(!res){// 失败
			goto RETURN_LAB;
		}
	}
	
	//满一包
	addr = SYS_ALARM_START_ADDR + g_sysalarm_struct.headindex*ALARM_PAGE_SIZE ;
	res = FlashWrite(addr, data, len);
	if(!res){// 失败
		goto RETURN_LAB;
	}
	
	g_sysalarm_struct.headindex = (g_sysalarm_struct.headindex+1)%SYS_ALARM_MAXINDEX;
	if(g_sysalarm_struct.headindex == g_sysalarm_struct.tailindex){
		//tail前移8包即整段擦除时舍掉其余7包数据；若要不舍需另外开一段临时保存旧数据，这样会增加擦除次数
		g_sysalarm_struct.tailindex = (g_sysalarm_struct.tailindex+4)%SYS_ALARM_MAXINDEX;
	}
	
	if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){	//无报警 或 无报警盲区
		page = (SYS_ALARM_MAXINDEX + g_sysalarm_struct.headindex - g_sysalarm_struct.tailindex)%SYS_ALARM_MAXINDEX;
		if(page > g_sysmiscrun_struct.save_sysAlarm_numb){	//只保留规定条数
			page = page - g_sysmiscrun_struct.save_sysAlarm_numb;
			g_sysalarm_struct.tailindex = (g_sysalarm_struct.tailindex+page)%SYS_ALARM_MAXINDEX;
		}
	}
	ProAlarmHeadTailSave();
	
RETURN_LAB:
	return;
}
void ProPutIntoAlarm(uint8 data[], uint16 len, uint8 cmd)
{
	uint8 tmpdata[512];
	uint16 tmplen;
	
	if(len == 0 ){
		tmplen = Pro_RealTime_Data(&tmpdata[4]);
	}
	else{
		memcpy(&tmpdata[4],data,len);
		tmplen = len;
	}
	
	
	//可以在这里读取下FLASH  来区分要不要擦除
//	if(g_sysalarm_struct.data[0] != VALID_VAL_2A){		
		tmpdata[0] = VALID_VAL_2A;
		tmpdata[1] = tmplen >> 8;
		tmpdata[2] = tmplen & 0xFF;
		tmpdata[3] = cmd;//PRO_UP_LSNAL_INFO_ID;//PRO_UP_REAL_INFO_ID;//cmd;
		
		tmpdata[tmplen+4] = U8SumCheck(tmpdata,tmplen+4);
		ProAlarmPageSave(tmpdata,tmplen+5);	
//	}
	
}

static void Read_Alarm_Index(void)
{
	uint8  data[9];
	union32 tmp;
	
	FlashRead(SYS_ALARM_INDEXPAGE, data, 9);
	
	if(data[8] == U8SumCheck(data,8)){		
		tmp.byte.HH = data[0];
		tmp.byte.HL = data[1];
		tmp.byte.LH = data[2];
		tmp.byte.LL = data[3];
		g_sysalarm_struct.headindex = tmp.dword;
		tmp.byte.HH = data[4];
		tmp.byte.HL = data[5];
		tmp.byte.LH = data[6];
		tmp.byte.LL = data[7];
		g_sysalarm_struct.tailindex = tmp.dword;
	}
	else {
		#ifdef SYS_TASK_DEBUG
		printf("IntFlashRead index ERROR\r\n");
		#endif
		g_sysalarm_struct.headindex = 0;
		g_sysalarm_struct.tailindex = 0;
		ProAlarmHeadTailSave();
	}
	printf("\r\n g_sysalarm_struct.headindex = %d   g_sysalarm_struct.tailindex = %d\r\n",g_sysalarm_struct.headindex,g_sysalarm_struct.tailindex);
}

static void Read_Lsnal_Index(void)
{
	uint8  data[9];
	union32 tmp;
	
	ExteFlashRead(SYS_LSNAL_SPIINDEX_ADDR, data, 9);
	
	if(data[8] == U8SumCheck(data,8)){		
		tmp.byte.HH = data[0];
		tmp.byte.HL = data[1];
		tmp.byte.LH = data[2];
		tmp.byte.LL = data[3];
		g_syslsnal_struct.headindex = tmp.dword;
		tmp.byte.HH = data[4];
		tmp.byte.HL = data[5];
		tmp.byte.LH = data[6];
		tmp.byte.LL = data[7];
		g_syslsnal_struct.tailindex = tmp.dword;
	}
	else {
		#ifdef SYS_TASK_DEBUG
		printf("ExteFlashRead index ERROR\r\n");
		#endif
		g_syslsnal_struct.headindex = 0;
		g_syslsnal_struct.tailindex = 0;
		ProLsnalHeadTailSave();
	}
//	printf("\r\n g_syslsnal_struct.headindex = %d   g_syslsnal_struct.tailindex = %d\r\n",g_syslsnal_struct.headindex,g_syslsnal_struct.tailindex);
}
void ProWrite_SysPara(void)
{
	if(g_sysmiscrun_struct.ProgramUpgrade_flag != 0){	
//		printf("正在升级,不保存参数设置\n");
		return ;
	}
	FlashErase(SYS_SAVEPARA_INDEXPAGE);
	g_propara_union.Item.check = U8SumCheck(g_propara_union.arry,PRO_PARA_UNION_LEN-1);
	FlashWrite(SYS_SAVEPARA_INDEXPAGE, g_propara_union.arry, PRO_PARA_UNION_LEN);
}
static void ProRead_SysPara(void)
{
	FlashRead(SYS_SAVEPARA_INDEXPAGE, g_propara_union.arry, PRO_PARA_UNION_LEN);
//	if(1){
	if(g_propara_union.Item.check != U8SumCheck(g_propara_union.arry,PRO_PARA_UNION_LEN-1)){
		#ifdef SYS_TASK_DEBUG
		printf("ProRead_SysPara ERROR\r\n");
		#endif
		// 恢复出厂并保存
		SetPara2FactoryReset();
		ProWrite_SysPara();
	}
}



//////////////////////////////////////////////////////////////
static void TerminalMiscPeriod(uint8 past_sec)
{		
	if(g_propara_union.Item.PAckTim!=0xFF && g_sysmiscrun_struct.PAckTim_count!=0xFF){
		if(++g_sysmiscrun_struct.PAckTim_count >= g_propara_union.Item.PAckTim){
			printf("平台超时\r\n");
			g_sysmiscrun_struct.PAckTim_count = 0xFF;
		}
	}
}

static void TerminalSavePeriod(uint16 past_ms)
{
	if(g_sysmiscrun_struct.have_sysAlarm_flag == 1){//有报警
		ProPutIntoAlarm(NULL, 0, PRO_UP_REAL_INFO_ID);		//1s执行一次保存
		
		//报警超时 停止1s报警上传转为正常上传  若仍需1s报警上传则把以下条件代码注释
		if(++g_sysmiscrun_struct.have_sysAlarm_count >= SYS_HAVEALARM_COUNT){
			g_sysmiscrun_struct.have_sysAlarm_count = SYS_HAVEALARM_COUNT;
			g_sysmiscrun_struct.have_sysAlarm_flag = 2;				//超时 置检测有无报警盲区
		}
	}
	else if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){	//无报警 或 无报警盲区
		g_sysmiscrun_struct.SavePeri_count += past_ms;
		if(g_sysmiscrun_struct.SavePeri_count >= g_propara_union.Item.SavePeri){
			
			ProPutIntoAlarm(NULL, 0, PRO_UP_LSNAL_INFO_ID);
			g_sysmiscrun_struct.SavePeri_count = 0;
		}
	}
}


static void TerminalUpLsnalAlrm(void)
{
	uint8  rx_buf[TMP_BUFF_MAXLEN-25],data[5];	//buff考虑帧头尾-25
	uint8  sum=0;
	uint16 len;
	uint32 addr;
	
	if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){	
		return;												//无报警 或 无报警盲区
	}
	
	if(g_sysalarm_struct.tailindex != g_sysalarm_struct.headindex){
		addr = SYS_ALARM_START_ADDR + g_sysalarm_struct.tailindex*ALARM_PAGE_SIZE ;
		FlashRead(addr, data, 4);
		
		if(data[0]==VALID_VAL_2A && (data[3]==PRO_UP_LSNAL_INFO_ID || data[3]==PRO_UP_REAL_INFO_ID)){
			len = (data[1]<<8) + data[2];
			sum = U8SumCheck(data, 4);			
			
			FlashRead(addr+4, &rx_buf[PRO_DATA_INDEX], len+1);	
			sum += U8SumCheck(&rx_buf[PRO_DATA_INDEX], len);	
			if(rx_buf[PRO_DATA_INDEX + len] == sum){//
				ProConstructFrameHead(rx_buf, len, data[3]);
				len += PRO_DATA_INDEX;
				ProConstructFrameTail(rx_buf, len);
				
				g_sysalarm_struct.tailindex = (g_sysalarm_struct.tailindex+1)%SYS_ALARM_MAXINDEX;
				ProPacket(rx_buf, len+1, data[3], FALSE);//TRUE);
				
			}
			else{
				goto RETURN_LAB;
			}
		}
		else{
			goto RETURN_LAB;
		}
		
	}
	else{
		if(g_sysmiscrun_struct.have_sysAlarm_flag == 2){	//超时报警盲区数据发完
			g_sysmiscrun_struct.have_sysAlarm_flag = 3;
		}
		
	}
	return;
	
RETURN_LAB:	
	g_sysalarm_struct.tailindex = (g_sysalarm_struct.tailindex+1)%SYS_ALARM_MAXINDEX;
	return;
}

static void TerminalAlrmUpPeriod(uint16 past_ms)
{
	if(g_sysmiscrun_struct.have_sysAlarm_flag == 1){//有报警		
		g_sysmiscrun_struct.AlrUpPeri_count += past_ms;
		if(g_sysmiscrun_struct.AlrUpPeri_count >= g_propara_union.Item.AlrUpPeri){
			
			ProUpRealFormation();
			g_sysmiscrun_struct.AlrUpPeri_count = 0;
		}
	}
	else if(g_sysmiscrun_struct.have_sysAlarm_flag == 0){//无报警		
		g_sysmiscrun_struct.have_sysAlarm_count = 0;
	}
}

static void TerminalNorUpPeriod(uint8 past_sec)
{		
	if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){//没有报警	或 
		if(++g_sysmiscrun_struct.NorUpPeri_count >= g_propara_union.Item.NorUpPeri){
			ProUpRealFormation();
			g_sysmiscrun_struct.NorUpPeri_count = 0;
		}
	}	
}

static void TerminalHeart(uint8 past_sec)
{
	if(++g_sysmiscrun_struct.upheart_count > g_propara_union.Item.HeartPeri){	// 1s累加，发送心跳
		ProUpHeartBeat();
		g_sysmiscrun_struct.upheart_count = 0;
	}
}

static void TerminalMainPowerStatu(void)
{
	switch(g_sysmiscrun_struct.main_power_statu){
		case 0x01:{//首次上电
			if(CHECK_POW() == LOW_STATE){
				g_sysmiscrun_struct.main_power_statu = 2;
			}
			break;
		}
		case 0x02:{//断电
			if(CHECK_POW() == HIGH_STATE){
				//再次上电，复位
				SysReset();
			}			
			break;
		}
		default:
			if(CHECK_POW() == HIGH_STATE){
				g_sysmiscrun_struct.main_power_statu = 1;				
			}			
		break;
	}
}

static void TerminalWakeUp(void)
{
	ErrorStatus HSEStartUpStatus;
	//HSE重新配置
	RCC_HSEConfig(RCC_HSE_ON); 														/*HSES使能*/  
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 					/*等待*/
	if(HSEStartUpStatus == SUCCESS) { 
		RCC_PLLCmd(ENABLE);																	/*使能*/
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)== RESET); 	/*等待PLL有效*/      
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);					/*将PLL作为系统时钟*/
		while(RCC_GetSYSCLKSource() != 0x08);								/*等待*/
	} 
	RCC->AHBENR = 0x14;
	RCC->APB1ENR = 0x18000000;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);	
	
	//开外电
	ON_485232_PWR();
	ON_CT5V_PWR();
//	ON_GPRS_PWR();  //模块开
	HIGH_GPRS_DTR(); 	//唤醒L218
	//重初始化	
	delay_init();	    //初始化systick
	PvdInit();
	UsartInit(LOCAL_USART,LOCAL_USART_BPR,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	UsartInit(GPS_USART  ,GPS_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);	
	ExteFlashInit();
	RtcInit();
	AdcInit();
	DmaInit();
//	BusiResetModule();
	
	if(0){//若唤醒出问题   复位
		SysReset();
	}
}
static void TerminalIntoSleep(void)
{
	uint8 rtc_counter;
	
	if(CHECK_POW() == HIGH_STATE){//有外电，不休眠
		return;
	}			

	//保存
	
	//关外电
	OFF_485232_PWR();
	OFF_CT5V_PWR();
//	OFF_GPRS_PWR();
		LOW_GPRS_DTR(); 	//休眠L218
	ADC_Cmd(ADC1, DISABLE);

	//进休眠
	rtc_counter = RTC_GetCounter();
	while(1){
		FeedWtd();
		RTC_WaitForLastTask();
		RTC_SetAlarm(rtc_counter++);	
		RTC_WaitForLastTask();
		PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);

		if(CHECK_POW() == HIGH_STATE){//有外电，唤醒
			break;
		}
	}
	FeedWtd();
	TerminalWakeUp();
	
}

///////////////////////////////////////////////////////////


/******************************************************
//应用程序任务
******************************************************/
//周期处理任务
void Period_task(void *pdata)
{
	uint16 count_time,period_time;
	
	ProRead_SysPara();
	OSTimeDlyHMSM(0, 0, 0, 500);	
	Read_Lsnal_Index();
	OSTimeDlyHMSM(0, 0, 0, 500);
	Read_Alarm_Index();
	OSTimeDlyHMSM(0, 0, 0, 500);	
	
	
	g_propara_union.Item.NorUpPeri = 30;		//1~30s 周期上传信息间隔   qlj 临时使用待删除
	while(1){
		OSTimeDlyHMSM(0, 0, 0, 200);	
		count_time = (count_time+1)%60000;
		period_time = count_time;
//		period_time = g_sysmiscrun_struct.time3_200ms_count;
		
		
		g_propostion_union.Item.status.byte = g_gps_struct.gpsinform.subitem.statu.byte;
		g_propostion_union.Item.longitude   = g_gps_struct.gpsinform.subitem.longitude;
		g_propostion_union.Item.latitude    = g_gps_struct.gpsinform.subitem.latitude;
		
			if(period_time%5 == 0){//1s			3级报警   4s+50ms
				if(g_pro_struct.login_center_flag == TRUE){	//已经登录	
					TerminalUpLsnalAlrm();
				}
			}
			
			if(period_time%4 == 1){//1s			保存数据   
				RtcGetCalendarTime(g_protime_union.arry);		//读取时间
				if(g_pro_struct.login_center_flag == TRUE){	//已经登录	
					TerminalSavePeriod(1000);
				}
			}
			if(period_time%150 == 2){//30		工作参数
				g_sysmiscrun_struct.need_check_time_flag = 0x01;			//从GPS校时						
			}
			
			if(period_time%5 == 3){//1		其他	
				ProPeriodTx(1);
				if(g_pro_struct.login_center_flag == TRUE){	//已经登录		
					TerminalNorUpPeriod(1);
					TerminalHeart(1);
				}
				
				CanMonitor(1);
				TerminalMainPowerStatu();
			}
		
				
	}
}

