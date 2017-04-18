#ifndef _FLASH_H 
#define _FLASH_H

#include "stdint.h"

	#ifdef INTFLASH_STRUCT_GLOBAL
		#define INTFLSAH_EXTERN
	#else
		#define INTFLSAH_EXTERN extern
	#endif
		
	
	/***************** 内部FLASH区域划分 ********************/
	/********************************************************/
	/*0x00000												*/
	/*			 BOOT程序	   扇区0 (8K)					*/
	/*0x01FFF												*/
	/********************************************************/
	/*0x02000												*/
	/*			 APP程序   		扇区1 - 扇区4 (112K) 		*/
	/*0x1DFFF												*/
	/********************************************************/
	/*0x1E000												*/
	/*			 用户自定义1	 扇区5 - 扇区6 (8K)	 		*/
	/*0x1FFFF												*/
	/********************************************************/
	/*0x20000												*/
	/*			 升级程序存储  扇区7 - 扇区8 (112K) 		*/
	/*0x3BFFF												*/
	/********************************************************/
	/*0x3C000												*/
	/*			 用户自定义2   扇区9 - 扇区11 (16K)			*/
	/*0x40000												*/
	/********************************************************/

/**************************** 注意 *****************************/
/*															   */	
/*	因为内部FLASH的扇区比较大，RAM小，存储数据时不适合作缓存,  */
/*	所以存储数据时，选择合适大小的扇区存储数据，在写入数据时， */
/*	会将地址所在扇区擦除，然后写入新的数据					   */ 
/***************************************************************/
		   
			//FLASH起始地址
	#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
	#define STM32_FLASH_SIZE 0x80000 		//STM32 FLASH的大小1M
			 
			

	
	
	#define ST32_PAGE_SIZE										0x800 	
	#define FlASH_STORE_START_ADDR						0x20000	
	#define SYS_BOOT_PARA_STRUCT_START_ADDR		(FlASH_STORE_START_ADDR-0x2000)
	#define FLASH_STORE_END_ADDR							0x40000
	#define FLASH_LSNAL_START_ADDR						FlASH_STORE_START_ADDR
	#define FLASH_LSNAL_MAX_PAGES							((FLASH_STORE_END_ADDR - FlASH_STORE_START_ADDR -  0x4000)/0x200)

	INTFLSAH_EXTERN uint8 FlashErase(uint32 page_addr);
	INTFLSAH_EXTERN uint8 FlashWrite(uint32 start_addr,uint8 wr_data[], uint16 len);
	INTFLSAH_EXTERN void FlashRead(uint32 start_addr, uint8 rd_data[], uint16 len);
	INTFLSAH_EXTERN uint8 CpuFlashWrite(uint32 Addr, uint8 *Data, uint16 Len);	
		
		
#endif
