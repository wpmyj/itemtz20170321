#ifndef __CAN_H
#define __CAN_H

#include "stdint.h"

	#ifdef CAN_GLOBAL
		#define EXTN_CAN
	#else
		#define EXTN_CAN extern
	#endif

	#define CAN_R_01 0x0C0328D0		//EVCU_DPU1		��������������״̬��Ϣһ
	#define CAN_R_02 0x0C0428D0		//EVCU_DPU2		��������������״̬��Ϣ��
	#define CAN_R_03 0x0C0528D0		//EVCU_DPU3		��������������״̬��Ϣ��	
	#define CAN_R_04 0x1C0628D0		//EVCU_DPU4		��������������״̬��Ϣ��
	#define CAN_R_05 0x18F00AD0		//EVCU_HVA		�����������Ը�ѹ����(DCDC,��ת,������,�յ���)�Ŀ�����Ϣ
	#define CAN_R_06 0x1801D028		//DPU_EVCU		�Ǳ�������״̬��Ϣ
	#define CAN_R_07 0x0CFF01F3		//BMS STATUS1	����״̬��Ϣһ
	#define CAN_R_08 0x0CFF02F3		//BMS STATUS2	����״̬��Ϣ��
	#define CAN_R_09 0x0CFF03F3		//BMS STATUS3	����״̬��Ϣ��
	#define CAN_R_10 0x0CFF04F3		//BMS STATUS4	����״̬��Ϣ��
	#define CAN_R_11 0x1CFF05F3		//BMS STATUS5	����״̬��Ϣ��		
	#define CAN_R_12 0x18FF06F3		//BMS STATUS6	����״̬��Ϣ��
	#define CAN_R_13 0x18FF37F3		//BMS STATUS7	����״̬��Ϣ��
	#define CAN_R_14 0x18F001D5		//ISU STATUS	��Ե����Ƿ���״̬��Ϣ
	#define CAN_R_15 0x18F010D6		//DCDC STATUS	DCDC ����״̬��Ϣ
	#define CAN_R_16 0x18F011D9		//ESU STATUS	��ת����������״̬��Ϣ
	#define CAN_R_17 0x18F012DA		//EPC STATUS	�����÷���״̬��Ϣ
	
	#define CAN_R_18 0x18FFE5F3		//���س���1
//	#define CAN_R_19 0x18FFE3F5		//���س���2
	

//	#define CAN_T_1  0x00000000
	
	#define CAN_R_06_index 				5
	
	#define CAN_LOCK_DATA_LEN        8
	#define ONE_PACKET_LEN 			 		12	//4+8
	#define CAN_RX_ID_NUM			 			20
	#define CAN_BUF_LEN 			 			240	//(ONE_PACKET_LEN*CAN_RX_ID_NUM)
	
	typedef union{
		uint8 arry[ONE_PACKET_LEN];
		struct{
			uint32 	id;				//��ַ		
			union08 data[8];	//����
		}Item;
	}CAN_FORMA;
	
	typedef struct 
	{
		uint8 tx_data[CAN_LOCK_DATA_LEN];///��������
		uint8 check_val;///У��ֵ
	}CAN_LOCK_STRUCT;
	
	typedef struct{
		CAN_FORMA rx_can_buf[CAN_RX_ID_NUM];	///CAN���ݽ���buff		
		uint8 can_open_flag;						///CAN���ߴ򿪱�־		1��
		uint8 rx_data_flag;							///��CAN����
	}CAN_STRUCT;
	
	EXTN_CAN CAN_STRUCT can_struct;
//	EXTN_CAN CAN_LOCK_STRUCT can_lock_struct;
	
//	void CanFilterInit(uint8 filter_index,uint32 can_id_1,uint32 can_id_2);
//	void CanDeInit(void);
//	void CanInit(void);
//	void CanTxData(uint8 data[],uint32 id);
//	void CanTx(void);
//	void CanRx(void);
//	void CanMonitor(uint16 past_sec);
//	void CanMain(void);
	
#endif

