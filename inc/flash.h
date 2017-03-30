#ifndef _FLASH_H 
#define _FLASH_H

		#include "stm32f10x.h"
		

		
			/***************** �ڲ�FLASH���򻮷� ********************/
			/********************************************************/
			/*0x00000												*/
			/*			 BOOT����	   ����0 (8K)					*/
			/*0x01FFF												*/
			/********************************************************/
			/*0x02000												*/
			/*			 APP����   		����1 - ����4 (112K) 		*/
			/*0x1DFFF												*/
			/********************************************************/
			/*0x1E000												*/
			/*			 �û��Զ���1	 ����5 - ����6 (8K)	 		*/
			/*0x1FFFF												*/
			/********************************************************/
			/*0x20000												*/
			/*			 ��������洢  ����7 - ����8 (112K) 		*/
			/*0x3BFFF												*/
			/********************************************************/
			/*0x3C000												*/
			/*			 �û��Զ���2   ����9 - ����11 (16K)			*/
			/*0x40000												*/
			/********************************************************/
		
		/**************************** ע�� *****************************/
		/*															   */	
		/*	��Ϊ�ڲ�FLASH�������Ƚϴ�RAMС���洢����ʱ���ʺ�������,  */
		/*	���Դ洢����ʱ��ѡ����ʴ�С�������洢���ݣ���д������ʱ�� */
		/*	�Ὣ��ַ��������������Ȼ��д���µ�����					   */ 
		/***************************************************************/
		   
		
		//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
#define STM32_FLASH_SIZE 0x80000 		//STM32 FLASH�Ĵ�С1M
		 
		
		//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((uint32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((uint32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((uint32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((uint32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((uint32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((uint32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((uint32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes,,��ʱ�洢������  
#define ADDR_FLASH_SECTOR_7     ((uint32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((uint32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((uint32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((uint32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((uint32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes 

		
		
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
