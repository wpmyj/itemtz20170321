#define BUSINESS_GLOABL
#define BUSINESS_DEBUG

#include "tiza_include.h"

//���Է�������
//uint8  GPRStestdata[13] = {0x26,0x50,0x05,0x18,0x60,0x49,0xaa,0x00,0x16,0x55,0xaa,0xbb,	0x62};
//uint8  GPRStestdata[600];
uint32 GPRStestcount = 1;
/***************************ģ�鸴λ���ƺ���***********
//ģ�鸴λ��������
******************************************************/
void BGprsResetPara(void)
{
//��ʼ��ģ��״̬
	g_gprs_data_struct.initStatus 		= GPRS_INIT_NO;
	g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;
//����״̬��
	g_gprs_ctr_struct.business = GPRS_INIT;
	g_gprs_ctr_struct.ope = AT_INDEX;
	
	
//ҵ�����߼�
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	g_business_struct.needopengps = _YES;
	
}


/******************************************************
//ģ�鷢��ǰ׼��
�����3��׼�����,������ʧ��
******************************************************/
static uint8 BusiSendReady(void)
{
	uint8 step = 0;
	
	if(g_business_struct.needlinkser == _NO){
		return step;
	}
	
	switch(step)
	{
		case 0:{
			if(g_gprs_data_struct.initStatus != GPRS_INIT_OK){
				g_gprs_ctr_struct.business = GPRS_INIT;
				g_gprs_ctr_struct.ope = AT_INDEX;
				break;
			}
			else
				step = 1;
		}
		case 1:{
			if(g_gprs_data_struct.netLinkStatus != GPRS_NETLINK_OK){
				g_gprs_ctr_struct.business = GPRS_CONNECT_NET;
				g_gprs_ctr_struct.ope = AT_NETCLOSE_INDEX;//AT_CIPMUX_INDEX;//
				break;
			}
			else
				step = 2;
		}
		case 2:{
			if(g_gprs_data_struct.MserLinkStatus != GPRS_SERLINK_OK){
				g_gprs_ctr_struct.business = GPRS_CONNECT_SERVER;
				break;
			}
			else
				step = 3;
		}
		default: break;
	}
	return step;
}

/******************************************************
//�Ͽ�ģ�������������
�����
******************************************************/
void BusiDisconSer(void)
{
	//����Ҫ���ӱ�־
	g_business_struct.needlinkser = _NO;
	//��״̬
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 				= GPRS_DISCONNECT_SERVER;
}
/******************************************************
//����ģ�������������
�����
******************************************************/
void BusiConSer(void)
{
	//����Ҫ���ӱ�־
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	//��״̬
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 				= GPRS_NULL;
}
/******************************************************
//�ر�GPS����
�����
******************************************************/
void BusiDisconGps(void)
{
	//����Ҫ���ӱ�־
	g_business_struct.needopengps = _NO;
	//��״̬
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 			 = GPS_CLOSE_NET;
}
/******************************************************
//����GPS����
�����
******************************************************/
void BusiConGps(void)
{
	//����Ҫ���ӱ�־
	g_business_struct.usemodule		= _YES;
	g_business_struct.needopengps = _YES;
	//��״̬
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//ִ��ҵ��
	g_gprs_ctr_struct.business 			 = GPS_OPEN_NET;
}
/******************************************************
//��λģ��
�����
******************************************************/
void BusiResetModule(void)
{
	//������ģ���־
	g_business_struct.usemodule = _NO;
	//����ģ���Դ	L218Reset();
	g_gprs_ctr_struct.business 	= GPRS_RESET;
	ModlueCalledProcess();
	//���ò���
	BGprsResetPara();
}


/******************************************************
//ģ��ִ���������
�����
******************************************************/
void ExecuteModuleTask(void)
{
		if(g_business_struct.usemodule == _NO){
			return;
		}
		///ģ����Ⱥ���
		ModlueCalledProcess();
			
		if(g_gprs_data_struct.initStatus != GPRS_INIT_OK){	
			g_sysprivatepara_struct.link_center_flag = 0;			
			return;
		}
		g_sysprivatepara_struct.link_center_flag = 1;		

		///           ������GPS               ��               GPS��ȷ�� 
		if(g_business_struct.needopengps==_NO || g_gprs_data_struct.GpsOpenStatus==GPS_OPEN_OK)
		{	//׼��ʹ��GPRS��������									
				switch(g_gprs_data_struct.sendDataStatus)
				{
					case GPRS_SENDDATA_ZERO:{
						if(BusiSendReady() == 3){
							g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
						}
						break;
					}
					case GPRS_SENDDATA_IDLE:{//���л��ͳɹ�
						if(g_pro_struct.try_login_statu == 0){		//�õ�¼��־��ֻһ��
							g_pro_struct.try_login_statu = 1;
						}
						g_gprs_ctr_struct.business = GPRS_NULL;
						g_sysprivatepara_struct.updata_sengding = 0;
						break;
					}
					case GPRS_SENDDATA_OUT:{//������Ҫ����
						g_gprs_ctr_struct.business = GPRS_SEND_DATA;
						break;
					}
					case GPRS_SENDDATA_BUSY:{//��������ʧ��
						#ifdef BUSINESS_DEBUG
							printf("\r\n L218 send data fail,we will reset l218 module\r\n");	
						#endif
						BusiResetModule();
						g_sysprivatepara_struct.updata_sengding = 0;
						break;
					}
					default:					break;
				}
 		}
		///                   ����GPS               ��               GPS��δ�ɹ��� 
		else if(g_business_struct.needopengps==_YES && g_gprs_data_struct.GpsOpenStatus!=GPS_OPEN_OK)
		{
				g_gprs_ctr_struct.business = GPS_OPEN_NET;
				g_gprs_ctr_struct.ope = AT_GETGPS_INDEX;
		}
}


/******************************************************
//L218����
******************************************************/
void L218_task(void *pdata)
{	 
	uint8 i =0;
	
	L218PowerInit();	
	BGprsResetPara();
	#ifdef L218_DEBUG
	printf("\r\n##### L218 POWER OK #####\r\n");	
	#endif	
	
	GPRStestdata[i++] = 0x26;
	GPRStestdata[i++] = 0x50;
	GPRStestdata[i++] = 0x05;
	GPRStestdata[i++] = 0x18;
	GPRStestdata[i++] = 0x60;
	GPRStestdata[i++] = 0x44;//0x49;
	GPRStestdata[i++] = 0xaa;
	GPRStestdata[i++] = 0x00;
	GPRStestdata[i++] = 0x16;
	GPRStestdata[31]  = 0x62;
	
	g_gprs_data_struct.SendDataLen=32;//8;
  g_gprs_data_struct.SendData=(uint8 *)GPRStestdata;
	g_gprs_data_struct.IP[0] = 218;
	g_gprs_data_struct.IP[1] = 94;
	g_gprs_data_struct.IP[2] = 153;
	g_gprs_data_struct.IP[3] = 146;
	g_gprs_data_struct.Port = 9904;//27055;//
	ProParaInit();
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 2, 0);
		
		ExecuteModuleTask();		
	}
}


//////////////////////����ʹ��///////////////////////////////////////////
void GPRStestfun(void)
{
	uint32 tmp;
	uint8  h,l;
	union08 byte;
	
	if(++GPRStestcount > 999999){
		GPRStestcount = 0;
	}
	tmp = GPRStestcount;
	l = tmp % 10;
	tmp =tmp / 10;
	h = tmp % 10;
	tmp =tmp / 10;
	GPRStestdata[11] = (h<<4) | l;
	l = tmp % 10;
	tmp =tmp / 10;
	h = tmp % 10;
	tmp =tmp / 10;
	GPRStestdata[10] = (h<<4) | l;
	l = tmp % 10;
	tmp =tmp / 10;
	h = tmp % 10;
	tmp =tmp / 10;
	GPRStestdata[9] = (h<<4) | l;
	
//	MemCpy(&GPRStestdata[12],g_gps_struct.gpsinform.array,19);
	GPRStestdata[12] = 0XFA;
	GPRStestdata[13] = 0XFB;
	GPRStestdata[14] = 0XFC;
	GPRStestdata[15] = 0XFD;
	tmp = g_gps_struct.gpsinform.subitem.latitude;
	byte.half.H = tmp / 10000000;
	tmp 				= tmp % 10000000;
	byte.half.L = tmp / 1000000;
	tmp         = tmp % 1000000;	
	GPRStestdata[16] = byte.byte;
	byte.half.H = tmp / 100000;
	tmp 				= tmp % 100000;
	byte.half.L = tmp / 10000;
	tmp         = tmp % 10000;	
	GPRStestdata[17] = byte.byte;
	byte.half.H = tmp / 1000;
	tmp 				= tmp % 1000;
	byte.half.L = tmp / 100;
	tmp         = tmp % 100;	
	GPRStestdata[18] = byte.byte;
	byte.half.H = tmp / 10;
	byte.half.L = tmp % 10;
	GPRStestdata[19] = byte.byte;
	
	GPRStestdata[20] = 0XFA;
	GPRStestdata[21] = 0XFB;
	GPRStestdata[22] = 0XFC;
	GPRStestdata[23] = 0XFD;
	tmp = g_gps_struct.gpsinform.subitem.longitude;
	byte.half.H = tmp / 100000000;
	tmp 				= tmp % 100000000;
	byte.half.L = tmp / 10000000;
	tmp         = tmp % 10000000;	
	GPRStestdata[24] = byte.byte;
	byte.half.H = tmp / 1000000;
	tmp 				= tmp % 1000000;
	byte.half.L = tmp / 100000;
	tmp         = tmp % 100000;	
	GPRStestdata[25] = byte.byte;
	byte.half.H = tmp / 10000;
	tmp 				= tmp % 10000;
	byte.half.L = tmp / 1000;
	tmp         = tmp % 1000;	
	GPRStestdata[26] = byte.byte;
	byte.half.H = tmp / 100;
	tmp 				= tmp % 100;
	byte.half.L = tmp / 10;
	GPRStestdata[27] = byte.byte;
	
	GPRStestdata[28] = 0XFE;
	GPRStestdata[29] = 0XFF;
	GPRStestdata[30] = g_gps_struct.gpsinform.subitem.statu.byte;

}
#if 0
uint8 PTMK_CHECKSUM_fun(void)
{//����PTMKУ����
	uint8 ptmk_str[] = {"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"};
	uint8 checksum=0,len,i;
	
	len = StrLen((const uint8 *)ptmk_str,0);
	for(i=0;i<len;i++){
		checksum ^= ptmk_str[i]; 
	}  
 
	printf("\r\nPTMK CHECK SUM IS:%.2X \r\n",checksum);
	return checksum;
}
#endif




















