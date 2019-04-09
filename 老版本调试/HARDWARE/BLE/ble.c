#include "variable.h"
#include "stm8l15x_it.h"
void BLE_Rest_Break()
{
  char Connect_Flag;
  Connect_Flag = GPIO_ReadInputDataBit(BLE_CHECK_COM, BLE_CHECK_PIN);
  while(0 == Connect_Flag)//ȷ����λ��Ͽ�
  {
    GPIO_ResetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
    delay_ms(100);//�ȴ����ŵ�ƽ�ȶ�
    u3_printf("TTM:RST-SYSTEMRESET");
    Connect_Flag = GPIO_ReadInputDataBit(BLE_CHECK_COM, BLE_CHECK_PIN);
  }
  GPIO_SetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
}
void BLE_Rest()//������λ����
{
  USART3_RX_STA = 0;        
  memset(Usart3_RecvBuf, 0, sizeof(Usart3_RecvBuf));
  while(USART3_RX_STA == 0)
  {
    USART3_RX_STA = 0;        
    memset(Usart3_RecvBuf, 0, sizeof(Usart3_RecvBuf));
    GPIO_ResetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
    delay_ms(100);//�ȴ����ŵ�ƽ�ȶ�
    u3_printf("TTM:RST-SYSTEMRESET");
    delay_ms(100);//��ֹ��ν��봮��3�ж�
  }
  delay_ms(100);//������λ���������������ͣ�
                //��ֹ�����ӡconnect�ڴ˴�����ʱ�ø�λ�������Żָ���λǰ״̬
  GPIO_SetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
}
void Message_Serverce_BLE()//Э���������������
{
    int i = 0; 
    unsigned char send_buffer[16] = {0};
    decrypt_methon((unsigned char *)Usart3_RecvBuf);//��ԭ����ǰ����    
    unsigned char *checkout = malloc((return_decrypt_array[1]+1)*sizeof(char));
    memcpy(checkout, return_decrypt_array+1, return_decrypt_array[1]+1);
    ARRAY_LENGTH = return_decrypt_array[1]+1;
    unsigned short result_code = crc_virification(checkout);
    pritnf_data(return_decrypt_array);//��ӡ��������
    unsigned long temp;//��������
    free(checkout);
    if(result_code == 0)
    {            
       if(return_decrypt_array[0] == 0x5a)      //����ͷ
       {
        if(return_decrypt_array[2] == 0x02)     //�����޶�ֵ
        {
          Lack_Money = return_decrypt_array[3];
        }
        else if(return_decrypt_array[2] == 0x03)     //Уʱ ��
        {
           RTC_DateStr.RTC_Date  = return_decrypt_array[5];//��
           RTC_DateStr.RTC_Month = (RTC_Month_TypeDef)return_decrypt_array[4];//��
           RTC_DateStr.RTC_Year  = return_decrypt_array[3];//��
           RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);
           
           RTC_TimeStr.RTC_Hours   = return_decrypt_array[6];//ʱ
           RTC_TimeStr.RTC_Minutes = return_decrypt_array[7];//��
           RTC_TimeStr.RTC_Seconds = return_decrypt_array[8];//��
           RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
           RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
           if(NoPower_flag)
           {
              NoPower_flag = 0;
              RTC_DateTypeDef *ptemp = NULL;
              ptemp = EEPROM_Read_nByte(DOWN_TIME_ADDR,sizeof(RTC_DateTypeDef));
              if(ptemp->RTC_Year  == RTC_DateStr.RTC_Year)
              {
                if(ptemp->RTC_Month  == RTC_DateStr.RTC_Month)
                {
                    Consumption_Month = EEPROM_ReadF32(CONSUMPTION_MONTH_ADDR);
                }else
                {
                  Consumption_Month = 0;
                }
              }else
              {
                Consumption_Month = 0;
              }
              free(ptemp);
              ptemp = NULL;
           }
           if(Handle_Mode == MANAGE_MODE)
           {
              Show_Flag &= 0x40;
              Show_Flag |= 0x20;
           }
            ARRAY_LENGTH = 3;            //�����޸�ʱ��ɹ�
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x03;
            send_buffer[2] = 0x22;  
            send_buffer[3] = 0x05;
            send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
            send_buffer[5] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            //delay_ms(1000);//APP������ʱ
            SendData16(return_encrypt_array);       //����
            u1_printf("recvice time OK\n");
         }
        else if(return_decrypt_array[2] == 0x05)//�޸Ľ���ˮ�ۡ�????
        {
          u1_printf("xxxxxxxx\n");
          if(return_decrypt_array[3] == 0x01)
          {
              First_Stage = return_decrypt_array[4]+((return_decrypt_array[5]+0.001)*0.01);
          }
          if(return_decrypt_array[3] == 0x02)
          {
            Second_Stage = return_decrypt_array[4]+((return_decrypt_array[5]+0.001)*0.01);
          }
          if(return_decrypt_array[3] == 0x03)
          {
            Third_Stage = return_decrypt_array[4]+((return_decrypt_array[5]+0.001)*0.01);
          }
            EEPROM_WriteF32(FIRST_STAGE, First_Stage);
            EEPROM_WriteF32(SECOND_STAGE, Second_Stage);
            EEPROM_WriteF32(THIRD_STAGE,   Third_Stage);
        }
        else if(return_decrypt_array[2] == 0x06)//����
        {
          Account_Flag  = 1;
          State1_Flag = State1_Flag|0x80;
          EEPROM_WriteByte(STATE_FLAG1_ADDR,State1_Flag);
          Money = 0;
          EEPROM_WriteF32(MONEY_ADDR,Money);
          Consumption_Month = 0;
          EEPROM_WriteF32(CONSUMPTION_MONTH_ADDR,Consumption_Month);
          for(i = 0;i<12;i++)
          { EEPROM_WriteF32(MONTH_BASE_ADDR + i*4, 0);}//��ʷ��ˮ��
        }
        else if(return_decrypt_array[2] == 0x07)//��/�ط���(�û�/����Ա)��
        {
          OC_Flag = 1;
          if(return_decrypt_array[3] == 0x55)
          {
            if(Handle_Mode == USER_MODE)
            { 
              ValveControl_Oppen_User();
            }
            else if(Handle_Mode == MANAGE_MODE)
            {
              ValveControl_Oppen_Administrator();
            }
          }
          else if(return_decrypt_array[3] == 0x99)
          {
            if(Handle_Mode == USER_MODE)
            { 
              ValveControl_Close_User();
            }
            else if(Handle_Mode == MANAGE_MODE)
            {
              ValveControl_Close_Administrator();
            }  
          }  
        }        
        else if(return_decrypt_array[2] == 0x0c)//���쳣
        {
         if(Handle_Mode != MANAGE_MODE)
            return;
         State1_Flag &= 0x8f;//����Ÿ��Ŵ���
         State2_Flag = 0;
         Magnetic_Interference_Times = 0;//�Ÿ��Ŵ���
         Show_Flag &= 0x40;
         Show_Flag |= 0x80;
         Show_Flag1 = 0;
         EEPROM_WriteByte(STATE_FLAG2_ADDR,State2_Flag);//��־λ
         EEPROM_WriteByte(STATE_FLAG1_ADDR,State1_Flag);
         ARRAY_LENGTH = 3;            
         send_buffer[0] = 0x5a;
         send_buffer[1] = 0x03;
         send_buffer[2] = 0x22;  
         send_buffer[3] = 0x07;
         send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
         send_buffer[5] = 0x4a;
         encrypt_methon(send_buffer); //������� 
         delay_ms(Xms);//APP������ʱ
         SendData16(return_encrypt_array);       //����
        }
        else if(return_decrypt_array[2] == 0x0d)//�����ݡ�
        {        
          if(return_decrypt_array[3] == 0x00)//������
          {
            ARRAY_LENGTH = 6;
            temp = (unsigned int)fabs(Money);
 
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x06;
            send_buffer[2] = 0x0f;
            send_buffer[3] = (Money < 0)? 1:0;           
            send_buffer[4] = temp >> 8;
            send_buffer[5] = temp & 0x00ff;
            if((Money-temp) >= 0)
              send_buffer[6] = (unsigned int)(((Money+0.00005)-temp)*100);
            else 
              send_buffer[6] = (unsigned int)(fabs((temp+(Money+0.00005))*100));
            send_buffer[7] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����          
            send_buffer[8] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array);       //����
            pritnf_data(send_buffer);
            u1_printf("Query balance %f\n", Money);
          }
          if(return_decrypt_array[3] == 0x01)//������ˮ����
          {
            ARRAY_LENGTH = 6;
            temp = (unsigned long )Consumption;       
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x06;
            send_buffer[2] = 0x10;  
            send_buffer[3] = (unsigned char)(Consumption/65536);
            send_buffer[4] = temp >> 8;
            send_buffer[5] = temp & 0x00ff;
            u32 ccc = ((Consumption+0.00005) - temp)*100;
            send_buffer[6] = (unsigned char)ccc;               
            send_buffer[7] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
            send_buffer[8] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array);       //����
            pritnf_data(send_buffer);
            u1_printf("check consumption :%f\n",Consumption);
          }
          if(return_decrypt_array[3] == 0x06)//������ˮ����
          {
            ARRAY_LENGTH = 5;
            temp = (unsigned int )Consumption_Month;            
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x05;
            send_buffer[2] = 0x13;  
            send_buffer[3] = temp >> 8;
            send_buffer[4] = temp & 0x00ff;
            u32 ccc = ((Consumption_Month+0.00005) - temp)*100;
            send_buffer[5] = (unsigned char)ccc;
            //send_buffer[5] =(unsigned char)(consumption - temp)*100;                 
            send_buffer[6] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
            send_buffer[7] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array);       //����
            u1_printf("check consumption_month :%f\n",Consumption_Month);
          }
          if(return_decrypt_array[3] == 0x02)//����ʷ��ˮ����
          {
  
              /*ˮ���ͣ�5A 06 11 X1 X2 X3 X4 CRC 4A
            ��ʷ��ˮ��������ֽ�ΪX1 �����x����  X2 X3 X4������λ
              ��һ�µ�ַ:eeprom ƫ�� 0x03
            */ 
            int i = 0;
            int temp;
            float money_tmp;
            ARRAY_LENGTH = 11;
            send_buffer[0] = 0x5A;
            send_buffer[1] = 0x0c;
            send_buffer[2] = 0x11;
            while(i < 12)
            {
              money_tmp = EEPROM_ReadF32(MONTH_BASE_ADDR + i*4); //������i-1�·�
              temp = (unsigned int )money_tmp;  
              send_buffer[3] = (i/3)+1;
              send_buffer[4] = temp >> 8;
              send_buffer[5] = temp & 0x00ff;
              send_buffer[6] =(unsigned char)((money_tmp - temp)*100); 
              i++;
              money_tmp = EEPROM_ReadF32(MONTH_BASE_ADDR + i*4); //������i-1�·�
              temp = (unsigned int )money_tmp;  
              send_buffer[7] = temp >> 8;
              send_buffer[8] = temp & 0x00ff;
              send_buffer[9] =(unsigned char)((money_tmp - temp)*100); 
              i++;
              money_tmp = EEPROM_ReadF32(MONTH_BASE_ADDR + i*4); //������i-1�·�
              temp = (unsigned int )money_tmp;  
              send_buffer[10] = temp >> 8;
              send_buffer[11] = temp & 0x00ff;
              send_buffer[12] =(unsigned char)((money_tmp - temp)*100); 
              i++;
              send_buffer[13] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
              send_buffer[14] = 0x4a;
              encrypt_methon(send_buffer); //������� 
              delay_ms(Xms);//APP������ʱ);//APP������ʱ
              SendData16(return_encrypt_array);       //����
              pritnf_data(send_buffer);
              u1_printf("succeed send %d...\n", i);
            }
          }
          if(return_decrypt_array[3] == 0x03)//��ˮ��״̬��
          {  
            ARRAY_LENGTH = 3;
            send_buffer[0]=0x5a;
            send_buffer[1]=0x03;
            send_buffer[2]=0x12;
            switch (Famen_Flag)//����
            {
              case 1:send_buffer[3] |= 0x40;break;//��
              case 2:send_buffer[3] |= 0x80;break;//��
              case 3:send_buffer[3] |= 0xc0;break;//�쳣
            } 
            if(Money < Lack_Money)
            {
              send_buffer[3] |= 0x08;//����
            }
            if(State2_Flag&0x10)//�Ÿ���
            {send_buffer[3] |= 0x04;}
            send_buffer[3] |= ((State2_Flag&0x03)<<4);//�͵���
            if(Account_Flag == 1)
            {
              send_buffer[3] |= (0x01<<1);
            }
            if(State2_Flag & 0x04)
            {
              send_buffer[3] |= (0x03<<4);
            }
            send_buffer[3] |= ((State2_Flag&0x08)>>3);     //����Ա
            send_buffer[4]=crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
            send_buffer[5]=0x4a;
            encrypt_methon(send_buffer); //������� 
            //delay_ms(1000);//APP������ʱ
            SendData16(return_encrypt_array);
           pritnf_data(send_buffer);
            u1_printf("read status OK\n");
          }
          if(return_decrypt_array[3] == 0x04)//��ˮ��ID��
          {
            ARRAY_LENGTH = 5;
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x06;
            send_buffer[2] = 0x1b;  
            send_buffer[3] = EEPROM_ReadByte(BLE_ID_ADDR);
            send_buffer[4] = EEPROM_ReadByte(BLE_ID_ADDR+1);
            send_buffer[5] = EEPROM_ReadByte(BLE_ID_ADDR+2);
            send_buffer[6] = EEPROM_ReadByte(BLE_ID_ADDR+3);
            send_buffer[7] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
            send_buffer[8] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array);
            Show_Flag &= 0x40;
            Show_Flag|=0x10;
            pritnf_data(send_buffer);
            u1_printf("read ID OK\n");
          }
          if(return_decrypt_array[3] == 0x05)//�����ۡ�
          {
            ARRAY_LENGTH = 7;
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x08;
            send_buffer[2] = 0x1a;  
            send_buffer[3] = ((unsigned int)First_Stage) & 0x00ff;
            send_buffer[4] = (unsigned int)((First_Stage-send_buffer[3])*100);
            send_buffer[5] = ((unsigned int)Second_Stage) & 0x00ff;
            send_buffer[6] = (unsigned int)((Second_Stage-send_buffer[5])*100);   
            send_buffer[7] = ((unsigned int)Third_Stage) & 0x00ff;
            send_buffer[8] = (unsigned int)((Third_Stage-send_buffer[7])*100);
            send_buffer[9] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
            send_buffer[10] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array);
            u1_printf("read price OK\n");
            pritnf_data(send_buffer);
          }
        }
        else if(return_decrypt_array[2] == 0x0e)//������
        {
          if(Handle_Mode != MANAGE_MODE)
              return;  
         Show_Flag &= 0x40;
         Show_Flag |= 0x08;

         Money = 0;
         Magnetic_Interference_Times = 0;//�Ÿ��Ŵ���

         First_Stage = 0;    //����ˮ�� ����
         Second_Stage = 0;
         Third_Stage = 0;
         First_Section = 0; //��������
         Second_Section = 0;
         
         State1_Flag &= 0x0f;//���������ϵ�����б�־λ
         State2_Flag = 0;
         Lack_Money = 0;//�����޶�ֵ
         Account_Flag  = 0;//���������Ϣ
         State1_Flag = State1_Flag&~(0x80);
         Water_Company_Number = 0;//���ˮ˾���
         
         Consumption_Month = 0;//����ˮ��
         Consumption = 0;//����ˮ��
         for(i = 0;i<12;i++)
         { EEPROM_WriteF32(MONTH_BASE_ADDR + i*4, 0);}//��ʷ��ˮ��
          EEPROM_WriteF32(MONEY_ADDR,Money);       //Ǯ
          EEPROM_WriteF32(CONSUMPTION,Consumption);//����ˮ��
          EEPROM_WriteF32(CONSUMPTION_MONTH_ADDR,Consumption_Month);//����ˮ��
          EEPROM_WriteByte(STATE_FLAG2_ADDR,State2_Flag);//��־λ
          EEPROM_WriteByte(STATE_FLAG1_ADDR,State1_Flag);
          EEPROM_WriteByte(LACK_MONEY,Lack_Money);
          EEPROM_WriteF32(FIRST_STAGE, First_Stage);
          EEPROM_WriteF32(SECOND_STAGE, Second_Stage);
          EEPROM_WriteF32(THIRD_STAGE,   Third_Stage);
          EEPROM_WriteF32(FIRST_SECTION, First_Section);
          EEPROM_WriteF32(SECOND_SECTION,Second_Section);
          EEPROM_WriteU16(WATER_COMPANY_NUMBER,Water_Company_Number);
         ValveControl_Close_User();//�ط�  
         ARRAY_LENGTH = 3;            
         send_buffer[0] = 0x5a;
         send_buffer[1] = 0x03;
         send_buffer[2] = 0x22;  
         send_buffer[3] = 0x06;
         send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
         send_buffer[5] = 0x4a;
         encrypt_methon(send_buffer); //������� 
         delay_ms(Xms);//APP������ʱ
         SendData16(return_encrypt_array);       //����
        }
        else if(return_decrypt_array[2] == 0x19)//͸֧��ˮ
        {
           OverDraft_Money = (return_decrypt_array[3]+return_decrypt_array[4]*0.01f);
           OverDraft_Money = 0;
        }
        else if(return_decrypt_array[2] == 0x1c)//�޸�����ˮ��
        {
            Consumption = ((return_decrypt_array[3]*65536)+((long)return_decrypt_array[4] *256)+(return_decrypt_array[5] & 0xff) )+ return_decrypt_array[6]*0.01f;
            Cumulative_Water_ALL(Consumption);
            ARRAY_LENGTH = 3;            
            send_buffer[0] = 0x5a;
            send_buffer[1] = 0x03;        
            send_buffer[2] = 0x22;  
            send_buffer[3] = 0x08;
            send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
            send_buffer[5] = 0x4a;
            encrypt_methon(send_buffer); //������� 
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array);       //����
            EEPROM_WriteF32(CONSUMPTION,Consumption);//����ˮ��
            u1_printf("consumption: %f\n",Consumption);
        }
        else if(return_decrypt_array[2] == 0x1D)//��ˮ����ѣ�ˮ�����䣨2 �����������????
        {
          First_Section = return_decrypt_array[3]+((return_decrypt_array[4]+0.001)*0.01);
          Second_Section = return_decrypt_array[5]+((return_decrypt_array[6]+0.001)*0.01);
          EEPROM_WriteF32(FIRST_SECTION,First_Section);
          EEPROM_WriteF32(SECOND_SECTION,Second_Section);
          u1_printf("wait define...\n");
        }
        else if(return_decrypt_array[2] == 0x15)//Ԥ��ֵ��
        { 
          ARRAY_LENGTH = 5;
          return_decrypt_array[2] = 0x17;   
          Confirm_Money = ((return_decrypt_array[3] & 0xff) << 8 | (return_decrypt_array[4] & 0xff) )+ return_decrypt_array[5]*0.01f;
          return_decrypt_array[6] = crc_generate_checkout_code(return_decrypt_array+1);//���ɵ�У����
          encrypt_methon(return_decrypt_array); //�������    
          if((Money + Confirm_Money)>LIMIT_MONEY)
          {
             ARRAY_LENGTH = 3;            
             send_buffer[0] = 0x5a;
             send_buffer[1] = 0x03;
             send_buffer[2] = 0x22;  
             send_buffer[3] = 0x02;  
             send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
             send_buffer[5] = 0x4a;
             encrypt_methon(send_buffer); //������� 
             delay_ms(Xms);//APP������ʱ
             SendData16(return_encrypt_array);       //����
          }
          else
          {
            delay_ms(Xms);//APP������ʱ
            SendData16(return_encrypt_array); 
            u1_printf("preloaded  money %f\n", Confirm_Money);
          }
         
        }
        else if(return_decrypt_array[2] == 0x16)//��ֵ��
        {
            Money += Confirm_Money;
            EEPROM_WriteF32(MONEY_ADDR,Money);
            Show_Flag &= 0x40;
            Show_Flag |= 0x04;
            if(Money > 0)
            {
              ValveControl_Oppen_User();
            }
           Insufficient_Balance_Flag = 0;//�����������ֻ�ط�һ��
            u1_printf("top-up succeed money %f\n", Money);
        }    
        else if(return_decrypt_array[2] == 0x1E)//ģʽУ���
        {
          //bit 3 4 5 6 ΪУ����
          if(0 ==strncmp((const char *)return_decrypt_array+3, Usr_Password, 4))
          {
              Handle_Mode = USER_MODE;  //�û�
          }
          else if(0 ==strncmp((const char *)return_decrypt_array+3, Mgr_Password, 4))
          {
              Handle_Mode = MANAGE_MODE; 
              ARRAY_LENGTH = 4;          
              send_buffer[0] = 0x5a;
              send_buffer[1] = 0x04;
              send_buffer[2] = 0x27;  
              send_buffer[3] = Water_Company_Number >> 8;
              send_buffer[4] = Water_Company_Number & 0x00ff;
              send_buffer[5] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
              send_buffer[6] = 0x4a;
              encrypt_methon(send_buffer); //������� 
              //delay_ms(1000);//APP������ʱ
              SendData16(return_encrypt_array);//����
          }else
          {
              Handle_Mode = UNDEFINE_MODE;
          }
          u1_printf("Handle_Mode 0x%x\n", Handle_Mode);
          Show_Flag &= 0x40;
          Show_Flag |= (0x02);
        } 
        else if(return_decrypt_array[2] == 0x23)//�۷�
        {
          float deduct_money = ((return_decrypt_array[3] & 0xff) << 8 | (return_decrypt_array[4] & 0xff) )+ return_decrypt_array[5]*0.01f;
          Money -= deduct_money;
          EEPROM_WriteF32(MONEY_ADDR,Money);
          ARRAY_LENGTH = 2;            
          send_buffer[0] = 0x5a;
          send_buffer[1] = 0x02;
          send_buffer[2] = 0x21;  
          send_buffer[3] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
          send_buffer[4] = 0x4a;
          encrypt_methon(send_buffer); //������� 
          delay_ms(Xms);//APP������ʱ
          SendData16(return_encrypt_array);       //����
        }
        else if(return_decrypt_array[2] == 0x25)//�������
        {
          Account_Flag  = 1;
          Money = ((return_decrypt_array[3] & 0xff) << 8 | (return_decrypt_array[4] & 0xff) )+ return_decrypt_array[5]*0.01f;
          ARRAY_LENGTH = 3;            
          send_buffer[0] = 0x5a;
          send_buffer[1] = 0x03;
          send_buffer[2] = 0x22;  
          send_buffer[3] = 0x09;
          send_buffer[4] = crc_generate_checkout_code(send_buffer+1);//���ɵ�У����  
          send_buffer[5] = 0x4a;
          encrypt_methon(send_buffer); //������� 
          delay_ms(Xms);//APP������ʱ
          SendData16(return_encrypt_array);       //����
        }
        else if(return_decrypt_array[2] == 0x26)//дˮ˾��
        {
          Water_Company_Number = (return_decrypt_array[3] & 0xff) << 8 | (return_decrypt_array[4] & 0xff);
          EEPROM_WriteU16(WATER_COMPANY_NUMBER,Water_Company_Number);
        }
      }    
    }
  else
  {
    ARRAY_LENGTH = 2;
    send_buffer[0]=0x5a;
    send_buffer[1]=0x02;
    send_buffer[2]=0xEE;
    send_buffer[3]=crc_generate_checkout_code(send_buffer+1);//���ɵ�У����
    send_buffer[4]=0x4a;
    encrypt_methon(send_buffer); //������� 
    //SendData16(return_encrypt_array);
    //u1_printf("verify error\n");
  }
}
void u3_pritnf_data(unsigned char *data)
{
  int i = 0;
  while(i < 16)
  u3_printf("%2x ",data[i++]);
}

void pritnf_data(unsigned char *data)
{
    int i = 0;
    while(i < 16)
    u1_printf("%2x ",data[i++]);
}
void pritnf_data_Name(unsigned char *data)
{
  int i = 0;
  while(i < 30)
  u1_printf("%c",data[i++]);
}
void UART_PutChar(USART_TypeDef* USARTx, char Data)
{
    USART_SendData8(USARTx, Data);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){}
}
void UART_PutStr (USART_TypeDef* USARTx, char *str)  
{  
    while (0 != *str)  
    {  
        UART_PutChar(USARTx, *str);  
        str++;  
    }  
}
void Change_BLE_Parameter(char *data)//�޸���������
{
  char* str=(char*)malloc(50);
  memcpy(str, data,50);
  str[50 -1] = '\0'; //��Ҫ�����ڲ�ʹ��bʱfree(str);
  UART_PutStr(USART3,str);
  free(str);
}
void u32tostr(unsigned long dat,char *str)
{
  char temp[20];
  unsigned char i=0,j=0;
  i=0;
  while(dat)
  {
    temp[i]=dat%10+0x30;
    i++;
    dat/=10;
  }
  j=i;
  for(i=0;i<j;i++)
  {
   str[i]=temp[j-i-1];
  }
  if(!i) {str[i++]='0';}
  str[i]=0;
  memset(temp, 0, sizeof(temp));
}
void Uart_GPIO_Switch()
{
  disableInterrupts();
  EXTI_DeInit();
  EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Falling);
  GPIO_Init( GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_IT );
 // ITC_SetSoftwarePriority(EXTI2_IRQn, ITC_PriorityLevel_3);
  enableInterrupts();
}
void Message_Serverce_Usart()//Э������������ڣ�
{
 if(strncmp((const char*)Usart1_RecvBuf,"TTM:",4)==0)
  {
    GPIO_ResetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
    delay_ms(100);
    u3_printf(Usart1_RecvBuf);
    delay_ms(100);
    GPIO_SetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
    memset(Usart1_RecvBuf, 0, sizeof(Usart1_RecvBuf));
    halt_flag = 0;
  }
  if(Usart1_RecvBuf[0] == 0x5a)
  {
    if(Usart1_RecvBuf[1] == 0x01)//д����ID��
    {  
      if(Account_Flag == 0)
       {       
         EEPROM_WriteByte(BLE_ID_ADDR,  Usart1_RecvBuf[2]);
         EEPROM_WriteByte(BLE_ID_ADDR+1,Usart1_RecvBuf[3]);
         EEPROM_WriteByte(BLE_ID_ADDR+2,Usart1_RecvBuf[4]);
         EEPROM_WriteByte(BLE_ID_ADDR+3,Usart1_RecvBuf[5]);       
         u32 BLE_Name = 0;
         char *pBuf = (char*)&BLE_Name;
         pBuf[0] = Usart1_RecvBuf[2];
         pBuf[1] = Usart1_RecvBuf[3];
         pBuf[2] = Usart1_RecvBuf[4];
         pBuf[3] = Usart1_RecvBuf[5];       
         char str[10] = {0};
         u32tostr(BLE_Name,str);
         char BLE_Name_Buf[20] = "TTM:REN-";
         strncat(BLE_Name_Buf,str,sizeof(str));
         GPIO_ResetBits(BLE_BRTS_COM,BLE_BRTS_PIN);
         delay_ms(1000);
         u3_printf(BLE_Name_Buf);
         delay_ms(100);
         GPIO_SetBits(BLE_BRTS_COM,BLE_BRTS_PIN);  
         memset(BLE_Name_Buf, 0, sizeof(BLE_Name_Buf));
         memset(str, 0, sizeof(str));
         u1_printf("write BLE ID OK\n");
       }
       else 
       {u1_printf("modification failed");}
    }
    else if(Usart1_RecvBuf[1] == 0x09)//д����������
    {
      
    }
    else if(Usart1_RecvBuf[1] == 0x04)//д����ϵ��
    {
        Pulse_Factor = Usart1_RecvBuf[2];
        State1_Flag &= ~(0x0f);
        State1_Flag |= Pulse_Factor;
        EEPROM_WriteByte(STATE_FLAG1_ADDR,State1_Flag);
        halt_flag = 0;
    }
  } 
}
