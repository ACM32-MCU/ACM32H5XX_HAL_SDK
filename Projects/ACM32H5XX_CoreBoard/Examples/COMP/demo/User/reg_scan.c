
/******************************************************************************
*@file  : reg_scan.c
*@brief : reg scan
*@ver   : 1.0.0
*@date  : 2022.10.20
******************************************************************************/

#include "reg_scan.h"   

//读出寄存器值，与期望值进行比较
uint8_t reg_default_compare(uint32_t addr, uint32_t cdata)
{
	uint32_t rdata;
	rdata = (*(volatile uint32_t *)addr); //读出数据
	if(rdata == cdata)
	{
		printf("reg default pass :addr-0x%08x = rdata-0x%08x\r\n", addr, rdata);
		return 0;
	}
	else
	{
		printf("reg default error:addr-0x%08x\r\n\t\t = rdata-0x%08x\r\n\t\t ? cdata-0x%08x\r\n", addr, rdata, cdata);
		return 1;
	}
}

//读出寄存器缺省值，与期望值进行比较，掩码位不比较
uint8_t reg_default_compare_mask(uint32_t addr, uint32_t mdata, uint32_t cdata)
{
	uint32_t rdata,mdata2;
    mdata2 = ~mdata;
	rdata = (*(volatile uint32_t *)addr); //读出数据
	if((rdata & mdata2) == (cdata & mdata2))
	{
		printf("reg default pass :addr-0x%08x (mdata-0x%08x) = rdata-0x%08x\r\n", addr, mdata, rdata);
		return 0;
	}
	else
	{
		printf("reg default error:addr-0x%08x (mdata-0x%08x)\r\n\t\t = rdata-0x%08x/0x%08x\r\n\t\t ? cdata-0x%08x/0x%08x\r\n", addr,mdata, rdata,rdata & mdata2, cdata,cdata & mdata2);
		return 1;
	}
}


//先写入寄存器值，再读出与期望值进行比较
uint8_t reg_write_compare(uint32_t addr, uint32_t wdata, uint32_t cdata)
{
	uint32_t rdata;
	(*(volatile uint32_t *)addr) = wdata; //写入数据

	rdata = (*(volatile uint32_t *)addr); //读出数据
	if(rdata == cdata)
	{
		printf("reg write pass : addr-0x%08x = wdata-0x%08x : rdata-0x%08x\r\n", addr, wdata, rdata);
		return 0;
	}
	else
	{
		printf("reg write error: addr-0x%08x\r\n\t\t = wdata-0x%08x\r\n\t\t : rdata-0x%08x\r\n\t\t ? cdata-0x%08x\r\n", addr, wdata, rdata, cdata);
		return 1;
	}
}

//先写入寄存器值，再读出与期望值进行比较，掩码位不写入和比较
uint8_t reg_write_compare_mask(uint32_t addr, uint32_t mdata, uint32_t wdata, uint32_t cdata)
{
	uint32_t rdata,m_wdata,m_rdata,m_cdata;
    
	m_wdata = (*(volatile uint32_t *)addr); //先读出mask位的数据
    m_wdata = (m_wdata & mdata) | (wdata & (~mdata)); //组合    
	(*(volatile uint32_t *)addr) = m_wdata; //写入mask后的数据

	rdata = (*(volatile uint32_t *)addr); //读出数据
    m_rdata = rdata & (~mdata);
    
    m_cdata = cdata & (~mdata);
    
	if(m_rdata == m_cdata)
	{
		//printfS("addr write pass :0x%8x = w-0x%08x : r-0x%08x\r\n", addr, wdata, m_wdata, rdata,  m_rdata, cdata, m_cdata);
        printf("reg mask write pass :addr-0x%08x (mdata-0x%08x) = wdata-0x%08x/0x%08x : rdata-0x%08x/0x%08x\r\n", addr, mdata, wdata, m_wdata, rdata,  m_rdata);
		return 0;
	}
	else
	{
		printf("reg mask write error:addr-0x%08x (mdata-0x%08x)\r\n\t\t = wdata-0x%08x/0x%08x\r\n\t\t : rdata-0x%08x/0x%08x\r\n\t\t ? cdata-0x%08x/0x%08x\r\n", addr, mdata, wdata, m_wdata, rdata,  m_rdata, cdata, m_cdata);
		return 1;
	}
}

