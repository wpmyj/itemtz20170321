#define DRIVER_GLOBAL

#include "tiza_include.h"


/******************************************************
GPIO��ʼ������
******************************************************/
void GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/***	 
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOF);
***/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD  | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
///�������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;///GPS�ƣ��ͣ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;///��POWER��,�ͣ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//ע��IwdgInit�����ж�IO���³�ʼ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	///WDIι��
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	///WDIι��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;///485232_EN��485/232��Դ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;///485_EN��485ͨ�ŷ��򣬿�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
//	//������tiza_l218.c->L218PowerInit�ж���
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; ///POWER_ON/OFF,4V��Դ������� 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;///POWERKEY_CONNECT��ģ��ػ���������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;///5V���ƣ�CAN�������س�繲��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
///��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;///CHECK_POW 1��ʾ�е�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	

	OFF_GPS_LED();
	ON_485232_PWR();
	ON_CAN_PWR();
	
//	OFF_GPRS_PWR();///�ػ�
//	OSTimeDlyHMSM(0, 0, 1, 0);
//	ON_GPRS_PWR();
//	OSTimeDlyHMSM(0, 0, 1, 0);
//	HIGH_GPRS_IGT();
//	UsartGprsDeInit();

}

/******************************************************
WATCHDOG��ʼ������

WATCHDOG FEED DOG
******************************************************/
void IwdgInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	///WDIι��
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	///WDIι��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(2000);///1.6S
  IWDG_ReloadCounter();
  IWDG_Enable();	
}
void FeedWtd(void)
{
	IWDG_ReloadCounter();
	FEED_WDT();	
}

/******************************************************
ADC��ʼ������

******************************************************/
uint16 adc_conv_buf[2];
uint16 adc_result[2];

void AdcInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
///																BAT_ADC				POWER_C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;										///����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;													///ɨ��ģʽ
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;										///������ת�����ֶ�ʹ��ת��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;										///����ת����ʹ�ܺ��Զ�ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		///���������ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								///�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;																///����ͨ��ת�����г���(2·)
	ADC_Init(ADC1, &ADC_InitStructure);

///	ADC_TempSensorVrefintCmd(ENABLE);			///ADC�����¶ȴ�����ʹ��

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_239Cycles5);

	ADC_DMACmd(ADC1, ENABLE);											/// ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
	ADC_Cmd(ADC1, ENABLE);												///����ADC

	ADC_ResetCalibration(ADC1);										///��ʼ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));		///�ȴ���ʼ�����
	ADC_StartCalibration(ADC1);										///��ʼ��У׼
	while(ADC_GetCalibrationStatus(ADC1));				///�ȴ�У׼���
	
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1��ʼת��
}

/******************************************************
DMA��ʼ������

******************************************************/
void DmaInit(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
   ///����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);		///DMA1  1ͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);//0x4001244C);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc_conv_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    ///������ã�ʹ����1�������AD_Value[0]������2�������AD_Value[1]..
    DMA_InitStructure.DMA_BufferSize = 2;	//��·ADC
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    ///ѭ��ģʽ������Bufferд�����Զ��ص���ʼ��ַ��ʼ����
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    ///������ɺ�����DMAͨ��
    DMA_Cmd(DMA1_Channel1, ENABLE);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);  
}

void SysClkConfigStop(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_HSEConfig(RCC_HSE_ON); 														/*HSESʹ��*/  
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 					/*�ȴ�*/
	if(HSEStartUpStatus == SUCCESS) 
	{ 
		RCC_PLLCmd(ENABLE);																	/*ʹ��*/
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)== RESET); 	/*�ȴ�PLL��Ч*/      
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);					/*��PLL��Ϊϵͳʱ��*/
		while(RCC_GetSYSCLKSource() != 0x08);								/*�ȴ�*/
	} 
}
void RtcConfiguration(void)
{
	uint16 i = 800;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();

	RCC_LSEConfig(RCC_LSE_ON);

	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){
		if(i>0){
			i--;
			FeedWtd();
			OSTimeDlyHMSM(0, 0, 0, 200);
//			LongTimeDly(2000);
		}
		else{
			break;
		}
	}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask();
}
void RtcSetCalendarTime(void)
{
	struct tm d_t;
	time_t d_t_sec;
	
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	
	d_t.tm_year = 116;//100 + sys_work_para_struct.date_time[0];
	d_t.tm_mon 	= 1;//sys_work_para_struct.date_time[1] - 1;
	d_t.tm_mday = 1;//sys_work_para_struct.date_time[2];
	d_t.tm_hour = 1;//sys_work_para_struct.date_time[3];
	d_t.tm_min 	= 1;//sys_work_para_struct.date_time[4];
	d_t.tm_sec 	= 1;//sys_work_para_struct.date_time[5];

	d_t_sec = mktime(&d_t);
	
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)d_t_sec);
	RTC_WaitForLastTask();
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}
void RtcInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RtcConfiguration();
	RtcSetCalendarTime();

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 11;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
}

void PvdInit(void)///��ѹ�ж�
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	///ռ�����ȼ��������ȼ��ɴ�ϵ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9); 											/// �趨��ط�ֵ
	PWR_PVDCmd(ENABLE); 																				/// ʹ��PVD 
	EXTI_StructInit(&EXTI_InitStructure); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line16; 								/// PVD���ӵ��ж���16��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 				/// ʹ���ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;			/// ��ѹ���ڷ�ֵʱ�����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 									/// ʹ���ж���
	EXTI_Init(&EXTI_InitStructure); 														/// ��ʼ���жϿ�����
}


void RtcGetCalendarTime(uint8 date_time[])
{
	time_t t_s;
	struct tm *d_t;
	#ifdef RTC_DEBUG
		char str_ch[256];
		uint8 str_len;
	#endif
	t_s = (time_t)RTC_GetCounter();
	d_t = localtime(&t_s);
	d_t->tm_year += 1900;
	
	date_time[0] = d_t->tm_year - 2000;
	date_time[1] = d_t->tm_mon + 1;
	date_time[2] = d_t->tm_mday;
	date_time[3] = d_t->tm_hour;
	date_time[4] = d_t->tm_min;
	date_time[5] = d_t->tm_sec;
	
	#ifdef RTC_DEBUG
		FeedWtd();
		str_len = sprintf(str_ch,"%2d��%02d��%02d��%2dʱ%02d��%02d��\r\n",
						 date_time[0],
						 date_time[1],
						 date_time[2],
						 date_time[3],
						 date_time[4],
						 date_time[5]
						 );
		LocalUartFixedLenSend((uint8*)str_ch,str_len);
		FeedWtd();
	#endif
}
//����ϵͳʱϵͳռ��
/*
void SysTickInit(void)
{
	RCC_ClocksTypeDef RCC_ClocksStatus;
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	SysTick_Config(RCC_ClocksStatus.SYSCLK_Frequency/1000);
}
void SysTickDisable(void)
{
	SysTick->CTRL  &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}
void SysTickEnable(void)
{
	SysTick->CTRL  |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}
*/

/******************************************************
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2��,APB1=36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
******************************************************/
void TIM3_Int_Init(uint16 arr,uint16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//ʱ��ʹ��
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 										//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 									//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 									//ʹ��ָ����TIM3�ж�,��������ж�
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  														//��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}



/******************************************************
UART��ʼ������

******************************************************/
void UsartInit(USART_TypeDef* USARTx, u32 uart_bpr,uint8 data_bits,uint8 stop_bits,uint8 parity_check)///�����ʣ�����λ��,ֹͣλ����żУ��
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	switch (*(uint32_t*)&USARTx)
	{
		case LOCAL_USART_BASE:	///LCD,PA.9,TX;PA.10,RX
		{
			g_local_uart_struct.rx_buf 		= local_uart_buf;
			g_local_uart_struct.rx_head = 0;
			g_local_uart_struct.rx_tail = 0;
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			break;
		}
		case GPS_USART_BASE:	///GPS,PA.2,TX;PA.3,RX
		{
			g_gps_uart_struct.rx_buf = gps_uart_buf;
			g_gps_uart_struct.rx_head = 0;
			g_gps_uart_struct.rx_tail = 0;
			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			break;
		}
		case GPRS_USART_BASE:	///GPRS,PB.10,TX;PB.11,RX
		{
			g_gprs_uart_struct.rx_buf = gprs_uart_buf;
			g_gprs_uart_struct.rx_head = 0;
			g_gprs_uart_struct.rx_tail = 0;
			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);			
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			break;
		}
		case METER_USART_BASE:	///METER,PC.10,TX;PC.11,RX
		{
			g_meter_uart_struct.rx_buf = meter_uart_buf;
			g_meter_uart_struct.rx_head = 0;
			g_meter_uart_struct.rx_tail = 0;
			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
 			GPIO_Init(GPIOC, &GPIO_InitStructure);			
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			break;
		}
		case RS485_USART_BASE:	///485,PC.12,TX;PD.2,RX
		{
			g_rs485_uart_struct.rx_buf = rs485_uart_buf;
			g_rs485_uart_struct.rx_head = 0;
			g_rs485_uart_struct.rx_tail = 0;
			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 			GPIO_Init(GPIOD, &GPIO_InitStructure);			
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			ENABLE_RX485();
			break;
		}
		default:
		{
			goto RETURN_LAB;
		}
	}

	USART_InitStructure.USART_BaudRate 		= uart_bpr;
	USART_InitStructure.USART_WordLength 	= data_bits;
	USART_InitStructure.USART_StopBits 		= stop_bits;
	USART_InitStructure.USART_Parity 			= parity_check;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	USART_Cmd(USARTx, ENABLE);
RETURN_LAB:
	return;
}

/******************************************************
GPAR_UART-->UART3,PB10=TX;PB11=RX  ����ʼ������

******************************************************/
void UsartGprsDeInit(void)///Gprs�������ŷ���ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/******************************************************
���ش��ڷ��ͺ���

******************************************************/
void LocalUartFixedLenSend(uint8 data[],uint16 len) 
{
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		USART_SendData(LOCAL_USART,data[i]);
		while(USART_GetFlagStatus(LOCAL_USART,USART_FLAG_TXE) == RESET);
	}
}

/******************************************************
GPRS���ڷ��ͺ���

******************************************************/
void GprsUartFixedLenSend(uint8 data[],uint16 len) 
{
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		USART_SendData(GPRS_USART,data[i]);
		while(USART_GetFlagStatus(GPRS_USART,USART_FLAG_TXE) == RESET);
	}
}
/******************************************************
GPS���ڷ��ͺ���

******************************************************/
void GpsUartFixedLenSend(uint8 data[],uint16 len) 
{
	uint16 i;
	
	for(i=0;i<len;i++)
	{
		USART_SendData(GPS_USART,data[i]);
		while(USART_GetFlagStatus(GPS_USART,USART_FLAG_TXE) == RESET);
	}
}

/******************************************************
�Ǳ��ڷ��ͺ���

******************************************************/
void MeterUartFixedLenSend(uint8 data[],uint16 len) 
{
	uint16 i;
	
 	for(i=0;i<len;i++)
 	{
 		USART_SendData(METER_USART,data[i]);
 		while(USART_GetFlagStatus(METER_USART,USART_FLAG_TXE) == RESET);
 	}
}

/******************************************************
RS485���ڷ��ͺ���

******************************************************/
void RS485UartFixedLenSend(uint8 data[],uint16 len) 
{
	uint16 i;
	
	ENABLE_TX485();
	OSTimeDlyHMSM(0, 0, 0, 10);	
	for(i=0;i<len;i++)
	{
		USART_SendData(RS485_USART,data[i]);
		while(USART_GetFlagStatus(RS485_USART,USART_FLAG_TXE) == RESET);
	}
	OSTimeDlyHMSM(0, 0, 0, 10);	
	ENABLE_RX485();
	OSTimeDlyHMSM(0, 0, 0, 10);	
}


