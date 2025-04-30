Demo：	DAC示例
功能：	示例中可选配置：
		DAC_Output_Noise：通过DAC1输出白噪声，其中DAC1通道1通过软件方式触发，通道2通过TIM6来触发
		DAC_Output_SAWTOOTH：通过DAC1输出锯齿波
		DAC_OutPut_Voltage： 通过DAC1输出固定电压。
		DAC_OutPut_Sinx：通过TIM6定时器的update中断，周期性的通过DAC1输出不同值的固定电压的方式来产生正弦波
		DAC_DMA_Output_Sinx：输出正弦波。通过DMA双通道模式，1个32bit数据对应DAC的两个通道。通过TIM6级联触发DMA请求   的方式，将正弦波							                                      数值表中的正弦波数值通过DMA方式搬动到DAC1的两个通道DOR寄存器
		DAC_DMADUALDATA_Output_Sinx：输出正弦波。DMA双数据模式，一个32bit数据（两个12bit数据，DHRXX1或DHRXX2），依次给DAC一个通道使用
		