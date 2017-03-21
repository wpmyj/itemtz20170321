#define CAN_GLOBAL
#include "tiza_include.h"


void CanFilterInit(uint8 filter_index,uint32 can_id_1,uint32 can_id_2)
{
	u32 tmp_u32_val;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterNumber = filter_index;		///ָ��������0 ����ֵ��Χ�� 0 - 13
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	///������λ�� 16 or 32; 16 bit ʱ ÿ����2��16λ��������ɣ�32ʱ��ֻ��һ��������

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

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;		///������������FIFO
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

	CAN_InitStructure.CAN_Prescaler=RCC_ClocksStatus.PCLK1_Frequency/(16*250000);      ///�����ʼ��㣬16M/��1+8+7��/4=250k
	if(!CAN_Init(CAN1, &CAN_InitStructure)){
		///CAN��ʼ��ʧ��
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
			case CAN_R_01:{//��������������״̬��Ϣһ
				index = 0;
				break;
			}
			case CAN_R_02:{//��������������״̬��Ϣ��
				index = 1;
				break;
			}
			case CAN_R_03:{//��������������״̬��Ϣ��
				index = 2;
				break;
			}
			case CAN_R_04:{//��������������״̬��Ϣ��
				index = 3;
				break;
			}
			case CAN_R_05:{//�����������Ը�ѹ����
				index = 4;
				break;
			}
			case CAN_R_06:{//�Ǳ�������״̬��Ϣ
				index = 5;
				break;
			}
			case CAN_R_07:{//����״̬��Ϣһ
				index = 6;
				break;
			}
			case CAN_R_08:{//����״̬��Ϣ��
				index = 7;
				break;
			}
			case CAN_R_09:{//����״̬��Ϣ��
				index = 8;
				break;
			}
			case CAN_R_10:{//����״̬��Ϣ��
				index = 9;
				break;
			}
			case CAN_R_11:{//����״̬��Ϣ��
				index = 10;
				break;
			}
			case CAN_R_12:{//����״̬��Ϣ��
				index = 11;
				break;
			}
			case CAN_R_13:{//����״̬��Ϣ��
				index = 12;
				break;
			}
			case CAN_R_14:{//��Ե����Ƿ���״̬��Ϣ
				index = 13;
				break;
			}
			case CAN_R_15:{//DCDC ����״̬��Ϣ
				index = 14;
				break;
			}
			case CAN_R_16:{//��ת����������״̬��Ϣ
				index = 15;
				break;
			}
			case CAN_R_17:{//�����÷���״̬��Ϣ
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
			case CAN_R_01:{//��������������״̬��Ϣһ
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;
				g_promotor_union[0].Item.ctr_in_vol = tmpval.word.L;						//��������������ѹ
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[4].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[5].byte;
				g_promotor_union[0].Item.ctr_bus_cur = tmpval.word.L;						//���������ֱ��ĸ�ߵ���
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[6].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[7].byte;				
				g_promotor_union[0].Item.rpm = tmpval.word.L;										//�������ת��	
				break;
			}
			case CAN_R_02:{//��������������״̬��Ϣ��
				g_promotor_union[0].Item.motor_tem = can_struct.rx_can_buf[index].Item.data[0].byte;		//����¶�
				g_promotor_union[0].Item.ctr_tem   = can_struct.rx_can_buf[index].Item.data[1].byte;		//�������¶�
				//��λ״̬
				if(can_struct.rx_can_buf[CAN_R_06_index].Item.data[0].bit.B1){//פ���ź� ע�����Ǵ�CAN_R_06 0x1801D028��ȡ��
					g_provehice_union.Item.stall.half.L = 0x0F;
					g_provehice_union.Item.stall.bit.B4 = 1;
					g_provehice_union.Item.stall.bit.B5 = 0;
				}
				else{
					tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[2].byte;
					tmpval.byte.LH = (tmpval.byte.LL>>5) & 0x07;
					if(tmpval.byte.LH){	//1~7��
						g_provehice_union.Item.stall.half.L = tmpval.byte.LH;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 1;
					}
					else if(tmpval.bit.B02){//����
						g_provehice_union.Item.stall.half.L = 0x0D;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 1;
					}
					else if(tmpval.bit.B01){//�յ�
						g_provehice_union.Item.stall.half.L = 0x00;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 0;
					}
					else if(tmpval.bit.B00){//�Զ�D��
						g_provehice_union.Item.stall.half.L = 0x0E;
						g_provehice_union.Item.stall.bit.B4 = 0;
						g_provehice_union.Item.stall.bit.B5 = 1;
					}
				}
				break;
			}
			case CAN_R_03:{//��������������״̬��Ϣ��
				g_provehice_union.Item.vehicle  = can_struct.rx_can_buf[index].Item.data[5].bit.B1;			//����״̬
				g_provehice_union.Item.DC_statu = can_struct.rx_can_buf[index].Item.data[7].bit.B2;			//DCDC����״̬
				g_provehice_union.Item.Accelerate = can_struct.rx_can_buf[index].Item.data[0].byte;			//����̤��
				g_provehice_union.Item.braking 		= can_struct.rx_can_buf[index].Item.data[1].byte;			//�ƶ�̤��
				
				break;
			}
			case CAN_R_04:{//��������������״̬��Ϣ��
				break;
			}
			case CAN_R_05:{//�����������Ը�ѹ����
				break;
			}
			case CAN_R_06:{//�Ǳ�������״̬��Ϣ
				g_provehice_union.Item.speed   = can_struct.rx_can_buf[index].Item.data[2].byte * 10;		//���� ����1����0.1
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[3].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[4].byte;
				tmpval.word.H  = can_struct.rx_can_buf[index].Item.data[5].byte;				
				g_provehice_union.Item.mileage = tmpval.dword;																						//�ۼ������
				
/* 				if(can_struct.rx_can_buf[index].Item.data[0].bit.B1){//פ���ź�
					g_provehice_union.Item.stall.half.L = 0x0F;
					g_provehice_union.Item.stall.bit.B4 = 1;
					g_provehice_union.Item.stall.bit.B5 = 0;
				} */
				break;
			}
			case CAN_R_07:{//����״̬��Ϣһ
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;		
				g_provehice_union.Item.voltage = tmpval.word.L;																						//�ܵ�ѹ	
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[2].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[3].byte;		
				g_provehice_union.Item.current = tmpval.word.L;																						//�ܵ���
				g_provehice_union.Item.SOC = can_struct.rx_can_buf[index].Item.data[7].byte;							//���ʣ�����SOC
				
				break;
			}
			case CAN_R_08:{//����״̬��Ϣ��
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;		
				g_proextreme_union.Item.maxvolbat_val = tmpval.word.L;																	//���ģ����ߵ�ѹ
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[2].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[3].byte;		
				g_proextreme_union.Item.minvolbat_val = tmpval.word.L;																	//���ģ����͵�ѹ
				g_proextreme_union.Item.maxtempro_val = can_struct.rx_can_buf[index].Item.data[4].byte;	//���ģ������¶�
				g_proextreme_union.Item.mintempro_val = can_struct.rx_can_buf[index].Item.data[5].byte;	//���ģ������¶�
				
				break;
			}
			case CAN_R_09:{//����״̬��Ϣ��
			
				break;
			}
			case CAN_R_10:{//����״̬��Ϣ��
				g_proextreme_union.Item.maxtempro_val = can_struct.rx_can_buf[index].Item.data[0].byte;	//�����ߵ�ѹ���
				g_proextreme_union.Item.mintempro_val = can_struct.rx_can_buf[index].Item.data[1].byte;	//�����͵�ѹ���
				g_proextreme_union.Item.maxtempro_val = can_struct.rx_can_buf[index].Item.data[2].byte;	//�������¶ȱ��
				g_proextreme_union.Item.mintempro_val = can_struct.rx_can_buf[index].Item.data[3].byte;	//�������¶ȱ��
				
				break;
			}
			case CAN_R_11:{//����״̬��Ϣ��
			
				break;
			}
			case CAN_R_12:{//����״̬��Ϣ��
			
				break;
			}
			case CAN_R_13:{//����״̬��Ϣ��
			
				break;
			}
			case CAN_R_14:{//��Ե����Ƿ���״̬��Ϣ
				tmpval.byte.LL = can_struct.rx_can_buf[index].Item.data[0].byte;
				tmpval.byte.LH = can_struct.rx_can_buf[index].Item.data[1].byte;		
				g_provehice_union.Item.resistance = tmpval.word.L;																						//��Ե����	
				
				break;
			}
			case CAN_R_15:{//DCDC ����״̬��Ϣ
			
				break;
			}
			case CAN_R_16:{//��ת����������״̬��Ϣ
			
				break;
			}
			case CAN_R_17:{//�����÷���״̬��Ϣ
			
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
	if(1){//ACC��
		if(can_struct.can_open_flag == 0){//CANû��
			can_struct.can_open_flag = TRUE;
			ON_CAN_PWR();
			CanInit();
			return;
		}		
//		CanTx();		
		if(can_struct.rx_data_flag == 1){//����CAN
			CanRx_Analyse();
		}
		else{
			if(0){///�Ǳ�ʱ��������,����
				#ifdef CAN_ERROR_ALARM
				printf("\r\n meter break down alarm��wo will restart...\r\n")
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
		if(0){//���ACC��
			if(can_struct.can_open_flag == 1){//CAN�Ѵ�
				can_struct.can_open_flag = FALSE;
				CanDeInit();
				OFF_CAN_PWR();
				RamZero(can_struct.rx_can_buf[0].arry,CAN_BUF_LEN);
			}
		}
	}
}


