/******************************************************************************
*@file  : app.c
*@brief : HRNG Demos 
******************************************************************************/
#include "app.h"


/************************************************************************
 * function   : reverse_DWORD
 * Description: endian transformation for uint32_t  
 * input : 
 *         var : pointer to uint32_t variable 
 * return: none 
 ************************************************************************/
void reverse_DWORD(uint32_t *var)
{
	uint8_t *P = (uint8_t *)var;
	uint8_t tmp;

	tmp = P[0];
	P[0] = P[3];
	P[3] = tmp;
	tmp = P[1];
	P[1] = P[2];
	P[2] = tmp;
}


void APP_SHA1_Test(void)
{
	uint8_t message[3]="abc";
	uint32_t expect_digest[5] = {0xA9993E36,0x4706816A,0xBA3E2571,0x7850C26C,0x9CD0D89D};
	uint32_t digest[5];
    uint8_t  error_code=0;
	int i;

	HAL_SHA1_Hash(message,3,(uint8_t*)digest);
	for(i=0;i<5;i++)
	{
		reverse_DWORD(digest+i);
	}
	for(i=0;i<5;i++)
	{
		if(digest[i]!=expect_digest[i])
		{
			 error_code = 1;
			 break;
		}
	}
	
	if(error_code)
	{
		printf("SHA1 test error.\n");
	}
	else
	{
		printf("SHA1 test success.\n");
	}
	
}

void APP_SHA256_Test(void)
{
	uint8_t message[3]="abc";
	uint32_t expect_digest[8] = {0xBA7816BF,0x8F01CFEA,0x414140DE,0x5DAE2223,0xB00361A3,0x96177A9C,0xB410FF61,0xF20015AD};
	uint32_t digest[8];
    uint8_t  error_code=0;
	int i;

	HAL_SHA256_Hash(message,3,(uint8_t*)digest);
	for(i=0;i<8;i++)
	{
		reverse_DWORD(digest+i);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=expect_digest[i])
		{
			 error_code = 1;
			 break;
		}
	}
	
	if(error_code)
	{
		printf("SHA256 test error.\n");
	}
	else
	{
		printf("SHA256 test success.\n");
	}	
}


