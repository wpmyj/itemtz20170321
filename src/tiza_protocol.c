#define PROTOCOL_GLOBAL
#define PROTOCOL_DEBUG

#include "tiza_include.h"


static void ProPutIntoLsnal(uint8 data[],uint16 len,uint8 cmd);
static void ProConstructFrameTail(uint8 data[],uint16 tx_len);


void ProWrite_VIN(uint8* str)
{
	uint8 data[20];
	//ͷ2A + VIN + У��
	// ��ʼ��VIN
	memcpy(&data[1], str, 17);
	data[0]  = 0x2A;
	data[18] = U8SumCheck(data, 18);
	FlashErase(SYS_SAVEVIN_INDEXPAGE);
	FlashWrite(SYS_SAVEVIN_INDEXPAGE, data, 19);		
	
	memcpy(g_VIN, &data[1], 17);
}
static void ProRead_VIN(void)
{
	uint8 data[20];
	//ͷ2A + VIN + У��
	FlashRead(SYS_SAVEVIN_INDEXPAGE, data, 19);

	if(data[0]==0x2A && data[18]==U8SumCheck(data, 18)){
		memcpy(g_VIN, &data[1], 17);
	}
	else {// ��ʼ��VIN		
		memcpy(&data[1], "01234567890123456", 17);
		ProWrite_VIN(&data[1]);	
	}
}
///����Э�������ʼ��
void ProParaInit(void)
{
 	uint8 i;
	
	g_sysmiscrun_struct.have_sysAlarm_flag = 0;//1;//		//ϵͳ������־ ͬʱsend_sysAlarm_count=0
	g_pro_struct.tx_lsnal_data_flag = FALSE;			//ä�������ݱ�־
	
	
//	g_pro_struct.try_login_statu  		= 0;
//	g_pro_struct.login_center_flag  	= FALSE;
//	g_sysmiscrun_struct.NLogTim_count = 0xFFFF;		///���ε�¼ʧ�ܺ����µ�¼ʱ����	
	

	////////////////////////////////////////////////////	
	g_proupgread_struct.flag = 0;							//����ӣ�1��Ҫ���� 2�������� 0��Ч
	g_pro_struct.tx_one_lsnal_falg = 0;						//1:��ä���������ڷ���  0��Ч
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){
		g_pro_struct.tx_struct.re_tx_full_flag[i] = 0;
	}
	g_sysprivatepara_struct.vehicle_start_flag 	= 0;			//����������־		1����			0δ����
	g_sysprivatepara_struct.link_center_flag 		= 0;			//����ƽ̨��־		1������		0δ����
	g_sysprivatepara_struct.updata_sengding 		= 1;			//�����ϴ���־		1�����ϴ� 0���� 
	//VIN��
//	ProRead_VIN();
	memcpy(g_VIN,"A1234567890123456",17);	
//	memcpy(g_VIN,"A0000013776516481",17);	//�ܽ��2 ���� �ط�1��
//	memcpy(g_VIN,"A0000013515127369",17);		//�����������1  30s
//	memcpy(g_VIN,"A0000013585107257",17);	//��·���  30s
//	memcpy(g_VIN,"A0000013601464223",17);	//����·��3  30s  ������ģ��
//	memcpy(g_VIN,"A0000015261409343",17);	//�ܽ��4	����  �ط�1��
		
	g_sysmiscrun_struct.SavePeri_count 	= 0;							///���ڱ����ʱ
	g_sysmiscrun_struct.NorUpPeri_count = 0;							///���������ϴ���Ϣ��ʱ
	g_sysmiscrun_struct.AlrUpPeri_count	= 0;							///�����ϴ������ʱ
	g_sysmiscrun_struct.upheart_count		= 0;							///ϵͳ�ϴ�������ʱ
//	g_sysmiscrun_struct.TAckTim_count		= 0;						///�ն�Ӧ��ʱʱ��		
	g_sysmiscrun_struct.PAckTim_count		= 0xFF;						///ƽ̨Ӧ��ʱʱ��		
	g_sysmiscrun_struct.need_check_time_flag = 0;					///��ҪУ��ϵͳ����ʱ��     01��GPS   02��ƽ̨  00��Ч
	g_sysmiscrun_struct.have_sysAlarm_count  = 0;					///ϵͳ���ֱ�������ʱ
	g_sysmiscrun_struct.save_sysAlarm_numb   = SYS_SAVEALARM_NUMB;		///δ���ֱ���ʱ ����INTFLAH�ļ�¼����  
	
	
	
//	ProRead_SysPara();
//	OSTimeDlyHMSM(0, 0, 0, 500);	
//	Read_Lsnal_Index();
//	OSTimeDlyHMSM(0, 0, 0, 500);
//	Read_Alarm_Index();
//	OSTimeDlyHMSM(0, 0, 0, 500);	
}
	
	
	
///********ä������*******/
//static void ProLsnalSysExit(void)///ϵͳ�˳�ǰä������
//{
//	uint8 i;
//	
//	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){
//		if(g_pro_struct.tx_struct.re_tx_full_flag[i]){
//			g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
//			ProPutIntoLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
//							g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX-1,
//							g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
//		}
//	}
//	
//}
static void ProLsnalDataInit(void)
{
	g_syslsnal_struct.data[0] = VALID_VAL_2A;	///��Ч��־
	g_syslsnal_struct.data[1] = 0x00;					///�ܳ���,2�ֽ�,�ӱ�־��ʼ������1�ֽ�У��
	g_syslsnal_struct.data[2] = 0x04;					///
	g_syslsnal_struct.data[3] = 0x00;					///ָ��
	g_syslsnal_struct.data[4] = 0x2E;					///У��
}
void ProLsnalHeadTailSave(void)			
{///����ҳ��
	uint8  data[9],res;

	data[0] = (uint8)(g_syslsnal_struct.headindex >> 24) & 0xFF;
	data[1] = (uint8)(g_syslsnal_struct.headindex >> 16) & 0xFF;
	data[2] = (uint8)(g_syslsnal_struct.headindex >>  8) & 0xFF;
	data[3] = (uint8) g_syslsnal_struct.headindex        & 0xFF;
	data[4] = (uint8)(g_syslsnal_struct.tailindex >> 24) & 0xFF;
	data[5] = (uint8)(g_syslsnal_struct.tailindex >> 16) & 0xFF;
	data[6] = (uint8)(g_syslsnal_struct.tailindex >>  8) & 0xFF;
	data[7] = (uint8) g_syslsnal_struct.tailindex        & 0xFF;
	
	data[8] = U8SumCheck(data,8);
	res = ExteFlashEraseSector(SYS_LSNAL_SPIINDEXSECTOR);	
	if(!res){// ʧ��
		return;
	}
	ExteFlashWrite(SYS_LSNAL_SPIOTHER_ADDR, data, 9);
}
static void ProLsnalPageSave(uint8 data[],uint16 len)
{
	uint8  res;
	uint16 sector;
	uint32 addr;
	
	//��һ��
	if((g_syslsnal_struct.headindex&0x07)==0){	//FLASH���� 4K�ֽ�/�� ��512*8
		sector = (uint16)(g_syslsnal_struct.headindex >> 3);// /8;
		res = ExteFlashEraseSector(sector);
		if(!res){// ʧ��
			goto RETURN_LAB;
		}
	}
	
	//��һ��
	addr = SYS_LSNAL_SPISTART_ADDR + g_syslsnal_struct.headindex*LSNAL_PAGE_SIZE ;
	res = ExteFlashWrite(addr, data, len);
	if(!res){// ʧ��
		goto RETURN_LAB;
	}
	
	g_syslsnal_struct.headindex = (g_syslsnal_struct.headindex+1)%SYS_LSNAL_SPIMAXINDEX;
	if(g_syslsnal_struct.headindex == g_syslsnal_struct.tailindex){
		//tailǰ��8�������β���ʱ�������7�����ݣ���Ҫ���������⿪һ����ʱ��������ݣ����������Ӳ�������
		g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex+8)%SYS_LSNAL_SPIMAXINDEX;
		printf("TIZA_PROTOCL.C LINE 132: \n");
	}
	
	ProLsnalHeadTailSave();
	
	g_syslsnal_struct.data[0] = INVALID_VAL_FF;
RETURN_LAB:
	return;
}
static void ProLsnalDataSave(uint8 data[],uint16 len,uint8 cmd)
{
//	uint16 tmp_len,tmp;
//	uint32 bias_addr;
	
	if(cmd == PRO_UP_LSNAL_INFO_ID){									// ä������������ٱ���
		goto RETURN_LAB;
	}	
	
	//512�ֽڷ�һ������ ��֮ǰ������
	//��Ч��־��������*2 �������� ... У��
	if(g_syslsnal_struct.data[0] != VALID_VAL_2A){		// ���޵��д���ʱ���ʼ��
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
			if(cmd == PRO_UP_LSNAL_INFO_ID){	//�����ڷ��ͱ�־
				g_pro_struct.tx_one_lsnal_falg = 0; 
			}
			ProLsnalDataSave(data,len,cmd);///дä��
			break;
		}
		case PRO_UP_LOGIN_ID:{
//			gsm_misc_struct.cur_mode = POWER_INIT_MODE;
			break;
		}
		default:		break;
	}
}




/*****����ͨѶ����******/
//���ģʽ����λ��ǰ
static uint8 ProVehicle(uint8 data[])
{
	uint8 index=0;
	
	
	data[index++] = (uint8)PRO_VEHICLE_FLAG;
	//0~19
	data[index++] = g_provehice_union.Item.vehicle;
	data[index++] = g_provehice_union.Item.charge;
	data[index++] = g_provehice_union.Item.mode;
	data[index++] = g_provehice_union.arry[5];	//speed
	data[index++] = g_provehice_union.arry[4];
	data[index++] = g_provehice_union.arry[3];	//mileage
	data[index++] = g_provehice_union.arry[2];
	data[index++] = g_provehice_union.arry[1];
	data[index++] = g_provehice_union.arry[0];
	data[index++] = g_provehice_union.arry[7];	//voltage
	data[index++] = g_provehice_union.arry[6];
	data[index++] = g_provehice_union.arry[9];	//current
	data[index++] = g_provehice_union.arry[8];	
	data[index++] = g_provehice_union.Item.SOC;
	data[index++] = g_provehice_union.Item.DC_statu;	
	data[index++] = g_provehice_union.Item.stall.byte;
	data[index++] = g_provehice_union.arry[11];	//resistance
	data[index++] = g_provehice_union.arry[10];	
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
		data[index++] = g_promotor_union[i].arry[3];	//rpm
		data[index++] = g_promotor_union[i].arry[2];
		data[index++] = g_promotor_union[i].arry[5];	//torque
		data[index++] = g_promotor_union[i].arry[4];
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
	data[index++] = g_profullcell_union.arry[11];	//maxconcent
	data[index++] = g_profullcell_union.arry[10];	
	data[index++] = g_profullcell_union.Item.maxconcent_id;
	data[index++] = g_profullcell_union.arry[13];	//maxpre_val
	data[index++] = g_profullcell_union.arry[12];	
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
	data[index++] = g_proengine_union.arry[1];	//rpm
	data[index++] = g_proengine_union.arry[0];
	data[index++] = g_proengine_union.arry[3];	//consumpt
	data[index++] = g_proengine_union.arry[2];
	
	return index;
}
static uint8 ProPostion(uint8 data[])
{
	uint8 index=0;
	
	data[index++] = (uint8)PRO_POSITION_FLAG;
	//0~8
	data[index++] = g_propostion_union.Item.status.byte;
	data[index++] = g_propostion_union.arry[3];	//longitude
	data[index++] = g_propostion_union.arry[2];
	data[index++] = g_propostion_union.arry[1];
	data[index++] = g_propostion_union.arry[0];
	data[index++] = g_propostion_union.arry[7];	//latitude
	data[index++] = g_propostion_union.arry[6];
	data[index++] = g_propostion_union.arry[5];
	data[index++] = g_propostion_union.arry[4];
	
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
	data[index++] = g_proalarm_union.arry[3];	//flag
	data[index++] = g_proalarm_union.arry[2];
	data[index++] = g_proalarm_union.arry[1];
	data[index++] = g_proalarm_union.arry[0];
	data[index++] = g_proalarm_union.Item.storagenum;
	for(i=0;i < g_proalarm_union.Item.storagenum;i++){
//		data[index++] = (uint8)((g_storage_alr[i]>>24) & 0xFF);	
//		data[index++] = (uint8)((g_storage_alr[i]>>16) & 0xFF);
//		data[index++] = (uint8)((g_storage_alr[i]>> 8) & 0xFF);
//		data[index++] = (uint8)( g_storage_alr[i]      & 0xFF);
		data[index++] = (uint8)((g_storage_alr[0]>>24) & 0xFF);	
		data[index++] = (uint8)((g_storage_alr[0]>>16) & 0xFF);
		data[index++] = (uint8)((g_storage_alr[0]>> 8) & 0xFF);
		data[index++] = (uint8)( g_storage_alr[0]      & 0xFF);
	}
	data[index++] = g_proalarm_union.Item.motornum;
	for(i=0;i < g_proalarm_union.Item.motornum;i++){
//		data[index++] = (uint8)((g_motro_alr[i]>>24) & 0xFF);	
//		data[index++] = (uint8)((g_motro_alr[i]>>16) & 0xFF);
//		data[index++] = (uint8)((g_motro_alr[i]>> 8) & 0xFF);
//		data[index++] = (uint8)( g_motro_alr[i]      & 0xFF);
		data[index++] = (uint8)((g_motro_alr[0]>>24) & 0xFF);	
		data[index++] = (uint8)((g_motro_alr[0]>>16) & 0xFF);
		data[index++] = (uint8)((g_motro_alr[0]>> 8) & 0xFF);
		data[index++] = (uint8)( g_motro_alr[0]      & 0xFF);
	}
	data[index++] = g_proalarm_union.Item.enginenum;
	for(i=0;i < g_proalarm_union.Item.enginenum;i++){
		data[index++] = 0;//(uint8)((g_engine_alr[i]>>24) & 0xFF);	
		data[index++] = 0;//(uint8)((g_engine_alr[i]>>16) & 0xFF);
		data[index++] = 0;//(uint8)((g_engine_alr[i]>> 8) & 0xFF);
		data[index++] = 0;//(uint8)( g_engine_alr[i]      & 0xFF);
	}
	data[index++] = g_proalarm_union.Item.othersnum;
	for(i=0;i < g_proalarm_union.Item.othersnum;i++){
//		data[index++] = (uint8)((g_others_alr[i]>>24) & 0xFF);	
//		data[index++] = (uint8)((g_others_alr[i]>>16) & 0xFF);
//		data[index++] = (uint8)((g_others_alr[i]>> 8) & 0xFF);
//		data[index++] = (uint8)( g_others_alr[i]      & 0xFF);
		data[index++] = (uint8)((g_others_alr[0]>>24) & 0xFF);	
		data[index++] = (uint8)((g_others_alr[0]>>16) & 0xFF);
		data[index++] = (uint8)((g_others_alr[0]>> 8) & 0xFF);
		data[index++] = (uint8)( g_others_alr[0]      & 0xFF);
	}
	
	return index;
}
static uint8 ProBVoltage(uint8 data[])
{
	uint8 index=0,i,j;
	
	data[index++] = (uint8)PRO_BVOLTAGE_FLAG;
	data[index++] = (uint8)PRO_BATTSYS_NUMBER;
	//0~12 *i
	for(i=0;i < PRO_BATTSYS_NUMBER;i++){
		data[index++] = g_provbattsys_union.Item.serial;
		data[index++] = g_provbattsys_union.arry[3];	//voltage
		data[index++] = g_provbattsys_union.arry[2];
		data[index++] = g_provbattsys_union.arry[5];	//current
		data[index++] = g_provbattsys_union.arry[4];
		data[index++] = g_provbattsys_union.arry[7];	//sigbatt_num
		data[index++] = g_provbattsys_union.arry[6];
		data[index++] = g_provbattsys_union.arry[9];	//framebatt_id
		data[index++] = g_provbattsys_union.arry[8];
		data[index++] = g_provbattsys_union.Item.framebatt_num;
		for(j=0; j < g_provbattsys_union.Item.framebatt_num; j++){
			data[index++] = (uint8)(g_sigbatt_val[i][j] >> 8);
			data[index++] = (uint8)(g_sigbatt_val[i][j] & 0xff);
		}
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
		index += g_protbattsys_union.Item.btprobe_num;//С��Խ��
	}
	
	return index;
}

uint16 Pro_RealTime_Data(uint8 data[])
{
	uint16 len;
	uint8  *p;
	
	g_provehice_union.Item.mileage++;// qlj �������������  ����ɾ��	
	
	p = data;
	// qlj ������������ Խ������ �Ƿ�ְ���
	memcpy(p,g_protime_union.arry,6);		//ʱ��
	p += 6;
	
	p += ProVehicle(p);					//01		21
	p += ProMotor(p);						//02		14
//	p += ProFuelCell(p);				//03		
//	p += ProEngine(p);					//04
	p += ProPostion(p);					//05		10
	p += ProExtreme(p);					//06		15
	p += ProAlarm(p);						//07		10
	p += ProBVoltage(p);				//08		204
	p += ProBTemperature(p);		//09		29
	
	len = p - data;
	return len;
}


	//	02 ʵʱ��Ϣ
void ProUpRealFormation(void)	// qlj ���ͳ��ȴ�����
{
	uint8 tx_buf[TMP_BUFF_MAXLEN-25],*p;	//buff����֡ͷβ-25
	uint16 tx_len;
	
	p = tx_buf;		
	
	p += Pro_RealTime_Data(p);	
	
	tx_len = p - tx_buf;
	if(tx_len > (TMP_BUFF_MAXLEN-25)) tx_len = TMP_BUFF_MAXLEN-25;
	ProPacket(tx_buf,tx_len,PRO_UP_REAL_INFO_ID,TRUE);
}
	// 03 ä����������
static void ProUpLsnalFormation(void)	
{
	uint8  tmpdata[LSNAL_PAGE_SIZE];
	uint16 tmplen;
	uint32 basicaddr;

	if(g_pro_struct.tx_one_lsnal_falg == 1)	{//��ä���������ڷ���
		goto RETURN_LAB;
	}
	
	
	if(g_syslsnal_struct.tailindex != g_syslsnal_struct.headindex){
		basicaddr = SYS_LSNAL_SPISTART_ADDR + g_syslsnal_struct.tailindex * LSNAL_PAGE_SIZE;
		if(ExteFlashRead(basicaddr, tmpdata, LSNAL_PAGE_SIZE) == 0){
			goto RETURN_LAB;
		}
		
		tmplen = (tmpdata[1] << 8) + tmpdata[2] + 4;//����У��
		if(tmpdata[0]!=VALID_VAL_2A || tmplen>=512){
		// ���������־���� Խ�� �ص�=head
			g_syslsnal_struct.tailindex = g_syslsnal_struct.headindex;
			ProLsnalHeadTailSave();
			goto RETURN_LAB;
		}		
		if(tmpdata[tmplen] != U8SumCheck(tmpdata, tmplen)){
			goto RETURN_LAB;
		}	
		
		ProPacket(&tmpdata[4], tmplen-4, PRO_UP_LSNAL_INFO_ID,TRUE); 
		g_pro_struct.tx_one_lsnal_falg = 1;
	}
RETURN_LAB:
	return;
}
	// 01 ����
static void ProUpLogin(void)
{
	uint8 tx_buf[231],tx_len;	//buff����֡ͷβ-25
	uint16 login_number;
	
	memcpy(tx_buf,g_protime_union.arry,6);		//ʱ��
	//������ˮ�� 1~65531
	login_number = (g_prologin_union.Item.login_number+1)%MAX65531;
	if(login_number  == 0) g_prologin_union.Item.login_number = MAX65531;	
	else									 g_prologin_union.Item.login_number = login_number;		
	tx_buf[6] = g_prologin_union.arry[1]; 		//���ģʽ����λ��ǰ
	tx_buf[7] = g_prologin_union.arry[0]; 
	//������¼��Ϣ
	memcpy(&tx_buf[8],&g_prologin_union.arry[2],22);		
	//�ɳ�索��ϵͳ���� ��ʱ������
	
	tx_len = 6 + 24;	
	ProPacket(tx_buf,tx_len,PRO_UP_LOGIN_ID,TRUE);//FALSE);	// qlj Ϊʲô����FALSE
}
	// 04 �ǳ�
void ProUpLogout(void)
{
	uint8 tx_buf[231],tx_len;	//buff����֡ͷβ-25
	
	memcpy(tx_buf,g_protime_union.arry,6);		//ʱ��
	//������ˮ�� 1~65531	 g_prologin_union.Item.login_number
	tx_buf[6] = g_prologin_union.arry[1]; 		//���ģʽ����λ��ǰ
	tx_buf[7] = g_prologin_union.arry[0]; 
	
	tx_len = 8;	
	ProPacket(tx_buf,tx_len,PRO_UP_LOGOUT_ID,TRUE);	
}
	// 07 ����	
void ProUpHeartBeat(void)
{
	ProPacket(NULL,0,(uint8)PRO_UP_HEARTBEAT_ID,FALSE);
}
	// 08 ��ƽ̨Уʱ	
void ProUpCheckTime(void)
{
	ProPacket(NULL,0,(uint8)PRO_UP_CHECKTIME_ID,FALSE);
}

///Ӧ��
	//	������ѯ																						
static uint16 ProUpQueryParams(uint8 data[], uint8 len, uint8* tx_buf)	
{
	uint16 tx_len;
	uint8  i,*p;
	p = tx_buf;
	
	if(len == 0){//�޲�������
		goto RETURN_LAB;
	}
		
	*p++ = len; //����������
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
				*p++ = (uint8)PRO_PARA_DOMAIN_ID;//ƽ̨����
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
				*p++ = (uint8)PRO_PARA_HDVERSION_ID;//Ӳ���汾
				memcpy(p,g_propara_union.Item.g_para_HDVers,5);		
				p += 5;
				break;
			}
			case PRO_PARA_SFVERSION_ID:{
				*p++ = (uint8)PRO_PARA_SFVERSION_ID;//����汾
				memcpy(p,g_propara_union.Item.g_para_SFVers,5);	
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
				*p++ = (uint8)PRO_PARA_PDOMAIN_ID;//����ƽ̨����
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
	if(tx_len > 200) tx_len = 200;
	return tx_len;
	
RETURN_LAB:
	return 0;
}

static void ProUpAck(uint8 data[], uint16 len, uint8 flag)
{
	uint8  tx_buf[225];	
	uint16 tx_len;
	
	while(g_sysprivatepara_struct.updata_sengding != 0){
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
	memcpy(tx_buf, data, len);
	tx_buf[3]   = flag;
	ProConstructFrameTail(tx_buf, len);
	ProPacket(tx_buf, len+1, PRO_UP_REAL_INFO_ID, FALSE);
}


/******����ͨѶ����******/

static void ProCtrUpgrade(uint8 data[],uint8 len)
{
	 //��;���ָ���ĳ������û����Ϊ��
	//URL��ַ;���ŵ�����;�����û���;��������;��ַ;�˿�;�����̴���;Ӳ���汾;�̼��汾;���ӵ�����������ʱ��
	uint8 i,j=0,index[PRO_UPGRADE_PARA_NUM];
	
	for(i=0; i < len; i++){
		if(data[i] == ';'){
			index[j++] = i;
		}
	}
	index[j++] = len;
	if(j!=PRO_UPGRADE_PARA_NUM){
		goto RETURN_LAB;
	}
	if(index[0]){
		memcpy(g_proupgread_struct.dial_APN, data, index[0]);
		g_proupgread_struct.dial_APN[j] = 0; //���ռӽ�����
	}
	i = index[0] + 1;//��ʼ�±�
	j = index[1] - i;//����
	if(j){
		memcpy(g_proupgread_struct.user_name, &data[i], j);
		g_proupgread_struct.user_name[j] = 0; //���ռӽ�����
	}				
	i = index[1] + 1;//��ʼ�±�
	j = index[2] - i;//����
	if(j){
		memcpy(g_proupgread_struct.pass_word, &data[i], j);
		g_proupgread_struct.pass_word[j] = 0; //���ռӽ�����
	}				
	i = index[2] + 1;//��ʼ�±�
	j = index[3] - i;//����
	if(j>=4){
		if(j==4){
			g_proupgread_struct.ip_domain[0] = 0;
			g_proupgread_struct.ip_domain[1] = 0;
			memcpy(&g_proupgread_struct.ip_domain[2], &data[i], j);
		}
		else{
			memcpy(g_proupgread_struct.ip_domain, &data[i], j);
		}
	}				
	i = index[3] + 1;//��ʼ�±�
	j = index[4] - i;//����
	if(j==2){
		g_proupgread_struct.port = (data[i]<<8) + data[i+1]; 
	}				
	i = index[4] + 1;//��ʼ�±�
	j = index[5] - i;//����
	if(j==4){
		g_proupgread_struct.terminal_id = (data[i]<<24) +(data[i+1]<<16) +(data[i+2]<<8) + data[i+3]; 
	}				
	i = index[5] + 1;//��ʼ�±�
	j = index[6] - i;//����
	if(j>=4){
		memcpy(g_proupgread_struct.HDVers, &data[i], 5);
	}
	i = index[6] + 1;//��ʼ�±�
	j = index[7] - i;//����
	if(j>=5){
		memcpy(g_proupgread_struct.SFVers, &data[i], 5);
	}	
	i = index[7] + 1;//��ʼ�±�
	j = index[8] - i;//����
	if(j){
		memcpy(g_proupgread_struct.file_path, &data[i], j);
		g_proupgread_struct.file_path[j] = 0; //���ռӽ�����
	}	
	i = index[8] + 1;//��ʼ�±�
	j = index[9] - i;//����
	if(j==2){
		g_proupgread_struct.overtime = (data[i]<<8) + data[i+1]; 
	}
	
	g_proupgread_struct.flag = 1;
	
	data[22] = 0;
	data[23] = 6;
 	ProUpAck(data, 30, PRO_ACK_SUCCEED_FLAG);// qlj Ҫ��ҪACK?
	
/* 	if(res)								// qlj ftp��������д
	{
		res = FtpMain();
		if(res)
		{
			upgrade_res = SUCCESS_ACK;
		}
		
		UdpIpPortInit();
		ProUpUpgradeResult(upgrade_res);///�ϴ���̨,������־
		
		if(res)
		{
			LocalDebug("upgrade_success\r\n",StrLen("upgrade_success\r\n",0),LOCAL_TEST_DEBUG);
			LocalCommFtpState(0,ftp_struct.ftp_file_total_size,ftp_struct.ftp_rx_file_byte_counter);
			SysReset();///�ó�����±�־����λ��
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
	uint8 i,j,num,tx_cmd,rx_cmd,rx_ack;

	rx_cmd = data[PRO_CMD_INDEX];
	rx_ack = data[PRO_ACK_INDEX];
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){//��ѯ�ȽϷ���ָ����Ӧ��ָ��ж�Ӧ���־
		tx_cmd = g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX];
		if((tx_cmd==rx_cmd)&&(g_pro_struct.tx_struct.re_tx_full_flag[i]==TRUE)){
			if(tx_cmd>=PRO_UP_LOGIN_ID && tx_cmd<=PRO_UP_LOGOUT_ID){//��Щ����ʱ��
				num = 0;
				for(j=0;j<6;j++){
					if(data[PRO_DATA_INDEX+j] == g_pro_struct.tx_struct.re_tx_buf[i][PRO_DATA_INDEX+j]){
						num++;
					}
				}
				if(num == 6){																					//ʱ��������У����ȷ
					g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;	//ȡ�����ͻ����־
				}
			}
			else{																										//����ʱ��ľͲ�У����
				g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;		//ȡ�����ͻ����־
			}
//			g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;	//����Ӧ��Դ�ȡ�����ͻ����־
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
	
//	#ifdef PROTOCOL_DEBUG
//		printf("g_sysmiscrun_struct.PAckTim_count = %d\r\n",g_sysmiscrun_struct.PAckTim_count);
//	#endif
	g_sysmiscrun_struct.PAckTim_count = 0xFF;			//��ƽ̨��ʱ��ʱ
	
	switch(rx_cmd){	
		case PRO_UP_LOGIN_ID:{
			g_pro_struct.try_login_statu = 3;
			g_sysmiscrun_struct.NLogTim_count = 0xFFFF;		//	��Ч
			g_pro_struct.login_center_flag = TRUE;
			break;
		}
		case PRO_UP_LOGOUT_ID:{
//			g_pro_struct.try_login_statu = 5;
//			g_pro_struct.login_center_flag  = FALSE;
			break;
		}
		case PRO_UP_CHECKTIME_ID:{
//			g_sysmiscrun_struct.need_check_time_flag = 0x02;			//��ƽ̨Уʱ
			break;
		}
		case PRO_UP_LSNAL_INFO_ID:{
			if(g_sysmiscrun_struct.have_sysAlarm_flag==0 || g_sysmiscrun_struct.have_sysAlarm_flag==3){
				if(g_pro_struct.tx_lsnal_data_flag == FALSE){
					//��ͨä������
					g_syslsnal_struct.tailindex = (g_syslsnal_struct.tailindex+1)%SYS_LSNAL_SPIMAXINDEX;
					ProLsnalHeadTailSave();				
					g_pro_struct.tx_one_lsnal_falg = 0; 
				}
			}
			break;
		}
		default:{
			break;
		}
	}
	
	g_pro_struct.tx_lsnal_data_flag = TRUE;
	
RETURN_LAB:
	return;
}
static void ProDownParaQuiry(uint8 data[],uint8 len)
{
	uint8  tx_buf[225];	//buff����֡ͷβ-25
	uint16 tx_len;
	uint8  tmp_num;
	
	tmp_num = data[PRO_DATA_INDEX+7];
	memcpy(tx_buf, data, PRO_DATA_INDEX);		//��������
	tx_len = ProUpQueryParams(data+PRO_DATA_INDEX+8, tmp_num, &tx_buf[PRO_DATA_INDEX]);
	if(tx_len){
		tx_len += PRO_DATA_INDEX;
		tx_buf[22] = (uint8)(tx_len >> 8);
		tx_buf[23] = (uint8)(tx_len & 0xFF);
		ProUpAck(tx_buf, tx_len, PRO_ACK_SUCCEED_FLAG);		
	}
	else{
		tx_buf[22] = 0;
		tx_buf[23] = 6;
		ProUpAck(tx_buf, tx_len, PRO_ACK_FAILED_FLAG);		
	}
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
				if(tmp>=0 && tmp<=60000)
					g_propara_union.Item.SavePeri = tmp;		
				else 
					goto RETURN_LAB;
				break;
			}
			case PRO_PARA_NORUPPERI_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp>=1 && tmp<=600)
					g_propara_union.Item.NorUpPeri = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_ALRUPPERI_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp>=0 && tmp<=60000)
					g_propara_union.Item.AlrUpPeri = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_DOMAINLEN_ID:{
				tmp = *p++;
				if(tmp <= PRO_DOMAINLEN_MAX)
					g_propara_union.Item.DomainLen = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_DOMAIN_ID:{
/* 				for(tmp=0;tmp < PRO_DOMAINLEN_MAX;tmp++){
					if(*(p+tmp) == 0){// qlj �������������������ַ�����0��β?
						p += (tmp+1);
						break;
					}
					tmp_data[tmp] = *(p+tmp);
				}
				if(tmp != PRO_DOMAINLEN_MAX){
					g_propara_union.Item.DomainLen = tmp;
					memcpy(g_propara_union.Item.Domain, tmp_data, g_propara_union.Item.DomainLen);	
				} */
				
				//����������ǰ�����������ȿ���ֱ����
				if(g_propara_union.Item.DomainLen <= PRO_DOMAINLEN_MAX){	//��ֹԽ��
					memcpy(g_propara_union.Item.Domain, p, g_propara_union.Item.DomainLen);
					p += g_propara_union.Item.DomainLen;
				}
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_PORT_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp>=0 && tmp<=65531)
					g_propara_union.Item.Port = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_HEARTPERI_ID:{
				tmp = *p++;
				if(tmp>=1 && tmp<=240)
					g_propara_union.Item.HeartPeri = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_T_ACK_TIM_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp>=1 && tmp<=600)
					g_propara_union.Item.TAckTim = tmp;		
				else 
					goto RETURN_LAB;			
				break;
			}
			case PRO_PARA_P_ACK_TIM_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp>=1 && tmp<=600)
					g_propara_union.Item.PAckTim = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_N_LOG_TIM_ID:{
				tmp = *p++;
				if(tmp>=1 && tmp<=240)
					g_propara_union.Item.NLogTim = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_PDOMAINLEN_ID:{
				tmp = *p++;
				if(tmp <= PRO_DOMAINLEN_MAX)
					g_propara_union.Item.PDomainLen = tmp;		
				else 
					goto RETURN_LAB;		
				break;
			}
			case PRO_PARA_PDOMAIN_ID:{
/* 				for(tmp=0;tmp < PRO_DOMAINLEN_MAX;tmp++){
					if(*(p+tmp) == 0){// qlj �������������������ַ�����0��β?
						p += (tmp+1);
						break;
					}
					tmp_data[tmp] = *(p+tmp);
				}
				if(tmp != PRO_DOMAINLEN_MAX){
					g_propara_union.Item.PDomainLen = tmp;
					memcpy(g_propara_union.Item.PDomain, tmp_data, g_propara_union.Item.PDomainLen);	
				} */
				
				//����������ǰ�����������ȿ���ֱ����
				if(g_propara_union.Item.PDomainLen <= PRO_DOMAINLEN_MAX){	//��ֹԽ��
					memcpy(g_propara_union.Item.PDomain, p, g_propara_union.Item.PDomainLen);
					p += g_propara_union.Item.PDomainLen;
				}
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_PPORT_ID:{
				tmp = *p++;
				tmp = (tmp<<8) + *p++;
				if(tmp>=0 && tmp<=65531)
					g_propara_union.Item.PPort = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			case PRO_PARA_MONITOR_ID:{
				tmp = *p++;
				if(tmp>=1 && tmp<=2)
					g_propara_union.Item.Monitor = tmp;		
				else 
					goto RETURN_LAB;	
				break;
			}
			
			default:	goto RETURN_LAB;	break;
		}
	}
	data[22] = 0;
	data[23] = 6;
 	ProUpAck(data, 30, PRO_ACK_SUCCEED_FLAG);// qlj Ҫ��ҪACK?
	ProWrite_SysPara();		//����
	SysReset();						//��λ
	return;	
	
RETURN_LAB:
	data[22] = 0;
	data[23] = 6;
 	ProUpAck(data, 30, PRO_ACK_FAILED_FLAG);// qlj Ҫ��ҪACK?
	return;	
}
static void ProDownControl(uint8 data[],uint8 len)	
{
	// qlj Ҫ��ҪӦ��
	uint8  tmp_id;
	uint16 tmp;
	
	tmp = PRO_DATA_INDEX+1 +7;	//25+6+1 ���ٶ����ֽ�
	tmp_id = data[PRO_DATA_INDEX+6];	//ʱ��6 ����1
	switch(tmp_id){
		case PRO_CTR_UPGRADE_ID:{	
			if(len > (tmp+24)){
				ProCtrUpgrade(&data[PRO_DATA_INDEX+7], len-tmp);
			}			
			break;
		}
		case PRO_CTR_OFFTERM_ID:{		// qlj ����������ô����
			break;
		}
		case PRO_CTR_RSTTERM_ID:{	
			//����ä��
			SysReset();						//��λ
			break;
		}
		case PRO_CTR_FACTRST_ID:{		
			//��û��Ҫ�����
			//�ָ���������
			SetPara2FactoryReset();
			ProWrite_SysPara();
			SysReset();						//��λ
			break;
		}
		case PRO_CTR_DISLINKSER_ID:{	
			//��û��Ҫ�����
			//�Ͽ�����	
			break;
		}
		case PRO_CTR_ALRTERM_ID:{		// qlj ��ô��������
			break;
		}
		case PRO_CTR_MONITOR_ID:{	
			g_propara_union.Item.Monitor = 0x01;	//�ó�����־
			// qlj ��ôȡ����־������ʱҪ����Щ������
			break;
		}
		
		default:	 break;
	}
	data[22] = 0;
	data[23] = 6;
 	ProUpAck(data, 30, PRO_ACK_SUCCEED_FLAG);// qlj Ҫ��ҪACK?
}


/*******�������********/
static void ProConstructFrameHead(uint8 data[],uint16 tx_len,uint8 cmd)
{//���ģʽ����λ��ǰ
	uint8 *p = data;

	*p++ = 0x23;									//'#'
	*p++ = 0x23;									//'#'
	*p++ = cmd;										//�����ʶ
	*p++ = PRO_ACK_CMD_FLAG;			//������֡
	memcpy(p,g_VIN,17);
	p += 17;
	*p++ = PRO_DISENCRYT_MODE;
	*p++ = (uint8)(tx_len >> 8);		//���ݵ�Ԫ����
	*p++ = (uint8)(tx_len & 0xFF);	
	//���ݵ�Ԫ
	//У����
	
	//g_pro_struct.tx_seq++;
}
static void ProConstructFrameTail(uint8 data[],uint16 tx_len)
{//���֡β
	data[tx_len] = XorCheck(data,tx_len);	//BCC���У��
}

/******************************************************
���д������
���룺tx_data ������
			tx_len  �����ݳ���
			tx_cmd	����ָ��
			ack_flag		Ӧ���־ 00 ����ҪӦ��  01 ��ҪӦ�� //02��ҪӦ���ұ���Ǳ���ä������
******************************************************/
void ProPacket(uint8 tx_data[],uint16 tx_len,uint8 tx_cmd,uint8 ack_flag)
{
	uint8 i,*p,tmp_data[TMP_BUFF_MAXLEN];
	uint16 tmp_len;
	#ifdef PRO_DEBUG
		char str_ch[10];
		uint8 str_len;
	#endif

	p = tmp_data;
	
 	if(g_sysprivatepara_struct.link_center_flag != 1){//û�е�¼
		goto RETURN_LAB;
	} 
	
	ProConstructFrameHead(p,tx_len,tx_cmd);
	p += PRO_DATA_INDEX;
	MemCpy(p,tx_data,tx_len);
	tmp_len = PRO_DATA_INDEX + tx_len;
	ProConstructFrameTail(tmp_data,tmp_len);
	tmp_len += 1;		//��һ��У����
	
	if(ack_flag == 0)
	{
		if(g_sysprivatepara_struct.updata_sengding == 0){
			#ifdef PRO_DEBUG
				str_len = sprintf(str_ch,"%s ","���У�");
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
			
			g_sysmiscrun_struct.upheart_count 			= 0;		///����������ʱ��0
			g_sysmiscrun_struct.PAckTim_count				= 0;		///ƽ̨��ʱ��ʱ
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
	if(g_sysmiscrun_struct.have_sysAlarm_flag!=0 && g_sysmiscrun_struct.have_sysAlarm_flag!=3){
		//�����򴫱���ä��ʧ�� ��ѹƬ��flash
		if(tx_cmd==PRO_UP_LSNAL_INFO_ID || tx_cmd==PRO_UP_REAL_INFO_ID){
			printf("�������index = %d \r\n",g_provehice_union.Item.mileage);
			ProPutIntoAlarm(tx_data, tx_len, PRO_UP_LSNAL_INFO_ID);	
		}
	}
	else{
		ProPutIntoLsnal(tx_data,tx_len,tx_cmd);//������
	}
RETURN_LAB:
	return;
}
///���ڷ���
void ProPeriodTx(uint16 past_sec)
{
	uint16 i;
	#ifdef PRO_DEBUG
		char str_ch[10];
		uint8 str_len;
		uint16 j;
	#endif
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){										//��ѯ�ط�buff
		if(g_pro_struct.tx_struct.re_tx_sec_counter[i] >= g_pro_struct.tx_struct.re_tx_time[i]){ //�ط�����		
			g_pro_struct.tx_struct.re_tx_sec_counter[i] = 0;
			if(g_pro_struct.tx_struct.re_tx_full_flag[i]){			//������
				if(g_pro_struct.tx_struct.re_tx_counter[i] < 3){  //�ط�С��3��		
					if(g_sysprivatepara_struct.updata_sengding == 0){
						g_pro_struct.tx_struct.re_tx_counter[i]++;			
						#ifdef PRO_DEBUG
							str_len = sprintf(str_ch,"%s ","���У�");
							LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);	
							for(j=0;j<g_pro_struct.tx_struct.re_tx_len[i];j++){
								str_len = sprintf(str_ch,"%02X ",g_pro_struct.tx_struct.re_tx_buf[i][j]);
								LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);
							}
							LocalDebug("\r\n",2,LOCAL_PRO_DEBUG);
						#endif
						
						if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
							g_gprs_data_struct.SendDataLen = g_pro_struct.tx_struct.re_tx_len[i];
							memcpy(GPRStestdata, g_pro_struct.tx_struct.re_tx_buf[i], g_gprs_data_struct.SendDataLen);
							g_sysprivatepara_struct.updata_sengding = 1;				
							g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
						}
						g_sysmiscrun_struct.upheart_count 			= 0;		///����������ʱ��0
						g_sysmiscrun_struct.PAckTim_count				= 0;		///ƽ̨��ʱ��ʱ
					}
					break;
				}
				else{
//					g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
					if(g_pro_struct.tx_struct.re_tx_buf[i][2] == PRO_UP_LOGIN_ID){
						g_pro_struct.tx_struct.re_tx_buf[i][2] = 0;	//�巢��buf
						g_sysmiscrun_struct.NLogTim_count = 0;			//��Ǳ���3�ε�¼ʧ��
					}
					else{	
						g_pro_struct.tx_struct.re_tx_full_flag[i] = FALSE;
						ProPutIntoLsnal(g_pro_struct.tx_struct.re_tx_buf[i]+PRO_DATA_INDEX,
										g_pro_struct.tx_struct.re_tx_len[i]-PRO_DATA_INDEX-1,
										g_pro_struct.tx_struct.re_tx_buf[i][PRO_CMD_INDEX]);
					}
				}
			}
		}
	}
	
	for(i=0;i<PRO_MAX_TX_BUF_ARRAY;i++){//��ѯ�ط�buff���������ݵ�buff�����
		if(g_pro_struct.tx_struct.re_tx_full_flag[i]){
			g_pro_struct.tx_struct.re_tx_sec_counter[i] += past_sec;
		}
	}
	
	if(g_sysmiscrun_struct.have_sysAlarm_flag!=0 && g_sysmiscrun_struct.have_sysAlarm_flag!=3){
		//�����򴫱���ä��ʧ�� �����ȷ���
	}
	else{
			if((g_pro_struct.login_center_flag == TRUE)&&(g_pro_struct.tx_lsnal_data_flag == TRUE)){
			g_pro_struct.tx_lsnal_data_flag = FALSE;
			ProUpLsnalFormation();
		}
	}
	
	if(g_sysprivatepara_struct.link_center_flag == 1){//�Ѿ����ӵ�����ƽ̨
		if(g_pro_struct.try_login_statu == 1){
			g_pro_struct.try_login_statu = 2;
	//		MemCpy(udp_struct.udp_dst_ip,g_pro_struct.comm_para_ip,4);
	//		MemCpy(udp_struct.udp_dst_port,g_pro_struct.comm_para_port,2);
	//		UdpIpPortInit();
			ProUpLogin();
		}
		else if(g_pro_struct.try_login_statu == 2){
			if(g_sysmiscrun_struct.NLogTim_count < 120){//g_propara_union.Item.NLogTim*60){				//ǰ3�ε�¼ʧ�ܣ���ʼ�ص�¼30min��ʱ
				g_sysmiscrun_struct.NLogTim_count += past_sec;
			}
			else if(g_sysmiscrun_struct.NLogTim_count == 0xFFFF){//������һ�β���Ҫ��ʱ
			}
			else {																						// 30min�����ص�¼
				g_pro_struct.try_login_statu = 1;				
				g_sysmiscrun_struct.NLogTim_count = 0xFFFF;				//��Ϊ��Ч
			}
		}
	}
}



/******************************************************
Э�鴦������
����	data	������
			len		�����ݳ���
			colon	��ser���յı�־���ȣ�0x23ǰ����������
******************************************************/
void ProProcess(uint8 data[], uint16 len, uint16 colon)
{
	uint16 tmp_len;
	
	#ifdef PRO_DEBUG
		uint8 str_len;
		char str_ch[20];
		uint16 i;

		str_len = sprintf(str_ch,"%s ","���У�");
		LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);	
		for(i=0;i<len;i++){
			str_len = sprintf(str_ch,"%02X ",data[i]);
			LocalDebug((uint8*)str_ch,str_len,LOCAL_PRO_DEBUG);
		}
		LocalDebug("\r\n",2,LOCAL_PRO_DEBUG);
	#endif

	if(data[0]!='#' || data[1]!='#' ){				//��ʼ������
		goto RETURN_LAB2;
	}
	tmp_len = data[PRO_DATALEN_INDEX];
	tmp_len += data[PRO_DATALEN_INDEX+1];
	tmp_len += 25;
	if(tmp_len >= len){												//���Ȳ��� tmp_lenԤ�ⳤ�ȣ�lenʵ�ʽ��ճ���
		goto RETURN_LAB1;
	}
	if(data[len-1] != XorCheck(data,len)){		//У�鲻��
		goto RETURN_LAB2;
	}
	ReadOverTailIndex(colon+tmp_len);	//�±��ƶ�����������
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
	
RETURN_LAB1:
	return;
	
RETURN_LAB2:	
	ReadOverTailIndex(colon);	//�±��ƶ�����Ч���ݿ�ͷ0x23
	return;
}

