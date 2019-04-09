#include "system.h"
#include "variable.h"

float First_Stage = 0;  //阶梯水价
float Second_Stage = 0;
float Third_Stage = 0;
float low_power_flage;
char NoPower_flag = 0;
char Wait_Oparet = 0;
float First_Section = 0; 
float Second_Section = 0;
float Third_Section = 0;
float Consumption;    //还没存入没存 00
char RTC_IT_Flag;
char Magnetic_Interference_Times = 0;//磁干扰次数
char Magnetic_Interference_Time = 0;//磁干扰时间
u8 reed_switch_A_time1 = 0,reed_switch_A_time = 0;
u8 reed_switch_B_time1 = 0,reed_switch_B_time = 0;
u8 reed_switch_A_flag  = 0,reed_switch_B_flag = 0;
u8 reed_switch_interval_time1 = 0,reed_switch_interval_time = 0;//间隔时间
u8 Pulse_Factor;//脉冲系数，需要存储
int USART3_RX_STA = 0;
int USART1_RX_STA = 0;
char halt_flag = 0;
char Usart3_RecvBuf[USART_RECV_LEN]; //串口 传输/接受 数组
char Usart3_SendBuf[USART_RECV_LEN];
char Usart1_RecvBuf[USART1_RECV_LEN];
char Usart1_SendBuf[USART1_SEND_LEN];
char BLE_Flag;
u8 Check_Date;
uint8_t Check_Month;   
char Account_Flag; 
char Times_Famen = 0;
float Money;    //余额
char Show_Flag = 0;//整体显示
char Show_Flag1 = 0;//异常报警显示
float Consumption_Month;
char ValveControl_Close_flag();//管理员关阀门标志
float OverDraft_Money;//透支补水金额
char Lack_Money;//透支补水金额
char DisCharging_flag = 0;//每天放电标识
char Famen_Flag;
char State1_Flag, State2_Flag;
char Handle_Mode = 0x01;   //管理员 或 用户 标识
float Confirm_Money = 0;
char Insufficient_Balance_Flag = 0;//余额不足关阀标志(用来判断余额不足只关一次阀门的)
char POWER_LOW_FLAG ;//低电量标志
char OC_Flag;//返回开关阀成功标志位（置位则返回，不置位则不返回）
int Water_Company_Number;//水司编号
char Times_BLE = 0;
RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_DateTypeDef   RTC_DateStr;
const char Usr_Password[] = {0x01, 0x02, 0x03, 0x04};
const char Mgr_Password[] = {0x04, 0x03, 0x02, 0x01};

void delay_1ms(void)//1ms延时函数
{
 int i=432;
 while(--i);
}
void delay_ms(int ms)//ms延时函数
{
 while(--ms)
 {
   delay_1ms();
 }
}
void System_Init()
{
  BLE_Init(); //引脚初始化
  LcdInit();      //液晶初始化
  USART3_Config();//初始化USART3 TIM3 蓝牙引脚初始化
  USART1_Config();//usart1
  Reed_Switch_Init(); //干簧管引脚初始化
  ValveInit();  //阀门初始化
  rtc_Init();  //设置RTC唤醒中断 1S
  Calendar_Init();//rtc日历初始化
  //Adc_Init();    //adc初始化
  ValveControl_Close_User();
  Reset_Detection();
  Wait_Oparet = (FAMEN_MAINTAIN1 | FAMEN_MAINTAIN2); 
  Show_Flag |= (0x01);//初次上电及复位显示
  BLE_Flag = BLE_DISCONNECT;
  Famen_Flag = FAMEN_CLOSE;
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr); //Check_Date检测天数初始化
  Check_Date = RTC_DateStr.RTC_Date; 
  Check_Month = RTC_DateStr.RTC_Month;
  power_down_recover();
  halt_flag =0;
  if(Account_Flag  == 0)
  {Handle_Mode = MANAGE_MODE;}
  enableInterrupts();   //开启中断
  BLE_Rest_Break();//蓝牙断开
  u1_printf("OK...\n");
}