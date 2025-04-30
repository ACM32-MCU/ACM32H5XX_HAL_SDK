Demo：	I2C Master示例
功能：	本示例含三个功能：
		TEST_MASTER_COMM：使用Loop方式，作为I2C主机和另一方的I2C从机（使用I2C Slave工程）进行主从数据收发通信，
		TEST_MASTER_IT：        使用中断方式，作为I2C主机和另一方的I2C从机（使用I2C Slave工程）进行主从数据收发通信，
		TEST_MASTER_EEPROM：需要外接I2C EEPROM，示例工程中使用AT24LC256或AT24C02，可在ic_at24.h中#define相关宏参数。
		需要注意SCL和SDA需要外接上拉电阻到VCC。

		
		