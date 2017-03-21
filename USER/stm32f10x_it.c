
//#include "stm32f10x_it.h" 
#include "tiza_include.h"


//����ϵͳʱϵͳռ��
/*void SysTick_Handler(void)
//{
//	sys_misc_run_struct.sys_tick_ms_counter++;
//	
//	if(gsm_misc_struct.gsm_ring_low_flag)
//	{
//		if(!RING_STATE())
//		{
//			gsm_misc_struct.gsm_ring_low_ms_counter++;
//		}
//		else
//		{
//			if(gsm_misc_struct.gsm_ring_low_ms_counter < 50)
//			{
//				gsm_misc_struct.ring_low_counter = 0;
//				gsm_misc_struct.gsm_ring_low_flag = FALSE;
//				gsm_misc_struct.gsm_ring_low_ms_counter = 0;
//			}
//		}
//	}
}
*/
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){  //���TIM3�����жϷ������
		
		g_sysmiscrun_struct.sys_time3_200ms_count = (g_sysmiscrun_struct.sys_time3_200ms_count+1)%60000;
		
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  		//���TIMx�����жϱ�־ 
	}
	OSIntExit();
}

void RTC_IRQHandler(void)
{	
	OSIntEnter();
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){
		
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}

	OSIntExit();
}

void PVD_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line16);
	if (PWR_GetFlagStatus(PWR_FLAG_PVDO))
	{
		while(1);
	}
}
void USART1_IRQHandler(void)///���ش���
{
	uint8 delay;
	
	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		g_local_uart_struct.rx_head = (g_local_uart_struct.rx_head+1) % LOCAL_UART_BUF_LEN;
		
		if(g_local_uart_struct.rx_head == g_local_uart_struct.rx_tail)
		{//buff����βҲ��ǰ�ƶ�
			g_local_uart_struct.rx_tail = (g_local_uart_struct.rx_tail+1) % LOCAL_UART_BUF_LEN;
		}
		g_local_uart_struct.rx_buf[g_local_uart_struct.rx_head] = USART_ReceiveData(USART1);
		
		
		//delay 50ms
		delay = 2;
		while(--delay){
			__NOP();__NOP();__NOP();__NOP();
		}
	}
	OSIntExit();
}
void USART2_IRQHandler(void)///GPS
{	
	OSIntEnter();
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		g_gps_uart_struct.rx_head = (g_gps_uart_struct.rx_head+1) % GPS_UART_BUF_LEN;
		
		if(g_gps_uart_struct.rx_head == g_gps_uart_struct.rx_tail)
		{//buff����βҲ��ǰ�ƶ�
			g_gps_uart_struct.rx_tail = (g_gps_uart_struct.rx_tail+1) % GPS_UART_BUF_LEN;
		}
		g_gps_uart_struct.rx_buf[g_gps_uart_struct.rx_head] = USART_ReceiveData(USART2);
		
		__NOP();__NOP();__NOP();__NOP();
	}
	OSIntExit();
}


void USART3_IRQHandler(void)///GPRS
{
	OSIntEnter();
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		g_gprs_uart_struct.rx_head = (g_gprs_uart_struct.rx_head+1) % GPRS_UART_BUF_LEN;
		
		if(g_gprs_uart_struct.rx_head == g_gprs_uart_struct.rx_tail)
		{//buff����βҲ��ǰ�ƶ�
			g_gprs_uart_struct.rx_tail = (g_gprs_uart_struct.rx_tail+1) % GPRS_UART_BUF_LEN;
		}
		g_gprs_uart_struct.rx_buf[g_gprs_uart_struct.rx_head] = USART_ReceiveData(USART3);
		
		__NOP();__NOP();__NOP();__NOP();
	}
	
	OSIntExit();
}

void UART4_IRQHandler(void)///METER�Ǳ�
{
	uint8 delay;
	
	OSIntEnter();
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		g_meter_uart_struct.rx_head = (g_meter_uart_struct.rx_head+1) % METER_UART_BUF_LEN;
		
		if(g_meter_uart_struct.rx_head == g_meter_uart_struct.rx_tail)
		{//buff����βҲ��ǰ�ƶ�
			g_meter_uart_struct.rx_tail = (g_meter_uart_struct.rx_tail+1) % METER_UART_BUF_LEN;
		}
		g_meter_uart_struct.rx_buf[g_meter_uart_struct.rx_head] = USART_ReceiveData(UART4);
		
		//delay 50ms
		delay = 2;
		while(--delay){
			__NOP();__NOP();__NOP();__NOP();
		}
	}
	OSIntExit();
	
}

void UART5_IRQHandler(void)///485
{
	uint8 delay;
	
	OSIntEnter();
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		g_rs485_uart_struct.rx_head = (g_rs485_uart_struct.rx_head+1) % RS485_UART_BUF_LEN;
		
		if(g_rs485_uart_struct.rx_head == g_rs485_uart_struct.rx_tail)
		{//buff����βҲ��ǰ�ƶ�
			g_rs485_uart_struct.rx_tail = (g_rs485_uart_struct.rx_tail+1) % RS485_UART_BUF_LEN;
		}
		g_rs485_uart_struct.rx_buf[g_rs485_uart_struct.rx_head] = USART_ReceiveData(UART5);
		
		//delay 50ms
		delay = 2;
		while(--delay){
			__NOP();__NOP();__NOP();__NOP();
		}
	}
	OSIntExit();
	
}

void DMA1_Channel1_IRQHandler(void)
{
	static uint32 adc_val[2] = {0x00,0x00};
	static uint8 s_conv_counter = 0;
	
	OSIntEnter();
	if(DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		
		adc_val[0] += adc_conv_buf[0];///PWR_C
		adc_val[1] += adc_conv_buf[1];///ADC01
	  s_conv_counter++;
		if(s_conv_counter >= 5)
		{
			s_conv_counter = 0x00;
			adc_val[0] = adc_val[0] / 5;
			adc_val[0] = adc_val[0]*303/4096;						///��ѹֵ����λ0.1V
			adc_result[0] = adc_val[0];
	
			adc_val[1] = adc_val[1] / 5;
			adc_val[1] = adc_val[1]*66/4096;						///��ѹֵ����λ0.1V
			adc_result[1] = adc_val[1];									///
		}
	}
	OSIntExit();
}
//=======================================================================================//
 
void NMI_Handler(void)
{
	OSIntEnter();
	OSIntExit();
}
 
void HardFault_Handler(void)
{
	OSIntEnter();
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
	OSIntExit();  
}
 
void MemManage_Handler(void)
{
	OSIntEnter();
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
	OSIntExit();  
}

 
void BusFault_Handler(void)
{
	OSIntEnter();
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
	OSIntExit();  
}
 
void UsageFault_Handler(void)
{
	OSIntEnter();
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
	OSIntExit();  
}
 
void SVC_Handler(void)
{
	OSIntEnter();
	OSIntExit();  
}
 
void DebugMon_Handler(void)
{
	OSIntEnter();
	OSIntExit();  
}

 
 

