#ifndef __BLE_H
#define __BLE_H

void Message_Serverce_BLE();//Э���������������
void Message_Serverce_Usart();//Э������������ڣ�
void u3_pritnf_data(unsigned char *data);
void pritnf_data(unsigned char *data);
void UART_PutChar(USART_TypeDef* USARTx, char Data);
void UART_PutStr (USART_TypeDef* USARTx, char *str);
void Change_BLE_Parameter(char *data);//�޸���������
void u32tostr(unsigned long dat,char *str);
void BLE_Rest();//������λ����
void Uart_GPIO_Switch();
#endif