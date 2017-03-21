#include <stdint.h>

#ifndef _FRAM_H 
#define _FRAM_H

	#define WREN_INST 0X06
	/// Write enable latch instruction (WREN)
	#define WRDI_INST 0X04
	///Write disable latch instruction (WRDI)
	#define WRSR_INST 0X01
	///Write status register instruction (WRSR)
	#define RDSR_INST 0X05
	///Read status register instruction (RDSR)
	#define WRITE_INST 0X02
	///Write memory instruction (WRITE)
	#define READ_INST 0X03
	///Read memory instruction (READ)
	#define STATUS_REG 0X00
	///Status register,BP1 and BP0 are all zero,no menory write protection

	#define BYTES_OF_ONE_PAGE 	32
	#define FRAM_MAX_WR_TIME  	3

	#define SPI_FRAM_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
	#define SPI_FRAM_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)


	void SpiFramInit(void);
	uint8 SpiFramWrByte(uint8 send_data);
	uint8 SpiFramRdByte(void);
	uint8 SpiFramRdRdsr(void);
	void SpiFramWaitFree(void);
	void SpiFramWrWrdi(void);
	void SpiFramWrWren(void);
	uint8 SpiFramPageWrite(uint16 wr_addr,uint8 *wr_data,uint16 len);
	uint8 SpiFramWrite(uint16 wr_addr,uint8 *wr_data,uint16 len);
	uint8 SpiFramRead(uint16 rd_addr, uint8 *rd_data, uint16 len);
#endif
