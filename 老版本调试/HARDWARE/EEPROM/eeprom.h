#ifndef __EEPROM_H__
#define __EEPROM_H__

void EEPROM_WriteByte(uint16_t Addr,uint8_t Data);
uint8_t EEPROM_ReadByte(uint8_t Addr);
void EEPROM_WriteU16(uint16_t Addr,uint16_t Data);
uint16_t EEPROM_ReadU16(uint8_t Addr);
void EEPROM_WriteU32(uint16_t Addr,uint32_t Data);
uint32_t EEPROM_ReadU32(uint8_t Addr);
void EEPROM_WriteF32(uint16_t Addr,float Data);
float EEPROM_ReadF32(uint8_t Addr);

void EEPROM_Write_nByte(uint16_t Addr,void* Data, unsigned char n);
void* EEPROM_Read_nByte(uint16_t Addr,unsigned char n);
#endif   
