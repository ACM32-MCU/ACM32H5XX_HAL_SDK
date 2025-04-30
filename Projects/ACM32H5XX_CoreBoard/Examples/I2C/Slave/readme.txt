Demo：	I2C Slave示例
功能：	本示例含三个功能：
		TEST_SLAVE_POLLING：    使用Loop方式，作为I2C从机和另一方的I2C主机（使用I2C Master工程）进行主从数据收发通信，
		TEST_SLAVE_INTERRUPT： 使用中断方式，作为I2C从机和另一方的I2C主机（使用I2C Master工程）进行主从数据收发通信，
		TEST_SLAVE_DMA：          使用DMA方式，作为I2C从机和另一方的I2C主机（使用I2C Master工程）进行主从数据收发通信，
		需要注意SCL和SDA需要外接上拉电阻到VCC。

		
		