#include "stm8l15x.h"
#ifndef _RTC_H_
#define _RTC_H_

void rtc_Init();
void Calendar_Init(void);
void WakeUp_DeInit();
void WakeUp_Init();
void count();
void RTC_Serverce();
void Send_Open_OK();
void Send_Close_OK();
#endif