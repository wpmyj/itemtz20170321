
#include "include.h"


/******************************************************
SPI_FLASH初始化函数

******************************************************/
void SpiFramInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  SPI_InitTypeDef  SPI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  SPI_FRAM_CS_HIGH();

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  ///时钟悬空低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; ///数据在第1个时钟边沿捕获
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;    ///NSS-片选，由软件管理
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;			//72/4=18MHZ
  ///SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;			///16/2=8MHZ

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_Cmd(SPI1, ENABLE);
}

/******************************************************
SPI_FLASH 写字节
SPI_FLASH 读字节

******************************************************/
uint8 SpiFramWrByte(uint8 send_data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, send_data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	return SPI_I2S_ReceiveData(SPI1);
}
uint8 SpiFramRdByte(void)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,0xAA);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	return SPI_I2S_ReceiveData(SPI1); 
}
/******************************************************
SPI_FLASH 读状态  return 状态寄存器
SPI_FLASH 等空闲

******************************************************/
uint8 SpiFramRdRdsr(void)
{
	uint8 data;

	SPI_FRAM_CS_LOW();
	SpiFramWrByte(RDSR_INST);
	data = SpiFramRdByte();
	SPI_FRAM_CS_HIGH();
	
	return data;
}
void SpiFramWaitFree(void)
{
	uint8 status = 0;
	
	while(1)
	{
		status=SpiFramRdRdsr();
		
		if((status & 0x01) == 0)
		{
			break;
		}
	}
}

/******************************************************
SPI_FLASH 写禁止
SPI_FLASH 写使能

******************************************************/
void SpiFramWrWrdi(void)///写禁能
{
	SpiFramWaitFree();
	SPI_FRAM_CS_LOW();
	SpiFramWrByte(WRDI_INST);
	SPI_FRAM_CS_HIGH();
}

void SpiFramWrWren(void)///FRAM写使能
{
	SpiFramWaitFree();
	SPI_FRAM_CS_LOW();
	SpiFramWrByte(WREN_INST);
	SPI_FRAM_CS_HIGH();
}

/******************************************************
SPI_FLASH 页写

******************************************************/
uint8 SpiFramPageWrite(uint16 wr_addr,uint8 *wr_data,uint16 len)
{
	uint8 i,j,res;
	uint8 tmp_data[BYTES_OF_ONE_PAGE];

	if(len == 0x0000)
	{
		return FALSE;
	}
	
///	__disable_irq();			///关中断
	
	for(i=0;i<FRAM_MAX_WR_TIME;i++)
	{
		SpiFramWrWren();		///写使能
		SpiFramWaitFree();
		
		SPI_FRAM_CS_LOW();
		SpiFramWrByte(WRITE_INST);			///写指令
		
		SpiFramWrByte((uint8)(wr_addr >> 8));
		SpiFramWrByte((uint8)(wr_addr));	///写入地址

		for(j=0;j<len;j++)
		{
			SpiFramWrByte(wr_data[j]);	///写入数据
		}

		SPI_FRAM_CS_HIGH();

		res = SpiFramRead(wr_addr,tmp_data,len);

		if(res)
		{
			for(j=0;j<len;j++)
			{
				if(wr_data[j] != tmp_data[j])
				{
					break;
				}
			}
			if(j == len)
			{
				break;
			}
		}	
	}

	if(i == FRAM_MAX_WR_TIME)
	{
		res = FALSE;
	}
	else
	{
		res = TRUE;
	}	

///	__enable_irq();		///开中断

	return res;
}


/******************************************************
SPI_FLASH 对外写接口

return：true ！=0、false ==0
******************************************************/
uint8 SpiFramWrite(uint16 wr_addr,uint8 *wr_data,uint16 len)
{
	uint16 tmp_len,res;
	
	if(len == 0x0000)
	{
		return FALSE;
	}
	
	res = TRUE;
	
	tmp_len = BYTES_OF_ONE_PAGE - wr_addr % BYTES_OF_ONE_PAGE;///页写
	if(len>tmp_len)
	{
		res = SpiFramPageWrite(wr_addr,wr_data,tmp_len);
		if(res)
		{
			len -= tmp_len;
			wr_addr += tmp_len;
			wr_data += tmp_len;
		}
	}
	
	if(res)
	{
		while(len>0)
		{
			if(len > BYTES_OF_ONE_PAGE)
			{
				tmp_len = BYTES_OF_ONE_PAGE;
			}
			else
			{
				tmp_len = len;
			}
			res = SpiFramPageWrite(wr_addr,wr_data,tmp_len);
			if(res)
			{
				len -= tmp_len;
				wr_addr += tmp_len;
				wr_data += tmp_len;
			}
			else
			{
				break;
			}
		}
	}
	
	if(len == 0)
	{
		res = TRUE;
	}
	else
	{
		res = FALSE;
	}
	
	return res;	
}
/******************************************************
SPI_FLASH 对外读接口

return：true ！=0、false ==0
******************************************************/
uint8 SpiFramRead(uint16 rd_addr, uint8 *rd_data, uint16 len)
{
	uint8 i,j,k,res;
	uint8 tmp_len,tmp_data[2][BYTES_OF_ONE_PAGE],*p_tmp;
	uint16 tmp_addr;

	if(len == 0x00)
	{
		return FALSE;
	}
	
	tmp_addr = rd_addr;
	
	while(len > 0)
	{
		if(len > BYTES_OF_ONE_PAGE)
		{
			tmp_len = BYTES_OF_ONE_PAGE;
		}
		else
		{
			tmp_len = len;
		}

		for(k=0;k<FRAM_MAX_WR_TIME;k++)
		{
			for(j=0;j<2;j++)
			{
				p_tmp = &tmp_data[j][0];
				SpiFramWaitFree();
				
				SPI_FRAM_CS_LOW();
				SpiFramWrByte(READ_INST);			///读指令
				SpiFramWrByte((uint8)(tmp_addr >> 8));
				SpiFramWrByte((uint8)(tmp_addr));	///读出地址

				for(i=0;i<tmp_len;i++)
				{
					p_tmp[i] = SpiFramRdByte();
				}

				SPI_FRAM_CS_HIGH();
			}

			for(i=0;i<tmp_len;i++)
			{
				if(tmp_data[0][i] != tmp_data[1][i])
				{
					break;
				}
			}

			if(i == tmp_len)
			{
				break;
			}
		}

		if(k == FRAM_MAX_WR_TIME)
		{
			break;
		}
		else
		{
			for(i=0;i<tmp_len;i++)
			{
				*rd_data++ = tmp_data[0][i];
			}
			tmp_addr += BYTES_OF_ONE_PAGE;
			len -= tmp_len;
		}		
	}

	if(len == 0)
	{
		res = TRUE;
	}
	else
	{
		res = FALSE;
	}	

	return res;
}
