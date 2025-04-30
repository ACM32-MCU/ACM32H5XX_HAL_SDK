/******************************************************************************
*@file  : app.c
*@brief : CRC Demos 
******************************************************************************/
#include "app.h"


uint8_t  test_data_buf[256];
uint16_t test_data_buf_16[128];
uint32_t test_data_buf_32[64];
DMA_HandleTypeDef DMA1_Ch0Handle;


void test_data_init(void)
{
	uint32_t i;
	for(i = 0; i < 256; i++ )
	{
		test_data_buf[i] = i;
	}
}

uint16_t CRC16_base_MSB(uint16_t crc,uint8_t data, uint16_t poly)
{
	uint8_t i;

	crc = crc ^ (uint16_t)(data <<8);
	for(i=0;i<8;i++)
	{
		if((crc&0x8000) == 0x8000)
		{
			crc = (crc << 1) ^ poly;
		}
		else
		{
			crc = crc << 1;
		}
	}
	return crc&0xffff;
}

uint16_t CRC16_soft_MSB(uint8_t crc_data[], uint32_t len, uint16_t poly, uint16_t init_data, uint16_t result_xor)
{
	uint32_t i;
	uint16_t reg_crc;

	reg_crc = init_data;

	for( i = 0; i < len; i++ )
	{
		reg_crc = CRC16_base_MSB(reg_crc,crc_data[i],poly);	
	}
	return (reg_crc^result_xor)&0xffff;
}



/******************************************************************************
*@brief : APP_CRC16_Test
*@param : none
*@return: none
******************************************************************************/
void APP_CRC16_Test(void)
{
	uint32_t result1,result2,result3;
	uint32_t poly,init,xor_out;
	uint32_t byte_len;

	CRC_HandleTypeDef handle_crc;

	test_data_init();
	byte_len = 256;
	
	poly = 0x1021;  //poly = x16+x12+x5+1
	init = 0xFFFF;
	xor_out = 0x0000;
	
	result1 = CRC16_soft_MSB(test_data_buf,byte_len,poly,init,xor_out);	//comput by software
	
	handle_crc.Instance = CRC;
	handle_crc.Init.PolyRev = CRC_POLY_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.OutxorRev = CRC_OUTXOR_REV_DIS;  //need to be fixed to REV_EN
	handle_crc.Init.InitRev = CRC_INIT_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.RsltRev = CRC_RSLT_REV_DIS;      //opposite to common CRC tools 
	handle_crc.Init.DataRev = CRC_DATA_REV_DISABLE; //opposite to common CRC tools
	handle_crc.Init.PolyLen = CRC_POLTY_LEN_16;
	handle_crc.Init.DataLen = CRC_DATA_LEN_1B;
	
	handle_crc.Init.PolyData = 0x1021;  //poly = x16+x12+x5+1(CRC-16/CCIT-FALSE)
	handle_crc.Init.InitData = 0xFFFF;
	handle_crc.Init.OutXorData = 0x0000;
	
	handle_crc.CRC_Data_Buff = test_data_buf;
	handle_crc.CRC_Data_Len = byte_len;
	
    HAL_CRC_Init(&handle_crc);

//	result2 = HAL_CRC_Calculate(&handle_crc); //comput by hardware

	handle_crc.CRC_Data_Buff = test_data_buf;
	handle_crc.CRC_Data_Len = byte_len/2;
    result2 = HAL_CRC_Calculate(&handle_crc); //comput by hardware
    
	handle_crc.CRC_Data_Buff = test_data_buf+byte_len/2;
	handle_crc.CRC_Data_Len = byte_len/2;
    result2 = HAL_CRC_Calculate(&handle_crc); //comput by hardware

    
    //use DMA
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	DMA1_Ch0Handle.Instance          = DMA1_Channel0;
	DMA1_Ch0Handle.Init.Mode         = DMA_MODE_NORMAL;
	DMA1_Ch0Handle.Init.DataFlow     = DMA_DATAFLOW_M2M;
	DMA1_Ch0Handle.Init.ReqID        = DMA1_REQ_M2M;
	DMA1_Ch0Handle.Init.SrcIncDec    = DMA_SRCINCDEC_INC;
	DMA1_Ch0Handle.Init.DestIncDec   = DMA_DESTINCDEC_DISABLE;
	DMA1_Ch0Handle.Init.SrcWidth     = DMA_SRCWIDTH_BYTE;
	DMA1_Ch0Handle.Init.DestWidth    = DMA_DESTWIDTH_BYTE;
	DMA1_Ch0Handle.Init.SrcBurst     = DMA_SRCBURST_16;
	DMA1_Ch0Handle.Init.DestBurst    = DMA_DESTBURST_16;
	
	HAL_DMA_Init(&DMA1_Ch0Handle);    
    
    HAL_DMA_ClearFlag(&DMA1_Ch0Handle, DMA_FLAG_HTC);
    HAL_DMA_ClearFlag(&DMA1_Ch0Handle, DMA_FLAG_TC);
    HAL_DMA_ClearFlag(&DMA1_Ch0Handle, DMA_FLAG_ERR);
        
    HAL_CRC_Init(&handle_crc);
       
    HAL_DMA_Start(&DMA1_Ch0Handle, (uint32_t)test_data_buf, (uint32_t)(&handle_crc.Instance->DATA), byte_len); //byte_len
    while (HAL_DMA_GetFlag(&DMA1_Ch0Handle, DMA_FLAG_RTC) == RESET){;}

        
    result3 = handle_crc.Instance->DATA;    
	
	printf("\nresult1(soft)   = 0x%x\n", result1);
	printf("result2(normal) = 0x%x\n", result2);
	printf("result3(DMA)    = 0x%x\n", result3);

	if ((result1 == result2) && (result1 == result3))
	{
		printf("\nCRC-16 test success\n");
	} 
	else
	{
		printf("CRC-16 test fail\n");
	}			

}

uint32_t crc32_base_MSB(uint32_t crc, uint8_t data, uint32_t poly)
{
	uint8_t i;

	crc = crc ^ (int)data << 24;
	for(i = 0; i < 8; i++)
	{
		if((crc & 0x80000000) == 0x80000000)
		{
			crc = crc << 1 ^ poly;
		}
		else
		{
			crc = crc << 1;
		}
	}
	return crc;
}

uint32_t CRC32_soft_MSB(uint8_t crc_data[], uint32_t len, uint32_t poly, uint32_t init_data, uint32_t result_xor)
{
	uint32_t i;
	uint32_t reg_crc;

	reg_crc = init_data;

	for( i = 0; i < len; i++ )
	{
		reg_crc = crc32_base_MSB(reg_crc,crc_data[i],poly);	
	}
	return (reg_crc ^ result_xor);
}

/******************************************************************************
*@brief : APP_CRC32_Test
*@param : none
*@return: none
******************************************************************************/
void APP_CRC32_Test(void)
{
    int i;
	uint32_t result1,result2,result3,result4;
	uint32_t poly,init,xor_out;
	uint32_t byte_len;
    uint16_t test_data_buf_16[128];
    uint32_t test_data_buf_32[64];

	CRC_HandleTypeDef handle_crc;

	test_data_init();
	byte_len = 256;
    
 	poly = 0x04C11DB7;  //CRC32/MPEG-2 : poly = x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 +x + 1
	init = 0xFFFFFFFF;
	xor_out = 0x00000000;
	
	/* input DataLen = 1 bytes             *
	 * indata: 0x00, 0x01, 0x02, ..., 0xFF */
	result1 = CRC32_soft_MSB(test_data_buf,byte_len,poly,init,xor_out);	//comput by software
	printf("\nresult1(soft)             = 0x%x\n", result1);
	
	handle_crc.Instance = CRC;
	handle_crc.Init.PolyRev = CRC_POLY_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.OutxorRev = CRC_OUTXOR_REV_DIS;  //need to be fixed to REV_EN
	handle_crc.Init.InitRev = CRC_INIT_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.RsltRev = CRC_RSLT_REV_DIS;      //opposite to common CRC tools 
	handle_crc.Init.DataRev = CRC_DATA_REV_DISABLE; //opposite to common CRC tools
	handle_crc.Init.PolyLen = CRC_POLTY_LEN_32;
	handle_crc.Init.DataLen = CRC_DATA_LEN_1B;
	
	handle_crc.Init.PolyData = 0x04C11DB7;  //CRC32/MPEG-2 : poly = x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 +x + 1
	handle_crc.Init.InitData = 0xFFFFFFFF;
	handle_crc.Init.OutXorData = 0x00000000;    
	
	handle_crc.CRC_Data_Buff = test_data_buf;
	handle_crc.CRC_Data_Len = byte_len;
	
    HAL_CRC_Init(&handle_crc);

	result2 = HAL_CRC_Calculate(&handle_crc); //comput by hardware

//	handle_crc.CRC_Data_Buff = test_data_buf;
//	handle_crc.CRC_Data_Len = byte_len/2;
//  result2 = HAL_CRC_Calculate(&handle_crc); //comput by hardware
//    
//	handle_crc.CRC_Data_Buff = test_data_buf+byte_len/2;
//	handle_crc.CRC_Data_Len = byte_len/2;
//  result2 = HAL_CRC_Calculate(&handle_crc); //comput by hardware
    
	printf("result2(normal)           = 0x%x\n", result2);
	
	/* input DataLen = 2 bytes                     *
	 * indata: 0x0001, 0x0203, 0x0405, ..., 0xFEFF */
    for(i = 0; i < 128; i++ )
	{
	    test_data_buf_16[i] = ((i*2)<<8) | (i*2 + 1);
    }    	

	handle_crc.Instance = CRC;
	handle_crc.Init.PolyRev = CRC_POLY_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.OutxorRev = CRC_OUTXOR_REV_DIS;  //need to be fixed to REV_EN
	handle_crc.Init.InitRev = CRC_INIT_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.RsltRev = CRC_RSLT_REV_DIS;      //opposite to common CRC tools 
	handle_crc.Init.DataRev = CRC_DATA_REV_BY_HALFWORD; //opposite to common CRC tools
	handle_crc.Init.PolyLen = CRC_POLTY_LEN_32;
	handle_crc.Init.DataLen = CRC_DATA_LEN_2B;
	
	handle_crc.Init.PolyData = 0x04C11DB7;  //CRC32/MPEG-2 : poly = x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 +x + 1
	handle_crc.Init.InitData = 0xFFFFFFFF;
	handle_crc.Init.OutXorData = 0x00000000;    
    HAL_CRC_Init(&handle_crc);
    
    handle_crc.CRC_Data_Buff = test_data_buf_16;
	handle_crc.CRC_Data_Len = sizeof(test_data_buf_16);

	result3 = HAL_CRC_Calculate(&handle_crc); //comput by hardware
	printf("result3(DataLen = 2bytes) = 0x%x\n", result3);

	/* input DataLen = 4 bytes                                     *
	 * indata: 0x00010203, 0x04050607, 0x08090A0B, ..., 0xFCFDFEFF */
	for(i = 0; i < 64; i++ )
	{
		test_data_buf_32[i] = ((i*4)<<24) | ((i*4+1)<<16) | ((i*4+2)<<8) | (i*4 + 3);
	}    

	handle_crc.Instance = CRC;
	handle_crc.Init.PolyRev   = CRC_POLY_REV_DIS;    //need to be fixed to REV_EN
	handle_crc.Init.OutxorRev = CRC_OUTXOR_REV_DIS;  //need to be fixed to REV_EN
	handle_crc.Init.InitRev = CRC_INIT_REV_DIS;      //need to be fixed to REV_EN
	handle_crc.Init.RsltRev = CRC_RSLT_REV_DIS;      //opposite to common CRC tools 
	handle_crc.Init.DataRev = CRC_DATA_REV_BY_WORD; //opposite to common CRC tools
	handle_crc.Init.PolyLen = CRC_POLTY_LEN_32;
	handle_crc.Init.DataLen = CRC_DATA_LEN_4B;
	
	handle_crc.Init.PolyData = 0x04C11DB7;  //CRC32/MPEG-2 : poly = x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 +x + 1
	handle_crc.Init.InitData = 0xFFFFFFFF;
	handle_crc.Init.OutXorData = 0x00000000;    
    HAL_CRC_Init(&handle_crc);
    
    handle_crc.CRC_Data_Buff = test_data_buf_32;
	handle_crc.CRC_Data_Len = sizeof(test_data_buf_32);

	result4 = HAL_CRC_Calculate(&handle_crc); //comput by hardware
	printf("result4(DataLen = 4bytes) = 0x%x\n", result4);
    
	if ((result1 == result2)&&(result2 == result3)&&(result3 == result4))
	{
		printf("\nCRC-32 test success\n");
	} 
	else
	{
		printf("CRC-32 test fail\n");
	}

}





