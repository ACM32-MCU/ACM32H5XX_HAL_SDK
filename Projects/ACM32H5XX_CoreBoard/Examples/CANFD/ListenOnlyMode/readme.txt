Demo：	FDCAN listen only mode(LOM监听模式)示例
功能：	FDCAN LOM模式用于监听CAN总线，在LOM模式下的节点①不能发送帧，②对其他节点发送的帧不会响应ACK。
		基于以上原因，运行该demo，需要三个节点，其中两个节点为正常模式，一个节点为LOM模式。

		app_test中可选配置：
		TEST_LOM：			可以接收到帧(该帧需要被另一个正常节点响应ACK)，但不能发送帧，不响应ACK
		TEST_LOM_LBME：		可以接收到帧(该帧需要被另一个正常节点响应ACK)，可以发送帧，不响应ACK
				