#ifndef __CAN_H
#define __CAN_H

#include "stdint.h"

	#ifdef CAN_GLOBAL
		#define EXTN_CAN
	#else
		#define EXTN_CAN extern
	#endif

	///ע��CAN�˲����䶯  CANR01xx_Filter CANR02xx_Filter CANR_PeriodChargeFilter
	#define CAN01MINODD 	 	0X01
	#define CAN01MINEVEN   	0X02
	#define CAN01MAX   			0X18
	#define CAN02MINODD 	 	0X3D
	#define CAN02MINEVEN   	0X3E
	#define CAN02MAX   			0X3F
		
	#define CAN_R_0101 0x1801D1F3		//BMS�����ѹ	1~60
	#define CAN_R_0102 0x1802D1F3		//
	#define CAN_R_0124 0x1818D1F3		//1~24				
	#define CAN_R_0160 CAN_R_0124		// 			// 0x183CD1F3		//BMS�����ѹ	1~60
		
	#define CAN_R_0201 0x183DD1F3		//BMS ģ����	1~30
	#define CAN_R_0202 0x183ED1F3		//
	#define CAN_R_0203 0x183FD1F3		//1~3
	#define CAN_R_0230 CAN_R_0203		//			// 0x185BD1F3		//BMS ģ����	1~30
	
	#define CAN_R_03 0x18F8289E		//BMS�����ѹ��ֵ����֡
	#define CAN_R_04 0x18F8299E		//BMS�¶�ģ�鼫ֵ����֡
	#define CAN_R_05 0x18F82B9E		//BMS����1
	#define CAN_R_06 0x18F82C9E		//BMS����2 
	#define CAN_R_07 0x18F82D9E		//BMSϵͳ��Ϣ1
	#define CAN_R_08 0x18F82E9E		//BMSϵͳ��Ϣ2
	#define CAN_R_09 0x10F8159E		//BMSϵͳ��Ϣ3
	#define CAN_R_10 0x18F104D0		//VCU����1
	#define CAN_R_11 0x18F105D0		//VCU����2		
	#define CAN_R_12 0x18F106D0		//VCU����3
	#define CAN_R_13 0x18F107D0		//VCU����4
	#define CAN_R_14 0x18F108D0		//VCU����5
	#define CAN_R_15 0x18F109D0		//VCU����6
	#define CAN_R_16 0x18FF2880		//��Ʒ���� 
	#define CAN_R_17 0x18FF2080		//�豸��� 
//	#define CAN_R_17 0x18FF2E80		//�豸��� 
	#define CAN_R_18 0x18FF2180		//Ӳ���汾 
	#define CAN_R_19 0x18FF2280		//����汾 
	#define CAN_R_20 0x18FF2F80		//ʱ�����
	#define CAN_R_21 0x18FF2583		//�ն˱��
	#define CAN_R_22 0x18FF2483		//SIM ���� 
	#define CAN_R_23 0x18FF2783		//�ն������Ϣ 
	#define CAN_R_24 0x18FF1083		//�ն�״̬ 
	#define CAN_R_25 0x18FF0583		//�ն���ʱ 
	#define CAN_R_26 0x18F101FB		//VIN��  3
	#define CAN_R_27 0x18F102FB		//T-Box
	

//	#define CAN_T_1  0x00000000


	typedef enum 
	{///����CAN����
		ECAN_R_0101 = 0, 	
		ECAN_R_0102 , 	
		ECAN_R_0103 , 	
		ECAN_R_0104 , 	
		ECAN_R_0105 , 	
		ECAN_R_0106 , 	
		ECAN_R_0107 , 	
		ECAN_R_0108 , 	
		ECAN_R_0109 , 	
		ECAN_R_0110 , 	
		ECAN_R_0111 , 	
		ECAN_R_0112 , 	
		ECAN_R_0113 , 	
		ECAN_R_0114 , 	
		ECAN_R_0115 , 	
		ECAN_R_0116 , 	
		ECAN_R_0117 , 	
		ECAN_R_0118 , 	
		ECAN_R_0119 , 	
		ECAN_R_0120 , 
		ECAN_R_0121 , 	
		ECAN_R_0122 , 	
		ECAN_R_0123 , 	
//		ECAN_R_0124 , 	
//		ECAN_R_0125 , 	
//		ECAN_R_0126 , 	
//		ECAN_R_0127 , 	
//		ECAN_R_0128 , 	
//		ECAN_R_0129 , 	
//		ECAN_R_0130 , 
//		ECAN_R_0131 , 	
//		ECAN_R_0132 , 	
//		ECAN_R_0133 , 	
//		ECAN_R_0134 , 	
//		ECAN_R_0135 , 	
//		ECAN_R_0136 , 	
//		ECAN_R_0137 , 	
//		ECAN_R_0138 , 	
//		ECAN_R_0139 , 	
//		ECAN_R_0140 , 
//		ECAN_R_0141 , 	
//		ECAN_R_0142 , 	
//		ECAN_R_0143 , 	
//		ECAN_R_0144 , 	
//		ECAN_R_0145 , 	
//		ECAN_R_0146 , 	
//		ECAN_R_0147 , 	
//		ECAN_R_0148 , 	
//		ECAN_R_0149 , 	
//		ECAN_R_0150 , 
//		ECAN_R_0151 , 	
//		ECAN_R_0152 , 	
//		ECAN_R_0153 , 	
//		ECAN_R_0154 , 	
//		ECAN_R_0155 , 	
//		ECAN_R_0156 , 	
//		ECAN_R_0157 , 	
//		ECAN_R_0158 , 	
//		ECAN_R_0159 , 	
		ECAN_R_0160 , 	
		ECAN_R_0201 , 	//60
		ECAN_R_0202 , 	
//		ECAN_R_0203 , 	
//		ECAN_R_0204 , 	
//		ECAN_R_0205 , 	
//		ECAN_R_0206 , 	
//		ECAN_R_0207 , 	
//		ECAN_R_0208 , 	
//		ECAN_R_0209 , 	
//		ECAN_R_0210 , 	
//		ECAN_R_0211 , 	
//		ECAN_R_0212 , 	
//		ECAN_R_0213 , 	
//		ECAN_R_0214 , 	
//		ECAN_R_0215 , 	
//		ECAN_R_0216 , 	
//		ECAN_R_0217 , 	
//		ECAN_R_0218 , 	
//		ECAN_R_0219 , 	
//		ECAN_R_0220 , 
//		ECAN_R_0221 , 	
//		ECAN_R_0222 , 	
//		ECAN_R_0223 , 	
//		ECAN_R_0224 , 	
//		ECAN_R_0225 , 	
//		ECAN_R_0226 , 	
//		ECAN_R_0227 , 	
//		ECAN_R_0228 , 	
//		ECAN_R_0229 , 	
		ECAN_R_0230 ,	//89
		
		ECAN_R_03 ,		//BMS�����ѹ��ֵ����֡
		ECAN_R_04 ,		//BMS�¶�ģ�鼫ֵ����֡
		ECAN_R_05 ,		//BMS����1
		ECAN_R_06 ,		//BMS����2 
		ECAN_R_07 ,		//BMSϵͳ��Ϣ1
		ECAN_R_08 ,		//BMSϵͳ��Ϣ2
		ECAN_R_09 ,		//BMSϵͳ��Ϣ3
		ECAN_R_10 ,		//VCU����1
		ECAN_R_11 ,		//VCU����2		
		ECAN_R_12 ,		//VCU����3
		ECAN_R_13 ,		//VCU����4
		ECAN_R_14 ,		//VCU����5
		ECAN_R_15 ,		//VCU����6
		// ECAN_R_16 ,		//��Ʒ���� 
		// ECAN_R_17 ,		//�豸��� 
		// ECAN_R_18 ,		//Ӳ���汾 
		// ECAN_R_19 ,		//����汾 
		// ECAN_R_20 ,		//ʱ�����
		// ECAN_R_21 ,		//�ն˱��
		// ECAN_R_22 ,		//SIM ���� 
		// ECAN_R_23 ,		//�ն������Ϣ 
		// ECAN_R_24 ,		//�ն�״̬ 
		// ECAN_R_25 ,		//�ն���ʱ 
		ECAN_R_2601 ,		//VIN��  1
		ECAN_R_2602 ,		//VIN��  2
		ECAN_R_2603 ,		//VIN��  3
		ECAN_R_27 ,		//T-Box
		
		ECAN_R_ALL ,		//������
	}ECAN_R_INDEX;

	#define CAN_R1_VT 		0x1800D1F3		//�ж�BMS�����ѹ�¶�&��
	#define CAN_RX_ID_NUM			 			ECAN_R_ALL //107

	
	
	#define CAN_LOCK_DATA_LEN       8
	#define ONE_PACKET_LEN 			 		8	//12	//4+8
	#define CAN_BUF_LEN 			 			240	//(ONE_PACKET_LEN*CAN_RX_ID_NUM)
	
	
	typedef union{
		uint8 arry[ONE_PACKET_LEN];
		struct{
//			uint32 	id;				//��ַ		
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
		uint8 rx_can_buf_flag[CAN_RX_ID_NUM];	///CAN���ݽ��������ݱ�־  FF ������	
		uint8 can_open_flag;						///CAN���ߴ򿪱�־		1��
		uint8 rx_data_flag;							///��CAN���� ֵΪ���������±�  FF��Ч 
	}CAN_STRUCT;
	
	EXTN_CAN CAN_STRUCT can_struct;

	
	EXTN_CAN void CanRx(void);
	EXTN_CAN void CanMonitor(uint16 past_sec);

	
#endif

