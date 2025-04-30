本例程使用HSUSB2的UTMI接口，bulk 传输 Demo, 使用航芯专有的USB驱动。
本例程运行在片外的SPI Flash中，程序起始地址为0x08002000,如果需要修改此地址，请修改VECT_TAB_ADDR的定义。
本示例中USB的收发采用DMA方式，因此需要将DCache关闭，否则数据虽然写到了SRAM中，但CPU认为数据无变化，因此不更新DCache中的内容。
实测证明，使用DMA的方式，DMA方式下的TX FIFO使用量要大于CPU方式，因此给EP1的TX FIFO分频了比CPU方式更大的空间。    