#include "variable.h"
//共阴极的数码管0~f的段编码是
unsigned char table1[]={//共阴极0~f数码管编码
0x3f,0x06,0x5b,0x4f,0x66,
0x6d,0x7d,0x07,0x7f,0x6f,//0--9    高位-GFEDCBA 低位
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

   —  a
  f\ \b
 g —  
 e \ \c
    —d
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
/*功能函数实现*/
unsigned char* Register_Data;
#define MONEY_DOWN_FLAG   Money<Lack_Money


void LcdInit()
{
    LCD_DeInit();//LCD相关寄存器恢复默认值  
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
    功能：用于显示上方通道号信息
    参数：通道号 1-8 
*/
//从左至右1--8
//显示一个数字
//parameter： n 显示的数值 或者 一些字符(字符可以通过宏定义CHAR_x类宏)   bit显示第几位数值

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
  Register_Data = memset(Register_Data, 0, 14);//清显示缓存
}

void Flush_ShowBuf()
{
    int j;
    for(j = 0; j < 14; j++)//把数据填充到LCD_RAM
    {
        LCD->RAM[j] =  Register_Data[j];
    }
}
/*
* 功能 ：显示小数 需要靠右边显示
* 参数 ：n 需要显示的小数   decimal place 显示小数需要的的位数
* 88888888 （w0 -> w7）
*/
void Show_Float(float n, int decimal_place)
{
    int j = 0;
    char *buf = malloc(20);
    char *head = buf;
    sprintf(buf, "%.3f", n);//默认3位小数
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


//显示时间 
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

//显示时间 
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

//显示除数字以外的其他部分
//汉字以拼音的方式传入函数  具体看lcd.h
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

            以下为用户调用函数

***************************************************/

//显示版本号
void Pic_Version(float version)
{
  Clean_ShowBuf();
  Show_Number(CHAR_u, 0);
  Show_Number(1, 1);
  Show_Float(version, 2);
  Flush_ShowBuf();
}


//connect succeed
//显示succ
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
//显示date
void Pic_Date()
{
  Clean_ShowBuf();
  Show_Number(CHAR_d, 2);
  Show_Number(CHAR_a, 3);
  Show_Number(CHAR_t, 4);
  Show_Number(CHAR_e, 5);
  Flush_ShowBuf();
}

//异常显示
/******* n表示异常类型   
n可以是以下类型：
E1000001	电压低于2.9V，1级低压	电池松动或更换电池
E1000002	电压低于2.7V，2级低压	电池松动或更换电池
E1000003	电压低于2.0V，掉电	电池松动或更换电池
E1000010	磁干扰	                撤销外部磁干扰
E1000020	5次以上磁干扰	        撤销外部磁干扰
E2000001	阀门故障	        检测阀门开关是否正常，电池是否低压		
E2000100	保存数据校验错误	检查存储器是否短接或断电
E2000200	外部存储器通讯错误	检查存储器是否短接或断电
*******/
void Pic_Abnormal(u32 n)
{
   Clean_ShowBuf();
   Show_Number(CHAR_e, 0);
   Show_Float(n, 0);
   Flush_ShowBuf();
}
//显示S-rEnE 
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

//显示Test
void Pic_Test()
{
  Clean_ShowBuf();
  Show_Number(CHAR_T, 2);
  Show_Number(CHAR_e, 3);
  Show_Number(CHAR_s, 4);
  Show_Number(CHAR_t, 5);
  Flush_ShowBuf();
}
//显示Corr
void Pic_Corr()
{
  Clean_ShowBuf();
  Show_Number(CHAR_d, 2);
  Show_Number(CHAR_a, 3);
  Show_Number(CHAR_T, 4);
  Show_Number(CHAR_e, 5);
  Flush_ShowBuf();
}
//显示水表号
void Pic_Id()
{
  Clean_ShowBuf();

  Show_Number(1, 3);
  Show_Number(CHAR_d, 4);

  Flush_ShowBuf();
}

//control
//显示cotl
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

//充值金额
void Top_Up_Amount(float money)
{
  Clean_ShowBuf();
  if(POWER_LOW_FLAG)
    show_param(HUAN_DIAN_CHI);
  
  show_param( CHONG_ZHI | YUAN);
  Show_Float(money, 2);
  Flush_ShowBuf();
}


//剩余金额  输入金额
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

//累计用水量  输入用水量
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

//当月用水量  输入用水量
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

//单价
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

//显示日期  输入日期结构体
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
//显示时间  输入时间结构体
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
void ALL_Display()//全显
{
  memset(Register_Data, 0xff, 14);
  Flush_ShowBuf();
}
void Clear_Display()//清屏
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
     case 2: Pic_Version(VERSION_NUMBER);break;//版本号
     case 3: FaMenOparet_Display();break;//阀门操作
     case 4: Remain_Amount(Money); break;//剩余金额
     case 5: Cumulative_Water_ALL(Consumption);break;//累计
     case 6: Cumulative_Water_Month(Consumption_Month);break;//当月
     case 7: 
          if(Consumption_Month <= First_Section)//月用水量小于十吨     
              Price_Water(First_Stage);
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//月用水量大于十吨小于20吨       
              Price_Water(Second_Stage);     
          else if(Consumption_Month > Second_Section)//月用水量大于20吨
              Price_Water(Third_Stage);
          break;//当前单价
     case 8: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//日期
     case 9: if(Account_Flag  == 0)
              {Cumulative_Water_ALL(Consumption);}
              else
              {Clear_Display();}
              Show_Flag &= ~(0x01);
              times = 0;
              u1_printf("SystemStart_Show End\n");
              break;//清标志
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
         case 2: Remain_Amount(Money); break;//剩余金额
         case 3: Cumulative_Water_ALL(Consumption);break;//累计
         case 4: Cumulative_Water_Month(Consumption_Month);break;//当月
         case 5: if(Consumption_Month <= First_Section)//月用水量小于十吨     
                      Price_Water(First_Stage);
                  else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//月用水量大于十吨小于20吨       
                      Price_Water(Second_Stage);     
                  else if(Consumption_Month > Second_Section)//月用水量大于20吨
                      Price_Water(Third_Stage);
                  break;//当前单价
         case 6: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
                 Show_Date(RTC_DateStr);
                 break;//日期
         default:Cumulative_Water_ALL(Consumption);
                  Times_BLE = 18;//防溢出
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
         case 2: Remain_Amount(Money); break;//剩余金额
         case 3: if(Consumption_Month <= First_Section)//月用水量小于十吨     
                      Price_Water(First_Stage);
                  else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//月用水量大于十吨小于20吨       
                      Price_Water(Second_Stage);     
                  else if(Consumption_Month > Second_Section)//月用水量大于20吨
                      Price_Water(Third_Stage);
                  break;//当前单价
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
//5.清表数据显示，然后依次显示剩余金额—累计用量—当月用量—单价—时间
     case 1: Remain_Amount(Money); break;//剩余金额
     case 2: Cumulative_Water_ALL(Consumption);break;//总用水量
     case 3: Cumulative_Water_Month(Consumption_Month);break;//当月用水量
     case 4: 
          if(Consumption_Month <= First_Section)//月用水量小于十吨     
              Price_Water(First_Stage);
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//月用水量大于十吨小于20吨       
              Price_Water(Second_Stage);     
          else if(Consumption_Month > Second_Section)//月用水量大于20吨
              Price_Water(Third_Stage);
          break;//当前单价
     case 5: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//日期
     case 6: Cumulative_Water_ALL(Consumption);
              Show_Flag &= ~(0x08);
              Clear_Data_Times = 0;
              u1_printf("Clear_Data_Show End\n");
              break;//清标志
     }
}

void Clear_anomaly_Show()//清异常显示
{
  char i;
  static char Clear_anomaly_Times = 0;
  i = (Clear_anomaly_Times++/3)+1;
     switch (i)
     {
//5.清表数据显示，然后依次显示剩余金额—累计用量—当月用量—单价—时间
     case 1: Remain_Amount(Money); break;//剩余金额
     case 2: Cumulative_Water_ALL(Consumption);break;//总用水量
     case 3: Cumulative_Water_Month(Consumption_Month);break;//当月用水量
     case 4: 
          if(Consumption_Month <= First_Section)//月用水量小于十吨     
              Price_Water(First_Stage);
          else if(Consumption_Month > First_Section && Consumption_Month <= Second_Section )//月用水量大于十吨小于20吨       
              Price_Water(Second_Stage);     
          else if(Consumption_Month > Second_Section)//月用水量大于20吨
              Price_Water(Third_Stage);
          break;//当前单价
     case 5: RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
             Show_Date(RTC_DateStr);
             break;//日期
     case 6: Cumulative_Water_ALL(Consumption);
              Show_Flag &= ~(0x80);
              Clear_anomaly_Times = 0;
              u1_printf("Clear_anomaly_Show End\n");
              break;//清标志
     }
}
void Read_ID_Show()
{
  char i;
  static char Read_ID_Times = 0;
  i = (Read_ID_Times++/3)+1;
     switch (i)
     {
  //8.读写表ID显示，液晶显示id
     case 1: 
       Pic_Id(); break;//show  ID
       
     case 2: Cumulative_Water_ALL(Consumption);
              Show_Flag &= ~(0x10);
              Read_ID_Times = 0;
              u1_printf("Read_ID_Show End\n");
              break;//清标志
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
             break;//日期
     case 3: RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
             Show_Time(RTC_TimeStr);
             break;
     case 4: Cumulative_Water_ALL(Consumption);
             Show_Flag &= ~(0x20);
             times = 0;
             break;//时分秒
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
  if(Show_Flag1 & 0x01)//电源故障
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
  if(Show_Flag1 & 0x08)//磁干扰
  {Pic_Abnormal(1000010);}
  else if(Show_Flag1 & 0x10)
  {Pic_Abnormal(1000020);}
  if(i <= 2)
  {return;}
    
  loop1: 
  if(Show_Flag1 & 0x20)//阀门故障
  {Pic_Abnormal(2000001);}
  if(i == 4)
  {times = 0;}
}
void System_Display()
{
  if(Show_Flag == 0)
  {
     
  }else if(Show_Flag & 0x01)//1.开机显示
  {
     SystemStart_Show();
  }else if(Show_Flag & 0x40) //7.异常显示  
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
  }else if(Show_Flag & 0x02)//2.蓝牙连接显示
  {
  loop2:
     BLE_Connect_Show();
  }else if(Show_Flag & 0x04) //3.充值成功显示
  {
     Recharge_Ok_Show();
  }else if(Show_Flag & 0x08) //4.清数据显示
  {
     Clear_Data_Show(); 
  }else if(Show_Flag & 0x10) //5.液晶显示id
  {
     Read_ID_Show();
  }else if(Show_Flag & 0x20) //6.修改时间显示
  {
     Chang_Date_Show();
  }
  else if(Show_Flag & 0x80) //8.清异常显示
  {
    Clear_anomaly_Show();
  }
}