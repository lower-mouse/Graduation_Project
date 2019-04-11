#ifndef __VARIABLE_H
#define __VARIABLE_H

#include "uart3.h"
#include "string.h"
#include <stdlib.h>
#include "LCD.h"
#include "computation.h"
#include "adc.h"
#include "eeprom.h"
#include "fakong.h"
#include <stdio.h>
#include "system.h"
#include "rtc.h"
#include  "encrpyt.h"
#include <math.h>
#include "ble.h"
#include "bc95.h"
//�ɻɹ�A�˿�λ�ö���
#define reed_switch_A_GPIO_TypeDef GPIOF
#define reed_switch_A_GPIO_PIN GPIO_Pin_7
//�ɻɹ�B�˿�λ�ö���
#define reed_switch_B_GPIO_TypeDef GPIOF
#define reed_switch_B_GPIO_PIN GPIO_Pin_6
//�ɻɹܹ���˿�λ�ö���
#define pul_power_GPIO_TypeDef GPIOC
#define pul_power_GPIO_PIN GPIO_Pin_0
#define BLE_CHECK_PIN    GPIO_Pin_6                                    //��������ָʾ����
#define BLE_CHECK_COM    GPIOG
#define BLE_EN_PIN       GPIO_Pin_5                                    //����EN
#define BLE_EN_COM       GPIOD
#define BLE_BRTS_PIN     GPIO_Pin_4                                    //����BRTS
#define BLE_BRTS_COM     GPIOG
#define BLE_BCRTS_PIN    GPIO_Pin_5                                    //����BCTS
#define BLE_BCTS_COM     GPIOG
#define VERSION_NUMBER   1.06                                          //�汾��
#define LIMIT_MONEY      60000                                         //�޶�ˮ���ֵ��
#define BLE_CONNECT    0x01                                            //�������ӱ�ʶ
#define BLE_DISCONNECT 0x02
#define FAMEN_OPEN   0X01                                              //���ű�ʶ
#define FAMEN_CLOSE  0X02
#define FAMEN_MAINTAIN1 0X08
#define FAMEN_MAINTAIN2 0X10
#define FAMEN_ERR    0X03
#define USER_MODE     0x01                                            
#define MANAGE_MODE   0x00                                            //����Ա״̬
#define UNDEFINE_MODE 0x02
#define USART_RECV_LEN    50                                        //���ݽ��ܳ���buffer��С
#define USART1_RECV_LEN   50                                         //����һ���ݽ��ܳ���buffer��С
#define USART_SEND_LEN    50                                         //���ݷ���buffer��С
#define USART1_SEND_LEN   50                                         //���ݷ���buffer��С
#define Xms               1                                         //������ʱʱ��
#define Check_Time        20                                         //���ų�ʱ�ж�ʱ��
extern char BLE_Flag;
/////////////////////////////////////
///     eeprom�еĵ�ַƫ��        ///
/////////////////////////////////////
#define CONSUMPTION               0x00                               //����ˮ�� size ��4                 
#define MONTH_BASE_ADDR           0x04                               //��ʷ��ˮ�� 0x04 -- 0x33  size ��48  
#define BLE_ID_ADDR               0x34                                 //����ID size : 4                    
#define MONEY_ADDR                0x38                               //��� size��4
#define OVERDRAFT_MONEY           0x78                                //͸֧��ˮ��� size��4
#define CONSUMPTION_MONTH_ADDR    0x4a                               //����ˮ��size��4
#define USR_PASSWORD              0x42                               //�û�����size��4
#define MGR_PASSWORD              0x46                               //����Ա����size��4
#define FIRST_STAGE               0x52                               //��һ����ˮ��size��4
#define SECOND_STAGE              0x56                               //�ڶ�����ˮ��size��4
#define THIRD_STAGE               0x5a                               //��������ˮ��size��4
#define FOUTH_STAGE               0x5c                               //���Ľ���ˮ��size��4
#define FIRST_SECTION             0x60                               //��һ����size��4
#define SECOND_SECTION            0x64                               //�ڶ�����size��4
#define THIRD_SECTION             0x68                               //��������size��4
#define FOUTH_SECTION             0x6c                               //��������size��4
#define SEWAGE_RATE               0x4e                               //��ˮ�����size��4
#define LACK_MONEY                0x70                               //�����޶�ֵ
#define WATER_COMPANY_NUMBER      0x71                               //ˮ˾���
#define DOWN_TIME_ADDR            0X74                               // ����ʱ��year��month��data�� size :4
#define STATE_FLAG1_ADDR          0x3c
/*  bit: 7        �Ƿ񿪻� 1���ѿ���     0��δ����
**  bit: 4-6     �Ÿ��Ŵ���
**  bit: 0-3     ����ϵ��
*/
#define STATE_FLAG2_ADDR     0x3d
/*  bit: 7      �ɻɹ�A��ͨ 1    ����ͨ 0
**  bit: 6      �ɻɹ�B��ͨ 1    ����ͨ 0
**  bit: 5      �����쳣       0����   1����
**  bit: 4      �Ÿ��Źط���ʶ 0����   1����
**  bit: 3      ����Ա�ط���ʶ 0����   1����
**  bit: 2      �����ʶ       0����   1����
**  bit: 0-1    �͵�����ʶ     00������
**                             01��һ����ѹ
**                             10��������ѹ
**                             11��������ѹ
*/
extern char Times_Famen;                                         //���Ŵ򿪣��رգ�ʱ��
extern int Water_Company_Number;                                     //ˮ˾���
extern char OC_Flag;                                                 //���ط����ر�־λ���˱�־λ��λ��ˮ���ؿ����أ����ɹ�
extern char Account_Flag;                                            //�Ƿ񿪻���־
extern u8 Check_Date;
extern char State1_Flag, State2_Flag;
extern int ARRAY_LENGTH;                                             //����У��������ݳ���  ��encrpyt.c�ļ��ж���
extern float First_Stage;                                            //����ˮ�� ����
extern float Second_Stage;
extern float Third_Stage;
extern char Handle_Mode;                                             //����Ա �� �û� ��ʶ
extern char Famen_Flag;                                              //����״̬��־����/�أ�
extern char Wait_Oparet;                                             //���Ŷ���
extern char Insufficient_Balance_Flag;                               //����ط���־(�����ж�����ֻ��һ�η��ŵ�)
extern char POWER_LOW_FLAG;                                          //�͵�����־
extern uint8_t Check_Month;                                          //�ж��·ݷ���
extern uint8_t Check_Date;                                           //�ж���������
extern char RTC_IT_Flag;                                             //RTC�жϱ�ʶλ
extern char Usr_Password[4];                                         //�û�����
extern char Mgr_Password[4];                                         //����Ա����
extern float Consumption;                                            //����ˮ��
extern float Consumption_Month;                                      //����ˮ��
extern char Times_BLE;                                               //������ʾʱ��
extern u8 reed_switch_A_time1,reed_switch_A_time;                    //�ɻɹ�A��ͨʱ��
extern u8 reed_switch_B_time1,reed_switch_B_time;                    //�ɻɹ�B��ͨʱ��
extern u8 reed_switch_A_flag ,reed_switch_B_flag;                    //�ɻɹ�A/B��ͨ��־
extern u8 reed_switch_interval_time1 ,reed_switch_interval_time;     //���ʱ��
extern float First_Section;                                         //��������
extern float Second_Section;
extern float Third_Section;
extern u8 Pulse_Factor;                                             //����ϵ������Ҫ�洢
extern char Show_Flag;                                             //LCD��ʾ����
extern char Show_Flag1;                                             //�쳣������ʾ
extern float Money;                                               //���
extern float OverDraft_Money;                                     //͸֧��ˮ���
extern char Lack_Money;                                          //�����޶�ֵ
extern char DisCharging_flag;                                     //ÿ��ŵ��ʶ
extern float    Confirm_Money;                                    //Ԥ��ֵ���
extern int      USART3_RX_STA;                                    //���ڽ��ܱ�־
extern int      USART1_RX_STA;
extern char     halt_flag;                                        //����haltģʽ����
extern char     NoPower_flag;                                     //�����־    
extern char Magnetic_Interference_Times;                          //�Ÿ��Ŵ���
extern char Magnetic_Interference_Time;                           //�Ÿ���ʱ��
extern char Usart3_RecvBuf[USART_RECV_LEN];                       //���� ����/���� ����
extern char Usart3_SendBuf[USART_RECV_LEN];
extern char Usart1_RecvBuf[USART1_RECV_LEN];
extern char Usart1_SendBuf[USART1_SEND_LEN];
extern unsigned char return_encrypt_array[16];                    //���� ������ݽ������������
extern unsigned char return_decrypt_array[16];                    //���� ������ݽ������������
extern RTC_InitTypeDef   RTC_InitStr; //RTC
extern RTC_TimeTypeDef   RTC_TimeStr;
extern RTC_DateTypeDef   RTC_DateStr;


#endif