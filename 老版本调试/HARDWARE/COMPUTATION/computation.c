#include "variable.h"


void Reed_Switch_Init()//�ɻɹܳ�ʼ��
{
    GPIO_Init(pul_power_GPIO_TypeDef,pul_power_GPIO_PIN,GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(pul_power_GPIO_TypeDef,pul_power_GPIO_PIN);
      
    GPIO_Init(GPIOG,GPIO_Pin_7,GPIO_Mode_Out_PP_High_Slow);
    GPIO_SetBits(GPIOG,GPIO_Pin_7);
          
    GPIO_Init(reed_switch_A_GPIO_TypeDef, reed_switch_A_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(reed_switch_B_GPIO_TypeDef, reed_switch_B_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
}

void JiLiang()//��������
{
  if(reed_switch_A_flag == 0)//��һ�β����½��ؼ�¼��ǰʱ������Ϊ��λ
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
  
  if(reed_switch_A_flag == 1)//��⵽�����ؼ�¼ʱ��
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
  if(reed_switch_B_flag == 0)//��һ�β����½��ؼ�¼��ǰʱ������Ϊ��λ
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
  
  if(reed_switch_B_flag == 1)//��⵽�����ؼ�¼ʱ��
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
      Consumption += 0.1f;//����ˮ��    
      Consumption_Month += 0.1f;//����ˮ��
      if(Account_Flag  == 0)
      {Cumulative_Water_ALL(Consumption);}
      
      RTC_GetDate(RTC_Format_BIN, &RTC_DateStr); //����������
      if(RTC_DateStr.RTC_Month == Check_Month)
      {
          if(Consumption_Month <= First_Section)//����ˮ��С��ʮ��
          {
              Money = Money - (First_Stage * 0.1f);
              //OverDraft_Money = OverDraft_Money - (first_stage * 0.1f);//͸֧��ˮ����
          }
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//����ˮ������ʮ��С��20��
          {
              Money = Money - (Second_Stage * 0.1f);
              //OverDraft_Money = OverDraft_Money - (second_stage * 0.1f);//͸֧��ˮ����
          }
          else if(Consumption_Month > Second_Section)//����ˮ������20��
          {
              Money = Money - (Third_Stage * 0.1f);
              //OverDraft_Money = OverDraft_Money - (third_stage * 0.1f);//͸֧��ˮ����
          }
         /**********   ��ʱ��������   ************/
          EEPROM_WriteF32(MONEY_ADDR,Money);       //Ǯ
          EEPROM_WriteF32(CONSUMPTION,Consumption);//����ˮ��
          EEPROM_WriteF32(CONSUMPTION_MONTH_ADDR,Consumption_Month);//����ˮ��
          /**********͸֧��ˮ����***********/
          /*if(OverDraft_Money <= 0)
          {
            OverDraft_Money = 0;
            if(money<0)
            ValveControl_Close_User();
          }*/
          if(Money < Lack_Money)//����ط����û��ɿ���
          {
            if(Insufficient_Balance_Flag == 0)
            {
              ValveControl_Close_User();
              Insufficient_Balance_Flag = 1;
            }
          }
      }
      else//һ���¼�����ȥ���汣��ǰһ������
      { 
        Money = Money - (First_Stage * 0.1f);
        Check_Month = RTC_DateStr.RTC_Month;
        if(Check_Month == 0x01)
        {
          Check_Month = 13;
        }
        float consumption_month_save = Consumption_Month - 0.1f;
        switch (Check_Month - 1)//������ȥ����һ���µ���ˮ�������ڴ�
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
        } //�洢��ˮ��  
        if(Check_Month == 0x0d)
        {
          Check_Month = 1;
        }
        Consumption_Month = 0.1;
      }
  }
}

void Magnetic_Interference()//�Ÿ���
{
  char breakoverA;//B�ɻɵ�ͨ��־
  char breakoverB;//B�ɻɵ�ͨ��־
  static char magnetic_interference_flage = 0;//�Ÿ��Źط���־
  static char magnetic_open_flage = 0;//�Ÿ���������־
  static char magnetic_open_time = 0;
  breakoverA = GPIO_ReadInputDataBit(reed_switch_A_GPIO_TypeDef,reed_switch_A_GPIO_PIN);
  breakoverB = GPIO_ReadInputDataBit(reed_switch_B_GPIO_TypeDef,reed_switch_B_GPIO_PIN);
  if(breakoverA == 0&&breakoverB == 0&&magnetic_interference_flage == 0)
  {
      Magnetic_Interference_Time++;  
      if(Magnetic_Interference_Time == 4)//һ����Ч�Ÿ���
      {
        Show_Flag |= 0x40;
        Show_Flag1 &= ~(0x18);//00011000
        Show_Flag1 |= 0x08;
        magnetic_interference_flage = 1;
        Magnetic_Interference_Time = 0;
        ValveControl_Close_User();//�ط�
        State2_Flag |= 0x10;
        Magnetic_Interference_Times++;       
        if(Magnetic_Interference_Time >= 5)//�Ÿ��Ŵ�������5��ʱ��ʾE1000020
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
          reed_switch_A_flag = 0;//��ֹ�Ÿ����ж�Ϊ��0.1��ˮ
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
          State2_Flag &= ~(0x10);//����Ÿ��Źط�
        }
      }
  }
}
