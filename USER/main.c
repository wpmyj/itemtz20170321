
/*

主频 72MHz




*/
//-#include "includes.h"
#include "tiza_include.h"


 

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			11 //开始任务的优先级设置为最低
#define LED0_TASK_PRIO       			7 
#define LOCAL_UART_TASK_PRIO      6 
#define L218_TASK_PRIO      			8 
#define PERIOD_TASK_PRIO      		9 
#define TEST_TASK_PRIO      			10 
//设置任务堆栈大小
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		128
#define LOCAL_UART_STK_SIZE  			512
#define L218_STK_SIZE  						512
#define PERIOD_STK_SIZE  					512
#define TEST_STK_SIZE  						256
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK LOCAL_UART_TASK_STK[LOCAL_UART_STK_SIZE];
OS_STK L218_TASK_STK[L218_STK_SIZE];
OS_STK PERIOD_TASK_STK[PERIOD_STK_SIZE];
OS_STK TEST_TASK_STK[TEST_STK_SIZE];
//任务函数
void start_task(void *pdata);	
void led0_task(void *pdata);
void local_uart_task(void *pdata);
void L218_task(void *pdata);
void Period_task(void *pdata);
void Test_task(void *pdata);



/******************************************************
系统一些模块的初始化
******************************************************/
void System_Mode_Init(void)
{
	GpioInit();
//	IwdgInit();	
	PvdInit();
	FeedWtd();
	UsartInit(LOCAL_USART,LOCAL_USART_BPR,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	UsartInit(GPS_USART  ,GPS_USART_BPR  ,USART_DATA_8B,USART_STOPBITS_1,USART_PARITY_NO);
	
	FeedWtd();
	ExteFlashInit();
	RtcInit();
	AdcInit();
	DmaInit();
//	TIM3_Int_Init(1999,7199);//10Khz的计数频率，计数到2000为200ms  
	FeedWtd();
	LocalUartFixedLenSend((uint8*)"Device start...\r\n",StrLen((uint8*)"Device start...\r\n",0));
	__NOP();__NOP();__NOP();__NOP();
}

/******************************************************
Main函数
******************************************************/
int main(void)
{	
	delay_init();	    //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	  	 
}
	

/******************************************************
//开始任务
******************************************************/
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;	
	pdata = pdata; 
	
	System_Mode_Init(); 
	
	OS_ENTER_CRITICAL();						//进入临界区(无法被中断打断)    
	//-------------- 任务指针	---------------------堆栈--------------------------------------------优先级     
#if 1		//LED0任务
	OSTaskCreate(led0_task,			 	(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],						LED0_TASK_PRIO);	
#endif	
#if 0		//LOCAL_UART任务
	OSTaskCreate(local_uart_task,	(void *)0,(OS_STK*)&LOCAL_UART_TASK_STK[LOCAL_UART_STK_SIZE-1],LOCAL_UART_TASK_PRIO);
#endif
#if 1		//L218任务
	OSTaskCreate(L218_task,				(void *)0,(OS_STK*)&L218_TASK_STK[L218_STK_SIZE-1],						L218_TASK_PRIO);
#endif
#if 1		//周期处理任务
	OSTaskCreate(Period_task,			(void *)0,(OS_STK*)&PERIOD_TASK_STK[PERIOD_STK_SIZE-1],				PERIOD_TASK_PRIO);
#endif
#if 1 	//测试任务	
	OSTaskCreate(Test_task,				(void *)0,(OS_STK*)&TEST_TASK_STK[TEST_STK_SIZE-1],				TEST_TASK_PRIO);
#endif
	
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();							//退出临界区(可以被中断打断)
}






//LED0任务
void led0_task(void *pdata)
{	 	
	
	while(1)
	{
		ON_WORK_LED();
		OFF_GPS_LED();
		OSTimeDlyHMSM(0, 0, 1, 0);		
		FeedWtd();
		
		OFF_WORK_LED();
		ON_GPS_LED();
		OSTimeDlyHMSM(0, 0, 1, 0);
		FeedWtd();
		
		printf("\r\n g_pro_struct.over3_relogin_time = %d \r\n",g_pro_struct.over3_relogin_time);
		printf("\r\n g_pro_struct.try_login_statu = %d \r\n",g_pro_struct.try_login_statu);
	};
}

//LOCAL_UART任务
void local_uart_task(void *pdata)
{	  
	uint8 i=0, count=0;//, data[256];	//最多处理256个数据
		
	while(1)
	{
		if(i < 5){
			OSTimeDlyHMSM(0, 0, 3, 0);
			
			if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
				g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
				GPRStestfun();
//				i++;
			}
		}
		else{
			BusiDisconSer();
			printf("\r\n ===========================================BusiDisconSer\r\n");
			OSTimeDlyHMSM(0, 2, 0, 0);
			BusiConSer();
			printf("\r\n ===========================================BusiConSer\r\n");
			for(count=0;count<6;count++){
				OSTimeDlyHMSM(0, 0, 20, 0);
				if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
					g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
				}
			}
			//OSTimeDlyHMSM(0, 2, 0, 0);
			BusiDisconGps();
			printf("\r\n ===========================================BusiDisconGps\r\n");
			for(count=0;count<6;count++){
				OSTimeDlyHMSM(0, 0, 20, 0);
				if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
					g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
				}
			}
			//OSTimeDlyHMSM(0, 2, 0, 0);
			BusiConGps();
			printf("\r\n ===========================================BusiConGps\r\n");
			for(count=0;count<6;count++){
				OSTimeDlyHMSM(0, 0, 20, 0);
				if(g_gprs_data_struct.sendDataStatus == GPRS_SENDDATA_IDLE){
					g_gprs_data_struct.sendDataStatus = GPRS_SENDDATA_OUT;
				}
			}
			//OSTimeDlyHMSM(0, 2, 0, 0);
			BusiResetModule();		
			printf("\r\n ===========================================BusiResetModule\r\n");
				
			i = 0;
		}
		

		
	}
}








