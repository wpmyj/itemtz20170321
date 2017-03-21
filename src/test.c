#define TEST_STRUCT_GLOBAL

#include "tiza_include.h"



//#define TEST_LOGINOUT		

uint8 test_loginout_count =0;





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
		
		
	}
}