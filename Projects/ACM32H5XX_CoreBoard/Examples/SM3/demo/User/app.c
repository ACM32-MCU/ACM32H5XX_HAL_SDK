/******************************************************************************
*@file  : app.c
*@brief : SM3 Demo 
******************************************************************************/

#include "app.h"  

UINT32 g_data_buf[1024];
#define DATABUF             ((UINT8 *)g_data_buf)

void APP_SM3_Test(void)
{
	UINT32 digest[8];
	//digest1 = 0x66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	UINT32 digest1[8]={0x66c7f0f4,0x62eeedd9,0xd1f2d46b,0xdc10e4e2,0x4167c487,0x5cf2f7a2,0x297da02b,0x8f4ba8e0};
	//digest2 = 0x7ae067b1 99020410 8c6783e0 7e67baee 9f825811 9c208198 49127103 0805fe65
	UINT32 digest2[8]={0x7ae067b1,0x99020410,0x8c6783e0,0x7e67baee,0x9f825811,0x9c208198,0x49127103,0x0805fe65};
	//digest3 = 0xdebe9ff9 2275b8a1 38604889 c18e5a4d 6fdb70e5 387e5765 293dcba3 9c0c5732
	UINT32 digest3[8]={0xdebe9ff9,0x2275b8a1,0x38604889,0xc18e5a4d,0x6fdb70e5,0x387e5765,0x293dcba3,0x9c0c5732};
	//digest4 = 0x9a032f0c f27e4b40 8f252452 d451cac5 1a422d43 ae73ab6c d7ec2483 241358e9
	UINT32 digest4[8]={0x9a032f0c,0xf27e4b40,0x8f252452,0xd451cac5,0x1a422d43,0xae73ab6c,0xd7ec2483,0x241358e9};
	int i;

	UINT8 error_code = 0;
	
	HAL_CDE_EnableCPx(0);
	
	SM3_CTX context;
	
	printfS("---------sm3 test-------\n");
	error_code = 0;
	
	DATABUF[0] = 0x61;	
	DATABUF[1] = 0x62;
	DATABUF[2] = 0x63;
	sm3_hash(DATABUF,3,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest1[i])
		{
			error_code = 1;
			printfS("\nsm3 error1\n");	
			return;		
		}
	}
	printfS("sm3 success1\n");
	//message is 0x11223344616263(big number format),digest is digest2
	DATABUF[0] = 0x11;	
	DATABUF[1] = 0x22;
	DATABUF[2] = 0x33;
	DATABUF[3] = 0x44;	
	DATABUF[4] = 0x61;
	DATABUF[5] = 0x62;
	DATABUF[6] = 0x63;	

	sm3_hash(DATABUF,7,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest2[i])
		{
			error_code = 1;
			printfS("\nsm3 error2\n");
			return;			
		}
	}
	printfS("sm3 success2\n");	

	//message is 0x61626364616263646162636461626364616263646162636461626364616263646162636461626364616263646162636461626364616263646162636461626364(big number format),digest is digest3
	for(i=0;i<64;i+=4)
	{
		DATABUF[i] = 0x61;
		DATABUF[i+1] = 0x62;
		DATABUF[i+2] = 0x63;
		DATABUF[i+3] = 0x64;
	}
	SM3_initial(&context);					

	SM3_update(&context,DATABUF,32);
	SM3_update(&context,DATABUF+32,32);	 
	SM3_final((UINT8*)digest,&context);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest3[i])
		{
			error_code = 1;
			printfS("\nsm3 error3\n");
			return;			
		}
	}
	printfS("sm3 success3\n");


	for(i=0;i<56;i+=4)
	{
		DATABUF[i] = 0x61;
		DATABUF[i+1] = 0x62;
		DATABUF[i+2] = 0x63;
		DATABUF[i+3] = 0x64;
	}
	sm3_hash(DATABUF,56,(UINT8*)digest);
	for(i=0;i<8;i++)
	{
		digest[i] = SM3_SWAP32(digest[i]);
	}
	for(i=0;i<8;i++)
	{
		if(digest[i]!=digest4[i])
		{
			error_code = 1;
			printfS("\nsm3 error4\n");
			return;			
		}
	}
	printfS("sm3 success4\n\n");
	printfS("sm3 test over\n");	
}


