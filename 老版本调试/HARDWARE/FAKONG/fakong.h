#ifndef __KEY_H_
#define __KEY_H_

#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

//马达“M_OPPEN”端口位置定义
#define M_OPPEN_GPIO_TypeDef GPIOF
#define M_OPPEN_GPIO_PIN GPIO_Pin_5
//马达“M_CLOSE”端口位置定义
#define M_CLOSE_GPIO_TypeDef GPIOF
#define M_CLOSE_GPIO_PIN GPIO_Pin_4
//马达“MOTOR_CHECK”端口位置定义
#define MOTOR_CHECK_GPIO_TypeDef GPIOD
#define MOTOR_CHECK_GPIO_PIN GPIO_Pin_4
//马达“RE+”端口位置定义
#define RE1_GPIO_TypeDef GPIOB
#define RE1_GPIO_PIN GPIO_Pin_7
//马达“RE-”端口位置定义
#define RE0_GPIO_TypeDef GPIOB
#define RE0_GPIO_PIN GPIO_Pin_6
//报警“LED”端口位置定义
#define LED_GPIO_TypeDef GPIOA
#define LED_GPIO_PIN GPIO_Pin_3



//功能函数声明
void ValveInit(void);//阀门初始化
void ValveControl_Oppen_User();//阀门开控制函数（用户模式）
void ValveControl_Oppen_Administrator();//阀门控制开函数（管理员模式）
void ValveControl_Close_User();//阀门关控制函数（用户模式）
void ValveControl_Close_Administrator();//阀门控制关函数（管理员模式）
#endif