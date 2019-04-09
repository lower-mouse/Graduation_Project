
#ifndef STM8L_TIME_H__
#define STM8L_TIME_H__




void Timer3Init(void);
void Timer5_Init();
void LedInit(void);
void u3_printf(char* fmt,...);
void USART3_Config(void);
void read_state();
void BLE_Init();
void BLE_Work();

void SendData16(unsigned char *data);
void USART1_Config(void);
void u1_printf(char* fmt,...);
#endif