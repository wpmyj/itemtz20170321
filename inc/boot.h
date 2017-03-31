#ifndef _BOOT_H 
#define _BOOT_H

#include <stdint.h>

		#ifdef  BOOT_GLOBAL
			#define BOOT_EXTERN
		#else
			#define BOOT_EXTERN extern
		#endif


		#define VALID_VAL_DWORD_AA		0xAAAAAAAA
		#define INVALID_VAL_DWORD_55	0x55555555
		#define VALID_VAL_AA			0xAA
		#define INVALID_VAL_55			0x55	
		#define VALID_VAL_2A			0x2A
		#define INVALID_VAL_FF			0xFF

		typedef struct
		{
			uint32 program_update_flag;///������±�־
			uint32 program_total_size;///�ֽ�����
			uint32 sys_para_init_flag;///�������³�ʼ����־
			
			uint8 total_packet_num;///�ܰ���
			uint8 cur_rx_packet_index;///��ǰ���հ����
			uint32 boot_ms_delay_counter;///boot ������ʱ
			uint32 sys_tick_ms_counter;	 ///�������
		}SYS_BOOT_PARA_STRUCT;///ϵͳ��������

		BOOT_EXTERN SYS_BOOT_PARA_STRUCT boot_struct;

#endif

