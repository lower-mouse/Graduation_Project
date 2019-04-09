#include "variable.h"
//�������������0~f�Ķα�����
unsigned char table1[]={//������0~f����ܱ���
0x3f,0x06,0x5b,0x4f,0x66,
0x6d,0x7d,0x07,0x7f,0x6f,//0--9    ��λ-GFEDCBA ��λ
0x40,0x3e,0x6d,0x39,0x5c,0x54,0x78,//'-','u','s','c','o','n','t'
0x50,0x5e,0x5f,0x79,0x31//'r','d','G','E','T'
};
#define CHAR__ 10
#define CHAR_u 11
#define CHAR_s 12
#define CHAR_c 13
#define CHAR_o 14
#define CHAR_n 15
#define CHAR_t 16
#define CHAR_r 17
#define CHAR_d 18
#define CHAR_a 19
#define CHAR_e 20
#define CHAR_T 21


/*

   ��  a
  f\ \b
 g ��  
 e \ \c
    ��d
*/
int number[8][7]=
{//A,  B,  C,  D,  E,  F,  G 
    0,  28, 56, 85, 57, 1,  29, //w 1
    2,  30, 58, 87, 59, 3,  31, //w 2
    4,  32, 60, 89, 61, 5,  33, //w 3
    6,  34, 62, 91, 63, 7,  35, //w 4
    8,  36, 64, 93, 65, 9,  37, //w 5
    10, 38, 66, 95, 67, 11, 39, //w 6
    12, 40, 68, 97, 69, 13, 41, //w 7
    14, 42, 70, 99, 71, 15, 43  //w 8
};

//int display_param[17]={84, 86, 88, 90, 92, 94, 96, 98,
//                       48, 49,104, 21, 20, 47, 79, 23, 103};//, segx + 28, segx, segx + 56, segx + 84};
//int display_param[18]={84, 86, 88, 90,     92, 94, 96, 98, 
//                       48, 49, 104, 21,    20, 47, 75, 19,    103, 77};

  int display_param[18]={84, 86, 88, 90,     92, 94, 96, 98, 
                         48, 49, 104, 21,    20, 55, 83, 27,    111, 77};
/*���ܺ���ʵ��*/
unsigned char* Register_Data;
#define MONEY_DOWN_FLAG   Money<Lack_Money


void LcdInit()
{
    LCD_DeInit();//LCD��ؼĴ����ָ�Ĭ��ֵ  
 /* Enable LCD/RTC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

    /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_1,LCD_Divider_16,LCD_Duty_1_4, LCD_Bias_1_4, LCD_VoltageSource_External);
    

    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x30);  //seg20 seg21 //SEG19
    LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x08);
    
   /* To set contrast to mean value */
    //LCD_ContrastConfig(LCD_Contrast_3V3);
    LCD_ContrastConfig(LCD_Contrast_Level_6);
    
    LCD_DeadTimeConfig(LCD_DeadTime_0);
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);
    
    /* Enable LCD peripheral */ 
    LCD_Cmd(ENABLE);
    Register_Data = (unsigned char *)malloc(14);
    Register_Data = memset(Register_Data, 0, 14);
}
/* 8 19--25  s0 s1-s7            0-7
**27--28 31  36 s8 s9 s10--s15   8--15
  47 48  64   s16 s17  s18       20 21 19
*/

/*
    ���ܣ�������ʾ�Ϸ�ͨ������Ϣ
    ������ͨ���� 1-8 
*/
//��������1--8
//��ʾһ������
//parameter�� n ��ʾ����ֵ ���� һЩ�ַ�(�ַ�����ͨ���궨��CHAR_x���)   bit��ʾ�ڼ�λ��ֵ

void Show_Number(int n, int bit)
{
    int ram_num, ram_bit;
    
    unsigned char tmp = table1[n];
    int i;
    for(i = 0; i<7; i++)
    {
      if(tmp & 0x01)
      {
        ram_num  = number[bit][i]/8;
        ram_bit  = number[bit][i]%8;
        Register_Data[ram_num ] |= (0x01<<ram_bit);
      }
      tmp >>= 1;
    }    
}

void Clean_ShowBuf()
{
  Register_Data = memset(Register_Data, 0, 14);//����ʾ����
}

void Flush_ShowBuf()
{
    int j;
    for(j = 0; j < 14; j++)//��������䵽LCD_RAM
    {
        LCD->RAM[j] =  Register_Data[j];
    }
}
/*
* ���� ����ʾС�� ��Ҫ���ұ���ʾ
* ���� ��n ��Ҫ��ʾ��С��   decimal place ��ʾС����Ҫ�ĵ�λ��
* 88888888 ��w0 -> w7��
*/
void Show_Float(float n, int decimal_place)
{
    int j = 0;
    char *buf = malloc(20);
    char *head = buf;
    sprintf(buf, "%.3f", n);//Ĭ��3λС��
    int len = strlen(buf); // buf[len] == '\0'
    buf[len - (3 - decimal_place)] = '\0';
    len = strlen(buf);
    while(*buf)
    {
      if(*buf >='0' &&*buf <='9')
      {
        Show_Number((int)(*buf-'0'), (9-len)+(j++));
      }else if(*buf == '-')
      {
        Show_Number(CHAR__,(9-len)+(j++));
      }
      else
      {
         show_param(POINT_P1>>(decimal_place-1));
        //show_param(DIS_ALL);
      }
      buf++;
    }
    free(head);
}


//��ʾʱ�� 
void Display_Time(RTC_TimeTypeDef   RTC_TimeStr)
{
  int i;
  u16 table1[8];
  table1[0]=RTC_TimeStr.RTC_Hours/10;
  table1[1]=RTC_TimeStr.RTC_Hours%10;
  table1[2]=10;
  table1[3]=RTC_TimeStr.RTC_Minutes/10;
  table1[4]=RTC_TimeStr.RTC_Minutes%10;
  table1[5]=10;
  table1[6]=RTC_TimeStr.RTC_Seconds/10;
  table1[7]=RTC_TimeStr.RTC_Seconds%10;
  show_param(SHI_JIAN);
  for(i = 0; i < 8; i++)
  {
    Show_Number(table1[i],i);
  }
}

//��ʾʱ�� 
void Display_Date(RTC_DateTypeDef   RTC_DateStr)
{
  int i;
  u16 table1[8];
  table1[0]=RTC_DateStr.RTC_Year/10;
  table1[1]=RTC_DateStr.RTC_Year%10;
  table1[2]=10;
  table1[3]=RTC_DateStr.RTC_Month/10;
  table1[4]=RTC_DateStr.RTC_Month%10;
  table1[5]=10;
  table1[6]=RTC_DateStr.RTC_Date/10;
  table1[7]=RTC_DateStr.RTC_Date%10;
  for(i = 0; i < 8; i++)
  {
    Show_Number(table1[i],i);
  }
}

//��ʾ�������������������
//������ƴ���ķ�ʽ���뺯��  ���忴lcd.h
void show_param(uint32_t Prarm)
{
  int i;
  int ram_num, ram_bit;
  for(i = 0; i < 18; i++)
  {
      if(Prarm & ((uint32_t)0x00000001 << i))
      {
        ram_num = display_param[i]/8;
        ram_bit = display_param[i]%8;
        Register_Data[ram_num ] |= (0x01<<ram_bit);
      }
  }
}

/***************************************************

            ����Ϊ�û����ú���

***************************************************/

//��ʾ�汾��
void Pic_Version(float version)
{
  Clean_ShowBuf();
  Show_Number(CHAR_u, 0);
  Show_Number(1, 1);
  Show_Float(version, 2);
  Flush_ShowBuf();
}


//connect succeed
//��ʾsucc
void Pic_Connect()
{
  Clean_ShowBuf();
  Show_Number(CHAR_s, 2);
  Show_Number(CHAR_u, 3);
  Show_Number(CHAR_c, 4);
  Show_Number(CHAR_c, 5);
  Flush_ShowBuf();
}

//
//��ʾdate
void Pic_Date()
{
  Clean_ShowBuf();
  Show_Number(CHAR_d, 2);
  Show_Number(CHAR_a, 3);
  Show_Number(CHAR_t, 4);
  Show_Number(CHAR_e, 5);
  Flush_ShowBuf();
}

//�쳣��ʾ
/******* n��ʾ�쳣����   
n�������������ͣ�
E1000001	��ѹ����2.9V��1����ѹ	����ɶ���������
E1000002	��ѹ����2.7V��2����ѹ	����ɶ���������
E1000003	��ѹ����2.0V������	����ɶ���������
E1000010	�Ÿ���	                �����ⲿ�Ÿ���
E1000020	5�����ϴŸ���	        �����ⲿ�Ÿ���
E2000001	���Ź���	        ��ⷧ�ſ����Ƿ�����������Ƿ��ѹ		
E2000100	��������У�����	���洢���Ƿ�̽ӻ�ϵ�
E2000200	�ⲿ�洢��ͨѶ����	���洢���Ƿ�̽ӻ�ϵ�
*******/
void Pic_Abnormal(u32 n)
{
   Clean_ShowBuf();
   Show_Number(CHAR_e, 0);
   Show_Float(n, 0);
   Flush_ShowBuf();
}
//��ʾS-rEnE 
void Pic_Srene()
{
  Clean_ShowBuf();
  
  Show_Number(CHAR_s, 1);
  Show_Number(CHAR__, 2);
  Show_Number(CHAR_r, 3);
  Show_Number(CHAR_e, 4);
  Show_Number(CHAR_n, 5);
  Show_Number(CHAR_e, 6);
  Flush_ShowBuf();
}

//��ʾTest
void Pic_Test()
{
  Clean_ShowBuf();
  Show_Number(CHAR_T, 2);
  Show_Number(CHAR_e, 3);
  Show_Number(CHAR_s, 4);
  Show_Number(CHAR_t, 5);
  Flush_ShowBuf();
}
//��ʾCorr
void Pic_Corr()
{
  Clean_ShowBuf();
  Show_Number(CHAR_d, 2);
  Show_Number(CHAR_a, 3);
  Show_Number(CHAR_T, 4);
  Show_Number(CHAR_e, 5);
  Flush_ShowBuf();
}
//��ʾˮ���
void Pic_Id()
{
  Clean_ShowBuf();

  Show_Number(1, 3);
  Show_Number(CHAR_d, 4);

  Flush_ShowBuf();
}

//control
//��ʾcotl
void Pic_Control()
{
  Clean_ShowBuf();
  
  Show_Number(CHAR_c, 1);
  Show_Number(CHAR_o, 2);
  Show_Number(CHAR_n, 3);
  Show_Number(CHAR_t, 4);
  Show_Number(CHAR_r, 5);
  Flush_ShowBuf();
}

//��ֵ���
void Top_Up_Amount(float money)
{
  Clean_ShowBuf();
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  
  show_param( CHONG_ZHI | YUAN);
  Show_Float(money, 2);
  Flush_ShowBuf();
}


//ʣ����  ������
void Remain_Amount(float money)
{
  Clean_ShowBuf();
  if(Famen_Flag  == FAMEN_OPEN)
    show_param(FA_KAI);
   else
    show_param(FA_GUAN);
   
  if(MONEY_DOWN_FLAG)
    show_param(QING |CHONG_ZHI);
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  show_param(SHENG_YU  | YUAN);
  Show_Float(money, 2);
  Flush_ShowBuf();
}

//�ۼ���ˮ��  ������ˮ��
void Cumulative_Water_ALL(float water)
{
  Clean_ShowBuf();
    if(Famen_Flag == FAMEN_OPEN)
    show_param(FA_KAI);
   else 
    show_param(FA_GUAN);
  if(MONEY_DOWN_FLAG)
    show_param(QING |CHONG_ZHI);
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  show_param(LEI_JI | LI_FANG_MI);
  Show_Float(water, 1);
  
  Flush_ShowBuf();
}

//������ˮ��  ������ˮ��
void Cumulative_Water_Month(float water)
{
  Clean_ShowBuf();
   if(Famen_Flag == FAMEN_OPEN)
    show_param(FA_KAI);
   else 
    show_param(FA_GUAN);
     if(MONEY_DOWN_FLAG)
    show_param(QING |CHONG_ZHI);
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  show_param(DANG_YUE  | LI_FANG_MI);
  Show_Float(water, 1);
  Flush_ShowBuf();
}

//����
void Price_Water(float Price)
{
  Clean_ShowBuf();
    if(Famen_Flag == FAMEN_OPEN)
    show_param(FA_KAI);
   else 
    show_param(FA_GUAN);
     if(MONEY_DOWN_FLAG)
    show_param(QING |CHONG_ZHI);
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  
  show_param(DAN_JIA | LI_FANG_MI);
  Show_Float(Price, 2);
  Flush_ShowBuf();
}

//��ʾ����  �������ڽṹ��
void Show_Date(RTC_DateTypeDef   RTC_DateStr)
{
  Clean_ShowBuf();
    if(Famen_Flag == FAMEN_OPEN)
    show_param(FA_KAI);
   else 
    show_param(FA_GUAN);
    if(MONEY_DOWN_FLAG)
    show_param(QING |CHONG_ZHI);
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  
  show_param(RI_QI);
  Display_Date(RTC_DateStr);
  Flush_ShowBuf();
}
//��ʾʱ��  ����ʱ��ṹ��
void Show_Time(RTC_TimeTypeDef   RTC_TimeStr)
{
  Clean_ShowBuf();
  
  if(Famen_Flag == FAMEN_OPEN)
    show_param(FA_KAI);
   else 
    show_param(FA_GUAN);
   
  if(MONEY_DOWN_FLAG)
    show_param(QING |CHONG_ZHI);
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  
  show_param(SHI_JIAN);
  Display_Time(RTC_TimeStr);
  Flush_ShowBuf();
}
void ALL_Display()//ȫ��
{
  memset(Register_Data, 0xff, 14);
  Flush_ShowBuf();
}
void Clear_Display()//����
{
  Clean_ShowBuf();
  Flush_ShowBuf();
}
void FaMenOparet_Display()//----
{
  Clean_ShowBuf();
  Show_Number(CHAR__, 2);
  Show_Number(CHAR__, 3);
  Show_Number(CHAR__, 4);
  Show_Number(CHAR__, 5);
  Flush_ShowBuf();
}
void SystemStart_Show()
{
  char i;
  static char times = 0;
  i = (times++/3)+1;
     switch (i)
     {
     case 1: ALL_Display();break;
     case 2: Pic_Version(VERSION_NUMBER);break;//�汾��
     case 3: FaMenOparet_Display();break;//���Ų���
     case 4: Remain_Amount(Money); break;//ʣ����
     case 5: Cumulative_Water_ALL(Consumption);break;//�ۼ�
     case 6: Cumulative_Water_Month(Consumption_Month);break;//����
     case 7: 
          if(Consumption_Month <= First_Section)//����ˮ��С��ʮ��     
              Price_Water(First_Stage);
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//����ˮ������ʮ��С��20��       
              Price_Water(Second_Stage);     
          else if(Consumption_Month > Second_Section)//����ˮ������20��
              Price_Water(Third_Stage);
          break;//��ǰ����
     case 8: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//����
     case 9: if(Account_Flag  == 0)
              {Cumulative_Water_ALL(Consumption);}
              else
              {Clear_Display();}
              Show_Flag &= ~(0x01);
              times = 0;
              u1_printf("SystemStart_Show End\n");
              break;//���־
     }
}
void BLE_Connect_Show()
{
  char i;
  i = (Times_BLE++/3)+1;
     switch (i)
     {
         case 1: if(Handle_Mode == USER_MODE)
                  {Pic_Connect();}
                 else if(Handle_Mode == MANAGE_MODE)
                  {Pic_Test();}
                  break;
         case 2: Remain_Amount(Money); break;//ʣ����
         case 3: Cumulative_Water_ALL(Consumption);break;//�ۼ�
         case 4: Cumulative_Water_Month(Consumption_Month);break;//����
         case 5: if(Consumption_Month <= First_Section)//����ˮ��С��ʮ��     
                      Price_Water(First_Stage);
                  else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//����ˮ������ʮ��С��20��       
                      Price_Water(Second_Stage);     
                  else if(Consumption_Month > Second_Section)//����ˮ������20��
                      Price_Water(Third_Stage);
                  break;//��ǰ����
         case 6: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
                 Show_Date(RTC_DateStr);
                 break;//����
         default:Cumulative_Water_ALL(Consumption);
                  Times_BLE = 18;//�����
                  break;
     }
}
void Recharge_Ok_Show()
{
  char i;
  static char times = 0;
  i = (times++/3)+1;
     switch (i)
     {
         case 1: Top_Up_Amount(Confirm_Money);break;
         case 2: Remain_Amount(Money); break;//ʣ����
         case 3: if(Consumption_Month <= First_Section)//����ˮ��С��ʮ��     
                      Price_Water(First_Stage);
                  else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//����ˮ������ʮ��С��20��       
                      Price_Water(Second_Stage);     
                  else if(Consumption_Month > Second_Section)//����ˮ������20��
                      Price_Water(Third_Stage);
                  break;//��ǰ����
         default:Cumulative_Water_ALL(Consumption);
                  Show_Flag &= ~0x04;
                  times = 0;
                  break;
     }
}
void Clear_Data_Show()
{
  char i;
  static char Clear_Data_Times = 0;
  i = (Clear_Data_Times++/3)+1;
     switch (i)
     {
//5.���������ʾ��Ȼ��������ʾʣ����ۼ��������������������ۡ�ʱ��
     case 1: Remain_Amount(Money); break;//ʣ����
     case 2: Cumulative_Water_ALL(Consumption);break;//����ˮ��
     case 3: Cumulative_Water_Month(Consumption_Month);break;//������ˮ��
     case 4: 
          if(Consumption_Month <= First_Section)//����ˮ��С��ʮ��     
              Price_Water(First_Stage);
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//����ˮ������ʮ��С��20��       
              Price_Water(Second_Stage);     
          else if(Consumption_Month > Second_Section)//����ˮ������20��
              Price_Water(Third_Stage);
          break;//��ǰ����
     case 5: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//����
     case 6: Cumulative_Water_ALL(Consumption);
              Show_Flag &= ~(0x08);
              Clear_Data_Times = 0;
              u1_printf("Clear_Data_Show End\n");
              break;//���־
     }
}

void Clear_anomaly_Show()//���쳣��ʾ
{
  char i;
  static char Clear_anomaly_Times = 0;
  i = (Clear_anomaly_Times++/3)+1;
     switch (i)
     {
//5.���������ʾ��Ȼ��������ʾʣ����ۼ��������������������ۡ�ʱ��
     case 1: Remain_Amount(Money); break;//ʣ����
     case 2: Cumulative_Water_ALL(Consumption);break;//����ˮ��
     case 3: Cumulative_Water_Month(Consumption_Month);break;//������ˮ��
     case 4: 
          if(Consumption_Month <= First_Section)//����ˮ��С��ʮ��     
              Price_Water(First_Stage);
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//����ˮ������ʮ��С��20��       
              Price_Water(Second_Stage);     
          else if(Consumption_Month > Second_Section)//����ˮ������20��
              Price_Water(Third_Stage);
          break;//��ǰ����
     case 5: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//����
     case 6: Cumulative_Water_ALL(Consumption);
              Show_Flag &= ~(0x80);
              Clear_anomaly_Times = 0;
              u1_printf("Clear_anomaly_Show End\n");
              break;//���־
     }
}
void Read_ID_Show()
{
  char i;
  static char Read_ID_Times = 0;
  i = (Read_ID_Times++/3)+1;
     switch (i)
     {
  //8.��д��ID��ʾ��Һ����ʾid
     case 1: 
       Pic_Id(); break;//show  ID
       
     case 2: Cumulative_Water_ALL(Consumption);
              Show_Flag &= ~(0x10);
              Read_ID_Times = 0;
              u1_printf("Read_ID_Show End\n");
              break;//���־
     }
}
void Chang_Date_Show()
{
  char i;
  static char times = 0;
  i = (times++/3)+1;
     switch (i)
     {
     case 1:Pic_Date();break;
     case 2:RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//����
     case 3: RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
             Show_Time(RTC_TimeStr);
             break;
     case 4: Cumulative_Water_ALL(Consumption);
             Show_Flag &= ~(0x20);
             times = 0;
             break;//ʱ����
     }
}
void Abnormal_Show()
{
  char i;
  static char times = 0;
  i = (times++/3)+1;
  
  if(i > 1&&Show_Flag1>0x04)
  {
    goto loop;
  }
  if(Show_Flag1 & 0x01)//��Դ����
  {Pic_Abnormal(1000001);}
  else if(Show_Flag1 & 0x02)
  {Pic_Abnormal(1000002);}
  else if(Show_Flag1 & 0x04)
  {Pic_Abnormal(1000003);}
  if(i <= 1)
  {return;}
  
  loop: 
  if(i > 2&&Show_Flag1>0x10)
  {
    goto loop1;
  }
  if(Show_Flag1 & 0x08)//�Ÿ���
  {Pic_Abnormal(1000010);}
  else if(Show_Flag1 & 0x10)
  {Pic_Abnormal(1000020);}
  if(i <= 2)
  {return;}
    
  loop1: 
  if(Show_Flag1 & 0x20)//���Ź���
  {Pic_Abnormal(2000001);}
  if(i == 4)
  {times = 0;}
}
void System_Display()
{
  if(Show_Flag == 0)
  {
     
  }else if(Show_Flag & 0x01)//1.������ʾ
  {
     SystemStart_Show();
  }else if(Show_Flag & 0x40) //7.�쳣��ʾ  
  {
    if((Show_Flag & 0x02)&&Times_BLE < 18)
    {
      goto loop2;
    }
    Abnormal_Show();
    if(Show_Flag1 == 0)
    {
      if(Account_Flag  == 0||BLE_Flag == BLE_CONNECT)
      {
        Cumulative_Water_ALL(Consumption);
      }
      else
      {Clear_Display();}
      Show_Flag &= ~0x40;  
    }
  }else if(Show_Flag & 0x02)//2.����������ʾ
  {
  loop2:
     BLE_Connect_Show();
  }else if(Show_Flag & 0x04) //3.��ֵ�ɹ���ʾ
  {
     Recharge_Ok_Show();
  }else if(Show_Flag & 0x08) //4.��������ʾ
  {
     Clear_Data_Show(); 
  }else if(Show_Flag & 0x10) //5.Һ����ʾid
  {
     Read_ID_Show();
  }else if(Show_Flag & 0x20) //6.�޸�ʱ����ʾ
  {
     Chang_Date_Show();
  }
  else if(Show_Flag & 0x80) //8.���쳣��ʾ
  {
    Clear_anomaly_Show();
  }
}