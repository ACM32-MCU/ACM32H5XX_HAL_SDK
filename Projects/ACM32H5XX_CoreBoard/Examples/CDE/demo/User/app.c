/******************************************************************************
*@file  : cde.c
*@brief : UART Demos 
******************************************************************************/

#include "app.h"  

#define __ARM_FEATURE_CDE   1
#include <arm_cde.h>
#include "hal_cde.h"
#include "hal_dwt.h"

uint32_t test_input[10240];

uint8_t reverse_byte(uint8_t in_data)
{
	uint8_t temp;
	uint8_t i;

	temp = 0;
	for(i = 0; i < 8; i++)
	{
		if(in_data & (1 << i))
		{
			temp |= 0x80 >> i;
		}
	}

	return temp;
}

uint32_t reverse_32(uint32_t in_data)
{
	uint32_t temp;
	uint8_t i;

	temp = 0;
	for(i = 0; i < 32; i++)
	{
		if(in_data & (1 << i))
		{
			temp |= 0x80000000 >> i;
		}
	}

	return temp;
}



/*
   Counting the number of ones in a 32 bit register.
   The output of this instruction is a 32 bit register, containing
   the number of ones
*/
uint32_t POPCNT(uint32_t a) {
  a = (a&0x55555555) + ((a&0xaaaaaaaa)>> 1);
  a = (a&0x33333333) + ((a&0xcccccccc)>> 2);
  a = (a&0x0f0f0f0f) + ((a&0xf0f0f0f0)>> 4);
  a = (a&0x00ff00ff) + ((a&0xff00ff00)>> 8);
  a = (a&0x0000ffff) + ((a&0xffff0000)>>16);
  return a;
}

/*
    CRC32 instruction for updating a 32 bit state register by 8 bit input
    data using the ISO 3309 standard.
    Polynomial representation (reversed) : 0xEDB88320
*/
#define POLYNOMIAL 0xedb88320
uint32_t CRC32(uint32_t crc, uint32_t d) {
  crc = crc ^ (d&0xff);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  if (crc&1) crc = (crc>>1) ^ POLYNOMIAL;  else crc = (crc>>1);
  return crc;
}

/*  This instruction extracts 32 bits from two 32 bit registers.
    res[ 0:23] = a[8:31]
    res[24:31] = b[0:7]
*/
uint32_t PACK8(uint32_t a, uint32_t b) {
  return (a >> 8) | (b << 24);
}


/*  This instruction extracts 32 bits from two 32 bit registers.
    res[ 0:15] = a[16:31]
    res[16:31] = b[0:15]
*/
uint32_t PACK16(uint32_t a, uint32_t b) {
  return (a >> 16) | (b << 16);
}


/*  Instruction for updating the FIR filter delay line by two 8 bit operands
    This instruction extracts 32 bits from two 32 bit registers.
    res[ 0:7] = a[24:31]
    res[8:31] = b[0:23]
*/
uint32_t PACK24(uint32_t a, uint32_t b) {
  return (a >> 24) | (b << 8);
}


/*  This instruction does four 8x8 bit unsigned multiplicaltions forming
    four 16 bit products.
    The four products and the accumulator 'a' are added, and the 32 bit
    result is returned from the instruction.
*/
uint32_t UMLAQ(uint32_t a, uint32_t b, uint32_t c) {
  uint16_t a0, a1, a2, a3;
  a0 = (uint16_t)((b >>  0) & 0xff) * (uint16_t)((c >>  0) & 0xff);
  a1 = (uint16_t)((b >>  8) & 0xff) * (uint16_t)((c >>  8) & 0xff);
  a2 = (uint16_t)((b >> 16) & 0xff) * (uint16_t)((c >> 16) & 0xff);
  a3 = (uint16_t)((b >> 24) & 0xff) * (uint16_t)((c >> 24) & 0xff);
  return a + (uint32_t)a0 + (uint32_t)a1 + (uint32_t)a2 + (uint32_t)a3;
}


/*  This instruction does four 8x8 bit unsigned multiplicaltions forming
    four 16 bit products.
    The four products are added, and the 32 bit result is returned from
    the instruction.
*/
uint32_t UMUAQ(uint32_t b, uint32_t c) {
  uint16_t a0, a1, a2, a3;
  a0 = (uint16_t)((b >>  0) & 0xff) * (uint16_t)((c >>  0) & 0xff);
  a1 = (uint16_t)((b >>  8) & 0xff) * (uint16_t)((c >>  8) & 0xff);
  a2 = (uint16_t)((b >> 16) & 0xff) * (uint16_t)((c >> 16) & 0xff);
  a3 = (uint16_t)((b >> 24) & 0xff) * (uint16_t)((c >> 24) & 0xff);
  return (uint32_t)a0 + (uint32_t)a1 + (uint32_t)a2 + (uint32_t)a3;
}
/*  This instruction does four 8 bit signed by 8 bit unsigned multi-
    plications forming four 16 bit products.
    The four products and the accumulator 'a' are added, and the 32 bit
    result is returned from the instruction.
*/
int32_t SUMLAQ(int32_t a, uint32_t b, uint32_t c) {
  int16_t a0, a1, a2, a3;
  a0 = (int16_t)(int8_t)((b >>  0) & 0xff) * (int16_t)(uint8_t)((c >>  0) & 0xff);
  a1 = (int16_t)(int8_t)((b >>  8) & 0xff) * (int16_t)(uint8_t)((c >>  8) & 0xff);
  a2 = (int16_t)(int8_t)((b >> 16) & 0xff) * (int16_t)(uint8_t)((c >> 16) & 0xff);
  a3 = (int16_t)(int8_t)((b >> 24) & 0xff) * (int16_t)(uint8_t)((c >> 24) & 0xff);
  return a + (int32_t)a0 + (int32_t)a1 + (int32_t)a2 + (int32_t)a3;
}


/*  This instruction does four 8 bit signed by 8 bit unsigned multi-
    plications forming four 16 bit products.
    The four products are added, and the 32 bit result is returned from
    the instruction.
*/
int32_t SUMUAQ(uint32_t b, uint32_t c) {
  int16_t a0, a1, a2, a3;
  a0 = (int16_t)(int8_t)((b >>  0) & 0xff) * (int16_t)(uint8_t)((c >>  0) & 0xff);
  a1 = (int16_t)(int8_t)((b >>  8) & 0xff) * (int16_t)(uint8_t)((c >>  8) & 0xff);
  a2 = (int16_t)(int8_t)((b >> 16) & 0xff) * (int16_t)(uint8_t)((c >> 16) & 0xff);
  a3 = (int16_t)(int8_t)((b >> 24) & 0xff) * (int16_t)(uint8_t)((c >> 24) & 0xff);
  return (int32_t)a0 + (int32_t)a1 + (int32_t)a2 + (int32_t)a3;
}



/*  This instruction does a round operation on a uint32_t */
uint32_t ROUND(uint32_t b, uint32_t c) {

  b = b>>((c-1)&31);
  b = b+1;
  b = b>>1;
  return b;
}

/*  This instruction does a round operation on an int32_t */
int32_t ROUND_INT(int32_t b, int32_t c) {
  b = b>>((c-1)&31);  /* Arithmetic shift */
  b = b+1;
  b = b>>1;           /* Arithmetic shift */
  return b;
}

/*
  This function calculates the hamming distance between two input registers
 
  Input registers takes any value, valid output values [0:31]
  Do not use {A} CDE type, proposed CDE instruction: cx3(p0, a, b, c, #)
*/
uint32_t HAMMING_DIST(uint32_t b, uint32_t c) { 
  uint32_t i, a, val;
 
  val = b ^ c;
  a = 0;
  for (i = 0; i < 32; i++) {
    a = a + (val & 1);
    val = val >> 1;
  }
 
  return a;
}

void popcnt_test(void)
{
    volatile uint32_t ret;
	  uint32_t g_y=2;
    g_y = 0;
    //ret = __arm_cx1a(0,g_y, 6);
    ret =  __HAL_CDE_POPCNT(g_y);  
	  if(ret!=0)
		{
			printfS("error! xxxxxxxxxxxxxxxx\n");
		}
		else
		{
			printfS("success!\n");
		}
		printfS("%x the number of ones is %x!\n\n",g_y,ret);
	  
    g_y = 0x55555555;
    //ret = __arm_cx1a(0,g_y, 6);
    ret =  __HAL_CDE_POPCNT(g_y);
	  if(ret!=16)
		{
			printfS("error! xxxxxxxxxxxxxxxx\n");
		}
		else
		{
			printfS("success!\n");
		}
		printfS("%x the number of ones is %x!\n\n",g_y,ret);

    g_y = 0xffffffff;
        
    //ret = __arm_cx1a(0,g_y, 6);
    ret =  __HAL_CDE_POPCNT(g_y);
	  if(ret!=32)
		{
			printfS("error! xxxxxxxxxxxxxxxx\n");
		}
		else
		{
			printfS("success!\n");
		}
		printfS("%x the number of ones is %x!\n\n",g_y,ret);
			
}


void cde_crc32_test(void)
{
	uint8_t  Data[64];
	uint32_t Data32[16];
	uint32_t i,j;
  uint32_t reg_crc;
	uint32_t result1,result2,result3,result4,result5;;
	int byte_len;	
	int word_len;
	int byte_remain;

	byte_len = 9;
	i = 0;
	while (i < byte_len)
	{
		Data[i] = i;
		Data[i] = reverse_byte(Data[i]);
		i = i + 1;
	}
	reg_crc = 0xffffffff;	
	reg_crc = reverse_32(reg_crc); 
	for( i = 0; i < byte_len; i++ )
	{
		reg_crc = CRC32(reg_crc,Data[i]);
	}	
	result1 = reverse_32(reg_crc);
	printf("result1 = %x\n", result1);
	
	i = 0;
	while (i < byte_len)
	{
		Data[i] = i;
		i = i + 1;
	}
	reg_crc = 0xffffffff;	
	for( i = 0; i < byte_len; i++ )
	{
		//reg_crc = __arm_cx2a(0,reg_crc,Data[i], 3);
		reg_crc = __HAL_CDE_CRC32_REVERSE(reg_crc,Data[i]);        
	}	
	result2 = reg_crc;
	printf("result2 = %x\n", result2);

	
	
	word_len = byte_len>>2;
	byte_remain = byte_len&0x3;
	i = 0;
	j = 0;
	while (i < byte_len)
	{
		Data32[j] = (uint32_t)(i) | (uint32_t)((i+1)<<8) | (uint32_t)((i+2)<<16) | (uint32_t)((i+3)<<24);
		i = i + 4;
		j++;
	}	
	printf("data = %x\n", Data32[0]);
	if(byte_remain)
	{
		Data32[j] &= 0xffffffff<<(32-byte_remain*8);
	}
	reg_crc = 0xffffffff;	
	for( i = 0; i < word_len; i++ )
	{
		//reg_crc = __arm_cx2a(0,reg_crc,Data32[i], 1);
		reg_crc = __HAL_CDE_CRC32_32BIT(reg_crc,Data32[i]);
	}	
	for( i = 0; i < byte_remain; i++ )
	{
		//reg_crc = __arm_cx2a(0,reg_crc,Data[i+(word_len<<2)], 3);
		reg_crc = __HAL_CDE_CRC32_REVERSE(reg_crc,Data[i+(word_len<<2)]);
	}		
	result3 = reg_crc;
	printf("result3 = %x\n", result3);	
	
	if ((result1 == result2) && (result2 == result3) )
	{
		printf("cde crc32 success\n\n");
	} 
	else
	{
		printf("cde crc32 fail xxxxxxxxxxxxxxxx\n\n");
		return ;
	}		
}

void cde_pack_test(void)
{
	uint32_t a,b;
	uint32_t result1,result2,result3;
	
	a = 0;
	b = 0;
	result1 = PACK8(a,b);
	//result2 = __arm_cx3(0,a,b, 4);
	result2 = __HAL_CDE_PACK8(a,b);    
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack8 success\n\n");
	} 
	else
	{
		printfS("pack8 fail xxxxxxxxxxxxxxxx\n\n");
	}		

	a = 0x11223344;
	b = 0x55667788;
	result1 = PACK8(a,b);
	//result2 = __arm_cx3(0,a,b, 4);
	result2 = __HAL_CDE_PACK8(a,b);
    
	result3 = __HAL_CDE_PACK8(a,b);    
    
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
 	printfS("result3 = %x\n", result3);   
	if ((result1 == result2) && (result3 == result2))
	{
		printfS("pack8 success\n\n");
	} 
	else
	{
		printfS("pack8 fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	a = 0x9900aabb;
	b = 0xccddeeff;
	result1 = PACK8(a,b);
	//result2 = __arm_cx3(0,a,b, 4);
	result2 = __HAL_CDE_PACK8(a,b);    
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack8 success\n\n");
	} 
	else
	{
		printfS("pack8 fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	a = 0xffffffff;
	b = 0xffffffff;
	result1 = PACK8(a,b);
	//result2 = __arm_cx3(0,a,b, 4);
	result2 = __HAL_CDE_PACK8(a,b);    
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack8 success\n\n");
	} 
	else
	{
		printfS("pack8 fail xxxxxxxxxxxxxxxx\n\n");
	}

	a = 0;
	b = 0;
	result1 = PACK16(a,b);
	//result2 = __arm_cx3(0,a,b, 5);
	result2 = __HAL_CDE_PACK16(a,b);        
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack16 success\n\n");
	} 
	else
	{
		printfS("pack16 fail xxxxxxxxxxxxxxxx\n\n");
	}		

	a = 0x11223344;
	b = 0x55667788;
	result1 = PACK16(a,b);
	//result2 = __arm_cx3(0,a,b, 5);
	result2 = __HAL_CDE_PACK16(a,b);  
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack16 success\n\n");
	} 
	else
	{
		printfS("pack16 fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	a = 0x9900aabb;
	b = 0xccddeeff;
	result1 = PACK16(a,b);
	//result2 = __arm_cx3(0,a,b, 5);
	result2 = __HAL_CDE_PACK16(a,b);  
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack16 success\n\n");
	} 
	else
	{
		printfS("pack16 fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	a = 0xffffffff;
	b = 0xffffffff;
	result1 = PACK16(a,b);
	//result2 = __arm_cx3(0,a,b, 5);
	result2 = __HAL_CDE_PACK16(a,b);  
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack16 success\n\n");
	} 
	else
	{
		printfS("pack16 fail xxxxxxxxxxxxxxxx\n\n");
	}


	a = 0;
	b = 0;
	result1 = PACK24(a,b);
	//result2 = __arm_cx3(0,a,b, 6);
	result2 = __HAL_CDE_PACK24(a,b);      
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack24 success\n\n");
	} 
	else
	{
		printfS("pack24 fail xxxxxxxxxxxxxxxx\n\n");
	}		

	a = 0x11223344;
	b = 0x55667788;
	result1 = PACK24(a,b);
	//result2 = __arm_cx3(0,a,b, 6);
	result2 = __HAL_CDE_PACK24(a,b);  
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack24 success\n\n");
	} 
	else
	{
		printfS("pack24 fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	a = 0x9900aabb;
	b = 0xccddeeff;
	result1 = PACK24(a,b);
	//result2 = __arm_cx3(0,a,b, 6);
	result2 = __HAL_CDE_PACK24(a,b);  
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack24 success\n\n");
	} 
	else
	{
		printfS("pack24 fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	a = 0xffffffff;
	b = 0xffffffff;
	result1 = PACK24(a,b);
	//result2 = __arm_cx3(0,a,b, 6);
	result2 = __HAL_CDE_PACK24(a,b);  
	printfS("a = %x  b = %x\n", a,b);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("pack24 success\n\n");
	} 
	else
	{
		printfS("pack24 fail xxxxxxxxxxxxxxxx\n\n");
	}	
	
	
}

void cde_umaq_test(void)
{
	
	uint32_t a,b,c;
	uint32_t result1,result2;
	
	b = 0;
	c = 0;
	result1 = UMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 7);
	result2 = __HAL_CDE_UMUAQ(b, c) ;    
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMUAQ success\n\n");
	} 
	else
	{
		printfS("UMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		

	b = 0x11223344;
	c = 0x55667788;
	result1 = UMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 7);
	result2 = __HAL_CDE_UMUAQ(b, c) ; 
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMUAQ success\n\n");
	} 
	else
	{
		printfS("UMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	b = 0x9900aabb;
	c = 0xccddeeff;
	result1 = UMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 7);
	result2 = __HAL_CDE_UMUAQ(b, c) ; 
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMUAQ success\n\n");
	} 
	else
	{
		printfS("UMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	b = 0xffffffff;
	c = 0xffffffff;
	result1 = UMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 7);
	result2 = __HAL_CDE_UMUAQ(b, c) ; 
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMUAQ success\n\n");
	} 
	else
	{
		printfS("UMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	a = 0;
	b = 0;
	c = 0;
	result1 = UMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 2);
	result2 = __HAL_CDE_UMLAQ(a,b,c) ;     
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMLAQ success\n\n");
	} 
	else
	{
		printfS("UMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	a = 0x99aabbcc;
	b = 0x11223344;
	c = 0x55667788;
	result1 = UMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 2);
	result2 = __HAL_CDE_UMLAQ(a,b,c) ;
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMLAQ success\n\n");
	} 
	else
	{
		printfS("UMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	a = 0x11223344;
	b = 0x9900aabb;
	c = 0xccddeeff;
	result1 = UMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 2);
	result2 = __HAL_CDE_UMLAQ(a,b,c) ;
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMLAQ success\n\n");
	} 
	else
	{
		printfS("UMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	a = 0xffffffff;
	b = 0xffffffff;
	c = 0xffffffff;
	result1 = UMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 2);
	result2 = __HAL_CDE_UMLAQ(a,b,c) ;
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("UMLAQ success\n\n");
	} 
	else
	{
		printfS("UMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	
	
}	


void cde_sumaq_test(void)
{
	
	uint32_t a,b,c;
	uint32_t result1,result2;
	
	b = 0;
	c = 0;
	result1 = SUMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 8);
	result2 = __HAL_CDE_SUMUAQ(b, c);    
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMUAQ success\n\n");
	} 
	else
	{
		printfS("SUMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		

	b = 0x11223344;
	c = 0x55667788;
	result1 = SUMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 8);
	result2 = __HAL_CDE_SUMUAQ(b, c); 
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMUAQ success\n\n");
	} 
	else
	{
		printfS("SUMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	b = 0x9900aabb;
	c = 0xccddeeff;
	result1 = SUMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 8);
	result2 = __HAL_CDE_SUMUAQ(b, c); 
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMUAQ success\n\n");
	} 
	else
	{
		printfS("UMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	b = 0xffffffff;
	c = 0xffffffff;
	result1 = SUMUAQ(b,c);
	//result2 = __arm_cx3(0,b,c, 8);
	result2 = __HAL_CDE_SUMUAQ(b, c); 
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMUAQ success\n\n");
	} 
	else
	{
		printfS("SUMUAQ fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	a = 0;
	b = 0;
	c = 0;
	result1 = SUMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 3);
	result2 = __HAL_CDE_SUMLAQ(a, b, c);     
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMLAQ success\n\n");
	} 
	else
	{
		printfS("UMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	a = 0x99aabbcc;
	b = 0x11223344;
	c = 0x55667788;
	result1 = SUMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 3);
	result2 = __HAL_CDE_SUMLAQ(a, b, c); 
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMLAQ success\n\n");
	} 
	else
	{
		printfS("UMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	a = 0x11223344;
	b = 0x9900aabb;
	c = 0xccddeeff;
	result1 = SUMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 3);
	result2 = __HAL_CDE_SUMLAQ(a, b, c); 
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMLAQ success\n\n");
	} 
	else
	{
		printfS("SUMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	a = 0xffffffff;
	b = 0xffffffff;
	c = 0xffffffff;
	result1 = SUMLAQ(a,b,c);
	//result2 = __arm_cx3a(0,a,b,c, 3);
	result2 = __HAL_CDE_SUMLAQ(a, b, c); 
	printfS("a= %x b = %x  c = %x\n",a,b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("SUMLAQ success\n\n");
	} 
	else
	{
		printfS("SUMLAQ fail xxxxxxxxxxxxxxxx\n\n");
	}			

	
	
}	

void cde_hammindist_test(void)
{
	
	uint32_t b,c;
	uint32_t result1,result2,result3;
	
	b = 0;
	c = 0;
	result1 = HAMMING_DIST(b,c);
	//result2 = __arm_cx3(0,b,c, 9);
	result2 = __HAL_CDE_HAMMING_DIST(b,c);    
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("HAMMING_DIST success\n\n");
	} 
	else
	{
		printfS("HAMMING_DIST fail xxxxxxxxxxxxxxxx\n\n");
	}		

	b = 0x11223344;
	c = 0x55667788;
	result1 = HAMMING_DIST(b,c);
	//result2 = __arm_cx3(0,b,c, 9);
    result2 = __HAL_CDE_HAMMING_DIST(b,c);
	//result3 = __arm_cx3(0,b,c, 9);
    result3 = __HAL_CDE_HAMMING_DIST(b,c);
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	printfS("result3 = %x\n", result3);
	if ((result1 == result2)&&(result2 == result3))
	{
		printfS("HAMMING_DIST success\n\n");
	} 
	else
	{
		printfS("HAMMING_DIST fail xxxxxxxxxxxxxxxx\n\n");
	}		
	
	b = 0x9900aabb;
	c = 0xccddeeff;
	result1 = HAMMING_DIST(b,c);
	//result2 = __arm_cx3(0,b,c, 9);
    result2 = __HAL_CDE_HAMMING_DIST(b,c);
	//result3 = __arm_cx3(0,b,c, 9);
    result3 = __HAL_CDE_HAMMING_DIST(b,c);
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	printfS("result3 = %x\n", result3);
	if ((result1 == result2)&&(result2 == result3))
	{
		printfS("HAMMING_DIST success\n\n");
	} 
	else
	{
		printfS("HAMMING_DIST fail xxxxxxxxxxxxxxxx\n\n");
	}		

	b = 0x5a5a5a5a;
	c = 0xa5a5a5a5;
	result1 = HAMMING_DIST(b,c);
	//result2 = __arm_cx3(0,b,c, 9);
    result2 = __HAL_CDE_HAMMING_DIST(b,c);
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("HAMMING_DIST success\n\n");
	} 
	else
	{
		printfS("HAMMING_DIST fail xxxxxxxxxxxxxxxx\n\n");
	}	

	b = 0x0;
	c = 0xffffffff;
	result1 = HAMMING_DIST(b,c);
	//result2 = __arm_cx3(0,b,c, 9);
    result2 = __HAL_CDE_HAMMING_DIST(b,c);
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("HAMMING_DIST success\n\n");
	} 
	else
	{
		printfS("HAMMING_DIST fail xxxxxxxxxxxxxxxx\n\n");
	}
	
	
	b = 0xffffffff;
	c = 0xffffffff;
	result1 = HAMMING_DIST(b,c);
	//result2 = __arm_cx3(0,b,c, 9);
    result2 = __HAL_CDE_HAMMING_DIST(b,c);
	printfS("b = %x  c = %x\n", b,c);
	printfS("result1 = %x\n", result1);
	printfS("result2 = %x\n", result2);
	if ((result1 == result2))
	{
		printfS("HAMMING_DIST success\n\n");
	} 
	else
	{
		printfS("HAMMING_DIST fail xxxxxxxxxxxxxxxx\n\n");
	}		

}	

uint32_t byte_reverse(uint32_t a) {
  uint32_t a1,a2,a3,a4;
	
	a1 = reverse_byte(a&0xff);
	a2 = reverse_byte((a>>8)&0xff);
	a3 = reverse_byte((a>>16)&0xff);
	a4 = reverse_byte((a>>24)&0xff);	
	
	a = a1 | (a2<<8) | (a3<<16) |(a4<<24);
  return a;
}

void byte_reverse_test(void)
{
    volatile uint32_t ret,ret1,ret2;
    uint32_t g_y=0;

    g_y = 0x11;
    ret =  __HAL_CDE_BYTE_REVERSE(g_y);  
    ret1 = byte_reverse(g_y);
    ret2 = __RBIT(g_y);
    ret2 = __REV(g_y);

    if((ret!=ret1)&&(ret!=ret2))
    {
        printf("error! xxxxxxxxxxxxxxxx\n\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte reverse is %x!\n\n",g_y,ret);

    g_y = 0x1122;
    ret =  __HAL_CDE_BYTE_REVERSE(g_y);  
    ret1 = byte_reverse(g_y);
    ret2 = __RBIT(g_y);
    ret2 = __REV(g_y);
    if((ret!=ret1)&&(ret!=ret2))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte reverse is %x!\n\n",g_y,ret);

    g_y = 0x112233;
    ret =  __HAL_CDE_BYTE_REVERSE(g_y);  
    ret1 = byte_reverse(g_y);
    ret2 = __RBIT(g_y);
    ret2 = __REV(g_y);
    if((ret!=ret1)&&(ret!=ret2))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte reverse is %x!\n\n",g_y,ret);

    g_y = 0x11223344;
    ret =  __HAL_CDE_BYTE_REVERSE(g_y);  
    ret1 = byte_reverse(g_y);
    ret2 = __RBIT(g_y);
    ret2 = __REV(g_y);
    if((ret!=ret1)&&(ret!=ret2))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte reverse is %x!\n\n",g_y,ret);			
}

void SM3_P0_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;

    g_y  = 0x11;
    ret  = __HAL_CDE_SM3_P0(g_y);  
    ret1 = SM3_P0(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P0 is %x!\n\n",g_y,ret);

    g_y = 0x1122;
    ret =  __HAL_CDE_SM3_P0(g_y);  
    ret1 = SM3_P0(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P0 is %x!\n\n",g_y,ret);

    g_y = 0x112233;
    ret =  __HAL_CDE_SM3_P0(g_y);  
    ret1 = SM3_P0(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P0 is %x!\n\n",g_y,ret);

    g_y = 0x11223344;
    ret =  __HAL_CDE_SM3_P0(g_y);  
    ret1 = SM3_P0(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P0 is %x!\n\n",g_y,ret);
}

void SM3_P1_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;

    g_y = 0x11;
    ret =  __HAL_CDE_SM3_P1(g_y);  
    ret1 = SM3_P1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P1 is %x!\n\n",g_y,ret);

    g_y = 0x1122;
    ret =  __HAL_CDE_SM3_P1(g_y);  
    ret1 = SM3_P1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P1 is %x!\n\n",g_y,ret);

    g_y = 0x112233;
    ret =  __HAL_CDE_SM3_P1(g_y);  
    ret1 = SM3_P1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P1 is %x!\n\n",g_y,ret);

    g_y = 0x11223344;
    ret =  __HAL_CDE_SM3_P1(g_y);  
    ret1 = SM3_P1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_P1 is %x!\n\n",g_y,ret);
}

void SM3_FF2_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_x=0;
    uint32_t g_y=0;
    uint32_t g_z=0;

    g_x = 0x11;
    g_y = 0x22;
    g_y = 0x33;
    ret =  __HAL_CDE_SM3_FF2(g_x,g_y,g_z);  
    ret1 = SM3_FF2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_FF2 is %x!\n\n",g_y,ret);


    g_x = 0x1122;
    g_y = 0x3344;
    g_y = 0x5566;
    ret =  __HAL_CDE_SM3_FF2(g_x,g_y,g_z);  
    ret1 = SM3_FF2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_FF2 is %x!\n\n",g_y,ret);

    g_x = 0x112233;
    g_y = 0x445566;
    g_y = 0x778899;
    ret =  __HAL_CDE_SM3_FF2(g_x,g_y,g_z);  
    ret1 = SM3_FF2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_FF2 is %x!\n\n",g_y,ret);

    g_x = 0x11223344;
    g_y = 0x55667788;
    g_y = 0x99aabbcc;
    ret =  __HAL_CDE_SM3_FF2(g_x,g_y,g_z);  
    ret1 = SM3_FF2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_FF2 is %x!\n\n",g_y,ret);
		
}

void SM3_GG2_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_x=0;
    uint32_t g_y=0;
    uint32_t g_z=0;

    g_x = 0x11;
    g_y = 0x22;
    g_y = 0x33;
    ret =  __HAL_CDE_SM3_GG2(g_x,g_y,g_z);  
    ret1 = SM3_GG2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_GG2 is %x!\n\n",g_y,ret);


    g_x = 0x1122;
    g_y = 0x3344;
    g_y = 0x5566;
    ret =  __HAL_CDE_SM3_GG2(g_x,g_y,g_z);  
    ret1 = SM3_GG2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_GG2 is %x!\n\n",g_y,ret);

    g_x = 0x112233;
    g_y = 0x445566;
    g_y = 0x778899;
    ret =  __HAL_CDE_SM3_FF2(g_x,g_y,g_z);  
    ret1 = SM3_FF2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_FF2 is %x!\n\n",g_y,ret);

    g_x = 0x11223344;
    g_y = 0x55667788;
    g_y = 0x99aabbcc;
    ret =  __HAL_CDE_SM3_GG2(g_x,g_y,g_z);  
    ret1 = SM3_GG2(g_x,g_y,g_z);  

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM3_GG2 is %x!\n\n",g_y,ret);
		
}

void SM4_T1_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;

    g_y = 0x11;
    ret =  __HAL_CDE_SM4_T1(g_y);  
    ret1 = SM4_T1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T1 is %x!\n\n",g_y,ret);

    g_y = 0x1122;
    ret =  __HAL_CDE_SM4_T1(g_y);  
    ret1 = SM4_T1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T1 is %x!\n\n",g_y,ret);

    g_y = 0x112233;
    ret =  __HAL_CDE_SM4_T1(g_y);  
    ret1 = SM4_T1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T1 is %x!\n\n",g_y,ret);

    g_y = 0x11223344;
    ret =  __HAL_CDE_SM4_T1(g_y);  
    ret1 = SM4_T1(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T1 is %x!\n\n",g_y,ret);
}
void SM4_T2_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;

    g_y = 0x11;
    ret =  __HAL_CDE_SM4_T2(g_y);  
    ret1 = SM4_T2(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T2 is %x!\n\n",g_y,ret);

    g_y = 0x1122;
    ret =  __HAL_CDE_SM4_T2(g_y);  
    ret1 = SM4_T2(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T2 is %x!\n\n",g_y,ret);

    g_y = 0x112233;
    ret =  __HAL_CDE_SM4_T2(g_y);  
    ret1 = SM4_T2(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T2 is %x!\n\n",g_y,ret);

    g_y = 0x11223344;
    ret =  __HAL_CDE_SM4_T2(g_y);  
    ret1 = SM4_T2(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x SM4_T2 is %x!\n\n",g_y,ret);
}

void cde_consistent_test(void)
{
    uint32_t a1,b1,c1;
    uint32_t a2,b2,c2;
    uint32_t a3,b3,c3;
    uint32_t result1,result2,result3;
    uint32_t cde_result1,cde_result2,cde_result3;

    a1 = 0x0;
    a2 = 0x55555555;
    a3 = 0xffffffff;
    result1 = __arm_cx1a(0,a1, 1);
    result2 = __arm_cx1a(0,a2, 1);
    result3 = __arm_cx1a(0,a3, 1);
    if((result1!=0) || (result2!=0x10) || (result3!=0x20))
    {
        printf("cx1a error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("cx1a success!\n");
    }

    b1 = 0;
    c1 = 0;
    result1 = UMUAQ(b1,c1);
    b2 = 0x9900aabb;
    c2 = 0xccdd1122;
    result2 = UMUAQ(b2,c2);
    b3 = 0xffffffff;
    c3 = 0xffffffff;
    result3 = UMUAQ(b3,c3);

    cde_result1 = __arm_cx3(0,0,0, 7);
    cde_result2 = __arm_cx3(0,0x9900aabb,0xccdd1122, 7);	
    cde_result3 = __arm_cx3(0,0xffffffff,0xffffffff, 7);
    if((result1!=cde_result1) || (result2!=cde_result2) || (result3!=cde_result3))
    {
        printf("cx3 error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("cx3 success!\n");
    }

    a1 = 0;
    b1 = 0;
    c1 = 0;
    result1 = UMLAQ(a1,b1,c1);

    a2 = 0x11223344;	
    b2 = 0x9900aabb;
    c2 = 0xccdd1122;
    result2 = UMLAQ(a2,b2,c2);

    a3 = 0xffffffff;	
    b3 = 0xffffffff;
    c3 = 0xffffffff;
    result3 = UMLAQ(a3,b3,c3);

    cde_result1 = __arm_cx3a(0,0,0,0, 2);
    cde_result2 = __arm_cx3a(0,0x11223344,0x9900aabb,0xccdd1122, 2);	
    cde_result3 = __arm_cx3a(0,0xffffffff,0xffffffff,0xffffffff, 2);
    if((result1!=cde_result1) || (result2!=cde_result2) || (result3!=cde_result3))
    {
        printf("cx3a error! xxxxxxxxxxxxxxxx\n\n");
    }
    else
    {
        printf("cx3a success!\n\n");
    }
	
}


uint32_t popcnt_loop(int count)
{
    uint32_t i=0,ret=0;
    
    for(;i<count;i++)
    {
        //ret += test_input[i];        
        ret += __builtin_clz(test_input[i]); 
        //ret += __HAL_CDE_POPCNT(test_input[i]);  
    } 
    return ret;
}

int popcnt_time_test(void)
{
    uint32_t ret;
    uint32_t tick1,tick2;
    memset(test_input,0x55,sizeof(test_input));
    tick1 = HAL_DWT_GetClkTick();
    ret = popcnt_loop(256);
    tick2 = HAL_DWT_GetClkTick();
    printfS("tick1=%d,tick2=%d,tick2-tick1=%d\r\n", tick1,tick2, tick2-tick1);
    printfS("%f clks per loop,ret = %d\r\n",((double)(tick2-tick1))/256,ret);

    return ret;
 
}



void builtin_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;
    
    g_y = 0x00;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);   

    g_y = 0x11;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);
    
    g_y = 0x1122;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);

    g_y = 0x80000000;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);    
}

void cde_parity_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;
    
    g_y = 0x11;
    ret =  __HAL_CDE_PARITY(g_y);  
    ret1 = __builtin_parity(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_parity is %x!\n\n",g_y,ret);   
    
    g_y = 0x23;
    ret =  __HAL_CDE_PARITY(g_y);  
    ret1 = __builtin_parity(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_parity is %x!\n\n",g_y,ret);
    
   
    g_y = 0xffff;
    ret =  __HAL_CDE_PARITY(g_y);  
    ret1 = __builtin_parity(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_parity is %x!\n\n",g_y,ret);
}

void cde_ctz_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;
    
    g_y = 0x00;
    ret =  __HAL_CDE_CTZ(g_y);  
    ret1 = __builtin_ctz(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ctz is %x!\n\n",g_y,ret);   
    
    g_y = 0x1122;
    ret =  __HAL_CDE_CTZ(g_y);  
    ret1 = __builtin_ctz(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ctz is %x!\n\n",g_y,ret);
    
   
    g_y = 0x80000000;
    ret =  __HAL_CDE_CTZ(g_y);  
    ret1 = __builtin_ctz(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ctz is %x!\n\n",g_y,ret);
}

/*
   from back to front, which position of bit 1 in a data value.
   For example, 0x00      : 32
				0x11      : 1
	            0x1122	  : 2
                0x80000000: 32
refrence C-code:
__builtin_ffs

*/
#define __HAL_CDE_FFS(a)       __arm_cx2a(0, a, 1, 3)

void cde_ffs_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;
    
    g_y = 0x00;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);  
    
    g_y = 0x11;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);  
    
    g_y = 0x1122;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);  
    
    g_y = 0x80000000;
    ret =  __HAL_CDE_FFS(g_y);  
    ret1 = __builtin_ffs(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x __builtin_ffs is %x!\n\n",g_y,ret);  
}

uint16_t reverse_u16(uint16_t in_data)
{
	uint16_t temp;
	uint16_t i;

	temp = 0;
	for(i = 0; i < 16; i++)
	{
		if(in_data & (1 << i))
		{
			temp |= 0x8000 >> i;
		}
	}

	return temp;
}

uint32_t u16_reverse(uint32_t a) {
  uint32_t a1,a2;
	a1 = reverse_u16(a&0xffff);
	a2 = reverse_u16((a>>16)&0xffff);
	a = a1 | (a2<<16);
  return a;
}

void cde_u16_reverse_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;
    
    g_y = 0x11;
    ret =  __HAL_CDE_U16_REVERSE(g_y);  
    ret1 = u16_reverse(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x reverse_u16 is %x!\n\n",g_y,ret);  
    
    g_y = 0x1122;
    ret =  __HAL_CDE_U16_REVERSE(g_y);  
    ret1 = u16_reverse(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x reverse_u16 is %x!\n\n",g_y,ret);  
    
    g_y = 0x112233;
    ret =  __HAL_CDE_U16_REVERSE(g_y);  
    ret1 = u16_reverse(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x reverse_u16 is %x!\n\n",g_y,ret);  
    
    g_y = 0x11223344;
    ret =  __HAL_CDE_U16_REVERSE(g_y);  
    ret1 = u16_reverse(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x reverse_u16 is %x!\n\n",g_y,ret);  
}

uint32_t byte_amp2(uint8_t a) 
{
  uint32_t a1,a2;
	uint32_t b=0;
	a1 = a&0x1;
	a1 |= a1<<1;
	b |= a1;

	a1 = a&0x2;
	a1 |= a1<<1;
	b |= a1<<1;

	a1 = a&0x4;
	a1 |= a1<<1;
	b |= a1<<2;

	a1 = a&0x8;
	a1 |= a1<<1;
	b |= a1<<3;	

	a1 = a&0x10;
	a1 |= a1<<1;
	b |= a1<<4;	
	
	a1 = a&0x20;
	a1 |= a1<<1;
	b |= a1<<5;

	a1 = a&0x40;
	a1 |= a1<<1;
	b |= a1<<6;

	a1 = a&0x80;
	a1 |= a1<<1;
	b |= a1<<7;	
	
  return b;
}

uint32_t byte_amp3(uint8_t a) 
{
  uint32_t a1,a2;
	uint32_t b=0;
	a1 = a&0x1;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1;

	a1 = a&0x2;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<2;

	a1 = a&0x4;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<4;

	a1 = a&0x8;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<6;	

	a1 = a&0x10;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<8;	
	
	a1 = a&0x20;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<10;

	a1 = a&0x40;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<12;

	a1 = a&0x80;
	a1 |= (a1<<1) | (a1<<2);
	b |= a1<<14;	
	
  return b;
}

uint32_t byte_amp4(uint8_t a) 
{
  uint32_t a1,a2;
	uint32_t b=0;
	a1 = a&0x1;
	a1 |= (a1<<1) | (a1<<2)  | (a1<<3);
	b |= a1;

	a1 = a&0x2;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<3;

	a1 = a&0x4;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<6;

	a1 = a&0x8;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<9;	

	a1 = a&0x10;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<12;	
	
	a1 = a&0x20;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<15;

	a1 = a&0x40;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<18;

	a1 = a&0x80;
	a1 |= (a1<<1) | (a1<<2)| (a1<<3);
	b |= a1<<21;	
	
  return b;
}

void cde_byte_amp_test(void)
{
    volatile uint32_t ret,ret1;
    uint32_t g_y=0;
    
    g_y = 0x6b;  //bit[7:0] = 01101011
    ret =  __HAL_CDE_BYTE_AMP(g_y,1);   
    ret1 = byte_amp2(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte_amp2 is %x!\n\n",g_y,ret);

    g_y = 0x6b;  //bit[7:0] = 01101011
    ret =  __HAL_CDE_BYTE_AMP(g_y,2);   
    ret1 = byte_amp3(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte_amp3 is %x!\n\n",g_y,ret); 
    
    
    g_y = 0x6b;  //bit[7:0] = 01101011
    ret =  __HAL_CDE_BYTE_AMP(g_y,4);   
    ret1 = byte_amp4(g_y);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x byte_amp4 is %x!\n\n",g_y,ret); 
}

uint8_t BYTE_PACKN(uint8_t a, uint8_t b, uint8_t n) {
  return (a >> n) | (b << (8-n));
}

uint32_t BYTE3_PACKN(uint32_t a, uint8_t n) {
	uint8_t c,c1,c2,c3;
	uint8_t a1,a2,a3,a4;
	a1 = (a&0xff);
	a2 = (a&0xff00)>>8;
	a3 = (a&0xff0000)>>16;
	a4 = (a&0xff000000)>>24;
	c1 = (a1 >> n) | (a2 << (8-n));
	c2 = (a2 >> n) | (a3 << (8-n));
	c3 = (a3 >> n) | (a4 << (8-n));
	c = (c1&0xff) | ((c2<<8)&0xff00) | ((c3<<16)&0xff0000);
  return c;
}

uint64_t BYTE7_PACKN(uint64_t a, uint8_t n) {
	uint64_t c,c1,c2,c3,c4,c5,c6,c7;
	uint8_t a1,a2,a3,a4,a5,a6,a7,a8;
	a1 = (a&0xff);
	a2 = (a&0xff00)>>8;
	a3 = (a&0xff0000)>>16;
	a4 = (a&0xff000000)>>24;
	a5 = (a&0xff00000000)>>32;
	a6 = (a&0xff0000000000)>>40;
	a7 = (a&0xff000000000000)>>48;
	a8 = (a&0xff00000000000000)>>56;
	c1 = (a1 >> n) | (a2 << (8-n));
	c2 = (a2 >> n) | (a3 << (8-n));
	c3 = (a3 >> n) | (a4 << (8-n));
	c4 = (a4 >> n) | (a5 << (8-n)); 
	c5 = (a5 >> n) | (a6 << (8-n));
	c6 = (a6 >> n) | (a7 << (8-n));
	c7 = (a7 >> n) | (a8 << (8-n));	
	c = (c1&0xff) | ((c2<<8)&0xff00) | ((c3<<16)&0xff0000) | ((c4<<24)&0xff000000) | ((c5<<32)&0xff00000000) | ((c6<<40)&0xff000000000) | ((c7<<48)&0xff000000000000);
  return c;
}

void cde_byte_pack_test(void)
{
    volatile uint32_t ret,ret1;
	uint8_t a,b,n;
    
	a = 0;
	b = 0;
	n = 0;
    ret =  __HAL_CDE_BYTE_PACKN(a,b,n);  
    ret1 = BYTE_PACKN(a,b,n);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x BYTE_PACKN is %x!\n\n",a,ret);  
    
	a = 0x11;
	b = 0x22;
	n = 2;
    ret =  __HAL_CDE_BYTE_PACKN(a,b,n);  
    ret1 = BYTE_PACKN(a,b,n);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x BYTE_PACKN is %x!\n\n",a,ret); 
    
	a = 0x34;
	b = 0x56;
	n = 4;
    ret =  __HAL_CDE_BYTE_PACKN(a,b,n);  
    ret1 = BYTE_PACKN(a,b,n);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x BYTE_PACKN is %x!\n\n",a,ret); 
    
	a = 0x78;
	b = 0x9a;
	n = 8;
    ret =  __HAL_CDE_BYTE_PACKN(a,b,n);  
    ret1 = BYTE_PACKN(a,b,n);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x BYTE_PACKN is %x!\n\n",a,ret); 
    
	a = 0x78;
	n = 3;
    ret =  __HAL_CDE_3BYTE_PACKN(a,n);  
    ret1 = BYTE3_PACKN(a,n);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x BYTE3_PACKN is %x!\n\n",a,ret);    
    
	a = 0x78;
	n = 2;
    ret =  __HAL_CDE_7BYTE_PACKN(a,n);  
    ret1 = BYTE7_PACKN(a,n);

    if((ret!=ret1))
    {
        printf("error! xxxxxxxxxxxxxxxx\n");
    }
    else
    {
        printf("success!\n");
    }
    printf("%x BYTE7_PACKN is %x!\n\n",a,ret);     
    
}


void APP_CDE_Test(void)
{
    HAL_CDE_EnableCPx(0);
    //HAL_CDE_DisableCPx(0);
 
    printf("test start...\r\n");
    
    popcnt_test();
    byte_reverse_test();
    SM3_P0_test();
    SM3_P1_test();
    SM3_FF2_test();
    SM3_GG2_test();	
    SM4_T1_test();
    SM4_T2_test();	
    cde_crc32_test();
    cde_pack_test();
    cde_umaq_test();
    cde_sumaq_test();	
    cde_hammindist_test();
    cde_consistent_test(); 
    
    cde_parity_test();
    cde_ctz_test();
    cde_ffs_test();	
    cde_u16_reverse_test();		
    cde_byte_amp_test();
    cde_byte_pack_test();
    
    printf("CDE test over\r\n");

}

