#ifndef _FLASH_H 
#define _FLASH_H

		#include "stm32f10x.h"
		

		
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
		 
		
		//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32)0x08040000) 	//扇区6起始地址, 128 Kbytes,,暂时存储数据区  
#define ADDR_FLASH_SECTOR_7     ((uint32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32)0x080E0000) 	//扇区11起始地址,128 Kbytes 

		
		
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
