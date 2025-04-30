Demo：	LBMI(Loop back mode internal)示例
功能：	LBMI 用于自测。 运行该demo不需要接CAN收发器。LBMI模式下的节点通过模块内部将CAN_TX短接到CAN_RX，同时会产生ACK（self-ACK）。因此可以接收到自己发送的帧。