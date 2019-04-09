#include "variable.h"

 void main(void)
{
 System_Init();
  while (1)
  {  
      RTC_Serverce();  
      if(halt_flag == 0)
      {                                         
          WakeUp_DeInit();   
          halt();    
           //wfi();
      }
      if(USART1_RX_STA & 0x80) 
      {
        Message_Serverce_Usart();
        USART1_RX_STA = 0;
      }
      if(USART3_RX_STA & 0x80)
      {
        if(strncmp((const char*)Usart3_RecvBuf,"TTM:CONNECTED",13)==0)
          u1_printf("connect\n");
        else if(strncmp((const char*)Usart3_RecvBuf,"TTM:DISCONNECT",14)==0)
          u1_printf("disconnect\n");
        else if(strncmp((const char*)Usart3_RecvBuf,"TTM:",4)==0)
        {
          //pritnf_data(Usart3_RecvBuf);
          pritnf_data_Name(Usart3_RecvBuf);
          BLE_Rest();
          /*USART3_RX_STA = 0;  
          GPIO_ResetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
          delay_ms(100);//等待引脚电平稳定
          u3_printf("TTM:RST-SYSTEMRESET");
          delay_ms(100);//等待引脚电平稳定
          GPIO_SetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
          delay_ms(100);*/
          halt_flag = 0;
        }
        else
        {
          Message_Serverce_BLE();
        } 
        USART3_RX_STA = 0;        
        memset(Usart3_RecvBuf, 0, sizeof(Usart3_RecvBuf));
      } 
    }
}
















