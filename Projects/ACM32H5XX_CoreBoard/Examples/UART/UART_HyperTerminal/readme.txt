Demo：	UART HyperTerminal通信示例
功能：	UART和上位机串口工具进行数据收发。 通过上位机先行发送数据，UART收到后返回接收到的数据。
		默认使用 UART1 TXD:PA9 , RXD:PA10， LOOP方式收发。
		也可选择TEST_UART1_IT，进行中断方式的收发；TEST_UART1_DMA，进行DMA方式收发