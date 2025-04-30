ITCM：共32KB，存放中断向量表，中断入口函数（例子中acm32h5xx_it.c中的代码）以及其他需要高效处理且执行周期要比较固定的代码。systeminit函数会使能ITCM。ITCM不会被Cache。
DTCM：共32KB, 用于堆栈空间，以及需要快速处理的大块数据（例子中app_data.c中的数组)以及全局变量(非连续访问的全局变量建议放到DTCM或SRAM)。reset_handler一开始就用汇编使能了DTCM。DTCM不会被Cache。
ICACHE：共16KB，指令Cache，第一次取指需要从外部存储器读到ICache，后续只需要从ICache读，因此同一段代码，第一次执行和第二次执行的时间是不同的。INS_ACCELERATE_ENABLE宏控制ICACHE使能开关。
DCACHE：共16KB，数据缓存，适用于连续读写，或者对一块数据多次读写。示例中对比了使能和禁止DCache的情况下，读写128次8KB数据的时间。DCacheDATA_ACCELERATE_ENABLE为控制DCACHE使能的开关。
使能DCACHE后，如果使用DMA访问数据，需要将DMA访问的数据区设置为不可Cache。     
PSRAM: 外部存储器，连续读写/同一块地址多次读写时可以让该区域可CACHE，单次非连续读写的场景可以通过MPU将该区域配置成不可Cache。
SDRAM：BGA176封装合封1片16BIT的SDRAM；BGA240封装合封两片16BIT的SDRAM，可以作为32BIT接口使用。使能MPU时，将SDRAM前1MB配置为可Cache用于执行程序或数据访问；后面空间作为数据访问，如果使能DMA和DCACHE。
       就需要禁止这部分地址的Cache。
MPU：配置存储区的Cache属性、读写权限、执行权限。     

示例中使用了MDK-ARM下的Project.sct。整个程序文件下载在0x08002000开始的SPI Flash空间中，acm32h5xx_it中的中断入口函数放在ITCM 0x800开始的空间，0x00开始的ITCM用于放中断向量表。
堆栈空间放在DTCM，需要保证快速访问或DMA访问的数据放在DTCM(app_data.c中定义的全局变量)，其他全局变量放在0x20008000开始的SRAM。   
LR_IROM1 0x08002000 0x00020000  {    ; load region size_region
  ER_IROM1 0x08002000 0x00020000  {  ; load address = execution address
   *.o (RESET, +First)
   *(InRoot$$Sections)
   .ANY (+RO)
   .ANY (+XO)
  }
  RW_IRAM1 0x20000000 0x00008000  {  
    startup_acm32h5xx.o (+ZI)  ; Stack
    app_data.o (+RW +ZI)       ; RW-ZI Fast access data
  }
  RW_IRAM2 0x20008000 0x00058000  {
   .ANY (+RW +ZI)
  }
  RW_IRAM3 0x800 0x7800  {
   acm32h5xx_it.o (+RO +XO)
  }
}

MPU分了8块区域：
区域1：0x00000000,   0x08080000-1  // SPI Flash 代码区，ITCM使能前的ROM区，配置为可执行，可Cache，Cache方式为Write-Through方式。用于存放代码。
区域2：0x08080000,   0x20000000-1  // SPI Flash 数据区，ROM区，配置为可执行，不可Cache，用于存放SPI Flash区数据。
区域3：0x20000000，  0x20058000-1  // SRAM区，可执行，可Cache，Write-Back方式，可用于存放代码和数据，DCACHE后不可用于DMA访问。
区域4：0x20058000，  0x20060000-1  // SRAM区，可执行，不可Cache，当DCACHE开启后，可用于DMA访问。
区域5：0x40000000，  0x60000000-1  // Device区，为外设地址空间，不可执行，不可Cache，必须对齐访问。
区域6：0x60000000，  0x70100000-1  // 1M 外部RAM区，可执行，可Cache，可用于存放代码和数据，DCACHE后不可用于DMA访问。 
区域7：0x70100000，  0xA0000000-1  // 外部RAM区，不可执行，不可Cache，可用于存放数据，DCACHE后可用于DMA访问。 
区域8：0xA0000000，  0xFFFFFFFF    // 系统区，不可执行，不可Cache，为系统寄存器地址空间。 





