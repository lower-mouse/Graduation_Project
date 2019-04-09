#include "variable.h"

void rtc_Init()
{
  CLK_LSEConfig(CLK_LSE_ON);//开LSE
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//选择外部晶振（32.768khz）作为时钟源
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);//等待LSE准备就绪  
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//使能外部时钟
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);//选择系统时钟为1MHZ 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div8);//配置唤醒定时  定时为1S
  RTC_SetWakeUpCounter(4095);
  RTC_ITConfig(RTC_IT_WUT,ENABLE);
  RTC_WakeUpCmd(ENABLE);        //使能唤醒
}
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
  RTC_ClearITPendingBit(RTC_IT_WUT); 
  RTC_IT_Flag = 1;
}
void Send_Open_OK()//返回打开阀门成功
{
    unsigned char send_buffer[16] = {0};
    ARRAY_LENGTH = 3;            
    send_buffer[0] = 0x5a;
    send_buffer[1] = 0x03;
    send_buffer[2] = 0x22;  
    send_buffer[3] = 0x01;
    send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//生成的校验码  
    send_buffer[5] = 0x4a;
    encrypt_methon(send_buffer); //数组加密 
    delay_ms(Xms);//APP适配延时
    SendData16(return_encrypt_array);       //发送
}
void Send_Close_OK()//返回关闭阀门成功
{
    unsigned char send_buffer[16] = {0};
    ARRAY_LENGTH = 3;            
    send_buffer[0] = 0x5a;
    send_buffer[1] = 0x03;
    send_buffer[2] = 0x22;  
    send_buffer[3] = 0x03;
    send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//生成的校验码  
    send_buffer[5] = 0x4a;
    encrypt_methon(send_buffer); //数组加密 
    delay_ms(Xms);//APP适配延时
    SendData16(return_encrypt_array);       //发送
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
      JiLiang();//计量函数
      JiLiang_Show();//计量显示函数
      Magnetic_Interference();//磁干扰检测函数
      //ADC_Show();
    
      down_check_times++;
      if(down_check_times == 30)//30秒 掉电检测
      {
        down_check_times = 0;
        power_down_check();
      }
      RTC_GetDate(RTC_Format_BIN, &RTC_DateStr); //日期任务检测
      if(RTC_DateStr.RTC_Date != Check_Date)
      {
        Check_Date = RTC_DateStr.RTC_Date;
        famen_check_times++;
        voltage_check_times++;
        DisCharging_flag = 1;//每天放电标识
        if(voltage_check_times == 5)//五天 电池电压检测 低压出现两次 低压报警
        {          
          voltage_check_flag = 1;
          voltage_check_times = 0;
        }
        if(famen_check_times == 15)//十五天 开关阀一次
        {
          famen_check_times = 0;
          famen_check_flag = 1;      
        }         
      } 
      if(voltage_check_flag == 1)//五天已经到了，凌晨两点检测
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
       if(famen_check_flag == 1)//十五天已经到了，凌晨三点检测
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
      if(DisCharging_flag == 1)//每天放电标识
      {
        char temp;
        static char DisCharging_time = 0;//放电时间为2秒
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
        temp=RTC_TimeStr.RTC_Hours;
        if(temp == 1)//凌晨一点放电
        {    
            halt_flag = 1;
            GPIO_Init(GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast);//带上放电电路
            GPIO_SetBits(GPIOG,GPIO_Pin_2); 
            DisCharging_time++;
            if(DisCharging_time == 2)//放电时间2s
            {
              DisCharging_flag = 0;
              GPIO_ResetBits(GPIOG,GPIO_Pin_2);//关放电电路
              halt_flag = 0;
            }
        }
      }      
      //阀门操作检测
      if(Wait_Oparet)
      {
        if(Wait_Oparet & FAMEN_OPEN)
        {
          Times_Famen++;
          if(0 == GPIO_ReadInputDataBit(RE1_GPIO_TypeDef,RE1_GPIO_PIN))//检测阀门是否打开完成
          {
            GPIO_ResetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);
            GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
            Times_Famen = 0;
            Wait_Oparet &= ~FAMEN_OPEN;
            Famen_Flag = FAMEN_OPEN;
            u1_printf("open famen ok\n");  
            if((Show_Flag != 0||Account_Flag == 0)&&!(Show_Flag & 0x40))//随时更新阀门状态
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
        else if(Wait_Oparet & FAMEN_CLOSE)//检测阀门是否关闭完成
        {
          Times_Famen++;
          if(0 == GPIO_ReadInputDataBit(RE0_GPIO_TypeDef,RE0_GPIO_PIN))
          {
            Times_Famen = 0;
            Wait_Oparet &= ~FAMEN_CLOSE;
            Famen_Flag = FAMEN_CLOSE;
            u1_printf("close famen ok\n");
            GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
            if((Show_Flag != 0||Account_Flag == 0)&&!(Show_Flag & 0x40))//随时更新阀门状态
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
              //ValveControl_Close_User();//可能的作用是复位阀门检测时阀门动一下就不动
            }
        }   
      }   
      //蓝牙广播检测 是否有APP连接上蓝牙
      if(0!=GPIO_ReadInputDataBit(BLE_CHECK_COM, BLE_CHECK_PIN))//蓝牙检测
      {
        if(BLE_Flag == BLE_CONNECT) //断开连接
        {
          BLE_Flag = BLE_DISCONNECT;
          Show_Flag &= ~(0xbe);
          Times_BLE = 0;
          if(Account_Flag  == 0)//如果未开户断开后常显累计用量
          {Cumulative_Water_ALL(Consumption);}
          else if (Show_Flag & 0x40)//如果有异常就立马显示异常
          {Abnormal_Show();}
          else
          {Clear_Display();}
          u1_printf("disconnect\n");
          halt_flag = 0;
        } 
      }
      else                         //连接
      {
        if(BLE_Flag == BLE_DISCONNECT)
        {
          USART3_Config();//初始化USART3 TIM3 蓝牙引脚初始化
          USART1_Config();//usart1
          BLE_Flag = BLE_CONNECT;
          Times_BLE = 0;
          u1_printf("connect\n");
          halt_flag = 1;
        }
      }
   //系统显示检测
   System_Display();
 }
}
void Calendar_Init(void)
{
   RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
   RTC_InitStr.RTC_AsynchPrediv = 0x7f;//64分频
   RTC_InitStr.RTC_SynchPrediv = 0x00ff;//512分频
   RTC_Init(&RTC_InitStr);//相乘真好是32768
   
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
    PWR_FastWakeUpCmd(ENABLE);//开启电源管理里的快速唤醒
    PWR_UltraLowPowerCmd(ENABLE);//使能电源的低功耗模式（这句话的作用很大)   
    RTC_ITConfig(RTC_IT_WUT, ENABLE);//使能RTC中断
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//接受中断使能
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
  Reed_Switch_Init(); //干簧管引脚初始化
}

