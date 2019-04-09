#include "encrpyt.h"

int ARRAY_LENGTH;

unsigned char crc_generate_checkout_code(unsigned char *verify)
{
    //生成校验码
    unsigned short crc;
    crc = verify[0]*256+verify[1];
    int verify_length = ARRAY_LENGTH;   
    int i ,j; 
    int count = 0;
    for(i=0;i<verify_length;i++)
    {
      count++;
      if(count<=verify_length-2)
      {
    	for(j =0 ; j<8 ; j++)
        {
	  if (crc & 0x8000)  /* 判断最高位是否为1 */
	  {
            /* 最高位为1，不需要异或，往左移一位，然后与0x31异或 */
            /* 0x131(多项式：x8+x5+x4+1，100110001)，最高位不需要异或，直接去掉 */
            crc = (crc << 1) ;
            unsigned short ss = 0x31*256;
            crc = crc^ss;
	  }
          else
          {
              /* 最高位为0时，不需要异或，整体数据往左移一位 */
              crc = (crc << 1);
          }          
        }
	crc =crc+verify[i+2];
      } 
      else
      {
        for(j =0 ; j<8; j++)
        {
            if (crc & 0x8000)  /* 判断最高位是否为1 */
            {
                /* 最高位为1，不需要异或，往左移一位，然后与0x31异或 */
                /* 0x131(多项式：x8+x5+x4+1，100110001)，最高位不需要异或，直接去掉 */
                crc = (crc << 1) ;
                unsigned short ss = 0x31*256;
                crc = crc^ss;
            }
            else
            {
                /* 最高位为0时，不需要异或，整体数据往左移一位 */
                crc = (crc << 1);
            }
         }
      }

    } 
    crc = crc/256;
    return crc;
}

unsigned short crc_virification(unsigned char verify[])
{
    //验证校验码
    int i,j;
    int count = 0;
    unsigned short crc;
    crc = verify[0]*256+verify[1];
    int verify_length = ARRAY_LENGTH;
    /* 数据往左移了8位，需要计算8次 */
     for(i=0;i<verify_length;i++)
     {
    	count++;
    	if(count<=verify_length-1)
        {       
          for(j =0 ; j<8 ; j++)
          {
            if (crc & 0x8000)  /* 判断最高位是否为1 */
            {
              /* 最高位为1，不需要异或，往左移一位，然后与0x31异或 */
              /* 0x131(多项式：x8+x5+x4+1，100110001)，最高位不需要异或，直接去掉 */
              crc = (crc << 1) ;
              unsigned short ss = 0x31*256;
              crc = crc^ss;
            }
            else
            {
                /* 最高位为0时，不需要异或，整体数据往左移一位 */
                crc = (crc << 1);
            }      
          }
	   crc =crc+verify[i+2];
        } 
        else
        {
          for(j =0 ; j<8; j++)
          {
              if (crc & 0x8000)  /* 判断最高位是否为1 */
              {
                  /* 最高位为1，不需要异或，往左移一位，然后与0x31异或 */
                  /* 0x131(多项式：x8+x5+x4+1，100110001)，最高位不需要异或，直接去掉 */
                  crc = (crc << 1) ;
                  unsigned short ss = 0x31*256;
                  crc = crc^ss;
              }
              else
              {
                  /* 最高位为0时，不需要异或，整体数据往左移一位 */
                  crc = (crc << 1);
              }
           }
          }
        } 		
    return crc;
}


int TIMES_MARKS=0;	//全局密钥扩展轮数标志变量 *- 
unsigned char organize_key[4][4] = {{"cdbl"},{"uewa"},{"term"},{"eter"}};//密钥数组
unsigned char key[4][4];
unsigned char return_encrypt_array[16];//加密后数组
unsigned char return_decrypt_array[16];//还原数组
unsigned char bit[8];
unsigned char W[44][4]; 		// 密钥扩展数组

unsigned char S[16][16] = { 
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

/**
 * 逆S盒
 */
unsigned char NI_S[16][16] = { 
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
    
/*2、字节转换为二进制位存放在数组中*/
void ByteToBits(unsigned char ch, unsigned char bit[])
{
	int i = 0;
	for(i =0; i < 8; i++){
		bit[i] = ( ch >> i) & 1;
	} 

}    
     
void S_change(unsigned char status[][4], unsigned char bit[])
{
	int i=0;
	for(i=0; i < 4; i++){
		int j=0;
	    for(j=0; j < 4; j++){
			ByteToBits(status[i][j], bit);		//一个字节的高4为作为s盒的行索引，低四位作为列索引 
			status[i][j] = S[( bit[7] * 8 + bit[6] * 4 + bit[5] * 2 + bit[4] )][( bit[3] * 8 + bit[2] * 4 + bit[1] * 2 + bit[0] )];

    	}	
	}
} 

void NI_S_change(unsigned char status[][4], unsigned char bit[]) 
{
	int i=0;
	for(i=0; i < 4; i++){
		int j=0;
	    for(j=0; j < 4; j++){
			ByteToBits(status[i][j], bit);		//一个字节的高4为作为s盒的行索引，低四位作为列索引 
			status[i][j] = NI_S[( bit[7] * 8 + bit[6] * 4 + bit[5] * 2 + bit[4] )][( bit[3] * 8 + bit[2] * 4 + bit[1] * 2 + bit[0] )];
    	}	
	}
}

//行移位 
void ShiftRows(unsigned char status[][4])
{
	unsigned char temp1 = status[1][0];
	unsigned char temp2 = status[2][0];
	unsigned char temp3 = status[2][1];
	unsigned char temp4 = status[3][0];
	unsigned char temp5 = status[3][1];
	unsigned char temp6 = status[3][2];
	int i=0;
	for( i= 0; i < 3; i++)
	    status[1][i] = status[1][( i + 1 )];
    status[1][3] = temp1;
    i=0;
    for( i=0; i < 2; i++)
        status[2][i] = status[2][( i + 2 )];
    status[2][2] = temp2;
	status[2][3] = temp3; 
    status[3][0] = status[3][3];
    status[3][1] = temp4;
    status[3][2] = temp5;
    status[3][3] = temp6;
} 

/*  逆行移位 */
void deShiftRows(unsigned char status[][4]) 
{
    unsigned char temp1 = status[1][3];
	unsigned char temp2 = status[2][0];
	unsigned char temp3 = status[2][1];
	unsigned char temp4 = status[3][0];
	int i;
	for(i=0;i<3;i++){
		status[1][3-i] = status[1][(2-i)];
	} 
	status[1][0] = temp1;
	
    for( i=0; i < 2; i++)
    status[2][i] = status[2][( i + 2 )];
    status[2][2] = temp2;
    status[2][3] = temp3;
    
    for(i=0;i<3;i++){
	status[3][i] = status[3][(i+1)];
	} status[3][3] = temp4;
}

/*7、列变换被调用函数x乘法(02乘法)*/
unsigned char ByteMultiply2(unsigned char c)
{
	unsigned char temp;
	temp = c << 1;
	unsigned char a2 = c & 0x80;
	if(a2 != 0 ){ // c 的首位为1 
		temp ^= 0x1b;
	}
	return temp;
} 

unsigned char ByteMultiply3(unsigned char c)
{
	unsigned char temp = ByteMultiply2(c);
	return temp^c;
} 

unsigned char ByteMultiply4(unsigned char c)
{
	return ByteMultiply2(ByteMultiply2(c));
} 

unsigned char ByteMultiply8(unsigned char c)
{
	return ByteMultiply2(ByteMultiply4(c));
} 

unsigned char ByteMultiply9(unsigned char c)
{
	return ByteMultiply8(c)^c;
} 

unsigned char ByteMultiply11(unsigned char c)
{
	return ByteMultiply9(c)^ByteMultiply2(c);
} 

unsigned char ByteMultiply12(unsigned char c){
	return ByteMultiply8(c)^ByteMultiply4(c);
} 

unsigned char ByteMultiply13(unsigned char c)
{
	return ByteMultiply12(c)^c;
} 

unsigned char ByteMultiply14(unsigned char c)
{
	return ByteMultiply12(c)^ByteMultiply2(c);
} 


unsigned char GFMul(unsigned char n, unsigned char s)
{
	unsigned char result ;
	if( n == 1)
		result = s;
	else if ( n == 2)
		result = ByteMultiply2(s);
	else if ( n == 3 )
		result = ByteMultiply3(s);
	else if(n == 0x9)
        result = ByteMultiply9(s);
    else if(n == 0xb)//11
        result =ByteMultiply11(s);
    else if(n == 0xd)//13
        result = ByteMultiply13(s);
    else if(n == 0xe)//14
        result = ByteMultiply14(s);
        
    return result;
}

 
/*8、列变换*/
void MixColumns(unsigned char status[][4])
{
	unsigned char colM[4][4] = {
	2,3,1,1,
	1,2,3,1,
	1,1,2,3,
	3,1,1,2
	} ;
	
	unsigned char temp[4][4];
	int i,j; 
	  for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            temp[i][j] = status[i][j];
	
	
	 
    for(i=0; i < 4; i++)
        for(j=0; j< 4; j++){
        	  status[i][j] = GFMul(colM[i][0],temp[0][j]) ^ GFMul(colM[i][1],temp[1][j]) 
                ^ GFMul(colM[i][2],temp[2][j]) ^ GFMul(colM[i][3], temp[3][j]);	
		}
} 

/*  逆列变换  */
void deMixColumns(unsigned char status[][4])
{
	unsigned char colM[4][4] = {
	0xe, 0xb, 0xd, 0x9,
    0x9, 0xe, 0xb, 0xd,
    0xd, 0x9, 0xe, 0xb,
    0xb, 0xd, 0x9, 0xe
	};
	
	unsigned char temp[4][4];
	int i,j; 
	  for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            temp[i][j] = status[i][j];
	
	
	 
    for(i=0; i < 4; i++)
        for(j=0; j< 4; j++)
        {
          status[i][j] = GFMul(colM[i][0],temp[0][j]) ^ GFMul(colM[i][1],temp[1][j]) 
        ^ GFMul(colM[i][2],temp[2][j]) ^ GFMul(colM[i][3], temp[3][j]);	

        }
} 

void KeyWriteInW(unsigned char organize_key[4][4],unsigned char W[44][4])
{
	int i,j;
	for(i= 0 ; i<4 ;i++)
	for(j=0; j<4 ;j++)
        {
          W[i+TIMES_MARKS][j] = organize_key[i][j];
	}
}

void BitXOR(unsigned char status[][4],unsigned char key[4][4])
{		//轮密钥加,按位异或 
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			status[i][j] = status[i][j]^key[i][j];
		}
	}
}



void T(unsigned char W[44][4],unsigned char bit[]) 
{
	
	unsigned char Rcon[10][4] = {		//轮常量 
		{0x01,0x00,0x00,0x00}, 
		{0x02,0x00,0x00,0x00}, 
		{0x04,0x00,0x00,0x00}, 
		{0x08,0x00,0x00,0x00}, 
		{0x10,0x00,0x00,0x00}, 
		{0x20,0x00,0x00,0x00}, 
		{0x40,0x00,0x00,0x00}, 
		{0x80,0x00,0x00,0x00}, 
		{0x1B,0x00,0x00,0x00}, 
		{0x36,0x00,0x00,0x00}, 		
		};

		
    unsigned char W_array[4];		//获取W[4*i+3] 
    int i,j;
    for(i=0;i<4;i++){
    	W_array[i] = W[TIMES_MARKS+3][i];	
	}

	//移位 
    unsigned char temp = W_array[0];
   	W_array[0] = W_array[1];
   	W_array[1] = W_array[2];
   	W_array[2] = W_array[3];
   	W_array[3] = temp;
	
	
	for(i=0;i<4;i++){
			ByteToBits(W_array[i],bit);		//一个字节的高4为作为s盒的行索引，低四位作为列索引 
			W_array[i] = S[( bit[7] * 8 + bit[6] * 4 + bit[5] * 2 + bit[4] )][( bit[3] * 8 + bit[2] * 4 + bit[1] * 2 + bit[0] )];
	}		//S盒替换 
	

	for(i=0;i<4;i++){
		W_array[i] = W_array[i] ^ Rcon[TIMES_MARKS/4][i];		//与轮常量进行异或运算 
	}
	
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){ 
			W_array[j] = W_array[j] ^ W[TIMES_MARKS+i][j];
			W[TIMES_MARKS+4+i][j] = W_array[j];
		}
	} 
	
}

void GetKey_from_W(unsigned char key[][4],unsigned char W[][4]) 
{
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			key[i][j] = W[TIMES_MARKS+4+i][j]; 
		}
	}
}
    
void NI_GetKey_from_W(unsigned char key[][4],unsigned char W[][4])
{
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			key[i][j] = W[TIMES_MARKS+i][j]; 
		}
	}
}

//将密钥进行十轮加密变成一个44的数组 
void init()
{
	TIMES_MARKS = 0;
	KeyWriteInW(organize_key,W);
	for(TIMES_MARKS=0;TIMES_MARKS<40;TIMES_MARKS+=4){
		if(TIMES_MARKS<36){
			T(W,bit);
		}else if(TIMES_MARKS==36){
			T(W,bit);
			TIMES_MARKS = 0;
			break;	
		}
	}
	
}

void encrypt_methon(unsigned char Encrypt_Clear_Array[])
{
	init();
	int i;
	int j;
	int count=0;
	unsigned char encrypt_status[4][4] ;
	for(i=0; i<4; i++){
		for(j=0; j <4; j++){
				encrypt_status[i][j] = Encrypt_Clear_Array[count];
				count++;
		}
	}
	BitXOR(encrypt_status,organize_key);	//organize_miyao是全局初始密钥，不能能改，只能使用 
	for(TIMES_MARKS=0;TIMES_MARKS<40;TIMES_MARKS+=4){
	//10轮加密 
		if(TIMES_MARKS<36){
			S_change(encrypt_status,bit);
			ShiftRows(encrypt_status);
			MixColumns(encrypt_status);
			GetKey_from_W(key,W);
			BitXOR(encrypt_status,key);	

		}else if(TIMES_MARKS==36){	//最后一轮不需要列变换 
			S_change(encrypt_status,bit);
			ShiftRows(encrypt_status);
			GetKey_from_W(key,W);
			BitXOR(encrypt_status,key);
		}
	}
	count=0;
	for( i = 0;i<4;i++){
		for( j=0;j<4;j++){
			return_encrypt_array[count] = encrypt_status[i][j];
			count++;
		}
	}
}

void decrypt_methon(unsigned char Decrypt_Secret_Array[])
{
	init();
	unsigned char decrypt_status[4][4] ;
	int i,j;
	int count =0;
	for(i=0; i <4 ; i++){
		for(j=0;j<4  ;j++){
				decrypt_status[i][j] = Decrypt_Secret_Array[count];
				count ++;				
		}
	}
	TIMES_MARKS = 40;
	NI_GetKey_from_W(key,W);
	BitXOR(decrypt_status,key);	
	for(TIMES_MARKS=36;TIMES_MARKS>=0;TIMES_MARKS-=4){
		if(TIMES_MARKS>0){	
			deShiftRows(decrypt_status);//逆行变换
			NI_S_change(decrypt_status,bit);//逆s盒字节替换
			NI_GetKey_from_W(key,W);
			BitXOR(decrypt_status,key);//轮密钥加 
			deMixColumns(decrypt_status);//逆列变换 
		}else if(TIMES_MARKS==0){
			deShiftRows(decrypt_status);//逆行变换
			NI_S_change(decrypt_status,bit);//逆s盒字节替换
			NI_GetKey_from_W(key,W);
			BitXOR(decrypt_status,key);//轮密钥加 
		} 
	}
	count=0;
	for( i = 0;i<4;i++){
		for( j=0;j<4;j++){
			return_decrypt_array[count] = decrypt_status[i][j];
			count++;
		}
	}
	
}


        
        
        