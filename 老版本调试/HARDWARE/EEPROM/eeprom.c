#include "iostm8l152c6.h"
#include "stdint.h"
#include "eeprom.h"
#define EEPROM_STARTADDR 0x1000 
#include <stdlib.h>
/******************************************************************************************************
*名    称:  void EEPROM_WriteByte(uint16_t Addr,uint8_t Data)
*功    能： 向EEPROM指定地址写入一字节数据
*入口参数： Addr:存放数据的偏移地址
*           Data:待写入的数据
*出口参数：无
*范    例：
******************************************************************************************************/
void EEPROM_WriteByte(uint16_t Addr,uint8_t Data)
{
   uint8_t  *Ptr_SegAddr; //Segment pointer
   Ptr_SegAddr = (uint8_t *)(EEPROM_STARTADDR+Addr);  //Initialize  pointer
   FLASH_DUKR=0xAE;
   FLASH_DUKR=0x56;
   while(!FLASH_IAPSR_DUL);//如果DUL没有置位,说明解锁不成功,最好加入超时判断
   asm("sim");//关闭中断
   *Ptr_SegAddr=Data; //指定地址，写1 字节
   while(!FLASH_IAPSR_EOP);//等待写操作完成,最好加入超时判断
   asm("rim");//打开中断
   FLASH_IAPSR_DUL = 0;//上锁,对DATA EEPROM写保护
}
/******************************************************************************************************
*名    称：uint8_t EEPROM_ReadByte(uint8_t Addr)
*功    能：从EEPROM中读取一个字节数据
*入口参数：Addr:存放数据的偏移地址
*出口参数：Data：读回的数据
*说    明：无
*范    例：无
******************************************************************************************************/

uint8_t EEPROM_ReadByte(uint8_t Addr)
{
   uint8_t Data=0;
   uint8_t *Ptr_SegAddr;
   Ptr_SegAddr = (uint8_t *)(EEPROM_STARTADDR+Addr);  //Initialize  pointer
   Data=*Ptr_SegAddr;
   return Data;
}
//低地址存低位， 高地址存高位
void EEPROM_WriteU16(uint16_t Addr,uint16_t Data)
{
  unsigned char low;
  unsigned char hight;
  unsigned char *Data_Addr = (unsigned char *)&Data; 
  low   = *Data_Addr;
  hight = *(Data_Addr + 1);
  
  EEPROM_WriteByte(Addr, low);
  EEPROM_WriteByte(Addr+1, hight);
  
}

uint16_t EEPROM_ReadU16(uint8_t Addr)
{
  unsigned char low;
  unsigned char hight;
  
  low = EEPROM_ReadByte(Addr);
  hight = EEPROM_ReadByte(Addr +1);
  return low<<8 | hight;
  
}

void EEPROM_Write_nByte(uint16_t Addr,void* Data, unsigned char n)
{
  char *d = (char *)Data;
  unsigned char i; 
 for(i = 0; i < n; i++)
    EEPROM_WriteByte(Addr+i, *(d+i));
}

void* EEPROM_Read_nByte(uint16_t Addr,unsigned char n)
{
 unsigned char i; 
 char *addr = NULL;
 
 addr = malloc(n);
 
 for(i = 0; i < n; i++)
    *(addr+i) = EEPROM_ReadByte(Addr+i);
 
 return addr;
}


void EEPROM_WriteU32(uint16_t Addr,uint32_t Data)
{
  unsigned char bit1, bit2, bit3, bit4;
  unsigned char *Data_Addr = (unsigned char *)&Data;
  
  bit1   = *Data_Addr;
  bit2 = *(Data_Addr + 1);
  bit3 = *(Data_Addr + 2);
  bit4 = *(Data_Addr + 3);
  
  EEPROM_WriteByte(Addr, bit1);
  EEPROM_WriteByte(Addr+1, bit2);
  EEPROM_WriteByte(Addr+2, bit3);
  EEPROM_WriteByte(Addr+3, bit4);
}

uint32_t EEPROM_ReadU32(uint8_t Addr)
{
   unsigned char bit1, bit2, bit3, bit4;
   uint32_t ret = 0;
  bit1 = EEPROM_ReadByte(Addr);
  bit2 = EEPROM_ReadByte(Addr +1);
  bit3 = EEPROM_ReadByte(Addr +2);
  bit4 = EEPROM_ReadByte(Addr +3);
  
  ret |= bit1 & 0xff;
  ret <<= 8;
  ret |= bit2 & 0xff;
  ret <<= 8;
  ret |= bit3 & 0xff;
  ret <<= 8;
  ret |= bit4 & 0xff;
  return ret;
  
}
//低地址存低位， 高地址存高位
void EEPROM_WriteF32(uint16_t Addr,float Data)
{
  unsigned char bit1, bit2, bit3, bit4;
  unsigned char *Data_Addr = (unsigned char *)&Data;
  
  bit1   = *Data_Addr;
  bit2 = *(Data_Addr + 1);
  bit3 = *(Data_Addr + 2);
  bit4 = *(Data_Addr + 3);
  
  EEPROM_WriteByte(Addr, bit1);
  EEPROM_WriteByte(Addr+1, bit2);
  EEPROM_WriteByte(Addr+2, bit3);
  EEPROM_WriteByte(Addr+3, bit4);
}

float EEPROM_ReadF32(uint8_t Addr)
{
   char *tmp;

   float ret = 0;
   tmp = (char *)&ret;
   *tmp = EEPROM_ReadByte(Addr);
   *(tmp + 1)= EEPROM_ReadByte(Addr +1);
   *(tmp + 2)= EEPROM_ReadByte(Addr +2);
   *(tmp + 3)= EEPROM_ReadByte(Addr +3);
  
  return ret;
  
}


