/******************************************************************************
*@file  : app.c
*@brief : AES Demos 
******************************************************************************/
#include "app.h"


#define SWAP32(a)       ((a<<24)|((a&0x0000ff00)<<8)|((a&0x00ff0000)>>8)|(a>>24))

void swap(uint32_t *indata,uint32_t len)
{
	uint32_t i;
	for(i = 0; i < len; i ++)
	{
		indata[i] = SWAP32(indata[i]);	
	}
}

uint32_t aes_ecb_test(void)  //ECB Test
{    
	uint32_t mode;
	uint32_t i;
	uint32_t result[8];
	uint32_t blk_len;
	
	uint32_t key_ecb_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	uint32_t iv_ecb_128[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
	uint32_t plain_ecb_128[4]   = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // plain_ecb  = 0xc42821e1 3ac9b622 72304ad5 e5f5937d
	uint32_t cipher_ecb_128[4]  = {0x1f4f49e4, 0xfb96cca5, 0xa309032f, 0xb1a46dde};  // cipher_ecb = 0x1f4f49e4 fb96cca5 a309032f b1a46dde

	uint32_t key_ecb_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	uint32_t iv_ecb_192[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
	uint32_t plain_ecb_192[4]   = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  // plain_ecb  = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	uint32_t cipher_ecb_192[4]  = {0xc8f26de1, 0x68a21c9d, 0x21323441, 0x9e11db4e};  // cipher_ecb = 0xc8f26de1 68a21c9d 21323441 9e11db4e

	uint32_t key_ecb_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	uint32_t iv_ecb_256[4]      = {0x00000000, 0x00000000, 0x00000000, 0x00000000};  // iv_ecb     = 0x0;
	uint32_t plain_ecb_256[4]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // plain_ecb  = 0xf38d8adf 367b0c53 1d273eb7 60632039
	uint32_t cipher_ecb_256[4]  = {0xaddf70c6, 0x3034834d, 0x5c3ad484, 0xebbf7ed0};  // cipher_ecb = 0xaddf70c6 3034834d 5c3ad484 ebbf7ed0


    //-----------------------    ECB mode (no swap)   ---------------------
//  128
	mode = AES_NORMAL_MODE;
//    mode = AES_SECURITY_MODE;
	blk_len = sizeof(plain_ecb_128) / 16;
	HAL_AES_SetKey(key_ecb_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
	HAL_AES_Crypt(plain_ecb_128, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_128, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_128[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_128, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_128, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_128[i])
		{
			return 1;
		}
	}

//  192
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_192) / 16;

	HAL_AES_SetKey(key_ecb_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
	HAL_AES_Crypt(plain_ecb_192, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_192, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_192[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_192, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_192, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_192[i])
		{
			return 1;
		}
	}

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_ecb_256) / 16;

	HAL_AES_SetKey(key_ecb_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
	HAL_AES_Crypt(plain_ecb_256, result, blk_len, AES_ENCRYPTION, AES_ECB_MODE, iv_ecb_256, mode);

	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != cipher_ecb_256[i])
		{
			return 1;
		}
	}    

	HAL_AES_Crypt(cipher_ecb_256, result, blk_len, AES_DECRYPTION,AES_ECB_MODE, iv_ecb_256, mode);
	for( i = 0; i < (blk_len*4); i ++)
	{
		if(result[i] != plain_ecb_256[i])
		{
			return 1;
		}
	}

	return 0;
}

uint32_t aes_cbc_test(void)   //CBC Test
{
	uint32_t mode;
	uint32_t i;
	uint32_t result[20];
	uint32_t blk_len;

//128	
	uint32_t key_cbc_128[4]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};  //key_cbc    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a
	uint32_t iv_cbc_128[4]      = {0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  //iv_cbc     = 0xc42821e1 3ac9b622 72304ad5 e5f5937d  
//  plain_cbc  = 0xf38d8adf 367b0c53 1d273eb7 60632039...
	uint32_t plain_cbc_128[20]   = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039, 
							 0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de, 
							 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
							 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000, 
							 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281};  
//  cipher_cbc = 0x099ed143 4af12194 c2cb0351 075ea970...
	uint32_t cipher_cbc_128[20]  = {0x099ed143, 0x4af12194, 0xc2cb0351, 0x075ea970, 
							  0x59c1e350, 0x9c5bcc75, 0x34503402, 0xe1ad79a9, 
							  0xae2bd3d2, 0x4ec9e23d, 0x35162f55, 0x993a1b83, 
							  0x35779406, 0x00d30e40, 0xc28639a1, 0x4ea39e14, 
							 0x5a94dd9d, 0x80adf249, 0x863da72a, 0x71021e78};  

//  192
	uint32_t key_cbc_192[6]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622
	uint32_t iv_cbc_192[4]      = {0x72304ad5, 0xe5f5937d, 0xf38d8adf, 0x367b0c53};  //iv_cbc    = 0x72304ad5 e5f5937d f38d8adf 367b0c53
	uint32_t plain_cbc_192[20]   = {0x1d273eb7, 0x60632039, 0x7e314c2f, 0xa1ab5e18, 
							  0x79aa587e, 0xb834e5de, 0x51d45354, 0xafcebfd6, 
							  0x7a9cd97c, 0x5be67590, 0x40f27cfd, 0xc60d77f7, 
							  0xf923668d, 0xb2fde000, 0x20020634, 0x3fee8224, 
							  0x097ed2f3, 0x4f872281, 0x4329f38c, 0x6c564821}; //plain_cbc  = 0x1d273eb7 60632039 7e314c2f a1ab5e18...
	uint32_t cipher_cbc_192[20]  = {0x4c9abdee, 0x35097f17, 0x37de6468, 0x0a14af58, 
							  0x20e48235, 0x31a46420, 0x93a283f4, 0x7e982a45, 
							  0x9e15fd81, 0xf80f2328, 0x0701d24d, 0x4c7bcb79, 
							  0xbcfd3d10, 0xdc3dceb2, 0x800fa9d1, 0xadc513f9, 
							  0xfa161563, 0x3c65e394, 0xbad169b6, 0xac2f04cd}; //cipher_cbc = 0x4c9abdee 35097f17 37de6468 0a14af58...

//  256
	uint32_t key_cbc_256[8]     = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0xc42821e1, 0x3ac9b622, 0x72304ad5, 0xe5f5937d};  // key_ecb    = 0xe04ad1a4 65fe706b 80cf2960 5da2636a c42821e1 3ac9b622 72304ad5 e5f5937d
	uint32_t iv_cbc_256[4]      = {0xf38d8adf, 0x367b0c53, 0x1d273eb7, 0x60632039};  // iv_cbc    = 0xf38d8adf 367b0c53 1d273eb7 60632039;
	uint32_t plain_cbc_256[20]   = {0x7e314c2f, 0xa1ab5e18, 0x79aa587e, 0xb834e5de,
							 0x51d45354, 0xafcebfd6, 0x7a9cd97c, 0x5be67590, 
							 0x40f27cfd, 0xc60d77f7, 0xf923668d, 0xb2fde000,
							 0x20020634, 0x3fee8224, 0x097ed2f3, 0x4f872281,
							 0x4329f38c, 0x6c564821, 0x6e897e46, 0x25ac03f3};  // plain_cbc  = 0x7e314c2f a1ab5e18 79aa587e b834e5de...
	uint32_t cipher_cbc_256[20]  = {0xc8324f39, 0xcf6969a7, 0x35f97dd9, 0x34a90318,
							 0x72866a00, 0x2fd8e17d, 0xf8003085, 0x4e796949,
							 0xcd70af28, 0xe13be0d2, 0x2d613def, 0x8da350f6,
							 0x7107db84, 0x1c3f5585, 0xc77b89dc, 0x4ccd5abc,
							 0x307463b8, 0x5098659e, 0xdd3c2694, 0xedd32cdb};  // cipher_cbc = 0xc8324f39 cf6969a7 35f97dd9 34a90318...


    //-----------------------    CBC mode (no swap)   ---------------------
//  128
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_128) / 16;
		
    HAL_AES_SetKey(key_cbc_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
    HAL_AES_Crypt(plain_cbc_128, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_128, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_128[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_128, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_128, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_128[i])
    	{
    		return 1;
    	}
    }

//  192
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_192) / 16;
		
    HAL_AES_SetKey(key_cbc_192, AES_KEY_192, AES_SWAP_DISABLE);	  //192bit key
    HAL_AES_Crypt(plain_cbc_192, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_192, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_192[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_192, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_192, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_192[i])
    	{
    		return 1;
    	}
    }

//  256
//	mode = AES_NORMAL_MODE;
    mode = AES_SECURITY_MODE;

	blk_len = sizeof(plain_cbc_256) / 16;
		
    HAL_AES_SetKey(key_cbc_256, AES_KEY_256, AES_SWAP_DISABLE);	  //256bit key
    HAL_AES_Crypt(plain_cbc_256, result, blk_len, AES_ENCRYPTION, AES_CBC_MODE, iv_cbc_256, mode);
    
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != cipher_cbc_256[i])
    	{
    		return 1;
    	}
    }
    
    HAL_AES_Crypt(cipher_cbc_256, result, blk_len, AES_DECRYPTION,AES_CBC_MODE, iv_cbc_256, mode);
    for( i = 0; i < (blk_len*4); i ++)
    {
    	if(result[i] != plain_cbc_256[i])
    	{
    		return 1;
    	}
    }
	
	//-----------------------   End of  CBC mode   ---------------------
	return 0;
}




uint32_t aes_ctr_test(void)
{
    uint32_t i;
    uint32_t result[32];
    uint32_t blk_len;	

//128	
  	uint32_t key_ctr_128[4]    = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a};
    uint32_t key_ctr_192[6]    = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0x45e95c1b, 0x7d65bc7a};
   	uint32_t key_ctr_256[8]    = {0xe04ad1a4, 0x65fe706b, 0x80cf2960, 0x5da2636a, 0x45e95c1b, 0x7d65bc7a, 0x2c31205a, 0x7ccb788a};

	uint32_t iv_ctr[4]     = {0xC3C3C3C3, 0xA5A5A5A5, 0x00007123, 0x02900040}; 

    uint32_t plain_ctr[32]  = {
    0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f, 0x10111213, 0x14151617, 0x18191a1b, 0x1c1d1e1f, 
    0x20212223, 0x24252627, 0x28292a2b, 0x2c2d2e2f, 0x30313233, 0x34353637, 0x38393a3b, 0x3c3d3e3f, 
    0x40414243, 0x44454647, 0x48494a4b, 0x4c4d4e4f, 0x50515253, 0x54555657, 0x58595a5b, 0x5c5d5e5f, 
    0x60616263, 0x64656667, 0x68696a6b, 0x6c6d6e6f, 0x70717273, 0x74757677, 0x78797a7b, 0x7c7d7e7f, 
    };      
    uint32_t cipher_ctr_128[32] = {
    0x209df31e, 0xd053c447, 0x64939785, 0x1c8802f1, 0x45e95c1b, 0x7d65bc7a, 0x2c31205a, 0x7ccb788a,
    0xb82345c5, 0xe015e38f, 0x9cc78fb2, 0x85cb6c36, 0x7e62eac2, 0xf9c953a9, 0x00232455, 0x857e5af0,
    0xd09b0a7d, 0x8085181c, 0x0d4899db, 0xde862593, 0x4ffa6442, 0xc31d0ba5, 0xbb5d7598, 0xba97e45d,
    0x292ae849, 0xbc12d9cf, 0x2f63d342, 0xd11b7925, 0x49de1d89, 0xa2bcc185, 0x76400ec0, 0x39ba742f
    };       
    uint32_t cipher_ctr_192[32] = {
    0xb2bfb762, 0x56f0a857, 0x0b9f36b8, 0x00628160, 0x736bf8af, 0x24602c35, 0x99d269cd, 0xdd018448,
    0x95fb58ea, 0x6c5ae7ea, 0x45341157, 0xb22da545, 0x89760fc9, 0x1e28d6ab, 0x29564180, 0x641fe11f,
    0x78ea60f2, 0xa39047a6, 0x6ea81211, 0x0af8d868, 0xf40af5be, 0xbb060c4b, 0x51b33ae8, 0xb24993c7,
    0x57e8fe1e, 0xff8907a0, 0x7bde0165, 0x0c3b3940, 0xa1dc13d6, 0xa193e2c5, 0xc2107e10, 0x073047ae
    }; 
    uint32_t cipher_ctr_256[32] = {
    0xc8d5a7ce, 0xb00453e7, 0x388540e7, 0x31a22ef0, 0xfee4f8b4, 0x311338c7, 0xcbf8d182, 0x0afcd661,
    0xe6218c67, 0xecb11f94, 0x8f15bed6, 0xd12740b8, 0xbbc0ffb8, 0x09798768, 0xcf1254bd, 0xae968e3f,
    0x0ca80bfc, 0xeb7fbea4, 0x1c9e4a48, 0xd9d47ba4, 0x44d898f3, 0xee2c9cd3, 0xe44e1594, 0xb025c8ae,
    0xb10fb422, 0x99edb59b, 0xe4a9e65b, 0xdcc36a89, 0x61be8e7c, 0x4464b5e1, 0xec363df5, 0x3de5eeba
    }; 
   //-----------------------    CTR mode (no swap)   ---------------------
//128
		blk_len = sizeof(plain_ctr) / 16;
		HAL_AES_SetKey(key_ctr_128, AES_KEY_128, AES_SWAP_DISABLE);	  //128bit key
        HAL_AES_Crypt(plain_ctr, result, blk_len, AES_ENCRYPTION, AES_CTR_MODE,iv_ctr, AES_NORMAL_MODE);
        
		for( i = 0; i < (blk_len*4); i ++)
		{
			if(result[i] != cipher_ctr_128[i])
			{
				return 1;
			}
		}
		
        
        HAL_AES_Crypt(result, cipher_ctr_128, blk_len, AES_DECRYPTION, AES_CTR_MODE,iv_ctr, AES_NORMAL_MODE);

		for( i = 0; i < (blk_len*4); i ++)
		{		
			if(cipher_ctr_128[i] != plain_ctr[i])
			{
				return 1;
			}
		}

//192
		blk_len = sizeof(plain_ctr) / 16;
		HAL_AES_SetKey(key_ctr_192, AES_KEY_192, AES_SWAP_DISABLE);	  //128bit key
        HAL_AES_Crypt(plain_ctr, result, blk_len, AES_ENCRYPTION, AES_CTR_MODE,iv_ctr, AES_NORMAL_MODE);
        
		for( i = 0; i < (blk_len*4); i ++)
		{
			if(result[i] != cipher_ctr_192[i])
			{
				return 1;
			}
		}
        
        HAL_AES_Crypt(result, cipher_ctr_192, blk_len, AES_DECRYPTION, AES_CTR_MODE,iv_ctr, AES_NORMAL_MODE);

		for( i = 0; i < (blk_len*4); i ++)
		{		
			if(cipher_ctr_192[i] != plain_ctr[i])
			{
				return 1;
			}
		}

//256
		blk_len = sizeof(plain_ctr) / 16;
		HAL_AES_SetKey(key_ctr_256, AES_KEY_256, AES_SWAP_DISABLE);	  //128bit key
        HAL_AES_Crypt(plain_ctr, result, blk_len, AES_ENCRYPTION, AES_CTR_MODE,iv_ctr, AES_NORMAL_MODE);
        
		for( i = 0; i < (blk_len*4); i ++)
		{
			if(result[i] != cipher_ctr_256[i])
			{
				return 1;
			}
		}
        
        HAL_AES_Crypt(result, cipher_ctr_256, blk_len, AES_DECRYPTION, AES_CTR_MODE,iv_ctr, AES_NORMAL_MODE);

		for( i = 0; i < (blk_len*4); i ++)
		{		
			if(cipher_ctr_256[i] != plain_ctr[i])
			{
				return 1;
			}
		}
	
    return 0;	
}



/******************************************************************************
*@brief : APP_AES_Test
*@param : none
*@return: none
******************************************************************************/
void APP_AES_Test(void)
{
    printfS(" AES Test Start!\r\n");
    
	if(aes_ecb_test())
		printfS(" AES ECB Test Fail!\r\n");
	else
		printfS(" AES ECB Test Pass!\r\n");

	if(aes_cbc_test())
		printfS(" AES CBC Test Fail!\r\n");
	else
		printfS(" AES CBC Test Pass!\r\n");	

	if(aes_ctr_test())
		printfS(" AES CTR Test Fail!\r\n");
	else
		printfS(" AES CTR Test Pass!\r\n\n");	

    printfS(" AES Test End!\r\n");

}





