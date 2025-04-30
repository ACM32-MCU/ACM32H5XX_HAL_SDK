Demo：	LPTIM_Counter
功能：	LPTIM1计数。
	LPTIM1时钟源XTL，32.768KHz。不分频，软件触发，内部计数时钟，立即更新。
	周期1000ms。
	使能重装载匹配中断，使能重复计数器下溢事件中断。
	连续计数。
	重装载匹配中断，每1000ms中断一次，输出LOG:LPTIM1 AutoReloadMatch。
	重复计数器下溢事件中断，第一次1000ms中断一次，之后每9000ms中断一次，输出LOG:LPTIM1 RepetitionCounter。
	