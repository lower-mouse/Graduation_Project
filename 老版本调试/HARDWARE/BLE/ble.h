#ifndef __BLE_H
#define __BLE_H

void Message_Serverce_BLE();//协议服务函数（蓝牙）
void Message_Serverce_Usart();//协议服务函数（串口）
void u3_pritnf_data(unsigned char *data);
void pritnf_data(unsigned char *data);
void UART_PutChar(USART_TypeDef* USARTx, char Data);
void UART_PutStr (USART_TypeDef* USARTx, char *str);
void Change_BLE_Parameter(char *data);//修改蓝牙参数
void u32tostr(unsigned long dat,char *str);
void BLE_Rest();//蓝牙复位函数
void Uart_GPIO_Switch();
#endif