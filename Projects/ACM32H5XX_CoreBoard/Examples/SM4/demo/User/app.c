/******************************************************************************
*@file  : app.c
*@brief : SM4 Demo 
******************************************************************************/

#include "app.h"  

UINT32 g_data_buf[1024];
#define SM4_DATABUF         ((UINT8 *)g_data_buf )

#define SM3_SWAP32(a)       ((a<<24)|((a&0x0000ff00)<<8)|((a&0x00ff0000)>>8)|(a>>24))
SM4_G_STR sm4_glb_str;

void swap(UINT32 *indata,UINT32 len)
{
	UINT32 i;
	for(i = 0; i < len; i ++)
	{
		indata[i] = SM3_SWAP32(indata[i]);	
	}
}

/******************************************************************************
* Function Name  : sm4_lib_variable_initial
* Description    : assign space to the struct point of SM4_G_STR before use sm4 security mode
* Input          : - *p_sm4_str  : the struct point of SM4_G_STR
*				 : - *sm4_data_buf  : the start address of buffer, the size of sm4_data_buf :2080 bytes
* Output         : NONE
* Return         : NONE
******************************************************************************/
void sm4_lib_variable_initial(SM4_G_STR *p_sm4_str, UINT8* sm4_data_buf)
{
	p_sm4_str->sm4_crypt_msbox0    = (sm4_data_buf);
	p_sm4_str->sm4_crypt_msbox1    = (sm4_data_buf + (256));
	p_sm4_str->sm4_crypt_msbox2    = (sm4_data_buf + 2 * (256));
	p_sm4_str->sm4_crypt_msbox3    = (sm4_data_buf + 3 * (256));
	p_sm4_str->sm4_crypt_msbox4    = (sm4_data_buf + 4 * (256));
	p_sm4_str->sm4_crypt_msbox5    = (sm4_data_buf + 5 * (256));
	p_sm4_str->sm4_crypt_msbox6    = (sm4_data_buf + 6 * (256));
	p_sm4_str->sm4_crypt_msbox7    = (sm4_data_buf + 7 * (256));
	p_sm4_str->sm4_mask            = ((unsigned int *)(sm4_data_buf + 8 * (256)));
}

UINT8 sm4_ecb_test(void)
{
    UINT32  mode;
	UINT32  i;
	UINT32	temp_ecb[4];
	UINT32  blk_len;
	
//  key_ecb   = 0x762a64e51cae4995658b0b5347ee7778;
	UINT32  key_ecb[4]    = {0x762a64e5, 0x1cae4995, 0x658b0b53, 0x47ee7778}; 
//  iv_ecb    = 0x0; 	
	UINT32  iv_ecb[4]     = {0, 0, 0, 0 };
//  plain_ecb = 0x2b3b317b40115e7d5a1d024219b940d3;	
	UINT32	plain_ecb[4]  = {0x2b3b317b, 0x40115e7d, 0x5a1d0242, 0x19b940d3}; 
//  crypt_ecb = 0x6414e511c7336970873a45905a4b2112;	
	UINT32	crypt_ecb[4]  = {0x6414e511, 0xc7336970, 0x873a4590, 0x5a4b2112};  
	
	blk_len = sizeof(plain_ecb)/16;
	sm4_lib_variable_initial((SM4_G_STR *)(&sm4_glb_str),SM4_DATABUF);

	//-----------------------    ECB mode (no swap)   ---------------------
    mode = SM4_NORMAL_MODE;
//	mode = SM4_SECURITY_MODE;

	sm4_set_key(key_ecb, SM4_SWAP_DISABLE);

	memcpy((UINT8*)temp_ecb,(UINT8*)plain_ecb,sizeof(plain_ecb));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_ENCRYPTION, SM4_ECB_MODE, iv_ecb, mode);	//ecb encryption

	for(i = 0; i < (blk_len*4); i++)
	{		
		if(temp_ecb[i] != crypt_ecb[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_DECRYPTION, SM4_ECB_MODE, iv_ecb, mode);	//ecb decryption
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != plain_ecb[i])
		{
			return 1;
		}

	}
	
//  mode = SM4_NORMAL_MODE;
	mode = SM4_SECURITY_MODE;
	
	sm4_set_key(key_ecb, SM4_SWAP_DISABLE);
	memcpy((UINT8*)temp_ecb,(UINT8*)plain_ecb,sizeof(plain_ecb));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_ENCRYPTION, SM4_ECB_MODE, iv_ecb, mode);	//ecb encryption

	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != crypt_ecb[i])
		{
			return 1;
		}

	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_DECRYPTION, SM4_ECB_MODE, iv_ecb, mode);	//ecb decryption
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != plain_ecb[i])
		{
			return 1;
		}


	}
	
		
	//-----------------------   ECB mode (swap)   ---------------------
  //mode = SM4_NORMAL_MODE;
	mode = SM4_SECURITY_MODE;

	swap(key_ecb,4);            //key_ecb[0] = 0xe5642a76,...,key_ecb[3] = 0x7877ee47
	swap(iv_ecb,4);
	swap(plain_ecb,blk_len*4);
	swap(crypt_ecb,blk_len*4);
	
	sm4_set_key(key_ecb, SM4_SWAP_ENABLE);
	memcpy((UINT8*)temp_ecb,(UINT8*)plain_ecb,sizeof(plain_ecb));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_ENCRYPTION, SM4_ECB_MODE, iv_ecb, mode);	 //cbc encryption
	
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != crypt_ecb[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_DECRYPTION, SM4_ECB_MODE, iv_ecb, mode);	 //cbc decryption
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != plain_ecb[i])
		{
			return 1;
		}
	}

//  mode = SM4_NORMAL_MODE;
	mode = SM4_SECURITY_MODE;

//	swap(key_ecb,4);            //key_ecb[0] = 0xe5642a76,...,key_ecb[3] = 0x7877ee47
//	swap(iv_ecb,4);
//	swap(plain_ecb,blk_len*4);
//	swap(crypt_ecb,blk_len*4);
	sm4_set_key(key_ecb, SM4_SWAP_ENABLE);
	memcpy((UINT8*)temp_ecb,(UINT8*)plain_ecb,sizeof(plain_ecb));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_ENCRYPTION, SM4_ECB_MODE, iv_ecb, mode);	 //cbc encryption

	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != crypt_ecb[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_ecb, temp_ecb, blk_len, SM4_DECRYPTION, SM4_ECB_MODE, iv_ecb, mode);	 //cbc decryption
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_ecb[i] != plain_ecb[i])
		{
			return 1;
		}
	}

	//-----------------------   End of  ECB mode   ---------------------
	return 0;
}
UINT8 sm4_cbc_test(void)
{
    UINT32 mode;
	UINT32 i;
	UINT32 temp_cbc[4];
	UINT32 blk_len;
	
//  key_cbc   = 0x09184810210c2d9b75d87616426b2e6c;	
	UINT32  key_cbc[4]    = {0x09184810, 0x210c2d9b, 0x75d87616, 0x426b2e6c}; 
//  iv_cbc    = 0x02af0fb437ed7e94251758dd438f3d98;	
	UINT32  iv_cbc[4]     = {0x02af0fb4, 0x37ed7e94, 0x251758dd, 0x438f3d98};
//  plain_cbc = 0x0edb79e666ac5dd32a682f3e1eea5b91;	  	
	UINT32	plain_cbc[4]  = {0x0edb79e6, 0x66ac5dd3, 0x2a682f3e, 0x1eea5b91};
//  crypt_cbc = 0xbd1a471138ce9712b5bcb148b2d291e5;	
	UINT32	crypt_cbc[4]  = {0xbd1a4711, 0x38ce9712, 0xb5bcb148, 0xb2d291e5};  

	blk_len = sizeof(plain_cbc)/16;
	sm4_lib_variable_initial((SM4_G_STR *)(&sm4_glb_str),SM4_DATABUF);

	//-----------------------    CBC mode (no swap)   ---------------------
	mode = SM4_NORMAL_MODE;
//	mode = SM4_SECURITY_MODE;

	sm4_set_key(key_cbc, SM4_SWAP_DISABLE);
	memcpy((UINT8*)temp_cbc,(UINT8*)plain_cbc,sizeof(plain_cbc));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_ENCRYPTION, SM4_CBC_MODE, iv_cbc, mode);

	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != crypt_cbc[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_DECRYPTION, SM4_CBC_MODE, iv_cbc, mode);
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != plain_cbc[i])
		{
			return 1;
		}
	}
//	mode = SM4_NORMAL_MODE;
	mode = SM4_SECURITY_MODE;

	sm4_set_key(key_cbc, SM4_SWAP_DISABLE);
	memcpy((UINT8*)temp_cbc,(UINT8*)plain_cbc,sizeof(plain_cbc));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_ENCRYPTION, SM4_CBC_MODE, iv_cbc, mode);

	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != crypt_cbc[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_DECRYPTION, SM4_CBC_MODE, iv_cbc, mode);
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != plain_cbc[i])
		{
			return 1;
		}
	}
	
	//-----------------------    CBC mode (swap)   ---------------------
	mode = SM4_NORMAL_MODE;
//	mode = SM4_SECURITY_MODE;
	
	swap(key_cbc,4);            //key_ecb[0] = 0xe5642a76,...,key_ecb[3] = 0x7877ee47
	swap(iv_cbc,4);
	swap(plain_cbc,blk_len*4);
	swap(crypt_cbc,blk_len*4);

	sm4_set_key(key_cbc, SM4_SWAP_ENABLE);
	memcpy((UINT8*)temp_cbc,(UINT8*)plain_cbc,sizeof(plain_cbc));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_ENCRYPTION, SM4_CBC_MODE, iv_cbc, mode);

	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != crypt_cbc[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_DECRYPTION, SM4_CBC_MODE, iv_cbc, mode);
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != plain_cbc[i])
		{
			return 1;
		}
	}
	
//	mode = SM4_NORMAL_MODE;
	mode = SM4_SECURITY_MODE;
	
//	swap(key_cbc,4);            //key_ecb[0] = 0xe5642a76,...,key_ecb[3] = 0x7877ee47
//	swap(iv_cbc,4);
//	swap(plain_cbc,blk_len*4);
//	swap(crypt_cbc,blk_len*4);

	sm4_set_key(key_cbc, SM4_SWAP_ENABLE);
	memcpy((UINT8*)temp_cbc,(UINT8*)plain_cbc,sizeof(plain_cbc));
	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_ENCRYPTION, SM4_CBC_MODE, iv_cbc, mode);

	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != crypt_cbc[i])
		{
			return 1;
		}
	}

	sm4_crypt((SM4_G_STR *)(&sm4_glb_str), temp_cbc, temp_cbc, blk_len, SM4_DECRYPTION, SM4_CBC_MODE, iv_cbc, mode);
	for(i = 0; i < (blk_len*4); i++)
	{
		if(temp_cbc[i] != plain_cbc[i])
		{
			return 1;
		}
	}
	//-----------------------   End of CBC mode   ---------------------
	return 0;
}





void APP_SM4_Test(void)
{
    HAL_CDE_EnableCPx(0);
    //HAL_CDE_DisableCPx(0);
 
	printfS("SM4 test start\n");

	if(sm4_ecb_test())
		printfS(" SM4 ECB test U32 fail\n");
	else
		printfS(" SM4 ECB test U32 pass\n");

	if(sm4_cbc_test())
		printfS(" SM4 CBC test U32 fail\n");
	else
		printfS(" SM4 CBC test U32 pass\n\n");	

	
	printfS("SM4 test over.\n");

}

