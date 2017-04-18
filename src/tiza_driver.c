#define DRIVER_GLOBAL

#include "tiza_include.h"

//#define RTC_DEBUG

/******************************************************
GPIO初始化函数
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
///输出引脚
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;///GPS灯，低，亮
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;///总POWER灯,低，亮
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;///LED1灯,低，亮
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	///WDI喂狗
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;///485232_EN，485/232电源，控制引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;///485_EN，485通信方向，控制引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
//	//以下在tiza_l218.c->L218PowerInit中定义
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; ///POWER_ON/OFF,4V电源输出控制 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;///POWERKEY_CONNECT，模块关机控制引脚
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;///5V控制，CAN开关与电池充电共用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
///输入引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;///CHECK_POW 1表示有电
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	

	OFF_GPS_LED();
	OFF_WORK_LED();
	OFF_ERR_LED();
	ON_485232_PWR();
	ON_CAN_PWR();
	
	
//	OFF_GPRS_PWR();///关机
//	OSTimeDlyHMSM(0, 0, 1, 0);
//	ON_GPRS_PWR();
//	OSTimeDlyHMSM(0, 0, 1, 0);
//	HIGH_GPRS_IGT();
//	UsartGprsDeInit();

}

/******************************************************
WATCHDOG初始化函数

WATCHDOG FEED DOG
******************************************************/
void IwdgInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	///WDI喂狗
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
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
系统复位初始化函数

******************************************************/
void SysReset(void)
{
	while(1)
	{
		NVIC_DISABLE();
	}
}
/******************************************************
ADC初始化函数

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

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;										///独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;													///扫描模式
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;										///不连续转换，手动使能转换
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;										///连续转换，使能后自动转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		///由软件控制转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								///右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;																///规则通道转换序列长度(2路)
	ADC_Init(ADC1, &ADC_InitStructure);

///	ADC_TempSensorVrefintCmd(ENABLE);			///ADC内置温度传感器使能

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_239Cycles5);

	ADC_DMACmd(ADC1, ENABLE);											/// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_Cmd(ADC1, ENABLE);												///开启ADC

	ADC_ResetCalibration(ADC1);										///初始化校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));		///等待初始化完成
	ADC_StartCalibration(ADC1);										///开始自校准
	while(ADC_GetCalibrationStatus(ADC1));				///等待校准完成
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC1开始转换
}

/******************************************************
DMA初始化函数

******************************************************/
void DmaInit(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
   ///启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);		///DMA1  1通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)&ADC1->DR);//0x4001244C);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc_conv_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    ///如此设置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]..
    DMA_InitStructure.DMA_BufferSize = 2;	//两路ADC
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    ///循环模式开启，Buffer写满后，自动回到初始地址开始传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    ///配置完成后，启动DMA通道
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
	RCC_HSEConfig(RCC_HSE_ON); 														/*HSES使能*/  
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 					/*等待*/
	if(HSEStartUpStatus == SUCCESS) 
	{ 
		RCC_PLLCmd(ENABLE);																	/*使能*/
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)== RESET); 	/*等待PLL有效*/      
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);					/*将PLL作为系统时钟*/
		while(RCC_GetSYSCLKSource() != 0x08);								/*等待*/
	} 
}

/******************************************************
RTC时间获取函数
年 月 日 时 分 秒
******************************************************/
void RtcGetCalendarTime(uint8 date_time[])
{
	uint8  mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	uint32 timecount=0,temp=0;
	uint16 year,temp1=0;	
//	static uint16 daycnt=0,temp1=0;
  
  timecount = RTC_GetCounter();	 
 	temp = timecount/86400;  				 	//得到天数(秒钟数对应的)
//	if(daycnt != temp){								//超过一天了 
//		daycnt = temp;
		temp1 = 1970;									//从1970年开始
		while(temp >= 365){				 
			if((temp1&0X03) == 0){				//是闰年
				if(temp>=366) temp -= 366;	//闰年的秒钟数
				else {
					temp1++;
					break;
				}  
			}
			else temp -= 365;	  					//平年 
			temp1++;  
		}   
		year = temp1;										//得到年份
		
		temp1 = 0;
		while(temp>=28){								//超过了一个月
			if(((year&0X03) == 0) && temp1==1){	//当年是不是闰年/2月份
				if(temp >= 29)temp -= 29;									//闰年的秒钟数
				else 					break; 
			}
			else {
				if(temp >= mon_table[temp1]){							//平年
					temp -= mon_table[temp1];
				}
				else break;
			}
			temp1++;  
		}
		date_time[1] = temp1+1;				///得到月份
		date_time[2] = temp +1;  			///得到日期 
//	}
	temp = timecount % 86400;     	//得到秒钟数   	   
	date_time[3] = temp / 3600;   	///小时
	date_time[4] = (temp%3600)/60; 	///分钟	
	date_time[5] = (temp%3600)%60; 	///秒钟
	date_time[0] = year - 2000;			///年
	#ifdef RTC_DEBUG
		printf("%2d年%02d月%02d日%2d时%02d分%02d秒\r\n",
						 date_time[0],
						 date_time[1],
						 date_time[2],
						 date_time[3],
						 date_time[4],
						 date_time[5]
						 );
	#endif
}
/******************************************************
RTC时间设置函数
年 月 日 时 分 秒
******************************************************/
void RtcSetCalendarTime(uint8 data[])
{
	uint8  mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	uint16 t,year = data[0] + 2000;
	uint32 seccount=0;
	
	if(year<1970||year>2099)return;	   
	
	for(t=1970;t<year;t++){													//把所有年份的秒钟相加
		if((t&0X03) == 0)	seccount+=31622400;					//闰年的秒钟数
		else 							seccount+=31536000; 				//平年的秒钟数
	}
	data[1]-=1;
	for(t=0;t<data[1];t++){	   											//把前面月份的秒钟数相加
		seccount += (uint32)mon_table[t]*86400;				//月份秒钟数相加
		if(((year&0X03) == 0)&&t==1)seccount+=86400;	//闰年2月份增加一天的秒钟数	   
	}
	seccount += (uint32)(data[2]-1)*86400;					//把前面日期的秒钟数相加 
	seccount += (uint32)data[3]*3600;								//小时秒钟数
  seccount += (uint32)data[4]*60;	 								//分钟秒钟数
	seccount += data[5];														//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);								//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);										//设置RTC计数器的值
	RTC_WaitForLastTask();											//等待最近一次对RTC寄存器的写操作完成  	

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

void RtcInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RtcConfiguration();
	RtcSetCalendarTime(g_protime_union.arry);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 11;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 
}

void PvdInit(void)///低压中断
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	///占先优先级，高优先级可打断低优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9); 											/// 设定监控阀值
	PWR_PVDCmd(ENABLE); 																				/// 使能PVD 
	EXTI_StructInit(&EXTI_InitStructure); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line16; 								/// PVD连接到中断线16上
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 				/// 使用中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;			/// 电压低于阀值时产生中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 									/// 使能中断线
	EXTI_Init(&EXTI_InitStructure); 														/// 初始化中断控制器
}


//运行系统时系统占用
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
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍,APB1=36M
//arr：自动重装值。
//psc：时钟预分频数
******************************************************/
void TIM3_Int_Init(uint16 arr,uint16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 				//时钟使能
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 									//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//根据指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 									//使能指定的TIM3中断,允许更新中断
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  														//初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}



/******************************************************
UART初始化函数

******************************************************/
void UsartInit(USART_TypeDef* USARTx, u32 uart_bpr,uint8 data_bits,uint8 stop_bits,uint8 parity_check)///波特率，数据位长,停止位，奇偶校验
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
GPAR_UART-->UART3,PB10=TX;PB11=RX  反初始化函数

******************************************************/
void UsartGprsDeInit(void)///Gprs串口引脚反初始化
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
本地串口发送函数

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
GPRS串口发送函数

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
GPS串口发送函数

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
仪表串口发送函数

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
RS485串口发送函数

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


