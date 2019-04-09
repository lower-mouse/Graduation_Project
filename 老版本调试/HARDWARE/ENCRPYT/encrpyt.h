#ifndef _NECRPYT_H_
#define _NECRPYT_H_



unsigned short crc_virification(unsigned char verify[]);//校验函数
unsigned char crc_generate_checkout_code(unsigned char *verify);//生成校验码函数

void decrypt_methon(unsigned char Decrypt_Secret_Array[]);//解密
void encrypt_methon(unsigned char Encrypt_Clear_Array[]);//加密


#endif