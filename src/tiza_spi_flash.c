#define SPI_FLASH_GLOBAL

#include "tiza_include.h"

//#define SPIFLASH_DEBUG

//ָ���
#define W25X_WriteEnable			0x06 
#define W25X_WriteDisable			0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData					0x03 
#define W25X_FastReadData			0x0B 
#define W25X_FastReadDual			0x3B 
#define W25X_PageProgram			0x02 
#define W25X_BlockErase				0xD8 
#define W25X_SectorErase			0x20 
#define W25X_ChipErase				0xC7 
#define W25X_PowerDown				0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID					0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


static void SpiFlashWaitBusy(void);           	//�ȴ�����
static void SpiFlashWriteEnable(void);  		//дʹ�� 
static void SpiFlashWriteDisable(void);			//д����
static uint8 SpiFlashReadSR(void);        		//��ȡ״̬�Ĵ��� 
static void SpiFlashWriteSR(uint8 sr);  		//д״̬�Ĵ���


//static uint8 SPIFLASHBUFFER[4096];		// ����

#define SPI_FLASH_CAPCITY	(8*1024*1024)

static void delay_ms(uint16 time)
{ 
  uint32 tmp;// = time * 21;
  while(time--)
  {
    tmp = 21000;
    while(tmp--)
    {
      __NOP();
      __NOP();
      __NOP();
      __NOP();
    }
  }
}


//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 


//��ʼ��SPI FLASH
uint8 ExteFlashInit(void)
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

  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  ///ʱ�����յ�
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; ///�����ڵ�1��ʱ�ӱ��ز���
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;    ///NSS-Ƭѡ�����������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;			//72/4=18MHZ
  ///SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;			///16/2=8MHZ

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_Cmd(SPI1, ENABLE);
	return 1;
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8 SPI1_ReadWriteByte(uint8 TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  

	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  

	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
}
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8 SpiFlashReadSR(void)   
{  
	uint8 byte=0;   
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     
	return byte;   
} 

//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void SpiFlashWriteSR(uint8 sr)   
{   
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
}   

//W25QXXдʹ��	
//��WEL��λ   
void SpiFlashWriteEnable(void)   
{
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
} 

//W25QXXд��ֹ	
//��WEL����  
void SpiFlashWriteDisable(void)   
{  
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
} 		

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
#if 0
uint16 SpiFlashReadID(void)
{
	uint16 Temp = 0;	  
	SPI_CS_ENABLE;				    
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_CS_DISABLE;				    
	return Temp;
}   		    
#endif

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
uint8 ExteFlashRead(uint32 ReadAddr, uint8* pBuffer, uint16 NumByteToRead)   
{ 
 	uint16 i;   										    
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
	SPI1_ReadWriteByte((uint8)((ReadAddr)>>16));  //����24bit��ַ    
	SPI1_ReadWriteByte((uint8)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((uint8)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
			pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
	}
	SPI_CS_DISABLE;  				    	      

	return 1;
}  

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
static void SpiFlashWritePage(uint8* pBuffer,uint32 WriteAddr,uint16 NumByteToWrite)
{
 	uint16 i;  
	SpiFlashWriteEnable();                  //SET WEL 
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
	SPI1_ReadWriteByte((uint8)((WriteAddr)>>16)); //����24bit��ַ    
	SPI1_ReadWriteByte((uint8)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((uint8)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ 
	SpiFlashWaitBusy();					   //�ȴ�д�����
} 

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
uint8 ExteFlashWrite(uint32 WriteAddr,uint8* pBuffer,uint16 NumByteToWrite)   
{ 			 		 
	uint16 pageremain;	   

	if(WriteAddr + NumByteToWrite >= SPI_FLASH_CAPCITY)
		return 0;
	
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		SpiFlashWritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    

	return 1;
} 

//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)  
#if 0
void SpiFlashWrite(uint32 WriteAddr, uint8* pBuffer, uint16 NumByteToWrite)   
{ 
	uint32 secpos;
	uint16 secoff;
	uint16 secremain;	   
 	uint16 i;    
	uint8 * W25QXX_BUF;	
  
  W25QXX_BUF=SPIFLASHBUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
	
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		SpiFlashRead(secpos*4096, W25QXX_BUF, 4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SpiFlashEraseSector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			SpiFlashWriteNoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else SpiFlashWriteNoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

			pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
			NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
#endif

//��������оƬ		  
//�ȴ�ʱ�䳬��...
uint8 ExteFlashEraseChip(void)   
{                                   
	SpiFlashWriteEnable();                  //SET WEL 
	SpiFlashWaitBusy();   
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	SpiFlashWaitBusy();   				   //�ȴ�оƬ��������

	return 1;
}   

//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
//0ʧ�� 1�ɹ�
uint8 ExteFlashEraseSector(uint16 SectorNum)   
{
	uint32 Dst_Addr;
	
	//����falsh�������,������   
	#ifdef SPIFLASH_DEBUG
 	printf("spi flash sector: 0x%X\r\n",SectorNum);	  
	#endif

	if(SectorNum * FLASH_SECTOR_SIZE > SPI_FLASH_CAPCITY)
		return 0;
	
	Dst_Addr = SectorNum * FLASH_SECTOR_SIZE;			// 4096 bytes per page

	SpiFlashWriteEnable();                  //SET WEL 	 
	SpiFlashWaitBusy();   
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
	SPI1_ReadWriteByte((uint8)((Dst_Addr)>>16));  //����24bit��ַ    
	SPI1_ReadWriteByte((uint8)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((uint8)Dst_Addr);  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	SpiFlashWaitBusy();   				   //�ȴ��������

	return 1;
}  

//�ȴ�����
void SpiFlashWaitBusy(void)   
{   
	while((SpiFlashReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  

//�������ģʽ
uint8 ExteFlashPowerDown(void)   
{ 
  	SPI_CS_ENABLE;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
    delay_ms(1);                               //�ȴ�TPD  

	return 1;
}   

//����
uint8 ExteFlashWakeUp(void)   
{  
	SPI_CS_ENABLE;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_CS_DISABLE;                            //ȡ��Ƭѡ     	      
	delay_ms(1);                               //�ȴ�TRES1

	return 1;
}   

