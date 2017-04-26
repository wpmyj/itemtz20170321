
/*

��Ƶ 72MHz

ע��protocol�����ڴ������ı��Ա˳������⣨�ϴ�ʱ���ܶ�˳����Ҫ��


*/
#include "includes.h"
#include "tiza_include.h"


 

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			11 //��ʼ��������ȼ�����Ϊ���
#define LED0_TASK_PRIO       			7 
#define L218_TASK_PRIO      			5 
#define PERIOD_TASK_PRIO      		6 
#define TEST_TASK_PRIO      			10 
//���������ջ��С
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		128
#define L218_STK_SIZE  						1024
#define PERIOD_STK_SIZE  					512
#define TEST_STK_SIZE  						256
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK L218_TASK_STK[L218_STK_SIZE];
OS_STK PERIOD_TASK_STK[PERIOD_STK_SIZE];
OS_STK TEST_TASK_STK[TEST_STK_SIZE];
//������
void start_task(void *pdata);	
void led0_task(void *pdata);
void L218_task(void *pdata);
void Period_task(void *pdata);
void Test_task(void *pdata);



/******************************************************
ϵͳһЩģ��ĳ�ʼ��
******************************************************/
void System_Mode_Init(void)
{
	//ʱ���ʼ��
	g_protime_union.Item.year 		= 17;
	g_protime_union.Item.month 		= 1;
	g_protime_union.Item.day 			= 1;
	g_protime_union.Item.hour 		= 1;
	g_protime_union.Item.minute 	= 1;
	g_protime_union.Item.second 	= 1;
	
	g_sysmiscrun_struct.have_sysAlarm_count  = 0;											///ϵͳ���ֱ�������ʱ
	g_sysmiscrun_struct.save_sysAlarm_numb   = SYS_SAVEALARM_NUMB;		///δ���ֱ���ʱ ����INTFLAH�ļ�¼����  
	g_sysmiscrun_struct.assist_gps_flag      = 2;
	g_provbattsys_union.Item.sigbatt_num   	 = PRO_SIGBATT_MAXNUMBER;// 0;//
	//g_provbattsys_union.Item.framebatt_num   = 0;//PRO_SIGBATT_MAXNUMBER;
	g_protbattsys_union.Item.btprobe_num		 = PRO_BTPROBE_MAXNUMBER;// 0;//
	memset(can_struct.rx_can_buf_flag, 0xFF, CAN_RX_ID_NUM);
	
	g_proupgread_struct.ip_domain[2] = 202;
	g_proupgread_struct.ip_domain[3] = 102;
	g_proupgread_struct.ip_domain[4] = 90;
	g_proupgread_struct.ip_domain[5] = 179;
	g_proupgread_struct.port         = 21;
	memcpy(g_proupgread_struct.user_name,"Vehicle",7);
	g_proupgread_struct.user_name[7] = 0;
	memcpy(g_proupgread_struct.pass_word,"Vehicle#*",9);
	g_proupgread_struct.pass_word[9] = 0;
	memcpy(g_proupgread_struct.file_path,"/TIZA_FTP.bin",13);
	g_proupgread_struct.file_path[13] = 0;
	
	GpioInit();
//	IwdgInit();	
	PvdInit();
	FeedWtd();
	UsartInit(LOCAL_USART,LOCAL_USART_BPR,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	UsartInit(GPS_USART  ,GPS_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	
	FeedWtd();
	ExteFlashInit();
	RtcInit();
//	AdcInit();
	DmaInit();
//	TIM3_Int_Init(1999,7199);//10Khz�ļ���Ƶ�ʣ�������2000Ϊ200ms  
	FeedWtd();
	LocalUartFixedLenSend((uint8*)"Device start...\r\n",StrLen((uint8*)"Device start...\r\n",0));
	__NOP();__NOP();__NOP();__NOP();
}

/******************************************************
Main����
******************************************************/
int main(void)
{	
	delay_init();	    //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 FeedWtd();
	
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	  	 
}
	

/******************************************************
//��ʼ����
******************************************************/
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;	
	pdata = pdata; 
	
	System_Mode_Init(); 
	
	OS_ENTER_CRITICAL();						//�����ٽ���(�޷����жϴ��)    
	//-------------- ����ָ��	---------------------��ջ--------------------------------------------���ȼ�     
#if 1		//LED0����
	OSTaskCreate(led0_task,			 	(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],						LED0_TASK_PRIO);	
#endif	
#if 1		//L218����
	OSTaskCreate(L218_task,				(void *)0,(OS_STK*)&L218_TASK_STK[L218_STK_SIZE-1],						L218_TASK_PRIO);
#endif
#if 1		//���ڴ�������
	OSTaskCreate(Period_task,			(void *)0,(OS_STK*)&PERIOD_TASK_STK[PERIOD_STK_SIZE-1],				PERIOD_TASK_PRIO);
#endif
#if 1 	//��������	
	OSTaskCreate(Test_task,				(void *)0,(OS_STK*)&TEST_TASK_STK[TEST_STK_SIZE-1],				TEST_TASK_PRIO);
#endif
	
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();							//�˳��ٽ���(���Ա��жϴ��)
}






//LED0����
void led0_task(void *pdata)
{	 	
	uint32 ftpflshadd;
	
	while(1)
	{
//		ON_WORK_LED();
//		OFF_GPS_LED();
//		CPL_ERR_LED();
		OSTimeDlyHMSM(0, 0, 1, 0);		
		FeedWtd();

	
		if(g_propostion_union.Item.status.bit.B0 ==1){
			OFF_GPS_LED();	//δ��λ
		}
		else{
			ON_GPS_LED();
		}
		if(g_pro_struct.login_center_flag == TRUE){	//�Ѿ�����
			ON_WORK_LED();
		}
		else{
			OFF_WORK_LED();	//�������ӱ�־
		}
		if(g_pro_struct.try_login_statu == 3){	//�Ѿ���¼	
			ON_ERR_LED();
		}
		else{
			OFF_ERR_LED();	//�������ӱ�־
		}
		
		
		
//		OFF_WORK_LED();
//		ON_GPS_LED();
		OSTimeDlyHMSM(0, 0, 1, 0);
		FeedWtd();
		
	};
}







