#ifndef __TIZA_SPI_FLASH_H
#define __TIZA_SPI_FLASH_H

#include <stdint.h>

#ifdef SPI_FLASH_GLOBAL
	#define EXTERN_SPI_FLASH
#else
	#define EXTERN_SPI_FLASH extern
#endif

// 一个SECTOR大小为4096
#define FLASH_SECTOR_SIZE	4096

#define SPI_FRAM_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FRAM_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)				
#define SPI_CS_ENABLE						SPI_FRAM_CS_LOW()       
#define SPI_CS_DISABLE					SPI_FRAM_CS_HIGH()      

EXTERN_SPI_FLASH uint8 ExteFlashInit(void);
EXTERN_SPI_FLASH uint8 ExteFlashWrite(uint32 WriteAddr,uint8* pBuffer,uint16 NumByteToWrite);	//不会检查所写扇区是否擦除
EXTERN_SPI_FLASH uint8 ExteFlashRead(uint32 ReadAddr, uint8* pBuffer, uint16 NumByteToRead);   //读取flash
EXTERN_SPI_FLASH uint8 ExteFlashEraseChip(void);    	  				//整片擦除，等待时间长
EXTERN_SPI_FLASH uint8 ExteFlashEraseSector(uint16 SectorNum);	//扇区擦除，擦除一个扇区的最少时间:150ms

EXTERN_SPI_FLASH uint8 ExteFlashPowerDown(void);        				//进入掉电模式
EXTERN_SPI_FLASH uint8 ExteFlashWakeUp(void);										//唤醒


#endif
