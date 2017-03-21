#ifndef _FLASH_H 
#define _FLASH_H

		#include "stm32f10x.h"
		
		
		
		#define ST32_PAGE_SIZE				0x800 
		
		
		#define FlASH_STORE_START_ADDR		0x20000
		
		#define SYS_BOOT_PARA_STRUCT_START_ADDR		(FlASH_STORE_START_ADDR-0x2000)

		#define FLASH_STORE_END_ADDR		0x40000
		#define FLASH_LSNAL_START_ADDR		FlASH_STORE_START_ADDR
		#define FLASH_LSNAL_MAX_PAGES		((FLASH_STORE_END_ADDR - FlASH_STORE_START_ADDR -  0x4000)/0x200)

		uint8 FlashErase(uint32 page_addr);
		uint8 FlashWrite(uint32 start_addr,uint8 wr_data[], uint16 len);
		void FlashRead(uint32 start_addr, uint8 rd_data[], uint16 len);
#endif
