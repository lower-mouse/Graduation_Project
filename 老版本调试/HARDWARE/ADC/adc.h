#ifndef _adc_H
#define _adc_H
#include "stm8l15x.h"
void Adc_Inter_Init();
void Adc_Out_Init();
void Adc_Get_Inter();
void Adc_Get_Out();
void Adc_Get_Out_Losepower();
float ADC_Inter_Show();
void Adc_Close();
float ADC_Show();
float ADC_Losepower_Show();
void power_down_save();
void power_down_recover();
void power_down_check();
void low_power();
void Reset_Detection();//…œµÁºÏ≤‚
#endif 