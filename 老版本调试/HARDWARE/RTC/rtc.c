#include "variable.h"

void rtc_Init()
{
  CLK_LSEConfig(CLK_LSE_ON);//��LSE
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//ѡ���ⲿ����32.768khz����Ϊʱ��Դ
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);//�ȴ�LSE׼������  
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//ʹ���ⲿʱ��
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);//ѡ��ϵͳʱ��Ϊ1MHZ 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);//���û��Ѷ�ʱ  ��ʱΪ1S
  RTC_SetWakeUpCounter(4095);
  RTC_ITConfig(RTC_IT_WUT,ENABLE);
  RTC_WakeUpCmd(ENABLE);        //ʹ�ܻ���
}
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
  RTC_ClearITPendingBit(RTC_IT_WUT); 
  RTC_IT_Flag = 1;
}
void Send_Open_OK()//���ش򿪷��ųɹ�
{
    unsigned char send_buffer[16] = {0};
    ARRAY_LENGTH = 3;            
    send_buffer[0] = 0x5a;
    send_buffer[1] = 0x03;
    send_buffer[2] = 0x22;  
    send_buffer[3] = 0x01;
    send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
    send_buffer[5] = 0x4a;
    encrypt_methon(send_buffer); //������� 
    delay_ms(Xms);//APP������ʱ
    SendData16(return_encrypt_array);       //����
}
void Send_Close_OK()//���عرշ��ųɹ�
{
    unsigned char send_buffer[16] = {0};
    ARRAY_LENGTH = 3;            
    send_buffer[0] = 0x5a;
    send_buffer[1] = 0x03;
    send_buffer[2] = 0x22;  
    send_buffer[3] = 0x03;
    send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
    send_buffer[5] = 0x4a;
    encrypt_methon(send_buffer); //������� 
    delay_ms(Xms);//APP������ʱ
    SendData16(return_encrypt_array);       //����
}

void RTC_Serverce()
{
 if(RTC_IT_Flag == 1)
 {
    RTC_IT_Flag = 0;
    static char famen_check_times = 0;
    static char voltage_check_times = 0;
    static char down_check_times = 0;
    static char voltage_check_flag = 0;
    static char famen_check_flag = 0;
      WakeUp_Init();
      JiLiang();//��������
      JiLiang_Show();//������ʾ����
      Magnetic_Interference();//�Ÿ��ż�⺯��
      //ADC_Show();
    
      down_check_times++;
      if(down_check_times == 30)//30�� ������
      {
        down_check_times = 0;
        power_down_check();
      }
      RTC_GetDate(RTC_Format_BIN, &RTC_DateStr); //����������
      if(RTC_DateStr.RTC_Date != Check_Date)
      {
        Check_Date = RTC_DateStr.RTC_Date;
        famen_check_times++;
        voltage_check_times++;
        DisCharging_flag = 1;//ÿ��ŵ��ʶ
        if(voltage_check_times == 5)//���� ��ص�ѹ��� ��ѹ�������� ��ѹ����
        {          
          voltage_check_flag = 1;
          voltage_check_times = 0;
        }
        if(famen_check_times == 15)//ʮ���� ���ط�һ��
        {
          famen_check_times = 0;
          famen_check_flag = 1;      
        }         
      } 
      if(voltage_check_flag == 1)//�����Ѿ����ˣ��賿������
      {
        char temp;
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
        temp=RTC_TimeStr.RTC_Hours;
        if(temp == 2)
        {
            voltage_check_flag = 0;
            low_power();
        }   
      }
       if(famen_check_flag == 1)//ʮ�����Ѿ����ˣ��賿������
      {
        char temp;
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
        temp=RTC_TimeStr.RTC_Hours;
        if(temp == 3)
        {
            famen_check_flag = 0;
            Wait_Oparet = (FAMEN_MAINTAIN1 | FAMEN_MAINTAIN2);    
        }   
      }
      if(DisCharging_flag == 1)//ÿ��ŵ��ʶ
      {
        char temp;
        static char DisCharging_time = 0;//�ŵ�ʱ��Ϊ2��
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
        temp=RTC_TimeStr.RTC_Hours;
        if(temp == 1)//�賿һ��ŵ�
        {    
            halt_flag = 1;
            GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//���Ϸŵ��·
            GPIO_SetBits(GPIOG,GPIO_Pin_2); 
            DisCharging_time++;
            if(DisCharging_time == 2)//�ŵ�ʱ��2s
            {
              DisCharging_flag = 0;
              GPIO_ResetBits(GPIOG,GPIO_Pin_2);//�طŵ��·
              halt_flag = 0;
            }
        }
      }      
      //���Ų������
      if(Wait_Oparet)
      {
        if(Wait_Oparet & FAMEN_OPEN)
        {
          Times_Famen++;
          if(0 == GPIO_ReadInputDataBit(RE1_GPIO_TypeDef,RE1_GPIO_PIN))//��ⷧ���Ƿ�����
          {
            GPIO_ResetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);
            GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
            Times_Famen = 0;
            Wait_Oparet &= ~FAMEN_OPEN;
            Famen_Flag = FAMEN_OPEN;
            u1_printf("open famen ok\n");  
            if((Show_Flag != 0||Account_Flag == 0)&&!(Show_Flag & 0x40))//��ʱ���·���״̬
            {
              Cumulative_Water_ALL(Consumption);
            }
          }
          if(Times_Famen > Check_Time)
          {
              Times_Famen = 0;
              State2_Flag |= 0x20;
              Show_Flag |= 0x40;
              Show_Flag1 |= 0x20;
              Wait_Oparet &= ~FAMEN_OPEN;
              Famen_Flag = FAMEN_ERR;
              GPIO_ResetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);
          }
        }
        else if(Wait_Oparet & FAMEN_CLOSE)//��ⷧ���Ƿ�ر����
        {
          Times_Famen++;
          if(0 == GPIO_ReadInputDataBit(RE0_GPIO_TypeDef,RE0_GPIO_PIN))
          {
            Times_Famen = 0;
            Wait_Oparet &= ~FAMEN_CLOSE;
            Famen_Flag = FAMEN_CLOSE;
            u1_printf("close famen ok\n");
            GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
            if((Show_Flag != 0||Account_Flag == 0)&&!(Show_Flag & 0x40))//��ʱ���·���״̬
            {
              Cumulative_Water_ALL(Consumption);
            }
          }       
          if(Times_Famen > Check_Time)
          {
              Times_Famen = 0;
              Famen_Flag = FAMEN_ERR;
              State2_Flag |= 0x20;
              Show_Flag |= 0x40;
              Show_Flag1 |= 0x20;
              Wait_Oparet &= ~FAMEN_CLOSE;
              GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
          }
        }
        else if(Wait_Oparet & (FAMEN_MAINTAIN1|FAMEN_MAINTAIN2))
        {
            if(Famen_Flag == FAMEN_CLOSE)
            {
              ValveControl_Oppen_User();
            }else
            {
              ValveControl_Close_User();
            }
            if(Wait_Oparet & FAMEN_MAINTAIN1)
            {
              Wait_Oparet &= ~FAMEN_MAINTAIN1;
            }else
            {
              Wait_Oparet &= ~FAMEN_MAINTAIN2;
              //ValveControl_Close_User();//���ܵ������Ǹ�λ���ż��ʱ���Ŷ�һ�¾Ͳ���
            }
        }   
      }   
      //�����㲥��� �Ƿ���APP����������
      if(0!=GPIO_ReadInputDataBit(BLE_CHECK_COM, BLE_CHECK_PIN))//�������
      {
        if(BLE_Flag == BLE_CONNECT) //�Ͽ�����
        {
          BLE_Flag = BLE_DISCONNECT;
          Show_Flag &= ~(0xbe);
          Times_BLE = 0;
          if(Account_Flag  == 0)//���δ�����Ͽ������ۼ�����
          {Cumulative_Water_ALL(Consumption);}
          else if (Show_Flag & 0x40)//������쳣��������ʾ�쳣
          {Abnormal_Show();}
          else
          {Clear_Display();}
          u1_printf("disconnect\n");
          halt_flag = 0;
        } 
      }
      else                         //����
      {
        if(BLE_Flag == BLE_DISCONNECT)
        {
          USART3_Config();//��ʼ��USART3 TIM3 �������ų�ʼ��
          USART1_Config();//usart1
          BLE_Flag = BLE_CONNECT;
          Times_BLE = 0;
          u1_printf("connect\n");
          halt_flag = 1;
        }
      }
   //ϵͳ��ʾ���
   System_Display();
 }
}
void Calendar_Init(void)
{
   RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
   RTC_InitStr.RTC_AsynchPrediv = 0x7f;//64��Ƶ
   RTC_InitStr.RTC_SynchPrediv = 0x00ff;//512��Ƶ
   RTC_Init(&RTC_InitStr);//��������32768
   
   RTC_DateTypeDef *ptemp = NULL;
   ptemp = EEPROM_Read_nByte(DOWN_TIME_ADDR,sizeof(RTC_DateTypeDef));
   RTC_DateStr.RTC_WeekDay = ptemp->RTC_WeekDay;
   RTC_DateStr.RTC_Date = ptemp->RTC_Date;
   RTC_DateStr.RTC_Month = ptemp->RTC_Month;
   RTC_DateStr.RTC_Year = ptemp->RTC_Year;
   RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);//2018-7-4
 

   RTC_TimeStr.RTC_Hours   = 22;
   RTC_TimeStr.RTC_Minutes = 40;
   RTC_TimeStr.RTC_Seconds = 00;
   RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);//10-40-00
}
void WakeUp_DeInit()
{
    PWR_FastWakeUpCmd(ENABLE);//������Դ������Ŀ��ٻ���
    PWR_UltraLowPowerCmd(ENABLE);//ʹ�ܵ�Դ�ĵ͹���ģʽ����仰�����úܴ�)   
    RTC_ITConfig(RTC_IT_WUT, ENABLE);//ʹ��RTC�ж�
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�����ж�ʹ��
  //sim();
    GPIO_Init(GPIOA,GPIO_Pin_2|GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);   
    GPIO_Init(GPIOB,GPIO_Pin_6|GPIO_Pin_7 ,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7|GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);
    ////GPIO_Init(GPIOD,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);
    ////GPIO_Init(GPIOD,GPIO_Pin_5,GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIOE,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOF,GPIO_Pin_0|GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOF,GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOG,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_7|GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOG,GPIO_Pin_4|GPIO_Pin_5,GPIO_Mode_Out_PP_High_Slow);
    
    GPIO_Init(GPIOG,GPIO_Pin_0,GPIO_Mode_In_PU_No_IT);//PG0 RXD
    GPIO_Init(GPIOG,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Fast);//PG1TXD
    
    GPIO_Init(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOE,GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow);
   
    ADC_Cmd(ADC1,DISABLE);
    ADC_VrefintCmd(DISABLE);  
    ADC_TempSensorCmd(DISABLE);
    
    Uart_GPIO_Switch();
    
    CLK_PeripheralClockConfig(CLK_Peripheral_USART3,DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1,DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM5,DISABLE);
}

void WakeUp_Init()
{
  GPIO_Init(RE1_GPIO_TypeDef, RE1_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(RE0_GPIO_TypeDef, RE0_GPIO_PIN, GPIO_Mode_In_PU_No_IT);  
  Reed_Switch_Init(); //�ɻɹ����ų�ʼ��
}

