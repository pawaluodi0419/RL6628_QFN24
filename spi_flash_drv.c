/*
 * spi_drv.c
 *
 *  Created on: 2014-3-9
 *      Author: Administrator
 */

#include "platform.h"


void sFLASH_WriteEnable(u32 SPI_BASEADDR)  //дʹ��
{

  sFLASH_CS_LOW(SPI_BASEADDR);  //����CS

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_WREN); //����дʹ������ 0x06

  sFLASH_CS_HIGH(SPI_BASEADDR);  //����CS
}


void sFLASH_WaitForWriteEnd(u32 SPI_BASEADDR)  //�ȴ�д����
{
  u8 flashstatus = 0;

  sFLASH_CS_LOW(SPI_BASEADDR);  //����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_RDSR);  //��״̬�Ĵ���

  XSpi_ReadReg(SPI_BASEADDR,XSP_DRR_OFFSET);  // clear SPI receive buffer
  do
  {

	Spi_SendByte(SPI_BASEADDR,0,sFLASH_DUMMY_BYTE);
    flashstatus = Spi_GetReceByte(SPI_BASEADDR); //����״̬�Ĵ�����Ϣ

  }
  while (flashstatus & sFLASH_WIP_FLAG); //�ж��Ƿ�����д��


  sFLASH_CS_HIGH(SPI_BASEADDR); //����CS
}


void sFLASH_EraseSector(u32 SPI_BASEADDR,u32 SectorAddr) //������  stm25p128 �� �������������� = w25x16�Ŀ����������
{
  sFLASH_WriteEnable(SPI_BASEADDR); //дʹ��

  sFLASH_CS_LOW(SPI_BASEADDR);  //����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_SE); //����������

  Spi_SendByte(SPI_BASEADDR,0,(u8)((SectorAddr & 0xFF0000) >> 16)); //���͵�ַ
  Spi_SendByte(SPI_BASEADDR,0,(u8)((SectorAddr & 0xFF00) >> 8));
  Spi_SendByte(SPI_BASEADDR,0,(u8)(SectorAddr & 0xFF));

  sFLASH_CS_HIGH(SPI_BASEADDR);  //����CS


  sFLASH_WaitForWriteEnd(SPI_BASEADDR);  //�ȴ�д����
}


void sFLASH_EraseBulk(u32 SPI_BASEADDR) //����   stm25p128 �� ������������� = w25x16��оƬ����������
{
  sFLASH_WriteEnable(SPI_BASEADDR);//дʹ��

  sFLASH_CS_LOW(SPI_BASEADDR);//����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_BE);//��������

  sFLASH_CS_HIGH(SPI_BASEADDR);//����CS

  sFLASH_WaitForWriteEnd(SPI_BASEADDR);  //�ȴ�д����
}



void sFLASH_EraseChip(u32 SPI_BASEADDR) //chip erase
{
  sFLASH_WriteEnable(SPI_BASEADDR);//дʹ��

  sFLASH_CS_LOW(SPI_BASEADDR);//����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_CE);//��������

  sFLASH_CS_HIGH(SPI_BASEADDR);//����CS

  sFLASH_WaitForWriteEnd(SPI_BASEADDR);  //�ȴ�д����
}



void sFLASH_WritePage(u32 SPI_BASEADDR,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)  //дҳ
{

  sFLASH_WriteEnable(SPI_BASEADDR);  //дʹ��

  sFLASH_CS_LOW(SPI_BASEADDR);   //����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_WRITE);  //д����

  Spi_SendByte(SPI_BASEADDR,0,(u8)((WriteAddr & 0xFF0000) >> 16)); //��ַ
  Spi_SendByte(SPI_BASEADDR,0,(u8)((WriteAddr & 0xFF00) >> 8));
  Spi_SendByte(SPI_BASEADDR,0,(u8)(WriteAddr & 0xFF));

  while (NumByteToWrite--)   //дNumByteToWrite������ ҪС��256��
  {

	  Spi_SendByte(SPI_BASEADDR,0,*pBuffer);

    pBuffer++;
  }

  sFLASH_CS_HIGH(SPI_BASEADDR);  //����cs

  sFLASH_WaitForWriteEnd(SPI_BASEADDR);  //��д����
}


void sFLASH_WriteBuffer(u32 SPI_BASEADDR,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)  //д��������  �����ڸ������
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = (u8)(WriteAddr % sFLASH_SPI_PAGESIZE);     //�õ���256���µĵ�ַ��
  count = (u8)(sFLASH_SPI_PAGESIZE - Addr);         //���µ�ַ�뻹����ٵ�256
  NumOfPage = (u8)(NumByteToWrite / sFLASH_SPI_PAGESIZE);  //�м�����ҳ
  NumOfSingle = (u8)(NumByteToWrite % sFLASH_SPI_PAGESIZE); //�ж��ٸ���ͷ

  if (Addr == 0) //����ַ����256
  {
    if (NumOfPage == 0) //Ҫд�������С��256
    {
      sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, NumByteToWrite);
    }
    else //Ҫд������ݴ���256
    {
      while (NumOfPage--)
      {
        sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else //��ַ����256������
  {
    if (NumOfPage == 0) //Ҫд���С��256
    {
      if (NumOfSingle > count) //��ͷ���ڵ�256��������ַ����ĸ���,Ҳ���� ��ַ+��ͷ ����256����,��Ҫ���
      {
        temp = (u8)(NumOfSingle - count);

        sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, temp);
      }
      else
      {
        sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else //Ҫд�ĸ����Ĵ���256
    {
      NumByteToWrite -= count;
      NumOfPage =  (u8)(NumByteToWrite / sFLASH_SPI_PAGESIZE);
      NumOfSingle = (u8)(NumByteToWrite % sFLASH_SPI_PAGESIZE);

      sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sFLASH_WritePage(SPI_BASEADDR,pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


void sFLASH_ReadBuffer(u32 SPI_BASEADDR,u8* pBuffer, u32 ReadAddr, u16 NumByteToRead) //��ȡ����
{
  sFLASH_CS_LOW(SPI_BASEADDR);  //����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_READ); //������

  Spi_SendByte(SPI_BASEADDR,0,(u8)((ReadAddr & 0xFF0000) >> 16));  //��ַ
  Spi_SendByte(SPI_BASEADDR,0,(u8)((ReadAddr& 0xFF00) >> 8));
  Spi_SendByte(SPI_BASEADDR,0,(u8)(ReadAddr & 0xFF));


  XSpi_ReadReg(SPI_BASEADDR,XSP_DRR_OFFSET);  // clear SPI receive buffer
  while (NumByteToRead--) //while����ȡ����
  {

	Spi_SendByte(SPI_BASEADDR,0,sFLASH_DUMMY_BYTE);
	*pBuffer  = Spi_GetReceByte(SPI_BASEADDR); //����״̬�Ĵ�����Ϣ

    pBuffer++;
  }

  sFLASH_CS_HIGH(SPI_BASEADDR);
}


u32 sFLASH_ReadID(u32 SPI_BASEADDR)  //��ID
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  sFLASH_CS_LOW(SPI_BASEADDR);  //����cs

  Spi_SendByte(SPI_BASEADDR,0,0x9F); //READ ID���� 9F

  XSpi_ReadReg(SPI_BASEADDR,XSP_DRR_OFFSET);  // clear SPI receive buffer

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_DUMMY_BYTE);
  Temp0 =  Spi_GetReceByte(SPI_BASEADDR);  //����ID

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_DUMMY_BYTE);
  Temp1 =  Spi_GetReceByte(SPI_BASEADDR);  //����ID

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_DUMMY_BYTE);
  Temp2 =  Spi_GetReceByte(SPI_BASEADDR);  //����ID

  sFLASH_CS_HIGH(SPI_BASEADDR);  //����CS

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;  //�ϲ�����

  return Temp;
}


void sFLASH_StartReadSequence(u32 SPI_BASEADDR,u32 ReadAddr)  //��������ʼ����
{
  sFLASH_CS_LOW(SPI_BASEADDR); //����cs

  Spi_SendByte(SPI_BASEADDR,0,sFLASH_CMD_READ); //������

  Spi_SendByte(SPI_BASEADDR,0,(u8)((ReadAddr & 0xFF0000) >> 16));  //��ַ

  Spi_SendByte(SPI_BASEADDR,0,(u8)((ReadAddr& 0xFF00) >> 8));

  Spi_SendByte(SPI_BASEADDR,0,(u8)(ReadAddr & 0xFF));

  //need CS pull high��

}


u8 proMcmFlash(u32 srcFlashDevAddr, u32 mcmFlashDevAddr,u8 proSize ,u8 accCheckEn, u8 eraseMcmBeforeProEn, u8 readCompEn)
{
	u8 i,j,k;
    u8 FLASH_RXBuff[256];
    u8 FLASH_RdCompBuff[256];
    u8 proResult=1;  // set the read compare result as pass.
    u32 temp;
    u32 rdAccValue;

//	XGpio_0_WriteBit(0,LED_D7_OFFSET,1);

	rdAccValue=0; //clear the read acc value.


	temp=sFLASH_ReadID(srcFlashDevAddr);
	temp=sFLASH_ReadID(mcmFlashDevAddr);
	if(eraseMcmBeforeProEn){                //erase MCM Flash
		sFLASH_EraseChip(mcmFlashDevAddr);
	}

	for(j=0;j<=proSize;j++){    //64K program loop

		if(!proResult){  //jump out the 64K program loop if read compare fail
			break;
		}

		for(i=0;i<=255;i++){     //page program loop

			if(!proResult){  //jump out the page program loop if read compare fail
				break;
			}

			sFLASH_ReadBuffer(srcFlashDevAddr,FLASH_RXBuff, i*256+j*65536, 256); //read page from source flash
			sFLASH_WritePage(mcmFlashDevAddr,FLASH_RXBuff, i*256+j*65536, 256); //write page to MCM flash

			if(readCompEn){  //read compare
				sFLASH_ReadBuffer(mcmFlashDevAddr,FLASH_RdCompBuff, i*256+j*65536, 256); //read page from MCM flash
				for(k=0;k<=255;k++){
					if(FLASH_RXBuff[k]!=FLASH_RdCompBuff[k]){
						proResult=0;
						break;
					}

					if(accCheckEn){
						rdAccValue=FLASH_RdCompBuff[k]+rdAccValue;
					}
				}
			}
		}

	}


//	XGpio_0_WriteBit(0,LED_D7_OFFSET,0); // program ended indicator

	return proResult;


}
