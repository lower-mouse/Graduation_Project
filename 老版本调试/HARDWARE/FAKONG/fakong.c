#include "variable.h"

void ValveInit()//阀门初始化
{
    GPIO_Init(M_OPPEN_GPIO_TypeDef, M_OPPEN_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_ResetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);  
    GPIO_Init(M_CLOSE_GPIO_TypeDef, M_CLOSE_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
    GPIO_Init(MOTOR_CHECK_GPIO_TypeDef, MOTOR_CHECK_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(RE1_GPIO_TypeDef, RE1_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(RE0_GPIO_TypeDef, RE0_GPIO_PIN, GPIO_Mode_In_PU_No_IT);        
}

void ValveControl_Oppen_User()//阀门控制函数(开对应RE+)
{
  float adc =  ADC_Show();
  ValveInit();  //阀门初始化
  if(0 == GPIO_ReadInputDataBit(RE1_GPIO_TypeDef,RE1_GPIO_PIN))//检测阀门是否打开完成
  {
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      Send_Open_OK();
    }
    return;
  }  
  if(Money > 0 && !(State2_Flag & 0x08) && adc >= 2.7 && Magnetic_Interference_Times<5)//开阀条件
  {     
    GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN); 
    GPIO_SetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN); 
    Wait_Oparet |= FAMEN_OPEN;
    Wait_Oparet &= ~FAMEN_CLOSE;
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      Send_Open_OK();
    }
    Times_Famen = 0;
  }  
  else
  {
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      unsigned char send_buffer[16] = {0};
      ARRAY_LENGTH = 6;  
      send_buffer[0] = 0x5a;
      send_buffer[1] = 0x06;
      send_buffer[2] = 0x14;
      if((State2_Flag & 0x08)&&(Handle_Mode != MANAGE_MODE))
      {send_buffer[3] = 0x01;}
      else{send_buffer[3] = 0;}
      if(Money <= 0)
      {send_buffer[4] = 0x01;}
      else{send_buffer[4] = 0;}
      if( Magnetic_Interference_Times>5)
      {send_buffer[5] = 0x01;}
      else{send_buffer[5] = 0;} 
      if(adc < 2.7)
      {send_buffer[6] = 0x01;}
      else{send_buffer[6] = 0;}
      send_buffer[7] = crc_generate_checkout_code(send_buffer+1);//生成的校验码  
      send_buffer[8] = 0x4a;
      encrypt_methon(send_buffer); //数组加密 
      delay_ms(Xms);//APP适配延时
      SendData16(return_encrypt_array);       //发送
    }
  }
}
void ValveControl_Oppen_Administrator()
{
  ValveInit();  //阀门初始化
  float adc =  ADC_Show();
  if(0 == GPIO_ReadInputDataBit(RE1_GPIO_TypeDef,RE1_GPIO_PIN))//检测阀门是否打开完成
  {
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      Send_Open_OK();
    }
    State2_Flag &= ~(0x08);//清除管理员关阀标志
    State2_Flag &= ~(0x10);//清除磁干扰关阀
    Show_Flag1 &= ~(0x10);//清除磁干扰常显
    Magnetic_Interference_Times = 0;//清除磁干扰次数
    return;
  }  
  if((Money > 0 /*|| OverDraft_Money > 0*/) && adc >= 2.7)//开阀条件
  {
      GPIO_ResetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN); 
      GPIO_SetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);
      State2_Flag &= ~(0x08);//清除管理员关阀标志
      State2_Flag &= ~(0x10);//清除磁干扰关阀
      Show_Flag1 &= ~(0x10);//清除磁干扰常显
      Magnetic_Interference_Times = 0;//清除磁干扰次数
      Wait_Oparet |= FAMEN_OPEN;
      Wait_Oparet &= ~FAMEN_CLOSE;
      if(OC_Flag == 1)
      {
        OC_Flag = 0;
        Send_Open_OK();
      }
      Times_Famen = 0;
  }
  else//返回错误原因
  {
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      unsigned char send_buffer[16] = {0};
      ARRAY_LENGTH = 6;  
      send_buffer[0] = 0x5a;
      send_buffer[1] = 0x06;
      send_buffer[2] = 0x14;  
      send_buffer[3] = 0;
      if(Money <= 0)
      {send_buffer[4] = 0x01;}
      else{send_buffer[4] = 0;}
      send_buffer[5] = 0;
      if(adc < 2.7)
      {send_buffer[6] = 0x01;}
      else{send_buffer[6] = 0;}
      send_buffer[7] = crc_generate_checkout_code(send_buffer+1);//生成的校验码  
      send_buffer[8] = 0x4a;
      encrypt_methon(send_buffer); //数组加密 
      delay_ms(Xms);//APP适配延时
      SendData16(return_encrypt_array);       //发送
    }
  }
}
void ValveControl_Close_User()//阀门关控制函数(开对应RE-)（用户）
{
   ValveInit();  //阀门初始化
   if(0 == GPIO_ReadInputDataBit(RE0_GPIO_TypeDef,RE0_GPIO_PIN))
   {
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      Send_Close_OK();
    }
    return;
   }  
    GPIO_ResetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);
    GPIO_SetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
    Wait_Oparet |= FAMEN_CLOSE;
    Wait_Oparet &= ~FAMEN_OPEN;
    Times_Famen = 0;
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      Send_Close_OK();
    }
}
//char array_30;
void ValveControl_Close_Administrator()//阀门控制函数(开对应RE-)（管理员）
{
   ValveInit();  //阀门初始化
   if(0 == GPIO_ReadInputDataBit(RE0_GPIO_TypeDef,RE0_GPIO_PIN))
   {
    if(OC_Flag == 1)
    {
      OC_Flag = 0;
      Send_Close_OK();
    }
    State2_Flag |= 0X08;
    return;
   }  
   GPIO_ResetBits(M_OPPEN_GPIO_TypeDef,M_OPPEN_GPIO_PIN);
   GPIO_SetBits(M_CLOSE_GPIO_TypeDef,M_CLOSE_GPIO_PIN);
   Wait_Oparet = FAMEN_CLOSE;
   Wait_Oparet &= ~FAMEN_OPEN;
   if(OC_Flag == 1)
   {
     OC_Flag = 0;
     Send_Close_OK();
   }
   State2_Flag |= 0X08;
   Times_Famen = 0;
}


