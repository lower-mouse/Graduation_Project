#include "variable.h"

static u16 AD_Vtemp_Inter = 0;
static u16 AD_Vtemp_Out = 0;
static u16 AD_Vtemp_Out_Losepower = 0;

void Adc_Inter_Init()
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//����ADC1ʱ��
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
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);//����ADC1ʱ��
  GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
  ADC_VrefintCmd(DISABLE);  
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);
  ADC_DeInit(ADC1);
  ADC_Init(ADC1, ADC_ConversionMode_Single,ADC_Resolution_12Bit, ADC_Prescaler_1);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE);   //���ⲿͨ��
  ADC_SchmittTriggerConfig(ADC1, ADC_Channel_24, DISABLE);//ADC_Channel_24 ʷ��˹�ر�
  
}
void Adc_Get_Inter()
{
    Adc_Inter_Init();
    ADC_SoftwareStartConv(ADC1); //�������ת��
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת������
    ADC_SchmittTriggerConfig(ADC1,ADC_Channel_Vrefint,DISABLE);
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//�����Ӧ��־
    AD_Vtemp_Inter = ADC_GetConversionValue(ADC1); //��ȡת��ֵ  
}
void Adc_Get_Out()//�͵���ADC���
{
    Adc_Out_Init();
    GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//�����ز���
    GPIO_SetBits(GPIOG,GPIO_Pin_2); 
    GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);
    GPIO_SetBits(GPIOA,GPIO_Pin_2);
    ADC_SoftwareStartConv(ADC1); //�������ת��
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת������
    ADC_SchmittTriggerConfig(ADC1,ADC_Channel_Vrefint,DISABLE);
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//�����Ӧ��־
    AD_Vtemp_Out = ADC_GetConversionValue(ADC1); //��ȡת��ֵ  
    
    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void Adc_Get_Out_Losepower()//����ADC���
{
   Adc_Out_Init();
   GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);
   GPIO_SetBits(GPIOA,GPIO_Pin_2);
   ADC_SoftwareStartConv(ADC1); //�������ת��
   while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת������
   ADC_SchmittTriggerConfig(ADC1,ADC_Channel_Vrefint,DISABLE);
   ADC_ClearFlag(ADC1,ADC_FLAG_EOC);//�����Ӧ��־
   AD_Vtemp_Out_Losepower = ADC_GetConversionValue(ADC1); //��ȡת��ֵ  
   GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
float ADC_Inter_Show()
{
   float value = 0;
   Adc_Get_Inter();
   value=(1.225*4096)/AD_Vtemp_Inter; //���VDD��ѹ����λV
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

float ADC_Show()//�͵���ADC���
{
   float value = 0;
   Adc_Get_Out();
   float a = ADC_Inter_Show();
   value=(a*AD_Vtemp_Out)/4096; //���VDD��ѹ����λV
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
   value=(a*AD_Vtemp_Out_Losepower)/4096; //���VDD��ѹ����λV
   value = (value*147.5)/47.5;
   USART1_Config();//usart1
   u1_printf("voltage :%f\n",value);
   Adc_Close();
   return value;
}
void low_power()//�͵�������
{
  float a;
  static int ADC_Check_Flag = 0;//��������־
  a = ADC_Show();
  if(a >= 2.9)
  {
    ADC_Check_Flag--;
    Show_Flag |= 0x40;
    Show_Flag1 &= ~(0x03); 
    State2_Flag &= ~(0x03);
    POWER_LOW_FLAG = 0;//�͵�����־
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
    if(a < 2.9 && a>2.7)//һ��
    {       
        Show_Flag |= 0x40;
        Show_Flag1 &= ~(0x07); 
        Show_Flag1 |= 0x01;
        State2_Flag &= ~(0x02);
        State2_Flag |= 0x01;  
        POWER_LOW_FLAG = 1;//�͵�����־
    }
    if(a < 2.7 && a > 2)//����
    {
        State2_Flag &= ~(0x01);
        State2_Flag |= 0x02;
        Show_Flag |= 0x40;        
        Show_Flag1 &= ~(0x07);
        Show_Flag1 |= 0x02;
        POWER_LOW_FLAG = 1;//�͵�����־
        ValveControl_Close_User();
    }
  }
}
void power_down_check()//������
{
  float a = ADC_Losepower_Show();
  if(a <= 2 && !(State2_Flag&0x04))
  {
    Show_Flag |= 0x40;
    Show_Flag1 &= ~(0x07);
    Show_Flag1 |= 0x04;
    NoPower_flag = 1; //�Ƿ���е������
    ValveControl_Close_User();
    power_down_save();
    State2_Flag |= (0x01<<2);
    //state2_flag |= 0x03;
  }else if(a >= 3 && (State2_Flag&0x04))
  {
     ValveControl_Oppen_User(); //����
     State2_Flag &= ~(0x01<<2); 
     Show_Flag |= 0x40;
     Show_Flag1 &= ~(0x07);
  } 
}

void Reset_Detection()//�ϵ���
{
  float a = ADC_Show();
  while(a < 3)
  {
    a = ADC_Show();
    GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//���Ϸŵ��·
    GPIO_SetBits(GPIOG,GPIO_Pin_2);
    delay_ms(500);
    GPIO_ResetBits(GPIOG,GPIO_Pin_2);
    delay_ms(500);
  }
}

void power_down_save()//���籣��
{
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
  EEPROM_Write_nByte(DOWN_TIME_ADDR, &RTC_DateStr,sizeof(RTC_DateTypeDef));//����ʱ��
  EEPROM_WriteF32(MONEY_ADDR,Money);       //Ǯ
  EEPROM_WriteF32(CONSUMPTION,Consumption);//����ˮ��
  EEPROM_WriteF32(CONSUMPTION_MONTH_ADDR,Consumption_Month);//����ˮ��
  
  EEPROM_WriteF32(MONTH_BASE_ADDR+(RTC_DateStr.RTC_Month*4) - 4 ,Consumption_Month);//������ʷ������ˮ��
  EEPROM_WriteF32(FIRST_STAGE, First_Stage);
  EEPROM_WriteF32(SECOND_STAGE, Second_Stage);
  EEPROM_WriteF32(THIRD_STAGE,   Third_Stage);
  EEPROM_WriteF32(FIRST_SECTION, First_Section);
  EEPROM_WriteF32(SECOND_SECTION,Second_Section);
 // EEPROM_WriteByte(OVERDRAFT_MONEY,OverDraft_Money); //Ԥ��  
  State1_Flag |= (Magnetic_Interference_Times << 4);

  
  EEPROM_WriteByte(STATE_FLAG2_ADDR,State2_Flag);//��־λ
  EEPROM_WriteByte(STATE_FLAG1_ADDR,State1_Flag);
  EEPROM_WriteByte(LACK_MONEY,Lack_Money);
  u1_printf("power_down_save\n");
}

void power_down_recover()//�ϵ�ָ�
{
  Money = EEPROM_ReadF32(MONEY_ADDR);       //Ǯ
  Consumption = EEPROM_ReadF32(CONSUMPTION);//����ˮ��
  Consumption_Month = EEPROM_ReadF32(CONSUMPTION_MONTH_ADDR);//����ˮ��
  State2_Flag = EEPROM_ReadByte(STATE_FLAG2_ADDR); //��־λ
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


