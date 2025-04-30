#ifndef __BMP_MAP_H__
#define __BMP_MAP_H__



//#define DOWNLOAD_PIC_EN



#define ADDR2STR_(str)							#str
#define ADDR2STR(str)							ADDR2STR_(str)
#define PRE_ADDR_STR							.ARM.__at_
#define LOCATION_ADDR(ADDR)						ADDR2STR(PRE_ADDR_STR) ADDR2STR(ADDR)
#define LOAD2SECTION_ADDR_(ADDR)	 			__attribute__((section(""LOCATION_ADDR(ADDR)"")))
#define LOAD2SECTION_ADDR(ADDR)	 				LOAD2SECTION_ADDR_(ADDR)



//code spi flash capacity: 4MBytes
#define RAW_PIC_BASE_ADDR						(SPI7_MEM_BASE_ADDR+(1024*512))
#define RAW_PIC1_BASE_ADDR						0x0800C000//(RAW_PIC_BASE_ADDR)
#define RAW_PIC2_BASE_ADDR						0x08183000//(RAW_PIC1_BASE_ADDR+(TEST_L1_AREA_WIDTH*TEST_L2_AREA_HEIGHT*4))


#endif
