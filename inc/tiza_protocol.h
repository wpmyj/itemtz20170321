
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "stdint.h"

	#ifdef PROTOCOL_GLOBAL
		#define PROTOCOL_EXTERN
	#else
		#define PROTOCOL_EXTERN extern
	#endif
/*
����������� 1
������ϵͳ����  1
����������		96   /4 = 24
̽�����				24	 /8 = 3
*/
	///��������ID	
	#define PRO_UP_LOGIN_ID						0X01		///��������
	#define PRO_UP_REAL_INFO_ID				0X02		///ʵʱ��Ϣ�ϱ�
	#define PRO_UP_LSNAL_INFO_ID			0X03		///������Ϣ�ϱ�
	#define PRO_UP_LOGOUT_ID					0X04		///�����ǳ�
	#define PRO_UP_HEARTBEAT_ID				0X07		///����
	#define PRO_UP_CHECKTIME_ID				0X08		///�ն�Уʱ
	//0x05~0x06 ƽ̨��������ռ��    �Զ���
	//0x09~0x7F ��������ϵͳԤ��    
	
	///��������ID
	#define PRO_DOWN_INQUIRY_ID				0X80		///��ѯ����
	#define PRO_DOWN_SET_ID						0X81		///��������
	#define PRO_DOWN_CONTROL_ID				0X82		///�����ն˿�������
	//0x83~0xBF ��������ϵͳԤ��    
	//0xC0~0xFE ƽ̨�����Զ�������    �Զ���
	
	///���Ԫ-Ӧ���־
	#define PRO_ACK_SUCCEED_FLAG			0X01		///���յ�����Ϣ��ȷ
	#define PRO_ACK_FAILED_FLAG				0X02		///����ʧ��
	#define PRO_ACK_VIN_ERR_FLAG			0X03		///VIN�ظ�����
	#define PRO_ACK_CMD_FLAG					0XFE		///��ʾ����Ϊ�����������Ӧ���
	
	///���ݼ��ܷ�ʽ
	#define PRO_DISENCRYT_MODE				0X01		///������
	#define PRO_RSAENCRYT_MODE				0X02		///RSA����
	#define PRO_AES128ENCRYT_MODE			0X03		///AES128����
	
	///��Ϣ���ͱ�־
	#define PRO_VEHICLE_FLAG					0X01		///��������
	#define PRO_MOTOR_FLAG						0X02		///�����������
	#define PRO_FUELCELL_FLAG					0X03		///ȼ�ϵ������
	#define PRO_ENGINE_FLAG						0X04		///����������
	#define PRO_POSITION_FLAG					0X05		///����λ��
	#define PRO_EXTREME_FLAG					0X06		///��ֵ����
	#define PRO_ALARM_FLAG						0X07		///��������
	#define PRO_BVOLTAGE_FLAG					0X08		///�ɳ�索��װ�õ�ѹ����
	#define PRO_BTEMPERATURE_FLAG			0X09		///�ɳ�索��װ���¶�����
	//0x0A~0x2F   ƽ̨����Э������
	//0x30~0x7F   Ԥ��
	//0x80~0xFE   �û��Զ���
	
	///
	#define MAX9999999	9999999
	#define MAX65531		65531
	#define MAX60000		60000
	#define MAX253			253
	#define MAX250			250
	///ʱ���ʽCMT+8
	typedef union {
		uint8 arry[6];
		
		struct{
			uint8 year;			//0-99
			uint8 month;		//1-12
			uint8 day;			//1-31
			uint8 hour;			//0-23
			uint8 minute;		//0-59
			uint8 second;		//0-59
		}Item;
	}PRO_TIME_UNION;
	
	///�����������ݸ�ʽ
	typedef union{
//		uint8 arry[34];
		uint8 arry[24];
		struct{
//			PRO_TIME_UNION time_union;					//���ݲɼ�ʱ��
			uint16 login_number;								//������ˮ��	+1 ���65531
			uint8  SIM_ICCID[20];								//�ն˴�SIM��õ�ICCID
			uint8  storage_num;									//�ɳ�索����ϵͳ��n=0~250
			uint8  storage_len;									//�ɳ�索����ϵͳ���볤��m=0~50;0��ʾ���ϴ��ñ���
//			uint8* storage_code;								//�ɳ�索��ϵͳ���룬�ն˴ӻ�ȡ
			
		}Item;
	}PRO_LOGIN_FORM_UNION;
	
	///ʵʱ��Ϣ�ϱ���ʽ
#if 1	//��Ϣ��	
	///��������
	typedef union {
		uint8 arry[20];
		
		struct{
			uint32 mileage;				//�ۼ����			0~9999999(0~999999.9km)    FFFFFFFE�쳣  FFFFFFFF��Ч
			uint16 speed;					//����					0~2200(0~220km/h) FFFE�쳣 FFFF��Ч
			uint16 voltage;				//�ܵ�ѹ				0~10000(0~1000V)  FFFE�쳣 FFFF��Ч
			uint16 current;				//�ܵ���				0~20000(-1000~+1000A)      FFFE�쳣      FFFF��Ч
			uint16 resistance;		//��Ե����			0~60000(0~60000kO)
			uint8  vehicle;				//����״̬			01����   02Ϩ��   03����   FE�쳣 FF��Ч
			uint8  charge;				//���״̬			01ͣ���� 02��ʻ�� 03δ��̬ 04���� FE�쳣 FF��Ч
			uint8  mode;					//����ģʽ			01����   02�춯   03ȼ��   FE�쳣 FF��Ч
			uint8  SOC;						//SOC						0~100(0%~100%)		FE�쳣   FF��Ч
			uint8  DC_statu;			//DC/DC״̬			01����   02�Ͽ�   FE�쳣   FF��Ч
			union08 stall;				//��λ					b5:1/0��/��������  b4:1/0��/���ƶ��� L:0�� 1~C�� D�� E�Զ�d�� Fͣ��p�� 
			uint8  Accelerate;		//����̤���г�ֵ0~100(0%~100%)    FE�쳣   FF��Ч
			uint8  braking;				//�ƶ�̤��״̬	0~100(0%~100%)    00�ƶ��� 65�ƶ���Ч    FE�쳣  FF��Ч
		}Item;
	}PRO_VEHICLE_UNION;
	///�����������
	#define PRO_MOTOR_NUMBER					1				//����������ݡ��������������	
	PROTOCOL_EXTERN uint8  g_motor_number;										//�����������	1~253
	typedef union{//��������ܳ���Ϣ�б�
		uint8 arry[12];
		
		struct{
			uint8  serial;										//����������	1~253
			uint8  status;										//״̬					01�ĵ� 02���� 03�ر�̬ 04׼��̬ FE�쳣 FF��Ч
			uint8  ctr_tem;										//�������¶�		0~250(-40~+210��)								FE�쳣 FF��Ч
			uint8  motor_tem;									//��������¶�	0~250(-40~+210��)								FE�쳣 FF��Ч
			uint16 rpm;												//ת��					0~65531(-20000~+45531r/min)	FFFE�쳣 FFFF��Ч
			uint16 torque;										//ת��					0~65531(-2000~+4553.1Nm)		FFFE�쳣 FFFF��Ч
			uint16 ctr_in_vol;								//�����������ѹ0~60000(0~6000V)						FFFE�쳣 FFFF��Ч
			uint16 ctr_bus_cur;								//������ĸ�ߵ���0~20000(-1000~+1000A)				FFFE�쳣 FFFF��Ч
		}Item;
	}PRO_MOTOR_UNION;
	
	///ȼ�ϵ������
	#define PRO_FTPROBE_NUMBER					24				//ȼ�ϵ�����ݡ����¶�̽�����	
	PROTOCOL_EXTERN uint8 g_TProbe_val[PRO_FTPROBE_NUMBER];		//̽���¶�ֵ	0~240(-40~+200��)
	typedef union{
		uint8 arry[18];
//		uint8 arry[18+PRO_FTPROBE_NUMBER];
		
		struct{
			uint16 voltage;										//��ѹ					0~20000(0~2000V)				FFFE�쳣 FFFF��Ч
			uint16 current;										//����					0~20000(0~2000A)				FFFE�쳣 FFFF��Ч
			uint16 consumpt;									//������				0~60000(0~600kg/100km)	FFFE�쳣 FFFF��Ч
			uint16 tprobe_num;								//�¶�̽�����	0~65531									FFFE�쳣 FFFF��Ч
//ע���arry	uint8 g_TProbe_val[PRO_FTPROBE_NUMBER];		//̽���¶�ֵ	0~240(-40~+200��)
			uint16 maxtem_val;								//��ϵͳ����¶�0~2400(-40~+200��)			FFFE�쳣 FFFF��Ч
			uint16 maxconcent;								//�������Ũ��	0~60000(0~50000mg/kg)		FFFE�쳣 FFFF��Ч
			uint16 maxpre_val;								//�������ѹ��	0~1000(0~100MPa)				FFFE�쳣 FFFF��Ч
			uint8  maxtem_id;								  //����¶�̽���0~252										FE�쳣 		FF��Ч
			uint8  maxconcent_id;						  //���Ũ�ȴ��к�0~252										FE�쳣 		FF��Ч
			uint8  maxpre_id;									//���ѹ�����к�0~252										FE�쳣 		FF��Ч
			uint8  preDCstatu;								//��ѹDCDC״̬	01���� 02�Ͽ� FE�쳣 FF��Ч
			}Item;
	}PRO_FUELCELL_UNION;	
	
	///����������
	typedef union{
		uint8 arry[5];
		
		struct{
			uint16 rpm;										//����ת��			0~60000(0~60000r/min)		FFFE�쳣 FFFF��Ч
			uint16 consumpt;							//������				0~60000(0~600L/100km)		FFFE�쳣 FFFF��Ч
			uint8  status;								//������״̬		01����̬ 02�ر�̬ FE�쳣 FF��Ч								
			}Item;
	}PRO_ENGINE_UNION;	
	
	///����λ��
	typedef union{
		uint8 arry[9];
		
		struct{		
			uint32 longitude;							//����			�Զ�Ϊ��λ*10^6����ȷ�������֮һ��
			uint32 latitude;							//γ��			ͬ��
			union08  status;							//��λ״̬	b0:1/0��/��Ч��λ	b1:1/0��/��γ	b2:1/0��/����		
			}Item;
	}PRO_POSTION_UNION;	
	
	///��ֵ����
	typedef union{
		uint8 arry[14];
		
		struct{
			uint8  maxvolsys_id;								//��ߵ�ѹ�����ϵͳ��	1~250										FE�쳣 		FF��Ч
			uint8  maxvolbat_id;								//��ߵ�ѹ��ص����		1~250										FE�쳣 		FF��Ч
			uint16 maxvolbat_val;								//��ߵ�ѹ��ص���ֵ		0~15000(0~15V)				FFFE�쳣 FFFF��Ч
			uint8  minvolsys_id;								//��͵�ѹ�����ϵͳ��	1~250										FE�쳣 		FF��Ч
			uint8  minvolbat_id;								//��͵�ѹ��ص����		1~250										FE�쳣 		FF��Ч
			uint16 minvolbat_val;								//��͵�ѹ��ص���ֵ		0~15000(0~15V)				FFFE�쳣 FFFF��Ч
			uint8  maxtemsys_id;								//����¶���ϵͳ��			1~250										FE�쳣 		FF��Ч
			uint8  maxtempro_id;								//����¶�̽���				1~250										FE�쳣 		FF��Ч
			uint8  maxtempro_val;								//����¶�̽��ֵ				0~250(-40~+210��)				FE�쳣 		FF��Ч
			uint8  mintemsys_id;								//����¶���ϵͳ��			1~250										FE�쳣 		FF��Ч
			uint8  mintempro_id;								//����¶�̽���				1~250										FE�쳣 		FF��Ч
			uint8  mintempro_val;								//����¶�̽��ֵ				0~250(-40~+210��)				FE�쳣 		FF��Ч
			}Item;
	}PRO_EXTREME_UNION;	
	
	///��������
	#define PRO_ASTORAGE_MAXNUMBER				4					//�������ݡ����ɳ�索��װ�ù��ϸ���	
	#define PRO_AMOTOR_MAXNUMBER					4					//�������ݡ�������������ϸ���	
//	#define PRO_AENFINE_MAXNUMBER					0					//�������ݡ������������ϸ���	
	#define PRO_AOTHERS_MAXNUMBER					4					//�������ݡ����������ϸ���	
	PROTOCOL_EXTERN uint32 g_storage_alr[PRO_ASTORAGE_MAXNUMBER];	//�ɳ�索��װ�ù�����	�Զ���
	PROTOCOL_EXTERN uint32 g_motro_alr[PRO_AMOTOR_MAXNUMBER];			//�������������				�Զ���
//	PROTOCOL_EXTERN uint32 g_engine_alr[PRO_AENFINE_MAXNUMBER];		//������������					�Զ���
	PROTOCOL_EXTERN uint32 g_others_alr[PRO_AOTHERS_MAXNUMBER];		//����������						�Զ���
	typedef union{
		uint8 arry[9];
		
		struct{
			union32 flag;					//ͨ�ñ�����־					b0~b18:0����/1����   b19~b31:Ԥ��
			uint8  rank;					//��߱����ȼ�					00�޹��� 1~3:�������� FE�쳣 FF��Ч		�ȼ������Զ���
			uint8  storagenum;		//�ɳ�索��װ�ù�����	0~252										FE�쳣 		FF��Ч
			uint8  motornum;			//�������������				0~252										FE�쳣 		FF��Ч
			uint8  enginenum;			//������������					0~252										FE�쳣 		FF��Ч
			uint8  othersnum;			//����������						0~252										FE�쳣 		FF��Ч
		}Item;
	}PRO_ALARM_UNION;	
	
	///�ɳ���ص�ѹ����
	#define PRO_BATTSYS_NUMBER						1						//�ɳ�索��װ�õ�ѹ���ݸ�ʽ�����ɳ�索����ϵͳ����	
	#define PRO_SIGBATT_MAXNUMBER					96					//��֡����������
	PROTOCOL_EXTERN uint16 g_sigbatt_val[PRO_BATTSYS_NUMBER][PRO_SIGBATT_MAXNUMBER];		//�����ص�ѹ		0~60000(0~60.000V)	FFFE�쳣 FFFF��Ч
	typedef union{//�ɳ�索����ϵͳ��ѹ��Ϣ�б�
		uint8 arry[10];
		
		struct{
			uint8  serial;								//��ϵͳ��					1~250
			uint8  framebatt_num;					//��֡����������	1~200										FE�쳣 FF��Ч
			uint16 voltage;								//װ�õ�ѹ					0~10000(0~1000V)			FFFE�쳣 FFFF��Ч
			uint16 current;								//װ�õ���					0~20000(-1000~+1000A)	FFFE�쳣 FFFF��Ч
			uint16 sigbatt_num;						//����������			1~65531								FFFE�쳣 FFFF��Ч
			uint16 framebatt_id;					//��֡��ʼ��غ�		1~65531								FFFE�쳣 FFFF��Ч	��֡���200��
		}Item;
	}PRO_VBATTSYS_UNION;
	
	///�ɳ�����¶�����
	/*�ɳ�索����ϵͳ����	ͬ��ѹ��������ʾ*/
	#define PRO_BTPROBE_MAXNUMBER					24				//�ɳ���¶�̽�����
	PROTOCOL_EXTERN uint8 g_btprobe_val[PRO_BATTSYS_NUMBER][PRO_BTPROBE_MAXNUMBER];		//��̽���¶�ֵ		0~250(-40~+210��)		FE�쳣 FF��Ч
	typedef union{//�ɳ�索����ϵͳ�¶���Ϣ�б�
		uint8 arry[3];
		
		struct{
			uint16 btprobe_num;						//�ɳ���¶�̽�����1~65531					FFFE�쳣 FFFF��Ч
			uint8  serial;								//��ϵͳ��					1~250
		}Item;
	}PRO_TBATTSYS_UNION;

	///�Զ�������
	#define PRO_SEFDAT_LEN	2						//1~65531		�Զ������ݳ���
	//PROTOCOL_EXTERN uint16 g_sefdata_len;								//1~65531		�Զ������ݳ���
	//PROTOCOL_EXTERN uint8  g_sefdata[PRO_SEFDAT_LEN];		//�Զ���
	
	///�����ǳ����ݸ�ʽ
	typedef union{
		uint8 arry[8];
		
		struct{
			PRO_TIME_UNION time_union;					//���ݲɼ�ʱ��
			uint16 logout_number;								//�ǳ���ˮ��  Ҫ�������ˮ��һ��
		}Item;
	}PRO_LOGOUT_FORM_UNION;
	
#endif		
	
	
	
	#define PRO_CMD_INDEX						 2	//�����־�±�
	#define PRO_ACK_INDEX						 3	//Ӧ���־�±�
	#define PRO_DATALEN_INDEX				22	//���ݳ����±�
	#define PRO_DATA_INDEX					24	//���ݿ�ʼ�±�
	#define PRO_TIME_INDEX1					24	//ʱ�俪ʼ�±�1
	
	PROTOCOL_EXTERN uint8 g_VIN[17];		//����ΨһVIN
	PROTOCOL_EXTERN PRO_TIME_UNION 				g_protime_union;
	PROTOCOL_EXTERN PRO_LOGIN_FORM_UNION	g_prologin_union;
	PROTOCOL_EXTERN PRO_VEHICLE_UNION			g_provehice_union;
	PROTOCOL_EXTERN PRO_MOTOR_UNION				g_promotor_union[PRO_MOTOR_NUMBER];
	PROTOCOL_EXTERN PRO_FUELCELL_UNION		g_profullcell_union;
	PROTOCOL_EXTERN PRO_ENGINE_UNION			g_proengine_union;
	PROTOCOL_EXTERN PRO_POSTION_UNION			g_propostion_union;
	PROTOCOL_EXTERN PRO_EXTREME_UNION			g_proextreme_union;
	PROTOCOL_EXTERN PRO_ALARM_UNION				g_proalarm_union;
	PROTOCOL_EXTERN PRO_VBATTSYS_UNION		g_provbattsys_union;
	PROTOCOL_EXTERN PRO_TBATTSYS_UNION		g_protbattsys_union;
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////
	
	///������ѯ����ID
	#define PRO_PARA_SAVEPERI_ID		0X01			///�ն˱��ر�������
	#define PRO_PARA_NORUPPERI_ID		0X02			///����ʱ�ϴ�����
	#define PRO_PARA_ALRUPPERI_ID		0X03			///����ʱ�ϴ�����
	#define PRO_PARA_DOMAINLEN_ID		0X04			///ƽ̨��������
	#define PRO_PARA_DOMAIN_ID			0X05			///ƽ̨����
	#define PRO_PARA_PORT_ID				0X06			///ƽ̨�˿�
	#define PRO_PARA_HDVERSION_ID		0X07			///�ն�Ӳ���汾 		R
	#define PRO_PARA_SFVERSION_ID		0X08			///�ն˹̼��汾			R
	#define PRO_PARA_HEARTPERI_ID		0X09			///�ն�������������
	#define PRO_PARA_T_ACK_TIM_ID		0X0A			///�ն�Ӧ��ʱʱ��
	#define PRO_PARA_P_ACK_TIM_ID		0X0B			///ƽ̨Ӧ��ʱʱ��
	#define PRO_PARA_N_LOG_TIM_ID		0X0C			///3��ʧ�ܵ�¼�´ε�¼ʱ����
	#define PRO_PARA_PDOMAINLEN_ID	0X0D			///����ƽ̨��������
	#define PRO_PARA_PDOMAIN_ID			0X0E			///����ƽ̨����
	#define PRO_PARA_PPORT_ID				0X0F			///����ƽ̨�˿�
	#define PRO_PARA_MONITOR_ID			0X10			///�Ƿ�������
	//0x11~0x7F		Ԥ��
	//0x80~0xFE		�û��Զ���
	
	///����ֵ
	#define PRO_DOMAINLEN_MAX			30											///ƽ̨�򹫹�ƽ̨������󳤶� �ݶ�
	#define PRO_PARA_UNION_LEN		90  //19+2*PRO_DOMAINLEN_MAX+11		///�������ýṹ�ܳ���

	typedef union{
		uint8 arry[PRO_PARA_UNION_LEN];
		
		struct{
			uint16 SavePeri;									//�ն˱��ر�������							0~60000(0~60000ms)			FFFE�쳣 FFFF��Ч
			uint16 NorUpPeri;									//����ʱ�ϴ�����								1~600(1~600s)						FFFE�쳣 FFFF��Ч
			uint16 AlrUpPeri;									//����ʱ�ϴ�����								0~60000(0~60000ms)			FFFE�쳣 FFFF��Ч
			uint8  DomainLen;									//ƽ̨��������									 										
			uint8  Domain[PRO_DOMAINLEN_MAX];	//ƽ̨����											 
			uint8  HeartPeri;									//�ն�������������							1~240(1~240s)						FE�쳣 		FF��Ч
			uint16 Port;											//ƽ̨�˿�											0~65531(0~15V)					FFFE�쳣 FFFF��Ч
			uint16 TAckTim;										//�ն�Ӧ��ʱʱ��							1~600(1~600s)						FFFE�쳣 FFFF��Ч
			uint16 PAckTim;										//ƽ̨Ӧ��ʱʱ��							1~600(1~600s)						FFFE�쳣 FFFF��Ч
			uint8  NLogTim;										//3��ʧ�ܵ�¼�´ε�¼ʱ����		1~240(1~24min)					FE�쳣 		FF��Ч
			uint8  PDomainLen;								//����ƽ̨��������							
			uint8  PDomain[PRO_DOMAINLEN_MAX];//����ƽ̨����									
			uint16 PPort;											//����ƽ̨�˿�									0~65531(0~15V)					FFFE�쳣 FFFF��Ч
			uint8  Monitor;										//�Ƿ���������								01��������� 02���ڳ��������	FE�쳣 		FF��Ч
			
			uint8  g_para_HDVers[5];		//Ӳ���汾��	Rֻ��			
			uint8  g_para_SFVers[5];		//�̼��汾��	Rֻ��
			uint8  check;							//У����  �����ȡ��
			}Item;
	}PRO_PARA_UNION;
	
	///��������ID
	#define PRO_CTR_UPGRADE_ID			0X01			///Զ������
	#define PRO_CTR_OFFTERM_ID			0X02			///�ն˹ػ�
	#define PRO_CTR_RSTTERM_ID			0X03			///�ն˸�λ
	#define PRO_CTR_FACTRST_ID			0X04			///�ն˻ָ���������
	#define PRO_CTR_DISLINKSER_ID		0X05			///�Ͽ�����ͨ����·
	#define PRO_CTR_ALRTERM_ID			0X06			///�ն˱�����Ԥ��
	#define PRO_CTR_MONITOR_ID			0X07			///�������������·
	//0x08~0x7F		Ԥ��
	//0x80~0xFE		�û��Զ���
	
	///Զ�����������ʽ
	#define PRO_UPGRADE_PARA_NUM		10				///������������
	typedef struct{		
		uint8  dial_APN[6];			//���ŵ�����
		uint8  user_name[16];		//�����û���
		uint8  pass_word[16];		//��������
		uint8  ip_domain[6];		//�����IPV4��ǰ��λΪ0;
		uint16 port;						//�˿ں�
		uint32 terminal_id;			//�ն�ID
		uint8  HDVers[5];//Ӳ���汾��*5
		uint8  SFVers[5];//����汾��*5
		uint8  file_path[64];		//����URL��ַ
		uint16 overtime;				//���ӵ�����������ʱ��min
		
		uint8 flag;							//����ӣ�1��Ҫ���� 2�������� 0��Ч
	}PRO_UPGREAD_STRUCT;
	
	
	
	PROTOCOL_EXTERN PRO_PARA_UNION 			g_propara_union;
	PROTOCOL_EXTERN PRO_UPGREAD_STRUCT	g_proupgread_struct;
	
	
	
////////////////////////////////////////////////////////////////////////////////	
	
	#define PRO_MAX_TX_BUF_ARRAY 					4			///���4�����ͻ���
	#define PRO_MAX_TX_BUF_COUNTER 				520		///ÿ�����ͻ������600�ֽ�
	#define PRO_NORMAL_RETX_TIME					8			///8���ط�����
	#define PRO_OVER3_RELOGIN_TIME				1800	///���ε�¼ʧ�ܺ�30min���µ�¼
//	#define PRO_ACC_ON_MAX_TX_BUF_COUNTER	256
	
	typedef struct
	{
		uint8  re_tx_buf[PRO_MAX_TX_BUF_ARRAY][PRO_MAX_TX_BUF_COUNTER];
		uint16 re_tx_len[PRO_MAX_TX_BUF_ARRAY];
		uint8  re_tx_counter[PRO_MAX_TX_BUF_ARRAY];
		uint8  re_tx_sec_counter[PRO_MAX_TX_BUF_ARRAY];						//�ط���������
		uint8  re_tx_time[PRO_MAX_TX_BUF_ARRAY];									//�ط�ʱ����	
		uint8  re_tx_full_flag[PRO_MAX_TX_BUF_ARRAY];							//�����ݱ�־
	}TX_STRUCT;
	typedef struct
	{
		// uint8 aquire_comm_para_flag;///��ȡͨ�Ų���
		// uint8 comm_para_ip[4];///����IP
		// uint8 comm_para_port[2];///����PORT
		uint8  try_login_statu;								///��¼״̬��־ 0��ʼ 1ʹ�� 2���ڵ�¼ 3�ѵ�¼ 4���ڵǳ� 5�ѵǳ� 6��Ч
		uint8  login_center_flag;							///��¼���ĳɹ���־
		uint8  tx_lsnal_data_flag;						///��һ��ָ��Ӧ��ɹ�ʹ�ܼ������ä�����ݷ���   1:ʹ��
		// uint8 setup_link_falg;///TRUE,�ɹ������Ľ�������
		// uint8 link_center_ip_index;///TRUE,���������ģ�FALSE�Ǹ�����
		// uint8 link_master_ip_counter;///���������ļ���
		
		// uint8 tx_flash_lsnal_falg;///���͵ı�����FLASH�е�ä�����ݱ�־
		uint8 tx_one_lsnal_falg;							///��EXTFLASH�ж�����һ��ä����¼���ڷ���   		1:�ڷ���	0��Ч
		// uint16 no_rx_data_sec_counter;///û���յ����ݼ���
		// uint16 tx_seq;
		// uint16 tx_len;
		// uint8  tx_cmd;
		 TX_STRUCT tx_struct;
	}PRO_STRUCT;
	
	
	#define LSNAL_PAGE_SIZE 					0x200				///�ݶ�512��ÿ�η�8��,<<9
//	#define SYS_LSNAL_SPIMAXSECTOR		1536				///96*16  0~1535
	#define SYS_LSNAL_SPIMAXINDEX			12288				///96*16*8
	#define SYS_LSNAL_SPISTART_ADDR		SPIFLASH_LSNAL_START_ADD
	#define SYS_LSNAL_SPIEND_ADDR			SPIFLASH_LSNAL_END_ADD
	#define SYS_LSNAL_SPIINDEX_ADDR		SPIFLASH_LSNAL_ADD	///���1M������
	#define SYS_LSNAL_SPIINDEXSECTOR	SPIFLASH_LSNAL_SEC	///ҳ���±걣����ʼ�κ�
	typedef struct
	{
		uint8  data[LSNAL_PAGE_SIZE];		//һ��ä�����ݴ�С
		uint32 headindex;							  //����(ÿ��16�Σ�ÿ��4Kbety)
		uint32 tailindex;								//����(ÿ��16�Σ�ÿ��4Kbety)
//		uint16 headsector;							//�κ�(�ݸ�6M��0~1535)
//		uint16 tailsector;							//�κ�(�ݸ�6M��0~1535)
	}SYS_LSNAL_STRUCT;
		
		
	
	PROTOCOL_EXTERN SYS_LSNAL_STRUCT g_syslsnal_struct;
	PROTOCOL_EXTERN PRO_STRUCT			 g_pro_struct;
		
		
	#define VALID_VAL_2A					0x2A  //syspara.h	�У�ע���ض���
	#define TMP_BUFF_MAXLEN				LSNAL_PAGE_SIZE	
	typedef struct	
	{//ע���ʼ��
		uint8  vehicle_start_flag;			//����������־		1����			0δ����
		uint8  link_center_flag;				//����ƽ̨��־		1������		0δ����
		uint8  updata_sending;					//�����ϴ���־		1�����ϴ� 0����  
		uint8  updata_noacksend;				//�ϴ�����ҪACK����		1��Ҫ�ϴ� 2���ڷ��� 0����  
	}SYS_PRIVATE_PARA_STRUCT;
	PROTOCOL_EXTERN SYS_PRIVATE_PARA_STRUCT	g_sysprivatepara_struct;
	PROTOCOL_EXTERN void ProWrite_VIN(uint8* str);
	PROTOCOL_EXTERN uint16 Pro_RealTime_Data(uint8 data[]);
	PROTOCOL_EXTERN void ProParaInit(void);
	PROTOCOL_EXTERN void ProLsnalHeadTailSave(void);
	PROTOCOL_EXTERN void ProUpRealFormation(void);
	PROTOCOL_EXTERN void ProUpLogout(void);
	PROTOCOL_EXTERN void ProUpHeartBeat(void);
	PROTOCOL_EXTERN void ProUpCheckTime(void);
	PROTOCOL_EXTERN void ProConstructFrameHead(uint8 data[],uint16 tx_len,uint8 cmd);
	PROTOCOL_EXTERN void ProConstructFrameTail(uint8 data[],uint16 tx_len);
	PROTOCOL_EXTERN void ProPacket(uint8 tx_data[],uint16 tx_len,uint8 tx_cmd,uint8 ack_flag);
	PROTOCOL_EXTERN void ProPeriodTx(uint16 past_sec);
	PROTOCOL_EXTERN void ProProcess(uint8 data[], uint16 len, uint16 colon);
	
#endif
