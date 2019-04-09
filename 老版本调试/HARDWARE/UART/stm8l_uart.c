#include "stdarg.h"	
#include "variable.h"    


void Timer3_Init(void)
{
    TIM3_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
    
    TIM3_TimeBaseInit(TIM3_Prescaler_32,TIM3_CounterMode_Up, 3133);
    
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    TIM3_ARRPreloadConfig(ENABLE);
    TIM3_Cmd(DISABLE);
    TIM3_SetCounter(0);
    TIM3_ClearITPendingBit(TIM3_IT_Update);  
} 
void Timer5_Init(void)
{
    TIM5_DeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM5,ENABLE);
    
    TIM5_TimeBaseInit(TIM5_Prescaler_32,TIM5_CounterMode_Up, 3133);
    
    TIM5_ITConfig(TIM5_IT_Update, ENABLE);
    TIM5_ARRPreloadConfig(ENABLE);
    TIM5_Cmd(DISABLE);
    TIM5_SetCounter(0);
    TIM5_ClearITPendingBit(TIM5_IT_Update);  
}
void USART3_Config(void)
{
  //���ô��ڷ���
  BLE_Init(); //���ų�ʼ��
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3,ENABLE);//ʱ�ӿ���
  USART_DeInit(USART3);//������Դ
  
  //������9600��8λ���ݣ�1λֹͣλ��������żУ��
  USART_Init(USART3,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Rx|USART_Mode_Tx));

  USART_ClearITPendingBit(USART3,USART_IT_RXNE);
  USART_ClearFlag(USART3,USART_FLAG_TC);
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�����ж�ʹ��
  USART_Cmd(USART3,ENABLE);//����ʹ��
  Timer3_Init();
}

void USART1_Config(void)
{
  //���ô��ڷ���
  GPIO_Init(GPIOC,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT); //Tx
  GPIO_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Fast);//Rx
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);//ʱ�ӿ���

  USART_DeInit(USART1);//������Դ
  //������9600��8λ���ݣ�1λֹͣλ��������żУ��
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Rx|USART_Mode_Tx));
  
  USART_ClearFlag(USART1,USART_FLAG_TC);
  
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ж�ʹ��
  USART_Cmd(USART1,ENABLE);//����ʹ��
  Timer5_Init();
  USART1_RX_STA = 0;
}


void BLE_Init()
{
  //UART3
  GPIO_Init(GPIOG,GPIO_Pin_0,GPIO_Mode_In_PU_No_IT);//PG0 RXD
  GPIO_Init(GPIOG,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Fast);//PG1TXD
  GPIO_Init(BLE_BRTS_COM, BLE_BRTS_PIN, GPIO_Mode_Out_PP_High_Fast); //BRTS
  GPIO_Init(BLE_EN_COM, BLE_EN_PIN, GPIO_Mode_Out_PP_Low_Fast);     //EN
  GPIO_ResetBits(BLE_EN_COM,BLE_EN_PIN);
  GPIO_Init(BLE_CHECK_COM, BLE_CHECK_PIN, GPIO_Mode_In_PU_No_IT);//PG6 ��������ָʾ���ţ�0�������ӣ�                                                                                                                                                    
}

//��ʱ��3 �жϴ�����
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
  if(TIM3_GetITStatus(TIM3_IT_Update)!= RESET)
  {
    TIM3_ClearITPendingBit(TIM3_IT_Update);   
    TIM3_SetCounter(0);
    USART3_RX_STA |= 0x80;
    TIM3_Cmd(DISABLE);
  }
}
//��ʱ��5 �жϴ�����
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
  if(TIM5_GetITStatus(TIM5_IT_Update)!= RESET)
  {
    TIM5_ClearITPendingBit(TIM5_IT_Update);   
    TIM5_SetCounter(0);
    USART1_RX_STA |= 0x80;
    TIM5_Cmd(DISABLE);
  }
}


INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
 unsigned int temp = 0;
 USART_ClearITPendingBit(USART3,USART_IT_RXNE);
  if(USART_GetITStatus(USART3, USART_IT_RXNE)) 
   {
     if((USART3_RX_STA&(0x80))==0)
     {
       if(USART3_RX_STA < USART_RECV_LEN+1)//������볤�ȴ���Ԥ�ڳ�����ǿ����Ϊ�������
       {  
         // ��1�ֽڣ����������
          TIM3_SetCounter(0);
          if(USART3_RX_STA == 0)
          {
             TIM3_Cmd(ENABLE);
          }
          temp = USART_ReceiveData8(USART3);     //��������
          Usart3_RecvBuf[USART3_RX_STA++] = temp;//����buffer
       }
       else
       {
          USART3_RX_STA |= 0x80;        
       }
     }
     else //���ݻ�û���ü�����  ���� ���ڷ��ͳ���һ��������
     {
        u1_printf("Data Transmit Error 0x%x\n",USART3_RX_STA);
        USART3_RX_STA = 0;
     }
     if(USART_GetFlagStatus(USART3,USART_FLAG_OR) == SET) // ���OR��־
     {
       USART_GetITStatus(USART3, USART_IT_RXNE);
       USART_ReceiveData8(USART3);   
     }
   }
}
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    unsigned char temp = 0;
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);//����жϱ�־λ
    if(USART_GetITStatus(USART1, USART_IT_RXNE)) 
    {
       if(USART1_RX_STA < 50)//������볤�ȴ���Ԥ�ڳ�����ǿ����Ϊ�������
       {  
          // ��1�ֽڣ����������
          TIM5_SetCounter(0);
          if(USART1_RX_STA == 0)
          {
             TIM5_Cmd(ENABLE);
          }
          temp = USART_ReceiveData8(USART1);     //��������
          Usart1_RecvBuf[USART1_RX_STA++] = temp;//����buffer
       }
       else
       {
          USART1_RX_STA |= 0x80;        
       }
      if(USART_GetFlagStatus(USART1,USART_FLAG_OR) == SET) // ���OR��־
      {
        USART_GetITStatus(USART1, USART_IT_RXNE);
        USART_ReceiveData8(USART1);   
      }
    }    
}

void SendData16(unsigned char *data) //����16������
{
    int i;
    GPIO_ResetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
    USART_ClearFlag(USART3,USART_FLAG_TC);
    delay_ms(100);
    for(i = 0;i<16;i++)
    { 
      USART_GetFlagStatus(USART3,USART_FLAG_TXE);
      USART_SendData8(USART3,data[i]);
      while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET); 
    }  
    delay_ms(100);
    GPIO_SetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
}


void u1_printf(char* fmt,...)  
{  
    u16 i,j; 
    va_list ap; 
    va_start(ap,fmt);
    vsprintf((char*)Usart1_SendBuf,fmt,ap);
    va_end(ap);
    i=strlen((const char*)Usart1_SendBuf);
    for(j=0;j<i;j++)							
    {
      USART_GetFlagStatus(USART1,USART_FLAG_TC);
      USART_SendData8(USART1,Usart1_SendBuf[j]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    } 
}

void u3_printf(char* fmt,...)  
{  
    u16 i,j; 
    va_list ap; 
    va_start(ap,fmt);
    vsprintf((char*)Usart3_SendBuf,fmt,ap);
    va_end(ap);
    i=strlen((const char*)Usart3_SendBuf);		
    for(j=0;j<i;j++)							
    {
      USART_GetFlagStatus(USART3,USART_FLAG_TC);
      USART_SendData8(USART3,Usart3_SendBuf[j]); 
      while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
    } 
}

