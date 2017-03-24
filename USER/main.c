
/*

��Ƶ 72MHz




*/
//-#include "includes.h"
#include "tiza_include.h"


 

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			11 //��ʼ��������ȼ�����Ϊ���
#define LED0_TASK_PRIO       			7 
#define LOCAL_UART_TASK_PRIO      6 
#define L218_TASK_PRIO      			8 
#define PERIOD_TASK_PRIO      		9 
#define TEST_TASK_PRIO      			10 
//���������ջ��С
#define START_STK_SIZE  					64
#define LED0_STK_SIZE  		    		128
#define LOCAL_UART_STK_SIZE  			512
#define L218_STK_SIZE  						512
#define PERIOD_STK_SIZE  					512
#define TEST_STK_SIZE  						256
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
OS_STK LOCAL_UART_TASK_STK[LOCAL_UART_STK_SIZE];
OS_STK L218_TASK_STK[L218_STK_SIZE];
OS_STK PERIOD_TASK_STK[PERIOD_STK_SIZE];
OS_STK TEST_TASK_STK[TEST_STK_SIZE];
//������
void start_task(void *pdata);	
void led0_task(void *pdata);
void local_uart_task(void *pdata);
void L218_task(void *pdata);
void Period_task(void *pdata);
void Test_task(void *pdata);



/******************************************************
ϵͳһЩģ��ĳ�ʼ��
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
#if 0		//LOCAL_UART����
	OSTaskCreate(local_uart_task,	(void *)0,(OS_STK*)&LOCAL_UART_TASK_STK[LOCAL_UART_STK_SIZE-1],LOCAL_UART_TASK_PRIO);
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

//LOCAL_UART����
void local_uart_task(void *pdata)
{	  
	uint8 i=0, count=0;//, data[256];	//��ദ��256������
		
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








