#ifndef __STM8l052R8_DG15069_H_
#define __STM8l052R8_DG15069_H_

#include "stm8l15x_lcd.h"


typedef enum {
  DANG_YUE = (uint32_t)0x00000001, 
  LEI_JI   = (uint32_t)0x00000002, 
  SHENG_YU = (uint32_t)0x00000004, 
  
  POINT_P4 = (uint32_t)0x00000008,
  POINT_P3 = (uint32_t)0x00000010, 
  POINT_P2 = (uint32_t)0x00000020, 
  POINT_P1 = (uint32_t)0x00000040, 
  
  LI_FANG_MI  = (uint32_t)0x00000080, 
  FA_KAI   = (uint32_t)0x00000100, 
  FA_GUAN  = (uint32_t)0x00000200, 
  CHONG_ZHI = (uint32_t)0x00000400,
  DAN_JIA   = (uint32_t)0x00000800, 
  
  HUAN_DIAN_CHI = (uint32_t)0x00001000,
  SHI_JIAN     = (uint32_t)0x00002000, 
  YI_CHANG     = (uint32_t)0x00004000, 
  RI_QI        = (uint32_t)0x00008000,
  
  YUAN         =  (uint32_t)0x00010000,
  QING         =  (uint32_t)0x00020000,
  DIS_ALL       = (uint32_t)0xffffffff
  
} LCD_Display_Param;
void Show_Float(float n, int decimal_place);
void LcdInit();
void Display_Time(RTC_TimeTypeDef   RTC_TimeStr);
void Show_Number(int n, int bit);//显示一个数字 
void Display_Number(uint32_t n);//全部位 显示同一个数 N
void show_param(LCD_Display_Param Prarm);
void Clean_ShowBuf();
void Flush_ShowBuf();

void Pic_Version(float version);
void Pic_Connect();
void Pic_Control();
void Pic_Date();
void Pic_Srene();
void Pic_Test();
void Pic_Corr();
void Pic_Id();
void Top_Up_Amount(float money);
void Remain_Amount(float money);
void Cumulative_Water_ALL(float water);
void Cumulative_Water_Month(float water);
void Price_Water(float money);
void Show_Date(RTC_DateTypeDef   RTC_DateStr);
void Show_Time(RTC_TimeTypeDef   RTC_TimeStr);
void Clear_Display();//清屏
void ALL_Display();//全显
void FaMenOparet_Display();//----
void System_Display();
void Abnormal_Show();
#endif
