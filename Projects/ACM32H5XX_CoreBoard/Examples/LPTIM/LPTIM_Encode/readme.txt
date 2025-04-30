Demo：	LPTIM_Encode
功能：	编码器工作原理。
	LPTIM1时钟源PCLK，不分频，软件触发，外部计数时钟，立即更新。
	使能UP中断，使能DOWN中断。
	用两个输出GPIO模拟编码器输出，循环更改方向。
	当UP中断（计数方向从递减变为递增），LED亮，输出LOG：+。
	当DOWN中断（计数方向从递增变为递减），LED灭，输出LOG：-。
