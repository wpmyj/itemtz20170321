#define PROTOCOL_GLOBAL
#define PROTOCOL_DEBUG

#include "tiza_include.h"


static void ProPutIntoLsnal(uint8 data[],uint16 len,uint8 cmd);

void ProParaInit(void)///上行协议参数初始化
{
 	uint8 i;
	
	g_propara_union.Item.HeartPeri = 30;	//1~240s
	g_propara_union.Item.NLogTim	 = 10;	//三次失败重登录时间间隔min
	
	
	
	
	
//	ftp_struct.ftp_upgrade_flag = FALSE;
//	g_pro_struct.aquire_comm_para_flag = FALSE;
	g_pro_struct.try_login_statu  = 0;
	g_pro_struct.over3_relogin_time = 0xFFFF;
	g_pro_struct.login_center_flag  = FALSE;
//	g_pro_struct.tx_lsnal_data_flag = FALSE;
//	sys_misc_run_struct.up_heart_beat_sec_counter = 0x00;
//	
//	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++)
//	{
//		if(g_pro_struct.tx_struct.re_tx_full_flag[i])
//		{
//			g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
//			ProPutIntoLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
//							g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX,
//							g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
//		}
//	}
//	
//	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++)
//	{
//		g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
//	} 
}
/*****上行通讯函数******/
//大端模式，高位在前
static uint8 ProVehicle(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_VEHICLE_FLAG;
	//0~19
	data[index++] = g_provehice_union.Item.vehicle;
	data[index++] = g_provehice_union.Item.charge;
	data[index++] = g_provehice_union.Item.mode;
	data[index++] = g_provehice_union.arry[4];	//speed
	data[index++] = g_provehice_union.arry[3];
	data[index++] = g_provehice_union.arry[8];	//mileage
	data[index++] = g_provehice_union.arry[7];
	data[index++] = g_provehice_union.arry[6];
	data[index++] = g_provehice_union.arry[5];
	data[index++] = g_provehice_union.arry[10];	//voltage
	data[index++] = g_provehice_union.arry[9];
	data[index++] = g_provehice_union.arry[12];	//current
	data[index++] = g_provehice_union.arry[11];	
	data[index++] = g_provehice_union.Item.SOC;
	data[index++] = g_provehice_union.Item.DC_statu;	
	data[index++] = g_provehice_union.Item.stall.byte;
	data[index++] = g_provehice_union.arry[17];	//resistance
	data[index++] = g_provehice_union.arry[16];	
	data[index++] = g_provehice_union.Item.Accelerate;
	data[index++] = g_provehice_union.Item.braking;
	
	return index;
}
static uint8 ProMotor(uint8 data[])
{
	uint8 index=0,i;
	
	data[index++] = (uint8)PRO_MOTOR_FLAG;
	data[index++] = (uint8)PRO_MOTOR_NUMBER;
	//0~12 *i
	for(i=0;i < PRO_MOTOR_NUMBER;i++){
		data[index++] = g_promotor_union[i].Item.serial;
		data[index++] = g_promotor_union[i].Item.status;
		data[index++] = g_promotor_union[i].Item.ctr_tem;
		data[index++] = g_promotor_union[i].arry[4];	//rpm
		data[index++] = g_promotor_union[i].arry[3];
		data[index++] = g_promotor_union[i].arry[6];	//torque
		data[index++] = g_promotor_union[i].arry[5];
		data[index++] = g_promotor_union[i].Item.motor_tem;
		data[index++] = g_promotor_union[i].arry[9];	//ctr_in_vol
		data[index++] = g_promotor_union[i].arry[8];	
		data[index++] = g_promotor_union[i].arry[11];	//ctr_bus_cur
		data[index++] = g_promotor_union[i].arry[10];	
	}
	
	return index;
}
static uint8 ProFuelCell(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_FUELCELL_FLAG;
	//0~18 + i
	data[index++] = g_profullcell_union.arry[1];	//voltage
	data[index++] = g_profullcell_union.arry[0];
	data[index++] = g_profullcell_union.arry[3];	//current
	data[index++] = g_profullcell_union.arry[2];
	data[index++] = g_profullcell_union.arry[5];	//consumpt
	data[index++] = g_profullcell_union.arry[4];
	g_profullcell_union.Item.tprobe_num = PRO_FTPROBE_NUMBER;
	data[index++] = g_profullcell_union.arry[7];	//tprobe_num
	data[index++] = g_profullcell_union.arry[6];	
	memcpy(&data[index],g_TProbe_val,g_profullcell_union.Item.tprobe_num);
	index += g_profullcell_union.Item.tprobe_num;	
	data[index++] = g_profullcell_union.arry[9];	//maxtem_val
	data[index++] = g_profullcell_union.arry[8];	
	data[index++] = g_profullcell_union.Item.maxtem_id;
	data[index++] = g_profullcell_union.arry[12];	//maxconcent
	data[index++] = g_profullcell_union.arry[11];	
	data[index++] = g_profullcell_union.Item.maxconcent_id;
	data[index++] = g_profullcell_union.arry[15];	//maxpre_val
	data[index++] = g_profullcell_union.arry[14];	
	data[index++] = g_profullcell_union.Item.maxpre_id;
	data[index++] = g_profullcell_union.Item.preDCstatu;
	
	return index;
}
static uint8 ProEngine(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_ENGINE_FLAG;
	//0~4
	data[index++] = g_proengine_union.Item.status;
	data[index++] = g_proengine_union.arry[2];	//rpm
	data[index++] = g_proengine_union.arry[1];
	data[index++] = g_proengine_union.arry[4];	//consumpt
	data[index++] = g_proengine_union.arry[3];
	
	return index;
}
static uint8 ProPostion(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_POSITION_FLAG;
	//0~8
	data[index++] = g_propostion_union.Item.status.byte;
	data[index++] = g_propostion_union.arry[4];	//longitude
	data[index++] = g_propostion_union.arry[3];
	data[index++] = g_propostion_union.arry[2];
	data[index++] = g_propostion_union.arry[1];
	data[index++] = g_propostion_union.arry[8];	//latitude
	data[index++] = g_propostion_union.arry[7];
	data[index++] = g_propostion_union.arry[6];
	data[index++] = g_propostion_union.arry[5];
	
	return index;
}
static uint8 ProExtreme(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_EXTREME_FLAG;
	//0~13
	data[index++] = g_proextreme_union.Item.maxvolsys_id;
	data[index++] = g_proextreme_union.Item.maxvolbat_id;
	data[index++] = g_proextreme_union.arry[3];	//maxvolbat_val
	data[index++] = g_proextreme_union.arry[2];
	data[index++] = g_proextreme_union.Item.minvolsys_id;
	data[index++] = g_proextreme_union.Item.minvolbat_id;
	data[index++] = g_proextreme_union.arry[7];	//minvolbat_val
	data[index++] = g_proextreme_union.arry[6];
	data[index++] = g_proextreme_union.Item.maxtemsys_id;
	data[index++] = g_proextreme_union.Item.maxtempro_id;
	data[index++] = g_proextreme_union.Item.maxtempro_val;
	data[index++] = g_proextreme_union.Item.mintemsys_id;
	data[index++] = g_proextreme_union.Item.mintempro_id;
	data[index++] = g_proextreme_union.Item.mintempro_val;
	
	return index;
}
static uint8 ProAlarm(uint8 data[])
{
	uint8 index=0,i;
	
	data[index++] = (uint8)PRO_ALARM_FLAG;
	//9+4(x+y+z+a)
	data[index++] = g_proalarm_union.Item.rank;
	data[index++] = g_proalarm_union.arry[4];	//flag
	data[index++] = g_proalarm_union.arry[3];
	data[index++] = g_proalarm_union.arry[2];
	data[index++] = g_proalarm_union.arry[1];
	data[index++] = g_proalarm_union.Item.storagenum;
	for(i=0;i < g_proalarm_union.Item.storagenum;i++){
		data[index++] = (uint8)((g_storage_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_storage_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_storage_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_storage_alr[i]      & 0xFF);
	}
	data[index++] = g_proalarm_union.Item.motornum;
	for(i=0;i < g_proalarm_union.Item.motornum;i++){
		data[index++] = (uint8)((g_motro_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_motro_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_motro_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_motro_alr[i]      & 0xFF);
	}
	data[index++] = g_proalarm_union.Item.enginenum;
	for(i=0;i < g_proalarm_union.Item.enginenum;i++){
		data[index++] = (uint8)((g_engine_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_engine_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_engine_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_engine_alr[i]      & 0xFF);
	}
	data[index++] = g_proalarm_union.Item.othersnum;
	for(i=0;i < g_proalarm_union.Item.othersnum;i++){
		data[index++] = (uint8)((g_others_alr[i]>>24) & 0xFF);	
		data[index++] = (uint8)((g_others_alr[i]>>16) & 0xFF);
		data[index++] = (uint8)((g_others_alr[i]>> 8) & 0xFF);
		data[index++] = (uint8)( g_others_alr[i]      & 0xFF);
	}
	
	return index;
}
static uint8 ProBVoltage(uint8 data[])
{
	uint8 index=0,i;
	
	data[index++] = (uint8)PRO_BVOLTAGE_FLAG;
	data[index++] = (uint8)PRO_BATTSYS_NUMBER;
	//0~12 *i
	for(i=0;i < PRO_BATTSYS_NUMBER;i++){
		data[index++] = g_provbattsys_union.Item.serial;
		data[index++] = g_provbattsys_union.arry[2];	//voltage
		data[index++] = g_provbattsys_union.arry[1];
		data[index++] = g_provbattsys_union.arry[4];	//current
		data[index++] = g_provbattsys_union.arry[3];
		data[index++] = g_provbattsys_union.arry[6];	//sigbatt_num
		data[index++] = g_provbattsys_union.arry[5];
		data[index++] = g_provbattsys_union.arry[8];	//framebatt_id
		data[index++] = g_provbattsys_union.arry[7];
		data[index++] = g_provbattsys_union.Item.framebatt_num;
		memcpy(&data[index],g_sigbatt_val[i],g_provbattsys_union.Item.framebatt_num);
		index += g_provbattsys_union.Item.framebatt_num;
	}
	
	return index;
}
static uint8 ProBTemperature(uint8 data[])
{
	uint8 index=0,i;
	
	data[index++] = (uint8)PRO_BTEMPERATURE_FLAG;
	data[index++] = (uint8)PRO_BATTSYS_NUMBER;
	//0~12 *i
	for(i=0;i < PRO_BATTSYS_NUMBER;i++){
		data[index++] = g_protbattsys_union.Item.serial;
		data[index++] = g_protbattsys_union.arry[2];	//btprobe_num
		data[index++] = g_protbattsys_union.arry[1];
		memcpy(&data[index],g_btprobe_val[i],g_protbattsys_union.Item.btprobe_num);
		index += g_protbattsys_union.Item.btprobe_num;//小心越界
	}
	
	return index;
}

void ProUpRealFormation(void)	// qlj 发送长度待处理
{//	02 实时信息
	uint8 tx_buf[TMP_BUFF_MAXLEN-25],*p;	//buff考虑帧头尾-25
	uint16 tx_len;
	
	p = tx_buf;
	memcpy(p,g_protime_union.arry,6);		//时间
	p += 6;
		
	// qlj 考虑数据量大 越界问题 是否分包？
	p += ProVehicle(p);
	p += ProMotor(p);
	p += ProFuelCell(p);
	p += ProEngine(p);
	p += ProPostion(p);
	p += ProExtreme(p);
	p += ProAlarm(p);
	p += ProBVoltage(p);
	p += ProBTemperature(p);	
	
	tx_len = p - tx_buf;
	if(tx_len > (TMP_BUFF_MAXLEN-25)) tx_len = TMP_BUFF_MAXLEN-25;
	ProPacket(tx_buf,tx_len,PRO_UP_REAL_INFO_ID,TRUE);
}
static void ProUpLsnalFormation(void)	
{// 03 盲区补发数据
//	uint8  tx_buf[PRO_MAX_TX_BUF_COUNTER];
//	uint16 tx_len;
	uint8  tmpdata[LSNAL_PAGE_SIZE];
	uint16 tmplen;
	uint32 basicaddr;

	if(g_pro_struct.tx_lsnal_falg)	{//有盲区数据正在发送
		goto RETURN_LAB;
	}
//	p = tx_buf;
	
	if(g_syslsnal_struct.tailindex != g_syslsnal_struct.headindex){
		basicaddr = SYS_LSNAL_SPISTART_ADDR + g_syslsnal_struct.tailindex * LSNAL_PAGE_SIZE;
		if(ExteFlashRead(basicaddr, tmpdata, LSNAL_PAGE_SIZE) == 0){
			goto RETURN_LAB;
		}
		
		tmplen = (tmpdata[1] << 8) + tmpdata[2] + 4;//不算校验
		if(tmpdata[0]!=VALID_VAL_2A || tmplen>=512){
		// 如果此条标志不对 越界 要不要跳过此条?
		//	g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex+1)%SYS_LSNAL_SPIMAXINDEX;
			goto RETURN_LAB;
		}		
		if(tmpdata[tmplen] != U8SumCheck(tmpdata, tmplen)){
			goto RETURN_LAB;
		}	
//		memcpy(tx_buf, &tmpdata[4], tmplen-4)
		
		ProPacket(&tmpdata[4], tmplen-4, PRO_UP_LSNAL_INFO_ID,TRUE); 
		g_pro_struct.tx_lsnal_falg = TRUE;
	}
RETURN_LAB:
	return;
}
static void ProUpLogin(void)
{// 01 登入
	uint8 tx_buf[231],tx_len;	//buff考虑帧头尾-25
	uint16 login_number;
	
//	RtcGetCalendarTime(g_protime_union.arry);
	memcpy(tx_buf,g_protime_union.arry,6);		//时间
	//登入流水号 1~65531
	login_number = (g_prologin_union.Item.login_number+1)%MAX65531;
	if(login_number  == 0) g_prologin_union.Item.login_number = MAX65531;	
	else									 g_prologin_union.Item.login_number = login_number;		
	tx_buf[6] = g_prologin_union.arry[1]; 		//大端模式，高位在前
	tx_buf[7] = g_prologin_union.arry[0]; 
	//其他登录信息
	memcpy(&tx_buf[8],&g_prologin_union.arry[2],22);		
	//可充电储能系统编码 暂时都不上
	
	tx_len = 6 + 24;	
	ProPacket(tx_buf,tx_len,PRO_UP_LOGIN_ID,TRUE);//FALSE);	// qlj 为什么不是FALSE
}
void ProUpLogout(void)
{// 04 登出
	uint8 tx_buf[231],tx_len;	//buff考虑帧头尾-25
	
//	RtcGetCalendarTime(g_protime_union.arry);
	memcpy(tx_buf,g_protime_union.arry,6);		//时间
	//登入流水号 1~65531	 g_prologin_union.Item.login_number
	tx_buf[6] = g_prologin_union.arry[1]; 		//大端模式，高位在前
	tx_buf[7] = g_prologin_union.arry[0]; 
	
	tx_len = 8;	
	ProPacket(tx_buf,tx_len,PRO_UP_LOGOUT_ID,TRUE);	
}
void ProUpHeartBeat(void)
{// 07 心跳	
	ProPacket(NULL,0,(uint8)PRO_UP_HEARTBEAT_ID,FALSE);
}
static void ProUpCheckTime(void)
{// 08 与平台校时	
	ProPacket(NULL,0,(uint8)PRO_UP_CHECKTIME_ID,FALSE);
}

//应答
static void ProUpQueryParams(uint8 data[], uint8 len)	// qlj 发送长度待处理
{//	参数查询
	uint8  tx_buf[231],*p;	//buff考虑帧头尾-25
	uint16 tx_len;
	uint8  i;
	p = tx_buf;
	
//	RtcGetCalendarTime(g_protime_union.arry);
	memcpy(p,g_protime_union.arry,6);		//时间
	p += 6;
	
	if(len == 0){//无参数请求
		goto RETURN_LAB;
	}
	// qlj 考虑数据量大 越界问题 是否分包？
	*p++ = len; //请求总条数
	for(i=0;i < len;i++){
		switch(data[i]){
			case PRO_PARA_SAVEPERI_ID:{
				*p++ = (uint8)PRO_PARA_SAVEPERI_ID;
				*p++ = (uint8)(g_propara_union.Item.SavePeri >> 8);
				*p++ = (uint8)(g_propara_union.Item.SavePeri & 0xFF);				
				break;
			}
			case PRO_PARA_NORUPPERI_ID:{
				*p++ = (uint8)PRO_PARA_NORUPPERI_ID;
				*p++ = (uint8)(g_propara_union.Item.NorUpPeri >> 8);
				*p++ = (uint8)(g_propara_union.Item.NorUpPeri & 0xFF);
				break;
			}
			case PRO_PARA_ALRUPPERI_ID:{
				*p++ = (uint8)PRO_PARA_ALRUPPERI_ID;
				*p++ = (uint8)(g_propara_union.Item.AlrUpPeri >> 8);
				*p++ = (uint8)(g_propara_union.Item.AlrUpPeri & 0xFF);
				break;
			}
			case PRO_PARA_DOMAINLEN_ID:{
				*p++ = (uint8)PRO_PARA_DOMAINLEN_ID;
				*p++ = g_propara_union.Item.DomainLen;	
				break;
			}
			case PRO_PARA_DOMAIN_ID:{
				*p++ = (uint8)PRO_PARA_DOMAIN_ID;//平台域名
				memcpy(p,g_propara_union.Item.Domain,g_propara_union.Item.DomainLen);		
				p +=  g_propara_union.Item.DomainLen;
				break;
			}
			case PRO_PARA_PORT_ID:{
				*p++ = (uint8)PRO_PARA_PORT_ID;
				*p++ = (uint8)(g_propara_union.Item.Port >> 8);
				*p++ = (uint8)(g_propara_union.Item.Port & 0xFF);
				break;
			}
			case PRO_PARA_HDVERSION_ID:{
				*p++ = (uint8)PRO_PARA_HDVERSION_ID;//硬件版本
				memcpy(p,g_para_HDVers,5);		
				p += 5;
				break;
			}
			case PRO_PARA_SFVERSION_ID:{
				*p++ = (uint8)PRO_PARA_SFVERSION_ID;//软件版本
				memcpy(p,g_para_SFVers,5);	
				p += 5;	
				break;
			}
			case PRO_PARA_HEARTPERI_ID:{
				*p++ = (uint8)PRO_PARA_HEARTPERI_ID;
				*p++ = g_propara_union.Item.HeartPeri;
				break;
			}
			case PRO_PARA_T_ACK_TIM_ID:{
				*p++ = (uint8)PRO_PARA_T_ACK_TIM_ID;
				*p++ = (uint8)(g_propara_union.Item.TAckTim >> 8);
				*p++ = (uint8)(g_propara_union.Item.TAckTim & 0xFF);
				break;
			}
			case PRO_PARA_P_ACK_TIM_ID:{
				*p++ = (uint8)PRO_PARA_P_ACK_TIM_ID;
				*p++ = (uint8)(g_propara_union.Item.PAckTim >> 8);
				*p++ = (uint8)(g_propara_union.Item.PAckTim & 0xFF);
				break;
			}
			case PRO_PARA_N_LOG_TIM_ID:{
				*p++ = (uint8)PRO_PARA_N_LOG_TIM_ID;
				*p++ = g_propara_union.Item.NLogTim;
				break;
			}
			case PRO_PARA_PDOMAINLEN_ID:{
				*p++ = (uint8)PRO_PARA_PDOMAINLEN_ID;
				*p++ = g_propara_union.Item.PDomainLen;
				break;
			}
			case PRO_PARA_PDOMAIN_ID:{
				*p++ = (uint8)PRO_PARA_PDOMAIN_ID;//公共平台域名
				memcpy(p,g_propara_union.Item.PDomain,g_propara_union.Item.PDomainLen);		
				p +=  g_propara_union.Item.PDomainLen;
				break;
			}
			case PRO_PARA_PPORT_ID:{
				*p++ = (uint8)PRO_PARA_PPORT_ID;
				*p++ = (uint8)(g_propara_union.Item.PPort >> 8);
				*p++ = (uint8)(g_propara_union.Item.PPort & 0xFF);
				break;
			}
			case PRO_PARA_MONITOR_ID:{
				*p++ = (uint8)PRO_PARA_MONITOR_ID;
				*p++ = g_propara_union.Item.Monitor;
				break;
			}
			
			default: break;
		}
	}
	
	tx_len = p - tx_buf;
	if(tx_len > 231) tx_len = 231;
	ProPacket(tx_buf,tx_len,PRO_UP_REAL_INFO_ID,TRUE);

RETURN_LAB:
	return;
}



/******下行通讯函数******/

static void ProCtrUpgrade(uint8 data[],uint8 len)
{
	 //‘;’分隔，某个参数没有则为空
	//URL地址;拨号点名称;拨号用户名;拨号密码;地址;端口;生产商代号;硬件版本;固件版本;连接到升级服务器时限
	uint8 i,j=0,index[PRO_UPGRADE_PARA_NUM];
	
	for(i=0; i < len; i++){
		if(data[i] == ';'){
			index[j++] = i;
		}
	}
	if(j!=PRO_UPGRADE_PARA_NUM){
		goto RETURN_LAB;
	}
	if(index[0]){
		memcpy(g_proupgread_struct.file_path, data, index[0]);
		g_proupgread_struct.file_path[index[0]] = 0; //保险加结束符
	}
	i = index[0] + 1;//起始下标
	j = index[1] - i;//长度
	if(j){
		memcpy(g_proupgread_struct.dial_APN, &data[i], j);
		g_proupgread_struct.dial_APN[j] = 0; //保险加结束符
	}				
	i = index[1] + 1;//起始下标
	j = index[2] - i;//长度
	if(j){
		memcpy(g_proupgread_struct.user_name, &data[i], j);
		g_proupgread_struct.user_name[j] = 0; //保险加结束符
	}				
	i = index[2] + 1;//起始下标
	j = index[3] - i;//长度
	if(j){
		memcpy(g_proupgread_struct.pass_word, &data[i], j);
		g_proupgread_struct.pass_word[j] = 0; //保险加结束符
	}				
	i = index[3] + 1;//起始下标
	j = index[4] - i;//长度
	if(j){
		if(j==4){
			g_proupgread_struct.ip_domain[0] = 0;
			g_proupgread_struct.ip_domain[1] = 0;
			memcpy(&g_proupgread_struct.ip_domain[2], &data[i], j);
		}
		else{
			memcpy(g_proupgread_struct.ip_domain, &data[i], j);
		}
	}				
	i = index[4] + 1;//起始下标
	j = index[5] - i;//长度
	if(j==2){
		g_proupgread_struct.port = (data[i]<<8) + data[i+1]; 
	}				
	i = index[5] + 1;//起始下标
	j = index[6] - i;//长度
	if(j==4){
		g_proupgread_struct.terminal_id = (data[i]<<24) +(data[i+1]<<16) +(data[i+2]<<8) + data[i+3]; 
	}
	i = index[6] + 1;//起始下标
	j = index[7] - i;//长度
	if(j>=5){
		memcpy(g_proupgread_struct.HDVers, &data[i], 5);
	}	
	i = index[7] + 1;//起始下标
	j = index[8] - i;//长度
	if(j>=5){
		memcpy(g_proupgread_struct.SFVers, &data[i], 5);
	}	
	i = index[8] + 1;//起始下标
	j = index[9] - i;//长度
	if(j==4){
		g_proupgread_struct.overtime = (data[i]<<24) +(data[i+1]<<16) +(data[i+2]<<8) + data[i+3]; 
	}
	
	g_proupgread_struct.flag = 1;
	
//	ProUpAck(data,ack);   // qlj 要不要ACK
	
/* 	if(res)								// qlj ftp升级待编写
	{
		res = FtpMain();
		if(res)
		{
			upgrade_res = SUCCESS_ACK;
		}
		
		UdpIpPortInit();
		ProUpUpgradeResult(upgrade_res);///上传后台,升级标志
		
		if(res)
		{
			LocalDebug("upgrade_success\r\n",StrLen("upgrade_success\r\n",0),LOCAL_TEST_DEBUG);
			LocalCommFtpState(0,ftp_struct.ftp_file_total_size,ftp_struct.ftp_rx_file_byte_counter);
			SysReset();///置程序更新标志，复位；
		}
		else
		{
			LocalDebug("upgrade_fail\r\n",StrLen("upgrade_fail\r\n",0),LOCAL_TEST_DEBUG);
			LocalCommFtpState(1,ftp_struct.ftp_file_total_size,ftp_struct.ftp_rx_file_byte_counter);
		}
	}  */
	
RETURN_LAB:
	return;
}
static void ProDownAck(uint8 data[],uint16 len)
{
	uint8 i,tx_cmd,rx_cmd,rx_ack;

	rx_cmd = data[PRO_CMD_INDEX];
	rx_ack = data[PRO_ACK_INDEX];
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){//轮询比较发送指令与应答指令，判断应答标志
		tx_cmd = g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX];
		if((tx_cmd==rx_cmd)&&(g_pro_struct.tx_struct.re_tx_full_flag[i]==TRUE)){
			g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;	//不管应答对错都取消发送缓存标志
			if(rx_ack!=PRO_ACK_SUCCEED_FLAG){
				#ifdef PROTOCOL_DEBUG
					printf("ACK ERROR ID = %d\r\n",rx_ack);
				#endif
				goto RETURN_LAB;
			}
			break;
		}
	}
	
	if(i == PRO_MAX_TX_BUF_ARRAY){
		goto RETURN_LAB;
	}
	
	switch(rx_cmd){	
		case PRO_UP_LOGIN_ID:{
//			if(rx_ack == PRO_ACK_SUCCEED_FLAG){
				g_pro_struct.try_login_statu = 3;
				g_pro_struct.login_center_flag = TRUE;
//			}
			break;
		}
		case PRO_UP_LOGOUT_ID:{
//			if(rx_ack == PRO_ACK_SUCCEED_FLAG){
				g_pro_struct.try_login_statu = 5;
			g_pro_struct.login_center_flag  = FALSE;
//			}
			break;
		}
		case PRO_UP_LSNAL_INFO_ID:{
			/* if(g_pro_struct.tx_flash_lsnal_falg)
			{
				sys_work_para_struct.lsnal_tail_page = (sys_work_para_struct.lsnal_tail_page+1) % FLASH_LSNAL_MAX_PAGES;
				ProLsnalHeadTailSave();
			}
			else
			{
				sys_data_struct.lsnal_data[0] = INVALID_VAL_FF;
				bias_addr = ((uint8*)sys_data_struct.lsnal_data) - ((uint8*)sys_data_struct.gps_info_of_gprs);
				SpiFramWrite(SYS_DATA_STRUCT_START_ADDR+bias_addr,sys_data_struct.lsnal_data,1);
			}
			g_pro_struct.tx_lsnal_falg = FALSE; */
			break;
		}
		default:
		{
			break;
		}
	}
//	g_pro_struct.tx_lsnal_data_flag = TRUE;
//	g_pro_struct.tx_struct.acc_on_tx_sec_counter = 2*SYS_TASK_SEC_TIMER;///ACC开,延时60秒到
RETURN_LAB:
	return;
}
static void ProDownParaQuiry(uint8 data[],uint8 len)
{
	uint8 tmp_num;
	
	tmp_num = data[PRO_DATA_INDEX+7];
	ProUpQueryParams(data+PRO_DATA_INDEX+8,tmp_num);
}
static void ProDownSetPara(uint8 data[],uint8 len)
{
	uint8  *p,tmp_data[PRO_DOMAINLEN_MAX];
	uint8  tmp_num,i,tmp_id;
	uint16 tmp;
	
	tmp_num = data[PRO_DATA_INDEX+7];
	p = data+PRO_DATA_INDEX+8;
	for(i=0;i < tmp_num;i++){
	tmp_id = *p++;
	switch(tmp_id){
		case PRO_PARA_SAVEPERI_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.SavePeri = tmp;		
			break;
		}
		case PRO_PARA_NORUPPERI_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.NorUpPeri = tmp;		
			break;
		}
		case PRO_PARA_ALRUPPERI_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.AlrUpPeri = tmp;		
			break;
		}
		case PRO_PARA_DOMAINLEN_ID:{
			g_propara_union.Item.DomainLen = *p++;
			break;
		}
		case PRO_PARA_DOMAIN_ID:{
			for(tmp=0;tmp < PRO_DOMAINLEN_MAX;tmp++){
				if(*(p+tmp) == 0){// qlj 若服务器传来的域名字符串以0结尾?
					p += (tmp+1);
					break;
				}
				tmp_data[tmp] = *(p+tmp);
			}
			if(tmp != PRO_DOMAINLEN_MAX){
				g_propara_union.Item.DomainLen = tmp;
				memcpy(g_propara_union.Item.Domain, tmp_data, g_propara_union.Item.DomainLen);	
			}
			
//			//若设置域名前设置域名长度可以直接用
//			memcpy(g_propara_union.Item.Domain, p, g_propara_union.Item.DomainLen);
//			p += g_propara_union.Item.DomainLen;
			break;
		}
		case PRO_PARA_PORT_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.Port = tmp;		
			break;
		}
		case PRO_PARA_HEARTPERI_ID:{
			g_propara_union.Item.HeartPeri = *p++;
			break;
		}
		case PRO_PARA_T_ACK_TIM_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.TAckTim = tmp;		
			break;
		}
		case PRO_PARA_P_ACK_TIM_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.PAckTim = tmp;		
			break;
		}
		case PRO_PARA_N_LOG_TIM_ID:{
			g_propara_union.Item.NLogTim = *p++;
			break;
		}
		case PRO_PARA_PDOMAINLEN_ID:{
			g_propara_union.Item.PDomainLen = *p++;
			break;
		}
		case PRO_PARA_PDOMAIN_ID:{
			for(tmp=0;tmp < PRO_DOMAINLEN_MAX;tmp++){
				if(*(p+tmp) == 0){// qlj 若服务器传来的域名字符串以0结尾?
					p += (tmp+1);
					break;
				}
				tmp_data[tmp] = *(p+tmp);
			}
			if(tmp != PRO_DOMAINLEN_MAX){
				g_propara_union.Item.PDomainLen = tmp;
				memcpy(g_propara_union.Item.PDomain, tmp_data, g_propara_union.Item.PDomainLen);	
			}
			
//			//若设置域名前设置域名长度可以直接用
//			memcpy(g_propara_union.Item.PDomain, p, g_propara_union.Item.PDomainLen);
//			p += g_propara_union.Item.PDomainLen;
			break;
		}
		case PRO_PARA_PPORT_ID:{
			tmp = *p++;
			tmp = (tmp<<8) + *p++;
			g_propara_union.Item.PPort = tmp;		
			break;
		}
		case PRO_PARA_MONITOR_ID:{
			g_propara_union.Item.Monitor = *p++;
			break;
		}
		
		default:	p -=1; break;
	}
}
	
/* 	ProUpAck(data,res);// qlj 要不要ACK?
	//保存
	//复位
	if(sys_boot_para_struct.sys_para_init_flag == VALID_VAL_DWORD_AA)
	{
		SysBootParaWrite();
		SysReset();
	} */
}
static void ProDownControl(uint8 data[],uint8 len)	// qlj 要不要应答
{
	uint8  tmp_id;
	uint16 tmp;
	
	tmp = PRO_DATA_INDEX+1 +7;	//25+6+1 至少多少字节
	tmp_id = data[PRO_DATA_INDEX+7];
	switch(tmp_id){
		case PRO_CTR_UPGRADE_ID:{	
		if(len > (tmp+24)){
			ProCtrUpgrade(&data[PRO_DATA_INDEX+8], len-tmp);
		}			
			break;
		}
		case PRO_CTR_OFFTERM_ID:{		// qlj 待机还是怎么？？
			break;
		}
		case PRO_CTR_RSTTERM_ID:{	
			//保存盲区
			//复位
			break;
		}
		case PRO_CTR_FACTRST_ID:{		
			//有没有要保存的
			//恢复出厂设置
			break;
		}
		case PRO_CTR_DISLINKSER_ID:{	
			//有没有要保存的
			//断开连接	
			break;
		}
		case PRO_CTR_ALRTERM_ID:{		// qlj 怎么操作？？
			break;
		}
		case PRO_CTR_MONITOR_ID:{	
			g_propara_union.Item.Monitor = 0x01;	//置抽样标志
			// qlj 怎么取消标志？抽样时要做哪些动作？
			break;
		}
		
		default:	 break;
	}
}
	
/********盲区补偿*******/
static void ProLsnalSysExit(void)///系统退出前盲区补偿
{
	uint8 i;
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){
		if(g_pro_struct.tx_struct.re_tx_full_flag[i]){
			g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
			ProPutIntoLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
							g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX-1,
							g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
		}
	}
	
}
static void ProLsnalDataInit(void)
{
	g_syslsnal_struct.data[0] = VALID_VAL_2A;	///有效标志
	g_syslsnal_struct.data[1] = 0x00;					///总长度,2字节,从标志开始，不含1字节校验
	g_syslsnal_struct.data[2] = 0x04;					///
	g_syslsnal_struct.data[3] = 0x00;					///包内保存总条数
	g_syslsnal_struct.data[4] = 0x2E;					///校验
}
static void ProLsnalHeadTailSave(void)///保存页码
{
	uint8  data[8],res;

	data[0] = (uint8)(g_syslsnal_struct.headindex >> 24) & 0xFF;
	data[1] = (uint8)(g_syslsnal_struct.headindex >> 16) & 0xFF;
	data[2] = (uint8)(g_syslsnal_struct.headindex >>  8) & 0xFF;
	data[3] = (uint8) g_syslsnal_struct.headindex        & 0xFF;
	data[4] = (uint8)(g_syslsnal_struct.tailindex >> 24) & 0xFF;
	data[5] = (uint8)(g_syslsnal_struct.tailindex >> 16) & 0xFF;
	data[6] = (uint8)(g_syslsnal_struct.tailindex >>  8) & 0xFF;
	data[7] = (uint8) g_syslsnal_struct.tailindex        & 0xFF;
	
	res = ExteFlashEraseSector(SYS_LSNAL_SPIINDEXSECTOR);	
	if(!res){// 失败
		return;
	}
	ExteFlashWrite(SYS_LSNAL_SPIOTHER_ADDR, data, 8);
}
static void ProLsnalPageSave(uint8 data[],uint16 len)
{
	uint8  res;
	uint16 sector;
	uint32 addr;
	
	//满一段
	if((g_syslsnal_struct.headindex&0x07)==0){	//FLASH擦除 4K字节/段 即512*8
		sector = (uint16)(g_syslsnal_struct.headindex >> 3);// /8;
		res = ExteFlashEraseSector(sector);
		if(!res){// 失败
			goto RETURN_LAB;
		}
	}
	
	//满一包
	addr = SYS_LSNAL_SPISTART_ADDR + g_syslsnal_struct.headindex*LSNAL_PAGE_SIZE ;
	res = ExteFlashWrite(addr, data, len);
	if(!res){// 失败
		goto RETURN_LAB;
	}
	
	g_syslsnal_struct.headindex = (g_syslsnal_struct.headindex+1)%SYS_LSNAL_SPIMAXINDEX;
	if(g_syslsnal_struct.headindex == g_syslsnal_struct.tailindex){
		//tail前移8包即整段擦除时舍掉其余7包数据；若要不舍需另外开一段临时保存旧数据，这样会增加擦除次数
		g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex+8)%SYS_LSNAL_SPIMAXINDEX;
	}
	
	ProLsnalHeadTailSave();
RETURN_LAB:
	return;
}
static void ProLsnalDataSave(uint8 data[],uint16 len,uint8 cmd)
{
//	uint16 tmp_len,tmp;
//	uint32 bias_addr;
	
	if(cmd == PRO_UP_LSNAL_INFO_ID){									// 盲区保存过不会再保存
		goto RETURN_LAB;
	}	
/*	//有效标志 总长*2 条数 单条命令 单条长度*2 ... 校验
	if(g_syslsnal_struct.data[0] != VALID_VAL_2A){		// 从无到有存入时需初始化
		ProLsnalDataInit();
	}	
 	tmp_len  = g_syslsnal_struct.data[1] << 8;				// 长度
	tmp_len += g_syslsnal_struct.data[2];
	
	if(U8SumCheck(g_syslsnal_struct.data,tmp_len) != g_syslsnal_struct.data[tmp_len]){//校验不对重初始化
		ProLsnalDataInit();
		tmp_len = 4;
	}
	// 已有长度+校验+本次数据总数+本次数据命令和长度
	if(tmp_len+1+len+3 > LSNAL_PAGE_SIZE){					// RAM长度超512  先保存RAM已有数据,
		ProLsnalPageSave(g_syslsnal_struct.data,tmp_len+1);	
		ProLsnalDataInit();
		tmp_len = 4;
	}
	tmp = tmp_len + len + 3;
	g_syslsnal_struct.data[1] = tmp >> 8;
	g_syslsnal_struct.data[2] = tmp & 0xFF;
	g_syslsnal_struct.data[3] += 1;
	
	g_syslsnal_struct.data[tmp_len] = cmd;
	g_syslsnal_struct.data[tmp_len+1] = len >> 8;
	g_syslsnal_struct.data[tmp_len+2] = len & 0xFF; 
	
	MemCpy(g_syslsnal_struct.data+3+tmp_len,data,len);	
	g_syslsnal_struct.data[tmp] = U8SumCheck(g_syslsnal_struct.data,tmp);
	//若RAMbuff没有凑满，此时复位了，buff中数据就会丢失，24N有临时存spieeprom，如果需要可以开4k临时存
*/
	
	//512字节放一条数据 与之前有区别
	//有效标志单条长度*2 单条命令 ... 校验
	if(g_syslsnal_struct.data[0] != VALID_VAL_2A){		// 从无到有存入时需初始化
		g_syslsnal_struct.data[0] = VALID_VAL_2A;
		g_syslsnal_struct.data[1] = len >> 8;
		g_syslsnal_struct.data[2] = len & 0xFF;
		g_syslsnal_struct.data[3] = cmd;
		
		MemCpy(g_syslsnal_struct.data+4, data, len);	
		g_syslsnal_struct.data[len+4] = U8SumCheck(g_syslsnal_struct.data,len+4);
		ProLsnalPageSave(g_syslsnal_struct.data,len+5);	
	}
	
RETURN_LAB:
	return;
}
static void ProPutIntoLsnal(uint8 data[],uint16 len,uint8 cmd)
{
	switch(cmd){
		case PRO_UP_REAL_INFO_ID:
		case PRO_UP_LSNAL_INFO_ID:{
			if(cmd == PRO_UP_LSNAL_INFO_ID){	//清正在发送标志
				g_pro_struct.tx_lsnal_falg = FALSE; 
			}
			ProLsnalDataSave(data,len,cmd);///写盲区
			break;
		}
		case PRO_UP_LOGIN_ID:{
//			gsm_misc_struct.cur_mode = POWER_INIT_MODE;
			break;
		}
		default:		break;
	}
}


/*******打包发送********/
static void ProConstructFrameHead(uint8 data[],uint16 tx_len,uint8 cmd)
{//大端模式，高位在前
	uint8 *p = data;

	*p++ = 0x23;									//'#'
	*p++ = 0x23;									//'#'
	*p++ = cmd;										//命令标识
	*p++ = PRO_ACK_CMD_FLAG;			//标命令帧
	memcpy(p,g_VIN,17);
	p += 17;
	*p++ = PRO_DISENCRYT_MODE;
	*p++ = (uint8)(tx_len >> 8);		//数据单元长度
	*p++ = (uint8)(tx_len & 0xFF);	
	//数据单元
	//校验码
	
	//g_pro_struct.tx_seq++;
}
static void ProConstructFrameTail(uint8 data[],uint16 tx_len)
{//添加帧尾
	data[tx_len] = XorCheck(data,tx_len);	//BCC异或校验
}

void ProPacket(uint8 tx_data[],uint16 tx_len,uint8 tx_cmd,uint8 ack_flag)
{
	uint8 i,*p,tmp_data[TMP_BUFF_MAXLEN];
	uint16 tmp_len;
	#ifdef PRO_DEBUG
		char str_ch[10];
		uint8 str_len;
	#endif

	p = tmp_data;
	
 	if(g_sysprivatepara_struct.link_center_flag != 1){//没有登录
		goto RETURN_LAB;
	} 
	
	ProConstructFrameHead(p,tx_len,tx_cmd);
	p += PRO_DATA_INDEX;
	MemCpy(p,tx_data,tx_len);
	tmp_len = PRO_DATA_INDEX + tx_len;
	ProConstructFrameTail(tmp_data,tmp_len);
	tmp_len += 1;		//加一个校验码
	
	if(!ack_flag)
	{
		if(g_sysprivatepara_struct.updata_sengding == 0){
			#ifdef PRO_DEBUG
				str_len = sprintf(str_ch,"%s ","上行：");
				LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);	
				for(i=0;i<tmp_len;i++){
					str_len = sprintf(str_ch,"%02X ",tmp_data[i]);
					LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);
				}
				LocalDebug("\r\n",2,LOCAL_PRO_DEBUG);
			#endif
			
			g_gprs_data_struct.SendDataLen = tmp_len;
			memcpy(GPRStestdata, tmp_data, g_gprs_data_struct.SendDataLen);
			g_sysprivatepara_struct.updata_sengding = 1;
			g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
			g_sysmiscrun_struct.sys_upheart_count = 0;		///上行心跳计时清0
		}
		goto RETURN_LAB;
	}
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){
		if(!g_pro_struct.tx_struct.re_tx_full_flag[i]){
			break;
		}
	}
	if(i == PRO_MAX_TX_BUF_ARRAY){
		if(ack_flag){
			goto LSNAL_LAB;
		}
	}
	
	MemCpy(g_pro_struct.tx_struct.re_tx_buf[i],tmp_data,tmp_len);
	switch(g_pro_struct.tx_struct.re_tx_buf[i][2]){
		case PRO_UP_LOGIN_ID:{
			g_pro_struct.tx_struct.re_tx_time[i] = g_propara_union.Item.NLogTim;//
			break;
		}	
		case PRO_UP_REAL_INFO_ID:
		case PRO_UP_LSNAL_INFO_ID:
		case PRO_UP_LOGOUT_ID:
		case PRO_UP_HEARTBEAT_ID:
		case PRO_UP_CHECKTIME_ID:{
			g_pro_struct.tx_struct.re_tx_time[i] = PRO_NORMAL_RETX_TIME;//
			break;
		}
		default : break;
	}
	g_pro_struct.tx_struct.re_tx_len[i] = tmp_len;
	g_pro_struct.tx_struct.re_tx_counter[i] = 0;
	g_pro_struct.tx_struct.re_tx_sec_counter[i] = g_pro_struct.tx_struct.re_tx_time[i];
	g_pro_struct.tx_struct.re_tx_full_flag[i] = TRUE;
	return;
LSNAL_LAB:
	ProPutIntoLsnal(tx_data,tx_len,tx_cmd);//纯数据
RETURN_LAB:
	return;
}
///周期发送
void ProPeriodTx(uint16 past_sec)
{
	uint16 i;
	#ifdef PRO_DEBUG
		char str_ch[10];
		uint8 str_len;
		uint16 j;
	#endif
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){										//轮询重发buff
		if(g_pro_struct.tx_struct.re_tx_sec_counter[i] >= g_pro_struct.tx_struct.re_tx_time[i]){ //重发机制		
			g_pro_struct.tx_struct.re_tx_sec_counter[i] = 0;
			if(g_pro_struct.tx_struct.re_tx_full_flag[i]){			//有数据
				if(g_pro_struct.tx_struct.re_tx_counter[i] < 3){  //重发小于3次		
					if(g_sysprivatepara_struct.updata_sengding == 0){
						g_pro_struct.tx_struct.re_tx_counter[i]++;			
						#ifdef PRO_DEBUG
							str_len = sprintf(str_ch,"%s ","上行：");
							LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);	
							for(j=0;j<g_pro_struct.tx_struct.re_tx_len[i];j++){
								str_len = sprintf(str_ch,"%02X ",g_pro_struct.tx_struct.re_tx_buf[i][j]);
								LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);
							}
							LocalDebug("\r\n",2,LOCAL_PRO_DEBUG);
						#endif
						
						g_gprs_data_struct.SendDataLen = g_pro_struct.tx_struct.re_tx_len[i];
						memcpy(GPRStestdata, g_pro_struct.tx_struct.re_tx_buf[i], g_gprs_data_struct.SendDataLen);
						g_sysprivatepara_struct.updata_sengding = 1;
						g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
					g_sysmiscrun_struct.sys_upheart_count = 0;		///上行心跳计时清0
					}
					break;
				}
				else{
					g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
					if(g_pro_struct.tx_struct.re_tx_buf[i][2] == PRO_UP_LOGIN_ID){
						g_pro_struct.over3_relogin_time = 0;			//标记本次3次登录失败
					}
					else{
						ProPutIntoLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
										g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX-1,
										g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
					}
				}
			}
		}
	}
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){//轮询重发buff，对有数据的buff秒计数
		if(g_pro_struct.tx_struct.re_tx_full_flag[i]){
			g_pro_struct.tx_struct.re_tx_sec_counter[i] += past_sec;
		}
	}
	
	ProUpLsnalFormation();
	
	if(g_sysprivatepara_struct.link_center_flag == 1){//已经连接到中心平台
		if(g_pro_struct.try_login_statu == 1){
			g_pro_struct.try_login_statu = 2;
	//		MemCpy(udp_struct.udp_dst_ip,g_pro_struct.comm_para_ip,4);
	//		MemCpy(udp_struct.udp_dst_port,g_pro_struct.comm_para_port,2);
	//		UdpIpPortInit();
			ProUpLogin();
		}
		else if(g_pro_struct.try_login_statu == 2){
			if(g_pro_struct.over3_relogin_time < 120){//g_propara_union.Item.NLogTim*60){				//前3次登录失败，开始重登录30min计时
				g_pro_struct.over3_relogin_time += past_sec;
			}
			else if(g_pro_struct.over3_relogin_time == 0xFFFF){//开机第一次不需要计时
			}
			else {																						// 30min到了重登录
				g_pro_struct.try_login_statu = 1;				
				g_pro_struct.over3_relogin_time = 0xFFFF;				//设为无效
			}
		}
	}
}


/*****协议处理主函数******/
void ProProcess(uint8 data[],uint16 len)
{
	uint16 tmp_len;
	
	#ifdef PRO_DEBUG
		uint8 str_len;
		char str_ch[20];
		uint16 i;

		str_len = sprintf(str_ch,"%s ","下行：");
		LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);	
		for(i=0;i<len;i++){
			str_len = sprintf(str_ch,"%02X ",data[i]);
			LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);
		}
		LocalDebug("\r\n",2,LOCAL_PRO_DEBUG);
	#endif

	if(data[0]!='#' || data[1]!='#' ){				//起始符不对
		goto RETURN_LAB;
	}
	tmp_len = data[PRO_DATALEN_INDEX];
	tmp_len += data[PRO_DATALEN_INDEX+1];
	tmp_len += 25;
	if(tmp_len >= len){												//长度不对 tmp_len预测长度，len实际接收长度
		goto RETURN_LAB;
	}
	if(data[len-1] != XorCheck(data,len)){		//校验不对
		goto RETURN_LAB;
	}
	if(data[PRO_ACK_INDEX] != 0xFE){
		ProDownAck(data,tmp_len);
	}
	else{
		switch(data[PRO_CMD_INDEX]){
			case PRO_DOWN_INQUIRY_ID:{
				ProDownParaQuiry(data,tmp_len);
				break;
			}
			case PRO_DOWN_SET_ID:{
				ProDownSetPara(data,tmp_len);
				break;
			}
			case PRO_DOWN_CONTROL_ID:{
				ProDownControl(data,tmp_len);
				break;
			}		
			default:			break;
		}
	}
	
RETURN_LAB:	
/* 	sys_work_para_struct.lsnal_min_counter = 0;
	sys_work_para_struct.lsnal_sys_reset_min_counter = 0;
	sys_work_para_struct.lsnal_sys_comm_reset_min_counter = 0;
	sys_work_para_struct.lsnal_sys_must_reset_sec_counter = 0;
	g_pro_struct.no_rx_data_sec_counter = 0; */
	__NOP();__NOP();__NOP();__NOP();
}

