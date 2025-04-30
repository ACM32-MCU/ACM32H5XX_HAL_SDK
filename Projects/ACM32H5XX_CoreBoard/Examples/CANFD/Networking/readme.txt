Demo：	FDCAN Networking示例
功能：	该mode用于和CAN总线上的其他节点进行CAN帧的收发通信。
		需要连接CAN收发器(支持CAN FD)到总线上与另外一个或多个CAN FD节点相连。
		
		1、系统时钟（CAN模块时钟）配置成80MHz，仲裁段/数据段比特速率为250K/2M
		2、系统时钟需要使用PLL外部晶体的方式，因为CAN的时钟对精度要求比较高。