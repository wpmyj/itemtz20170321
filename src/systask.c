
#include "tiza_include.h"




static void TerminalHeart(uint16 past_sec)
{
	if(g_pro_struct.login_center_flag == TRUE){	//已经登录
		if(++g_sysmiscrun_struct.sys_upheart_count > g_propara_union.Item.HeartPeri){	// 1s累加，发送心跳
			ProUpHeartBeat();
			g_sysmiscrun_struct.sys_upheart_count = 0;
		}
	}
			
}


/******************************************************
//应用程序任务
******************************************************/
//周期处理任务
void Period_task(void *pdata)
{
	uint16 count_time,period_time;
	
	while(1){
		OSTimeDlyHMSM(0, 0, 0, 200);	
		count_time = (count_time+1)%60000;
		period_time = count_time;
//		period_time = g_sysmiscrun_struct.sys_time3_200ms_count;
		
		
		g_propostion_union.Item.status.byte = g_gps_struct.gpsinform.subitem.statu.byte;
		g_propostion_union.Item.longitude   = g_gps_struct.gpsinform.subitem.longitude;
		g_propostion_union.Item.latitude    = g_gps_struct.gpsinform.subitem.latitude;
		
		if(period_time%5 == 0){//1s			3级报警
		
		}
		if(period_time%150 == 2){//30		工作参数
			if(g_pro_struct.login_center_flag == TRUE){	//已经登录
				ProUpRealFormation();
			}			
		}
		
		if(period_time%5 == 3){//1		其他
			ProPeriodTx(1);
			TerminalHeart(1);
			
		}
		
	}
}