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
			uint32 program_update_flag;///程序更新标志
			uint32 program_total_size;///字节总数
			uint32 sys_para_init_flag;///参数重新初始化标志
			
			uint8 total_packet_num;///总包数
			uint8 cur_rx_packet_index;///当前接收包序号
			uint32 boot_ms_delay_counter;///boot 毫秒延时
			uint32 sys_tick_ms_counter;	 ///毫秒计数
		}SYS_BOOT_PARA_STRUCT;///系统启动参数

		BOOT_EXTERN SYS_BOOT_PARA_STRUCT boot_struct;

#endif

