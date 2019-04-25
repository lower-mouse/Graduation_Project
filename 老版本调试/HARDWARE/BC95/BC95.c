#include "bc95.h"
#include "variable.h"   


#define SEND_MAX 200



//发送命令
int BcSendCmd(char *cmd)
{
    u1_printf("%s\n\r", cmd);
}


//发送数据
int BcSendData(int len, char *data)//AT+NMGS=<length>,<data>
{
    int i;
    char str[SEND_MAX];
    int index = 0;

    sprintf(str, "AT+NMGS=%d,", len);
    index = strlen(str);
    
    for(i = 0; i < len; i++)
    {
        sprintf(&str[index + i*2], "%2x", data[i]);
    }
    sprintf(&str[index + len*2], "\n\r");
    index += len*2 + 2;
    //index = (index > SEND_MAX? SEND_MAX:index);
    if(index > SEND_MAX)
    {
        index = SEND_MAX;
    }
    for(i = 0 ;i < index ;i++)							
    {
      USART_GetFlagStatus(USART1,USART_FLAG_TC);
      USART_SendData8(USART1,str[i]);
      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    }
}













