Demo：	LBME_SACK (Loop back mode external) 配合Self-ACK示例
功能：	LBME_SACK 用于自测。 运行该demo需要将CAN_TX, CAN_RX接CAN收发器（可以只接收发器，收发器未挂接在总线上）。	通过CAN收发器，LBME的节点可以接收到自己发送出去的帧。如果收发器的CANH,CANL挂接在CAN总线上，其他节点也会收到帧。