#include "variable.h"

static u16 AD_Vtemp_Inter = 0;
static u16 AD_Vtemp_Out = 0;
static u16 AD_Vtemp_Out_Losepower = 0;

void Adc_Inter_Init()
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//开启ADC1时钟
    GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
    ADC_DeInit(ADC1); 
    ADC_Cmd(ADC1, ENABLE);
    ADC_Init(ADC1, ADC_ConversionMode_Single,ADC_Resolution_12Bit, ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
    ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);
    ADC_VrefintCmd(ENABLE);
}

void Adc_Out_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//开启ADC1时钟
  GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
  ADC_VrefintCmd(DISABLE);  
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);
  ADC_DeInit(ADC1);
  ADC_Init(ADC1, ADC_ConversionMode_Single,ADC_Resolution_12Bit, ADC_Prescaler_1);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE);   //打开外部通道
  ADC_SchmittTriggerConfig(ADC1, ADC_Channel_24, DISABLE);//ADC_Channel_24 史密斯关闭
  
}
void Adc_Get_Inter()
{
    Adc_Inter_Init();
    ADC_SoftwareStartConv(ADC1); //开启软件转换
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
    ADC_SchmittTriggerConfig(ADC1,ADC_Channel_Vrefint,DISABLE);
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
    AD_Vtemp_Inter = ADC_GetConversionValue(ADC1); //获取转换值  
}
void Adc_Get_Out()//低电量ADC检测
{
    Adc_Out_Init();
    GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//带负载测量
    GPIO_SetBits(GPIOG,GPIO_Pin_2); 
    GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);
    GPIO_SetBits(GPIOA,GPIO_Pin_2);
    ADC_SoftwareStartConv(ADC1); //开启软件转换
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
    ADC_SchmittTriggerConfig(ADC1,ADC_Channel_Vrefint,DISABLE);
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
    AD_Vtemp_Out = ADC_GetConversionValue(ADC1); //获取转换值  
    
    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void Adc_Get_Out_Losepower()//掉电ADC检测
{
   Adc_Out_Init();
   GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);
   GPIO_SetBits(GPIOA,GPIO_Pin_2);
   ADC_SoftwareStartConv(ADC1); //开启软件转换
   while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
   ADC_SchmittTriggerConfig(ADC1,ADC_Channel_Vrefint,DISABLE);
   ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//清除对应标志
   AD_Vtemp_Out_Losepower = ADC_GetConversionValue(ADC1); //获取转换值  
   GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
float ADC_Inter_Show()
{
   float value = 0;
   Adc_Get_Inter();
   value=(1.225*4096)/AD_Vtemp_Inter; //获得VDD电压，单位V
   return value;
}
void Adc_Close()
{
  ADC_DeInit(ADC1);
  ADC_Cmd(ADC1, DISABLE);   
  ADC_VrefintCmd(DISABLE);
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);
  ADC_ChannelCmd(ADC1, ADC_Channel_24, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  
  GPIO_ResetBits(GPIOG,GPIO_Pin_2);
  GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}

float ADC_Show()//低电量ADC检测
{
   float value = 0;
   Adc_Get_Out();
   float a = ADC_Inter_Show();
   value=(a*AD_Vtemp_Out)/4096; //获得VDD电压，单位V
   value = (value*147.5)/47.5;
   USART1_Config();//usart1
   u1_printf("voltage :%f\n",value);
   Adc_Close();
   return value;
}
float ADC_Losepower_Show()
{
   float value = 0;
   Adc_Get_Out_Losepower();
   float a = ADC_Inter_Show();
   value=(a*AD_Vtemp_Out_Losepower)/4096; //获得VDD电压，单位V
   value = (value*147.5)/47.5;
   USART1_Config();//usart1
   u1_printf("voltage :%f\n",value);
   Adc_Close();
   return value;
}
void low_power()//低电量报警
{
  float a;
  static int ADC_Check_Flag = 0;//检测次数标志
  a = ADC_Show();
  if(a >= 2.9)
  {
    ADC_Check_Flag--;
    Show_Flag |= 0x40;
    Show_Flag1 &= ~(0x03); 
    State2_Flag &= ~(0x03);
    POWER_LOW_FLAG = 0;//低电量标志
    if(ADC_Check_Flag < 0)
    {
      ADC_Check_Flag = 0;
    }
  }
  else
  {
    ADC_Check_Flag++;
  }
  if(ADC_Check_Flag == 2)
  {
    ADC_Check_Flag = 0;
    if(a < 2.9 && a>2.7)//一级
    {       
        Show_Flag |= 0x40;
        Show_Flag1 &= ~(0x07); 
        Show_Flag1 |= 0x01;
        State2_Flag &= ~(0x02);
        State2_Flag |= 0x01;  
        POWER_LOW_FLAG = 1;//低电量标志
    }
    if(a < 2.7 && a > 2)//二级
    {
        State2_Flag &= ~(0x01);
        State2_Flag |= 0x02;
        Show_Flag |= 0x40;        
        Show_Flag1 &= ~(0x07);
        Show_Flag1 |= 0x02;
        POWER_LOW_FLAG = 1;//低电量标志
        ValveControl_Close_User();
    }
  }
}
void power_down_check()//掉电检测
{
  float a = ADC_Losepower_Show();
  if(a <= 2 && !(State2_Flag&0x04))
  {
    Show_Flag |= 0x40;
    Show_Flag1 &= ~(0x07);
    Show_Flag1 |= 0x04;
    NoPower_flag = 1; //是否进行掉电操作
    ValveControl_Close_User();
    power_down_save();
    State2_Flag |= (0x01<<2);
    //state2_flag |= 0x03;
  }else if(a >= 3 && (State2_Flag&0x04))
  {
     ValveControl_Oppen_User(); //开阀
     State2_Flag &= ~(0x01<<2); 
     Show_Flag |= 0x40;
     Show_Flag1 &= ~(0x07);
  } 
}

void Reset_Detection()//上电检测
{
  float a = ADC_Show();
  while(a < 3)
  {
    a = ADC_Show();
    GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//带上放电电路
    GPIO_SetBits(GPIOG,GPIO_Pin_2);
    delay_ms(500);
    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
    delay_ms(500);
  }
}

void power_down_save()//掉电保存
{
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
  EEPROM_Write_nByte(DOWN_TIME_ADDR, &RTC_DateStr,sizeof(RTC_DateTypeDef));//掉电时间
  EEPROM_WriteF32(MONEY_ADDR,Money);       //钱
  EEPROM_WriteF32(CONSUMPTION,Consumption);//总用水量
  EEPROM_WriteF32(CONSUMPTION_MONTH_ADDR,Consumption_Month);//月用水量
  
  EEPROM_WriteF32(MONTH_BASE_ADDR+(RTC_DateStr.RTC_Month*4) - 4 ,Consumption_Month);//存入历史当月用水量
  EEPROM_WriteF32(FIRST_STAGE, First_Stage);
  EEPROM_WriteF32(SECOND_STAGE, Second_Stage);
  EEPROM_WriteF32(THIRD_STAGE,   Third_Stage);
  EEPROM_WriteF32(FIRST_SECTION, First_Section);
  EEPROM_WriteF32(SECOND_SECTION,Second_Section);
 // EEPROM_WriteByte(OVERDRAFT_MONEY,OverDraft_Money); //预留  
  State1_Flag |= (Magnetic_Interference_Times << 4);

  
  EEPROM_WriteByte(STATE_FLAG2_ADDR,State2_Flag);//标志位
  EEPROM_WriteByte(STATE_FLAG1_ADDR,State1_Flag);
  EEPROM_WriteByte(LACK_MONEY,Lack_Money);
  u1_printf("power_down_save\n");
}

void power_down_recover()//上电恢复
{
  Money = EEPROM_ReadF32(MONEY_ADDR);       //钱
  Consumption = EEPROM_ReadF32(CONSUMPTION);//总用水量
  Consumption_Month = EEPROM_ReadF32(CONSUMPTION_MONTH_ADDR);//月用水量
  State2_Flag = EEPROM_ReadByte(STATE_FLAG2_ADDR); //标志位
  State1_Flag = EEPROM_ReadByte(STATE_FLAG1_ADDR);
  Pulse_Factor = State1_Flag&0x0f;  
  Magnetic_Interference_Times = (State1_Flag&0x70)>>4;
  Account_Flag = (State1_Flag&0x80)>>7;
  Lack_Money = EEPROM_ReadByte(LACK_MONEY);
  First_Stage = EEPROM_ReadF32(FIRST_STAGE);
  Second_Stage = EEPROM_ReadF32(SECOND_STAGE);
  Third_Stage = EEPROM_ReadF32(THIRD_STAGE);
  First_Section = EEPROM_ReadF32(FIRST_SECTION);
  Second_Section = EEPROM_ReadF32(SECOND_SECTION);
  Water_Company_Number = EEPROM_ReadU16(WATER_COMPANY_NUMBER);
}


