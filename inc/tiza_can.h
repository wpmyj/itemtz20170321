#ifndef __CAN_H
#define __CAN_H

#include "stdint.h"

	#ifdef CAN_GLOBAL
		#define EXTN_CAN
	#else
		#define EXTN_CAN extern
	#endif

	#define CAN_R_01 0x0C0328D0		//EVCU_DPU1		整车控制器发送状态信息一
	#define CAN_R_02 0x0C0428D0		//EVCU_DPU2		整车控制器发送状态信息二
	#define CAN_R_03 0x0C0528D0		//EVCU_DPU3		整车控制器发送状态信息三	
	#define CAN_R_04 0x1C0628D0		//EVCU_DPU4		整车控制器发送状态信息四
	#define CAN_R_05 0x18F00AD0		//EVCU_HVA		整车控制器对高压辅件(DCDC,动转,打气泵,空调等)的控制信息
	#define CAN_R_06 0x1801D028		//DPU_EVCU		仪表发送整车状态信息
	#define CAN_R_07 0x0CFF01F3		//BMS STATUS1	发送状态信息一
	#define CAN_R_08 0x0CFF02F3		//BMS STATUS2	发送状态信息二
	#define CAN_R_09 0x0CFF03F3		//BMS STATUS3	发送状态信息三
	#define CAN_R_10 0x0CFF04F3		//BMS STATUS4	发送状态信息四
	#define CAN_R_11 0x1CFF05F3		//BMS STATUS5	发送状态信息五		
	#define CAN_R_12 0x18FF06F3		//BMS STATUS6	发送状态信息六
	#define CAN_R_13 0x18FF37F3		//BMS STATUS7	发送状态信息七
	#define CAN_R_14 0x18F001D5		//ISU STATUS	绝缘检测仪发送状态信息
	#define CAN_R_15 0x18F010D6		//DCDC STATUS	DCDC 发送状态信息
	#define CAN_R_16 0x18F011D9		//ESU STATUS	动转控制器发送状态信息
	#define CAN_R_17 0x18F012DA		//EPC STATUS	打气泵发送状态信息
	
	#define CAN_R_18 0x18FFE5F3		//车载充电机1
//	#define CAN_R_19 0x18FFE3F5		//车载充电机2
	

//	#define CAN_T_1  0x00000000
	
	#define CAN_R_06_index 				5
	
	#define CAN_LOCK_DATA_LEN        8
	#define ONE_PACKET_LEN 			 		12	//4+8
	#define CAN_RX_ID_NUM			 			20
	#define CAN_BUF_LEN 			 			240	//(ONE_PACKET_LEN*CAN_RX_ID_NUM)
	
	typedef union{
		uint8 arry[ONE_PACKET_LEN];
		struct{
			uint32 	id;				//地址		
			union08 data[8];	//数据
		}Item;
	}CAN_FORMA;
	
	typedef struct 
	{
		uint8 tx_data[CAN_LOCK_DATA_LEN];///发送数据
		uint8 check_val;///校验值
	}CAN_LOCK_STRUCT;
	
	typedef struct{
		CAN_FORMA rx_can_buf[CAN_RX_ID_NUM];	///CAN数据接收buff		
		uint8 can_open_flag;						///CAN总线打开标志		1打开
		uint8 rx_data_flag;							///有CAN数据
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

