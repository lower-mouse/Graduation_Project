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
//干簧管A端口位置定义
#define reed_switch_A_GPIO_TypeDef GPIOF
#define reed_switch_A_GPIO_PIN GPIO_Pin_7
//干簧管B端口位置定义
#define reed_switch_B_GPIO_TypeDef GPIOF
#define reed_switch_B_GPIO_PIN GPIO_Pin_6
//干簧管供电端口位置定义
#define pul_power_GPIO_TypeDef GPIOC
#define pul_power_GPIO_PIN GPIO_Pin_0
#define BLE_CHECK_PIN    GPIO_Pin_6                                    //蓝牙连接指示引脚
#define BLE_CHECK_COM    GPIOG
#define BLE_EN_PIN       GPIO_Pin_5                                    //蓝牙EN
#define BLE_EN_COM       GPIOD
#define BLE_BRTS_PIN     GPIO_Pin_4                                    //蓝牙BRTS
#define BLE_BRTS_COM     GPIOG
#define BLE_BCRTS_PIN    GPIO_Pin_5                                    //蓝牙BCTS
#define BLE_BCTS_COM     GPIOG
#define VERSION_NUMBER   1.06                                          //版本号
#define LIMIT_MONEY      60000                                         //限定水表充值额
#define BLE_CONNECT    0x01                                            //蓝牙连接标识
#define BLE_DISCONNECT 0x02
#define FAMEN_OPEN   0X01                                              //阀门标识
#define FAMEN_CLOSE  0X02
#define FAMEN_MAINTAIN1 0X08
#define FAMEN_MAINTAIN2 0X10
#define FAMEN_ERR    0X03
#define USER_MODE     0x01                                            
#define MANAGE_MODE   0x00                                            //管理员状态
#define UNDEFINE_MODE 0x02
#define USART_RECV_LEN    50                                        //数据接受长度buffer大小
#define USART1_RECV_LEN   50                                         //串口一数据接受长度buffer大小
#define USART_SEND_LEN    50                                         //数据发送buffer大小
#define USART1_SEND_LEN   50                                         //数据发送buffer大小
#define Xms               1                                         //适配延时时间
#define Check_Time        20                                         //阀门超时判断时间
extern char BLE_Flag;
/////////////////////////////////////
///     eeprom中的地址偏移        ///
/////////////////////////////////////
#define CONSUMPTION               0x00                               //总用水量 size ：4                 
#define MONTH_BASE_ADDR           0x04                               //历史用水量 0x04 -- 0x33  size ：48  
#define BLE_ID_ADDR               0x34                                 //蓝牙ID size : 4                    
#define MONEY_ADDR                0x38                               //余额 size：4
#define OVERDRAFT_MONEY           0x78                                //透支补水金额 size：4
#define CONSUMPTION_MONTH_ADDR    0x4a                               //月用水量size：4
#define USR_PASSWORD              0x42                               //用户密码size：4
#define MGR_PASSWORD              0x46                               //管理员密码size：4
#define FIRST_STAGE               0x52                               //第一阶梯水价size：4
#define SECOND_STAGE              0x56                               //第二阶梯水价size：4
#define THIRD_STAGE               0x5a                               //第三阶梯水价size：4
#define FOUTH_STAGE               0x5c                               //第四阶梯水价size：4
#define FIRST_SECTION             0x60                               //第一区间size：4
#define SECOND_SECTION            0x64                               //第二区间size：4
#define THIRD_SECTION             0x68                               //第三区间size：4
#define FOUTH_SECTION             0x6c                               //第四区间size：4
#define SEWAGE_RATE               0x4e                               //污水处理费size：4
#define LACK_MONEY                0x70                               //余额不足限定值
#define WATER_COMPANY_NUMBER      0x71                               //水司编号
#define DOWN_TIME_ADDR            0X74                               // 掉电时间year／month／data　 size :4
#define STATE_FLAG1_ADDR          0x3c
/*  bit: 7        是否开户 1：已开户     0：未开户
**  bit: 4-6     磁干扰次数
**  bit: 0-3     脉冲系数
*/
#define STATE_FLAG2_ADDR     0x3d
/*  bit: 7      干簧管A导通 1    不导通 0
**  bit: 6      干簧管B导通 1    不导通 0
**  bit: 5      阀门异常       0：无   1：有
**  bit: 4      磁干扰关阀标识 0：无   1：有
**  bit: 3      管理员关阀标识 0：无   1：有
**  bit: 2      掉电标识       0：无   1：有
**  bit: 0-1    低电量标识     00：正常
**                             01：一级低压
**                             10：二级低压
**                             11：三级低压
*/
extern char Times_Famen;                                         //阀门打开（关闭）时间
extern int Water_Company_Number;                                     //水司编号
extern char OC_Flag;                                                 //开关阀返回标志位，此标志位置位则水表返回开（关）阀成功
extern char Account_Flag;                                            //是否开户标志
extern u8 Check_Date;
extern char State1_Flag, State2_Flag;
extern int ARRAY_LENGTH;                                             //生成校验码的数据长度  在encrpyt.c文件中定义
extern float First_Stage;                                            //阶梯水价 单价
extern float Second_Stage;
extern float Third_Stage;
extern char Handle_Mode;                                             //管理员 或 用户 标识
extern char Famen_Flag;                                              //阀门状态标志（开/关）
extern char Wait_Oparet;                                             //阀门动作
extern char Insufficient_Balance_Flag;                               //余额不足关阀标志(用来判断余额不足只关一次阀门的)
extern char POWER_LOW_FLAG;                                          //低电量标志
extern uint8_t Check_Month;                                          //判断月份方法
extern uint8_t Check_Date;                                           //判断天数方法
extern char RTC_IT_Flag;                                             //RTC中断标识位
extern char Usr_Password[4];                                         //用户密码
extern char Mgr_Password[4];                                         //管理员密码
extern float Consumption;                                            //总用水量
extern float Consumption_Month;                                      //月用水量
extern char Times_BLE;                                               //蓝牙显示时间
extern u8 reed_switch_A_time1,reed_switch_A_time;                    //干簧管A导通时间
extern u8 reed_switch_B_time1,reed_switch_B_time;                    //干簧管B导通时间
extern u8 reed_switch_A_flag ,reed_switch_B_flag;                    //干簧管A/B导通标志
extern u8 reed_switch_interval_time1 ,reed_switch_interval_time;     //间隔时间
extern float First_Section;                                         //阶梯区间
extern float Second_Section;
extern float Third_Section;
extern u8 Pulse_Factor;                                             //脉冲系数，需要存储
extern char Show_Flag;                                             //LCD显示类型
extern char Show_Flag1;                                             //异常报警显示
extern float Money;                                               //余额
extern float OverDraft_Money;                                     //透支补水金额
extern char Lack_Money;                                          //余额不足限定值
extern char DisCharging_flag;                                     //每天放电标识
extern float    Confirm_Money;                                    //预充值金额
extern int      USART3_RX_STA;                                    //串口接受标志
extern int      USART1_RX_STA;
extern char     halt_flag;                                        //进入halt模式条件
extern char     NoPower_flag;                                     //掉电标志    
extern char Magnetic_Interference_Times;                          //磁干扰次数
extern char Magnetic_Interference_Time;                           //磁干扰时间
extern char Usart3_RecvBuf[USART_RECV_LEN];                       //串口 传输/接受 数组
extern char Usart3_SendBuf[USART_RECV_LEN];
extern char Usart1_RecvBuf[USART1_RECV_LEN];
extern char Usart1_SendBuf[USART1_SEND_LEN];
extern unsigned char return_encrypt_array[16];                    //加密 后的数据将放在这个数组
extern unsigned char return_decrypt_array[16];                    //解密 后的数据将放在这个数组
extern RTC_InitTypeDef   RTC_InitStr; //RTC
extern RTC_TimeTypeDef   RTC_TimeStr;
extern RTC_DateTypeDef   RTC_DateStr;


#endif