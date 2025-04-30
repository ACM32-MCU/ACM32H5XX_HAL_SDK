ADC应用示例工程：

示例demo提供了几个测试项，根据Test_mode_t枚举选择特定的功能：
    Test_ADC_Regular：规则模式下使用轮询方式采集ADC1所有通道
    Test_ADC_Inject：注入模式下使用轮询方式采集ADC2 CH4、CH14、CH5、CH5四个通道
    Test_ADC_Interrupt：规则模式下使用中断方式采集ADC3 CH15通道
    Test_ADC_External_Trigger：注入模式下使用外部触发（EXTI_LINE15 PB15）方式采集ADC1 CH1通道
    Test_ADC_DMA：规则模式下使用DMA方式（DMA2  CHANNEL1）采集CH10通道
    Test_ADC_AnalogWatchdog：在所有规则通道上使用模拟开门狗，低阈值为0x200，高阈值为0xB00
    Test_ADC_Dual：双ADC模式下混合的规则同步+注入同步模式 
    Test_ADC_TemperatureSensor: 采集温度传感器通道
	Test_ADC_Regular_ContinuousMode: 规则模式下使用轮询方式连续采集ADC1所有通道
    
使用流程：
1、参考电源VREFP来源选择：VREF+或VREFBUF；HAL_ADC_VrefpConfig()
2、出厂基本参数设置：相关指标的TRIM; HAL_ADC_FactoryBasicParameterConfig()
3、选择是否进行校准（硬件校准或软件校准）；ADC_Calibration()
4、ADC配置

	