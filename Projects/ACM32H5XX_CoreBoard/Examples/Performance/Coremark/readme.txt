Coremark应用示例工程：


1、Keil MDK-ARM工程：Score (Coremarks/MHz):   3.37
	编译环境：KeilV5.33，ARM Compiler V6.15
	工程配置：Optimizations[-ofast]，close link-time optimization

	程序配置：
	系统默认打开指令、数据加速器，主频为220MHZ，IRAM1选择0x20038000
	使用DWT计时
	
1、IAR EWARM工程：Score (Coremarks/MHz):   3.90
	编译环境：IAR8.50.9.33462
	工程配置：Optimizations[High-SPeed-No Size constraints]

	程序配置：
	系统默认打开指令、数据加速器，主频为220MHZ，IRAM1选择0x20038000
	使用DWT计时




    

	