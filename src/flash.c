
#include "include.h"


uint8 FlashErase(uint32 page_addr)
{
	FLASH_Status status;
	uint8 res = FALSE;
	
	FLASH_Unlock();	
	status = FLASH_ErasePage(0x8000000 + page_addr);
	FLASH_Lock();
	
	if(status == FLASH_COMPLETE)
	{
		res = TRUE;
	}
	
	return res;
}
uint8 FlashWrite(uint32 start_addr,uint8 wr_data[], uint16 len)
{
	uint8 res;
	uint32 i,dst,src;
	
	res = TRUE;
	dst = 0x8000000 + start_addr;		
	src = (uint32)wr_data;
	
	FLASH_Unlock();
	for(i=0;i<len;i+=4)
	{
		FLASH_ProgramWord(dst, *(u32*)src);
		if(*(u32*)dst != *(u32*)src)
		{
			res = FALSE;
			break;
		}
		dst += 4;
		src += 4;
	}
	FLASH_Lock();
	
	return res;
}
void FlashRead(uint32 start_addr, uint8 rd_data[], uint16 len)
{	
	uint16 i=0;
	uint32 tmp_addr;
	
	tmp_addr = 0x8000000 + start_addr;
	
 	for(i=0;i<len;i++)
	{
		rd_data[i] = *(uint8 *)tmp_addr;
		tmp_addr++;
	}
}



