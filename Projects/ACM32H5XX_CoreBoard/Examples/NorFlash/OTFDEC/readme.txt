该demo展示了OTFDEC使能后程序的运行。通过ROMAPI中的加密写和XIP读方式，可以将密文写入以及将密文解密读出。
步骤如下：
a,通过上位机配置efuse来使能OTFDEC并且采用userkey后复位：选择写efuse功能，在0x114地址写入4B3C，在0x110地址写入513F，可以通过读efuse来确认efuse写入的数据。
b,将程序加密下载到SPI Flash中，设置的解密密钥需要和demo中的key一致，设置的start address 和end address 需要包含demo中读写数据的空间。start address = 0x08002000, end address =  0817FFFF
  user key输入为11223344556677889900AABBCCDDEEFF。
c,程序复位运行，调试器不能处于连接状态。
d,程序展示了三块SPI Flash区域的访问：前8KB的明文区，和应用程序共享密钥的加密区1，独立的加密区2。   

