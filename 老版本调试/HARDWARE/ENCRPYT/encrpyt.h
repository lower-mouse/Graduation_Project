#ifndef _NECRPYT_H_
#define _NECRPYT_H_



unsigned short crc_virification(unsigned char verify[]);//У�麯��
unsigned char crc_generate_checkout_code(unsigned char *verify);//����У���뺯��

void decrypt_methon(unsigned char Decrypt_Secret_Array[]);//����
void encrypt_methon(unsigned char Encrypt_Clear_Array[]);//����


#endif