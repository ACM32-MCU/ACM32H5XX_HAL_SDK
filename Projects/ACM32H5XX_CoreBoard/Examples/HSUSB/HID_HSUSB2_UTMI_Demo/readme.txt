本例程使用HSUSB2的UTMI接口，展示了HID Demo。运行Tool下的hid_test.bat，主机将会发送HID report，然后接收HID report，然后对比发送和
接收的数据中除了report ID以外的数据是否完全一致。
本例程运行在片外的SPI Flash中，程序起始地址为0x08002000,如果需要修改此地址，请修改VECT_TAB_ADDR的定义。
中断端点的最大包为64字节，对于高速模式，最大包也可以为512字节。
本Demo能通过USB第九章测试，也可用于眼图测试。
默认情况下，USB Log关闭，为了调试，可以打开USB Log，只需要打开宏定义USB_DEBUG。