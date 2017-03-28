
#include "tiza_include.h"


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



//��ȡָ����ַ����(32λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
static uint32 STMFLASH_ReadWord(uint32 faddr)
{
	return *(uint32*)faddr; 
}  



//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F

// 0 : success    1 : failed
uint8 CpuFlashWrite(uint32 Addr, uint8 *Data, uint16 Len)
{ 
  	FLASH_Status status = FLASH_COMPLETE;
	uint32 addrx = 0;
	uint32 endaddr = 0;	
	uint32 *Data_pt;
	bool flag = TRUE;
	
	if(Addr < STM32_FLASH_BASE || (Addr + Len) >= (STM32_FLASH_BASE + STM32_FLASH_SIZE))
		return 1;	//�Ƿ���ַ
		
	FLASH_Unlock();									//���� 
	//-FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
 		
	addrx = Addr;				//д�����ʼ��ַ
	endaddr = Addr + Len;	//д��Ľ�����ַ

	if(addrx < 0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
	{
		while(addrx < endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx) != 0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
				//-status = FLASH_EraseSector(STMFLASH_GetFlashSector(addrx), VoltageRange_3);//VCC=2.7~3.6V֮��!!
				status = FLASH_ErasePage(addrx);
				if(status != FLASH_COMPLETE)
				{
					flag = FALSE;
					break;	//����������
				}
			}
			else
			{
				addrx+=4;
			}
		} 
	}

	Data_pt = (uint32 *)Data;
	
	if(status == FLASH_COMPLETE)
	{
		while(Addr < endaddr)//д����
		{
			//-if(FLASH_ProgramByte(Addr, *Data) != FLASH_COMPLETE)//д������
			if(FLASH_ProgramWord(Addr, *Data_pt) != FLASH_COMPLETE)//д������
			{ 
				break;	//д���쳣
			}
			
			Addr += 4;
			Data_pt += 1;
		} 
	}
	
	//-FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����

	if(flag == TRUE)
	{
		return 0;
	}
	else
	{
		return 1;
	}
} 






