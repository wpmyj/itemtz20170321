#define CAN_GLOBAL
#include "tiza_include.h"


void CanFilterInit(uint8 filter_index,uint32 can_id_1,uint32 can_id_2)
{
	u32 tmp_u32_val;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterNumber = filter_index;		///指定过滤器0 ，该值范围是 0 - 13
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	///过滤器位宽 16 or 32; 16 bit 时 每组由2个16位过滤器组成，32时则只有一个过滤器

	tmp_u32_val = can_id_1;
	tmp_u32_val = tmp_u32_val << 3;
	tmp_u32_val |= CAN_Id_Extended;
	CAN_FilterInitStructure.CAN_FilterIdHigh = tmp_u32_val >> 16;			
	CAN_FilterInitStructure.CAN_FilterIdLow = tmp_u32_val;
	
	tmp_u32_val = can_id_2;
	tmp_u32_val = tmp_u32_val << 3;
	tmp_u32_val |= CAN_Id_Extended;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = tmp_u32_val >> 16;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = tmp_u32_val;

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;		///过滤器关联到FIFO
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}

 void CanDeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	CAN_DeInit(CAN1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void CanInit(void)
{
	GPIO_InitTypeDef 	   GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	///CanTxMsg TxMessage;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	///Configure CAN pin: TX 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	RCC_GetClocksFreq(&RCC_ClocksStatus);
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	/// CAN cell init
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_13tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;

	CAN_InitStructure.CAN_Prescaler=RCC_ClocksStatus.PCLK1_Frequency/(16*250000);      ///波特率计算，16M/（1+8+7）/4=250k
	if(!CAN_Init(CAN1, &CAN_InitStructure)){
		///CAN初始化失败
		return;
	}
	CanFilterInit(0,CAN_R_01,CAN_R_02);
	CanFilterInit(1,CAN_R_03,CAN_R_04);
	CanFilterInit(2,CAN_R_05,CAN_R_06);
	CanFilterInit(3,CAN_R_07,CAN_R_08);
	CanFilterInit(4,CAN_R_09,CAN_R_10);
	CanFilterInit(5,CAN_R_11,CAN_R_12);
	CanFilterInit(6,CAN_R_13,CAN_R_14);
	CanFilterInit(7,CAN_R_15,CAN_R_16);
	CanFilterInit(8,CAN_R_17,CAN_R_18);
//	CanFilterInit(9,CAN_R_19,CAN_R_20);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void CanTxData(uint8 data[],uint32 id)
{
	CanTxMsg TxMessage;
	TxMessage.ExtId = id;
	TxMessage.IDE = CAN_ID_EXT;   
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 8;
	MemCpy(TxMessage.Data,data,8);
	CAN_Transmit(CAN1, &TxMessage);
}

void CanTx(void)
{
//	uint8 heart_data[CAN_LOCK_DATA_LEN] = {0xFD,0x20,0x00,0x00,0x00,0x00,0x00,0x00};
//	
//	CanTxData(heart_data,CAN_T_1);
}

void CanRx(void)
{
	uint8 index = 0xff;
	CanRxMsg can_msg;
	uint32 decode_val;
	
	CAN_Receive(CAN1,CAN_FIFO0,&can_msg);
	
	if(0x08 == can_msg.DLC){
		switch(can_msg.ExtId){
			case CAN_R_01:{//整车控制器发送状态信息一
				index = 0;
				break;
			}
			case CAN_R_02:{//整车控制器发送状态信息二
				index = 1;
				break;
			}
			case CAN_R_03:{//整车控制器发送状态信息三
				index = 2;
				break;
			}
			case CAN_R_04:{//整车控制器发送状态信息四
				index = 3;
				break;
			}
			case CAN_R_05:{//整车控制器对高压辅件
				index = 4;
				break;
			}
			case CAN_R_06:{//仪表发送整车状态信息
				index = 5;
				break;
			}
			case CAN_R_07:{//发送状态信息一
				index = 6;
				break;
			}
			case CAN_R_08:{//发送状态信息二
				index = 7;
				break;
			}
			case CAN_R_09:{//发送状态信息三
				index = 8;
				break;
			}
			case CAN_R_10:{//发送状态信息四
				index = 9;
				break;
			}
			case CAN_R_11:{//发送状态信息五
				index = 10;
				break;
			}
			case CAN_R_12:{//发送状态信息六
				index = 11;
				break;
			}
			case CAN_R_13:{//发送状态信息七
				index = 12;
				break;
			}
			case CAN_R_14:{//绝缘检测仪发送状态信息
				index = 13;
				break;
			}
			case CAN_R_15:{//DCDC 发送状态信息
				index = 14;
				break;
			}
			case CAN_R_16:{//动转控制器发送状态信息
				index = 15;
				break;
			}
			case CAN_R_17:{//打气泵发送状态信息
				index = 16;
				break;
			}
			case CAN_R_18:{//
				index = 17;
				break;
			}
			
			default:				break;
		}
	}
	
	can_struct.rx_data_flag = 1;
	
	if(index == 0xFF){
		return;
	}
//	Uint32ToU8(p_tmp,can_msg.ExtId);
//	p_tmp += 4;

	can_struct.rx_can_buf[index].Item.id = can_msg.ExtId;
	MemCpy(&can_struct.rx_can_buf[index].Item.data[0].byte,can_msg.Data,8);
}


void CanRx_Analyse(void)
{
	uint8  index ;
	union32 tmpval;
	
	for(index=0;index < CAN_RX_ID_NUM; index++){
		switch(can_struct.rx_can_buf[index].Item.id){
			case CAN_R_01:{//整车控制器发送状态信息一
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;
				g_promotor_union[0].Item.ctr_in_vol = tmpval.word.L;						//电机控制器输入电压
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[4].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[5].byte;
				g_promotor_union[0].Item.ctr_bus_cur = tmpval.word.L;						//电机控制器直流母线电流
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[6].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[7].byte;				
				g_promotor_union[0].Item.rpm = tmpval.word.L;										//驱动电机转速	
				break;
			}
			case CAN_R_02:{//整车控制器发送状态信息二
				g_promotor_union[0].Item.motor_tem = can_struct.rx_can_buf[index].Item.data[0].byte;		//电机温度
				g_promotor_union[0].Item.ctr_tem   = can_struct.rx_can_buf[index].Item.data[1].byte;		//控制器温度
				//档位状态
				if(can_struct.rx_can_buf[CAN_R_06_index].Item.data[0].bit.B1){//驻车信号 注意这是从CAN_R_06 0x1801D028读取的
					g_provehice_union.Item.stall.half.L = 0x0F;
					g_provehice_union.Item.stall.bit.B4 = 1;
					g_provehice_union.Item.stall.bit.B5 = 0;
				}
				else{
					tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[2].byte;
					tmpval.byte.LH = (tmpval.byte.LL>>5) & 0x07;
					if(tmpval.byte.LH){	//1~7档
						g_provehice_union.Item.stall.half.L = tmpval.byte.LH;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 1;
					}
					else if(tmpval.bit.B02){//倒挡
						g_provehice_union.Item.stall.half.L = 0x0D;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 1;
					}
					else if(tmpval.bit.B01){//空挡
						g_provehice_union.Item.stall.half.L = 0x00;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 0;
					}
					else if(tmpval.bit.B00){//自动D档
						g_provehice_union.Item.stall.half.L = 0x0E;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 1;
					}
				}
				break;
			}
			case CAN_R_03:{//整车控制器发送状态信息三
				g_provehice_union.Item.vehicle  = can_struct.rx_can_buf[index].Item.data[5].bit.B1;			//车辆状态
				g_provehice_union.Item.DC_statu = can_struct.rx_can_buf[index].Item.data[7].bit.B2;			//DCDC控制状态
				g_provehice_union.Item.Accelerate = can_struct.rx_can_buf[index].Item.data[0].byte;			//加速踏板
				g_provehice_union.Item.braking 		= can_struct.rx_can_buf[index].Item.data[1].byte;			//制动踏板
				
				break;
			}
			case CAN_R_04:{//整车控制器发送状态信息四
				break;
			}
			case CAN_R_05:{//整车控制器对高压辅件
				break;
			}
			case CAN_R_06:{//仪表发送整车状态信息
				g_provehice_union.Item.speed   = can_struct.rx_can_buf[index].Item.data[2].byte * 10;		//车速 精度1不是0.1
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[3].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[4].byte;
				tmpval.word.H  = can_struct.rx_can_buf[index].Item.data[5].byte;				
				g_provehice_union.Item.mileage = tmpval.dword;																						//累计里程数
				
/* 				if(can_struct.rx_can_buf[index].Item.data[0].bit.B1){//驻车信号
					g_provehice_union.Item.stall.half.L = 0x0F;
					g_provehice_union.Item.stall.bit.B4 = 1;
					g_provehice_union.Item.stall.bit.B5 = 0;
				} */
				break;
			}
			case CAN_R_07:{//发送状态信息一
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;		
				g_provehice_union.Item.voltage = tmpval.word.L;																						//总电压	
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[2].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[3].byte;		
				g_provehice_union.Item.current = tmpval.word.L;																						//总电流
				g_provehice_union.Item.SOC = can_struct.rx_can_buf[index].Item.data[7].byte;							//电池剩余电量SOC
				
				break;
			}
			case CAN_R_08:{//发送状态信息二
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;		
				g_proextreme_union.Item.maxvolbat_val = tmpval.word.L;																	//电池模块最高电压
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[2].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[3].byte;		
				g_proextreme_union.Item.minvolbat_val = tmpval.word.L;																	//电池模块最低电压
				g_proextreme_union.Item.maxtempro_val = can_struct.rx_can_buf[index].Item.data[4].byte;	//电池模块最高温度
				g_proextreme_union.Item.mintempro_val = can_struct.rx_can_buf[index].Item.data[5].byte;	//电池模块最低温度
				
				break;
			}
			case CAN_R_09:{//发送状态信息三
			
				break;
			}
			case CAN_R_10:{//发送状态信息四
				g_proextreme_union.Item.maxtempro_val = can_struct.rx_can_buf[index].Item.data[0].byte;	//电池最高电压编号
				g_proextreme_union.Item.mintempro_val = can_struct.rx_can_buf[index].Item.data[1].byte;	//电池最低电压编号
				g_proextreme_union.Item.maxtempro_val = can_struct.rx_can_buf[index].Item.data[2].byte;	//电池最高温度编号
				g_proextreme_union.Item.mintempro_val = can_struct.rx_can_buf[index].Item.data[3].byte;	//电池最低温度编号
				
				break;
			}
			case CAN_R_11:{//发送状态信息五
			
				break;
			}
			case CAN_R_12:{//发送状态信息六
			
				break;
			}
			case CAN_R_13:{//发送状态信息七
			
				break;
			}
			case CAN_R_14:{//绝缘检测仪发送状态信息
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;		
				g_provehice_union.Item.resistance = tmpval.word.L;																						//绝缘电阻	
				
				break;
			}
			case CAN_R_15:{//DCDC 发送状态信息
			
				break;
			}
			case CAN_R_16:{//动转控制器发送状态信息
			
				break;
			}
			case CAN_R_17:{//打气泵发送状态信息
			
				break;
			}
			case CAN_R_18:{//
			
				break;
			}
			
			default:break;
		}
		
		can_struct.rx_can_buf[index].Item.id = 0;
	}
		
}

void CanMonitor(uint16 past_sec)
{
	if(1){//ACC开
		if(can_struct.can_open_flag == 0){//CAN没打开
			can_struct.can_open_flag = TRUE;
			ON_CAN_PWR();
			CanInit();
			return;
		}		
//		CanTx();		
		if(can_struct.rx_data_flag == 1){//解析CAN
			CanRx_Analyse();
		}
		else{
			if(0){///仪表长时间无数据,重启
				#ifdef CAN_ERROR_ALARM
				printf("\r\n meter break down alarm，wo will restart...\r\n")
				#endif				
				OFF_CAN_PWR();			
				CanDeInit();
				OSTimeDlyHMSM(0, 0, 2, 0);
				CanInit();
				ON_CAN_PWR();
			}
		}
	}
	else{
		if(0){//如果ACC关
			if(can_struct.can_open_flag == 1){//CAN已打开
				can_struct.can_open_flag = FALSE;
				CanDeInit();
				OFF_CAN_PWR();
				RamZero(can_struct.rx_can_buf[0].arry,CAN_BUF_LEN);
			}
		}
	}
}


