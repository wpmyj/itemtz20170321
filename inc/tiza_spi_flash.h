#ifndef __TIZA_SPI_FLASH_H
#define __TIZA_SPI_FLASH_H

#include <stdint.h>

#ifdef SPI_FLASH_GLOBAL
	#define EXTERN_SPI_FLASH
#else
	#define EXTERN_SPI_FLASH extern
#endif

// һ��SECTOR��СΪ4096
#define FLASH_SECTOR_SIZE	4096

#define SPI_FRAM_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FRAM_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)				
#define SPI_CS_ENABLE						SPI_FRAM_CS_LOW()       
#define SPI_CS_DISABLE					SPI_FRAM_CS_HIGH()      

EXTERN_SPI_FLASH uint8 ExteFlashInit(void);
EXTERN_SPI_FLASH uint8 ExteFlashWrite(uint32 WriteAddr,uint8* pBuffer,uint16 NumByteToWrite);	//��������д�����Ƿ����
EXTERN_SPI_FLASH uint8 ExteFlashRead(uint32 ReadAddr, uint8* pBuffer, uint16 NumByteToRead);   //��ȡflash
EXTERN_SPI_FLASH uint8 ExteFlashEraseChip(void);    	  				//��Ƭ�������ȴ�ʱ�䳤
EXTERN_SPI_FLASH uint8 ExteFlashEraseSector(uint16 SectorNum);	//��������������һ������������ʱ��:150ms

EXTERN_SPI_FLASH uint8 ExteFlashPowerDown(void);        				//�������ģʽ
EXTERN_SPI_FLASH uint8 ExteFlashWakeUp(void);										//����


#endif
