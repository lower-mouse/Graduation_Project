#ifndef __KEY_H_
#define __KEY_H_

#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

//��M_OPPEN���˿�λ�ö���
#define M_OPPEN_GPIO_TypeDef GPIOF
#define M_OPPEN_GPIO_PIN GPIO_Pin_5
//��M_CLOSE���˿�λ�ö���
#define M_CLOSE_GPIO_TypeDef GPIOF
#define M_CLOSE_GPIO_PIN GPIO_Pin_4
//��MOTOR_CHECK���˿�λ�ö���
#define MOTOR_CHECK_GPIO_TypeDef GPIOD
#define MOTOR_CHECK_GPIO_PIN GPIO_Pin_4
//��RE+���˿�λ�ö���
#define RE1_GPIO_TypeDef GPIOB
#define RE1_GPIO_PIN GPIO_Pin_7
//��RE-���˿�λ�ö���
#define RE0_GPIO_TypeDef GPIOB
#define RE0_GPIO_PIN GPIO_Pin_6
//������LED���˿�λ�ö���
#define LED_GPIO_TypeDef GPIOA
#define LED_GPIO_PIN GPIO_Pin_3



//���ܺ�������
void ValveInit(void);//���ų�ʼ��
void ValveControl_Oppen_User();//���ſ����ƺ������û�ģʽ��
void ValveControl_Oppen_Administrator();//���ſ��ƿ�����������Աģʽ��
void ValveControl_Close_User();//���Źؿ��ƺ������û�ģʽ��
void ValveControl_Close_Administrator();//���ſ��ƹغ���������Աģʽ��
#endif