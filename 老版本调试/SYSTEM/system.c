#include "system.h"
#include "variable.h"

float First_Stage = 0;  //����ˮ��
float Second_Stage = 0;
float Third_Stage = 0;
float low_power_flage;
char NoPower_flag = 0;
char Wait_Oparet = 0;
float First_Section = 0; 
float Second_Section = 0;
float Third_Section = 0;
float Consumption;    //��û����û�� 00
char RTC_IT_Flag;
char Magnetic_Interference_Times = 0;//�Ÿ��Ŵ���
char Magnetic_Interference_Time = 0;//�Ÿ���ʱ��
u8 reed_switch_A_time1 = 0,reed_switch_A_time = 0;
u8 reed_switch_B_time1 = 0,reed_switch_B_time = 0;
u8 reed_switch_A_flag  = 0,reed_switch_B_flag = 0;
u8 reed_switch_interval_time1 = 0,reed_switch_interval_time = 0;//���ʱ��
u8 Pulse_Factor;//����ϵ������Ҫ�洢
int USART3_RX_STA = 0;
int USART1_RX_STA = 0;
char halt_flag = 0;
char Usart3_RecvBuf[USART_RECV_LEN]; //���� ����/���� ����
char Usart3_SendBuf[USART_RECV_LEN];
char Usart1_RecvBuf[USART1_RECV_LEN];
char Usart1_SendBuf[USART1_SEND_LEN];
char BLE_Flag;
u8 Check_Date;
uint8_t Check_Month;   
char Account_Flag; 
char Times_Famen = 0;
float Money;    //���
char Show_Flag = 0;//������ʾ
char Show_Flag1 = 0;//�쳣������ʾ
float Consumption_Month;
char ValveControl_Close_flag();//����Ա�ط��ű�־
float OverDraft_Money;//͸֧��ˮ���
char Lack_Money;//͸֧��ˮ���
char DisCharging_flag = 0;//ÿ��ŵ��ʶ
char Famen_Flag;
char State1_Flag, State2_Flag;
char Handle_Mode = 0x01;   //����Ա �� �û� ��ʶ
float Confirm_Money = 0;
char Insufficient_Balance_Flag = 0;//����ط���־(�����ж�����ֻ��һ�η��ŵ�)
char POWER_LOW_FLAG ;//�͵�����־
char OC_Flag;//���ؿ��ط��ɹ���־λ����λ�򷵻أ�����λ�򲻷��أ�
int Water_Company_Number;//ˮ˾���
char Times_BLE = 0;
RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_DateTypeDef   RTC_DateStr;
const char Usr_Password[] = {0x01, 0x02, 0x03, 0x04};
const char Mgr_Password[] = {0x04, 0x03, 0x02, 0x01};

void delay_1ms(void)//1ms��ʱ����
{
 int i=432;
 while(--i);
}
void delay_ms(int ms)//ms��ʱ����
{
 while(--ms)
 {
   delay_1ms();
 }
}
void System_Init()
{
  BLE_Init(); //���ų�ʼ��
  LcdInit();      //Һ����ʼ��
  USART3_Config();//��ʼ��USART3 TIM3 �������ų�ʼ��
  USART1_Config();//usart1
  Reed_Switch_Init(); //�ɻɹ����ų�ʼ��
  ValveInit();  //���ų�ʼ��
  rtc_Init();  //����RTC�����ж� 1S
  Calendar_Init();//rtc������ʼ��
  //Adc_Init();    //adc��ʼ��
  ValveControl_Close_User();
  Reset_Detection();
  Wait_Oparet = (FAMEN_MAINTAIN1 | FAMEN_MAINTAIN2); 
  Show_Flag |= (0x01);//�����ϵ缰��λ��ʾ
  BLE_Flag = BLE_DISCONNECT;
  Famen_Flag = FAMEN_CLOSE;
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr); //Check_Date���������ʼ��
  Check_Date = RTC_DateStr.RTC_Date; 
  Check_Month = RTC_DateStr.RTC_Month;
  power_down_recover();
  halt_flag =0;
  if(Account_Flag  == 0)
  {Handle_Mode = MANAGE_MODE;}
  enableInterrupts();   //�����ж�
  BLE_Rest_Break();//�����Ͽ�
  u1_printf("OK...\n");
}