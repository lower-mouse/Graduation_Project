#include "variable.h"


void Reed_Switch_Init()//干簧管初始化
{
    GPIO_Init(pul_power_GPIO_TypeDef,pul_power_GPIO_PIN,GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(pul_power_GPIO_TypeDef,pul_power_GPIO_PIN);
      
    GPIO_Init(GPIOG,GPIO_Pin_7,GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(GPIOG,GPIO_Pin_7);
          
    GPIO_Init(reed_switch_A_GPIO_TypeDef, reed_switch_A_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(reed_switch_B_GPIO_TypeDef, reed_switch_B_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
}

void JiLiang()//计量函数
{
  if(reed_switch_A_flag == 0)//第一次捕获到下降沿记录当前时间以秒为单位
  {
    if(GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN)==0)
    {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN)==0)
      {
          reed_switch_A_time1++;
          if(reed_switch_A_time1 > Pulse_Factor)
          {  
            reed_switch_A_flag = 1;
            State2_Flag |= 0X80;
          }
      }
    }
  }
  
  if(reed_switch_A_flag == 1)//检测到上升沿记录时间
  {
    if(GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN)!=0)
    {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN)!=0)
      {         
          reed_switch_interval_time1++;
          if(reed_switch_interval_time1 > 3)
          {
            reed_switch_A_time = reed_switch_A_time1;
            reed_switch_A_time1 = 0; 
            reed_switch_interval_time = reed_switch_interval_time1;
            reed_switch_interval_time1 = 0;
            reed_switch_A_flag = 0;
            State2_Flag &= ~(0X80);
          }
      }
    }
  }
  if(reed_switch_B_flag == 0)//第一次捕获到下降沿记录当前时间以秒为单位
  {
    if(GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN)==0)
    {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN)==0)
      {         
          reed_switch_B_time1++;
          if(reed_switch_B_time1 > Pulse_Factor)
          {      
            reed_switch_B_flag = 1;
            State2_Flag |= 0x40;
          }
      }
    }
  }
  
  if(reed_switch_B_flag == 1)//检测到上升沿记录时间
  {
    if(GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN)!=0)
    {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN)!=0)
      {
          reed_switch_B_time = reed_switch_B_time1;
          reed_switch_B_time1 = 0;
          reed_switch_B_flag = 0;
          State2_Flag &= ~(0x40);
      }
    }
  }  
}
void JiLiang_Show()
{
  if((reed_switch_A_time>=Pulse_Factor)&&(reed_switch_interval_time>=3)&&(reed_switch_B_time>=Pulse_Factor))
  {
      reed_switch_interval_time = 0;
      reed_switch_A_time = 0;
      reed_switch_B_time = 0;
      Consumption += 0.1f;//总用水量    
      Consumption_Month += 0.1f;//月用水量
      if(Account_Flag  == 0)
      {Cumulative_Water_ALL(Consumption);}
      
      RTC_GetDate(RTC_Format_BIN, &RTC_DateStr); //日期任务检测
      if(RTC_DateStr.RTC_Month == Check_Month)
      {
          if(Consumption_Month <= First_Section)//月用水量小于十吨
          {
              Money = Money - (First_Stage * 0.1f);
              //OverDraft_Money = OverDraft_Money - (first_stage * 0.1f);//透支补水功能
          }
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//月用水量大于十吨小于20吨
          {
              Money = Money - (Second_Stage * 0.1f);
              //OverDraft_Money = OverDraft_Money - (second_stage * 0.1f);//透支补水功能
          }
          else if(Consumption_Month > Second_Section)//月用水量大于20吨
          {
              Money = Money - (Third_Stage * 0.1f);
              //OverDraft_Money = OverDraft_Money - (third_stage * 0.1f);//透支补水功能
          }
         /**********   随时储存数据   ************/
          EEPROM_WriteF32(MONEY_ADDR,Money);       //钱
          EEPROM_WriteF32(CONSUMPTION,Consumption);//总用水量
          EEPROM_WriteF32(CONSUMPTION_MONTH_ADDR,Consumption_Month);//月用水量
          /**********透支补水功能***********/
          /*if(OverDraft_Money <= 0)
          {
            OverDraft_Money = 0;
            if(money<0)
            ValveControl_Close_User();
          }*/
          if(Money < Lack_Money)//余额不足关阀，用户可开阀
          {
            if(Insufficient_Balance_Flag == 0)
            {
              ValveControl_Close_User();
              Insufficient_Balance_Flag = 1;
            }
          }
      }
      else//一个月即将过去保存保存前一月数据
      { 
        Money = Money - (First_Stage * 0.1f);
        Check_Month = RTC_DateStr.RTC_Month;
        if(Check_Month == 0x01)
        {
          Check_Month = 13;
        }
        float consumption_month_save = Consumption_Month - 0.1f;
        switch (Check_Month - 1)//将将过去的这一个月的用水量存入内存
        {
        case 1: EEPROM_WriteF32(0x04,consumption_month_save);
          break;
        case 2: EEPROM_WriteF32(0x08,consumption_month_save);
          break;
        case 3: EEPROM_WriteF32(0x0c,consumption_month_save);
          break;      
        case 4: EEPROM_WriteF32(0x10,consumption_month_save);
          break;
        case 5: EEPROM_WriteF32(0x14,consumption_month_save);
          break;
        case 6: EEPROM_WriteF32(0x18,consumption_month_save);
          break;
        case 7: EEPROM_WriteF32(0x1c,consumption_month_save);
          break;
        case 8: EEPROM_WriteF32(0x20,consumption_month_save);
          break;
        case 9: EEPROM_WriteF32(0x24,consumption_month_save);
          break;
        case 10:EEPROM_WriteF32(0x28,consumption_month_save);
          break;
        case 11:EEPROM_WriteF32(0x2c,consumption_month_save);
          break;
        case 12:EEPROM_WriteF32(0x30,consumption_month_save);//30-33
          break;  
        default:
          break;
        } //存储用水量  
        if(Check_Month == 0x0d)
        {
          Check_Month = 1;
        }
        Consumption_Month = 0.1;
      }
  }
}

void Magnetic_Interference()//磁干扰
{
  char breakoverA;//B干簧导通标志
  char breakoverB;//B干簧导通标志
  static char magnetic_interference_flage = 0;//磁干扰关阀标志
  static char magnetic_open_flage = 0;//磁干扰消除标志
  static char magnetic_open_time = 0;
  breakoverA = GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN);
  breakoverB = GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN);
  if(breakoverA == 0&&breakoverB == 0&&magnetic_interference_flage == 0)
  {
      Magnetic_Interference_Time++;  
      if(Magnetic_Interference_Time == 4)//一次有效磁干扰
      {
        Show_Flag |= 0x40;
        Show_Flag1 &= ~(0x18);//00011000
        Show_Flag1 |= 0x08;
        magnetic_interference_flage = 1;
        Magnetic_Interference_Time = 0;
        ValveControl_Close_User();//关阀
        State2_Flag |= 0x10;
        Magnetic_Interference_Times++;       
        if(Magnetic_Interference_Time >= 5)//磁干扰次数大于5次时显示E1000020
        {
            Show_Flag |= 0x40;
            Show_Flag1 &= ~(0x18);
            Show_Flag1 |= 0x10;
            State2_Flag |= 0x10;
        }
      }
  }
  else 
  {
    Magnetic_Interference_Time = 0;
  }
  if(magnetic_interference_flage == 1)
  {
    breakoverA = GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN);
    breakoverB = GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN);
      if((breakoverA != 0 || breakoverB != 0)&&Magnetic_Interference_Times < 5)
      {
          reed_switch_A_flag = 0;//防止磁干扰判断为加0.1吨水
          Magnetic_Interference_Time = 0;
          magnetic_open_flage = 1;
          magnetic_interference_flage = 0;
      }
  }
  if(magnetic_open_flage == 1)
  {
      magnetic_open_time++;
      if(magnetic_open_time == 10)
      {
        magnetic_open_time = 0;
        magnetic_open_flage = 0;
        ValveControl_Oppen_User();
        Show_Flag |= 0x40;
        Show_Flag1 &= ~(0x08);
        if(Magnetic_Interference_Times<5)
        {
          State2_Flag &= ~(0x10);//清除磁干扰关阀
        }
      }
  }
}
