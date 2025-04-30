
SDRAM速度测试示例工程：

	采用叠封SDRAM的型号进行测试，比如BGA240芯片
	
	代码在SPIFLASH中运行，优化等级O1，ICACHE使能
	
	CPU 采用FOR循环方式，读写16位宽SDRAM、32位宽SDRAM。
	SDRAM的位宽通过芯片型号来进行配置。
	
	分别测试8bit，16bit，32bit位宽的读写速度。
	
	采用连续读写（每次循环地址加1）和跳地址读写（每次循环地址加64）
	
	分别测试使能DCACHE和禁止DCACHE情况下的速度。	
	通过配置acm32h5xx_hal_conf.h文件中的 DATA_ACCELERATE_ENABLE 宏定义，使能和禁止DCACHE

