#define BUSINESS_GLOABL
#define BUSINESS_DEBUG

#include "tiza_include.h"

//测试发送数据
//uint8  GPRStestdata[13] = {0x26,0x50,0x05,0x18,0x60,0x49,0xaa,0x00,0x16,0x55,0xaa,0xbb,	0x62};
//uint8  GPRStestdata[600];
uint32 GPRStestcount = 1;
/***************************模块复位控制函数***********
//模块复位启动函数
******************************************************/
void BGprsResetPara(void)
{
//初始化模块状态
	g_gprs_data_struct.initStatus 		= GPRS_INIT_NO;
	g_gprs_data_struct.netLinkStatus 	= GPRS_NETLINK_NO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.GpsOpenStatus 	= GPS_OPEN_NO;
//设置状态机
	g_gprs_ctr_struct.business = GPRS_INIT;
	g_gprs_ctr_struct.ope = AT_INDEX;
	
	
//业务上逻辑
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	g_business_struct.needopengps = _YES;
	
}


/******************************************************
//模块发送前准备
输出：3：准备完成,其他：失败
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
//断开模块与服务器连接
输出：
******************************************************/
void BusiDisconSer(void)
{
	//关需要连接标志
	g_business_struct.needlinkser = _NO;
	//清状态
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//执行业务
	g_gprs_ctr_struct.business 				= GPRS_DISCONNECT_SERVER;
}
/******************************************************
//启用模块与服务器连接
输出：
******************************************************/
void BusiConSer(void)
{
	//开需要连接标志
	g_business_struct.usemodule		= _YES;
	g_business_struct.needlinkser = _YES;
	//清状态
	g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_ZERO;
	g_gprs_data_struct.MserLinkStatus = GPRS_SERLINK_NO;
	//执行业务
	g_gprs_ctr_struct.business 				= GPRS_NULL;
}
/******************************************************
//关闭GPS数据
输出：
******************************************************/
void BusiDisconGps(void)
{
	//关需要连接标志
	g_business_struct.needopengps = _NO;
	//清状态
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//执行业务
	g_gprs_ctr_struct.business 			 = GPS_CLOSE_NET;
}
/******************************************************
//开启GPS数据
输出：
******************************************************/
void BusiConGps(void)
{
	//开需要连接标志
	g_business_struct.usemodule		= _YES;
	g_business_struct.needopengps = _YES;
	//清状态
	g_gprs_data_struct.GpsOpenStatus = GPS_OPEN_NO;
	//执行业务
	g_gprs_ctr_struct.business 			 = GPS_OPEN_NET;
}
/******************************************************
//复位模块
输出：
******************************************************/
void BusiResetModule(void)
{
	//关启用模块标志
	g_business_struct.usemodule = _NO;
	//重启模块电源	L218Reset();
	g_gprs_ctr_struct.business 	= GPRS_RESET;
	ModlueCalledProcess();
	//重置参数
	BGprsResetPara();
}


/******************************************************
//模块执行任务调用
输出：
******************************************************/
void ExecuteModuleTask(void)
{
		if(g_business_struct.usemodule == _NO){
			return;
		}
		///模块调度函数
		ModlueCalledProcess();
			
		if(g_gprs_data_struct.initStatus != GPRS_INIT_OK){	
			g_sysprivatepara_struct.link_center_flag = 0;			
			return;
		}
		g_sysprivatepara_struct.link_center_flag = 1;		

		///           不启用GPS               或               GPS正确打开 
		if(g_business_struct.needopengps==_NO || g_gprs_data_struct.GpsOpenStatus==GPS_OPEN_OK)
		{	//准备使用GPRS发送数据									
				switch(g_gprs_data_struct.sendDataStatus)
				{
					case GPRS_SENDDATA_ZERO:{
						if(BusiSendReady() == 3){
							g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_IDLE;
						}
						break;
					}
					case GPRS_SENDDATA_IDLE:{//空闲或发送成功
						if(g_pro_struct.try_login_statu == 0){		//置登录标志，只一次
							g_pro_struct.try_login_statu = 1;
						}
						g_gprs_ctr_struct.business = GPRS_NULL;
						g_sysprivatepara_struct.updata_sengding = 0;
						break;
					}
					case GPRS_SENDDATA_OUT:{//有数据要发送
						g_gprs_ctr_struct.business = GPRS_SEND_DATA;
						break;
					}
					case GPRS_SENDDATA_BUSY:{//阻塞或发送失败
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
		///                   启用GPS               且               GPS尚未成功打开 
		else if(g_business_struct.needopengps==_YES && g_gprs_data_struct.GpsOpenStatus!=GPS_OPEN_OK)
		{
				g_gprs_ctr_struct.business = GPS_OPEN_NET;
				g_gprs_ctr_struct.ope = AT_GETGPS_INDEX;
		}
}


/******************************************************
//L218任务
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


//////////////////////测试使用///////////////////////////////////////////
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
{//计算PTMK校验用
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




















