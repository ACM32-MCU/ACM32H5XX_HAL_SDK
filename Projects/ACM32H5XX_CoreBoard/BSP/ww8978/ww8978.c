#include "acm32h5xx_coreboard.h"
#include "ww8978.h"
#include "string.h"
#include "stdio.h"

static uint16_t g_reg_tab[58]=
{
	0x0000, 0x0000, 0x0000, 0x0000, 0x0050, 0x0000, 0x0140, 0x0000,
	0x0000, 0x0000, 0x0000, 0x00FF, 0x00FF, 0x0000, 0x0100, 0x00FF,
	0x00FF, 0x0000, 0x012C, 0x002C, 0x002C, 0x002C, 0x002C, 0x0000,
	0x0032, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0038, 0x000B, 0x0032, 0x0000, 0x0008, 0x000C, 0x0093, 0x00E9,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0003, 0x0010, 0x0010, 0x0100,
	0x0100, 0x0002, 0x0001, 0x0001, 0x0039, 0x0039, 0x0039, 0x0039,
	0x0001,0x0001
};

uint32_t BSP_WM8978_Init(uint32_t mode)
{
	BSP_WM8978_Write_Reg(0,0);	                        // 软复位WM8978
	
    if (mode == 0)
    {
        // 播放
        
        BSP_WM8978_Write_Reg(1, 0x1B);                      // R1,MICEN设置为1(MIC使能),BIASEN设置为1(模拟器工作),VMIDSEL[1:0]设置为:11(5K)
        BSP_WM8978_Write_Reg(2, 0x1B0);                     // R2,ROUT1,LOUT1输出使能(耳机可以工作),BOOSTENR,BOOSTENL使能
        BSP_WM8978_Write_Reg(3, 0x6C);	                    // R3,LOUT2,ROUT2输出禁止(喇叭不工作),RMIX,LMIX使能	
        BSP_WM8978_Write_Reg(6, 0);	                        // R6,MCLK由外部提供 //0x0d
        BSP_WM8978_Write_Reg(43, 1 << 4);	                // R43,INVROUT2反向,驱动喇叭
        BSP_WM8978_Write_Reg(47, 1 << 8);	                // R47设置,PGABOOSTL,左通道MIC未获得20倍增益
        BSP_WM8978_Write_Reg(48, 1 << 8);	                // R48设置,PGABOOSTR,右通道MIC未获得20倍增益
        BSP_WM8978_Write_Reg(49, 1 << 1);	                // R49,TSDEN,开启过热保护 
        BSP_WM8978_Write_Reg(49, 1 << 2);	                // R49,TSDEN,开启过热保护 
        BSP_WM8978_Write_Reg(10, 1 << 3);	                // R10,SOFTMUTE关闭,128x采样,最佳SNR 
        BSP_WM8978_Write_Reg(14, 1 << 3);	                // R14,ADC 128x采样率
    
        
        BSP_WM8978_ADDA_Cfg(1, 0);		                // 开启DAC 
        BSP_WM8978_Input_Cfg(0, 0, 0);	                // 关闭输入通道(MIC&LINE IN)
        BSP_WM8978_Output_Cfg(1, 0);	                // 开启DAC输出 
        BSP_WM8978_MIC_Gain(0);			                // MIC增益设置为0 
        BSP_WM8978_I2S_Cfg(2, 0);		                // 飞利浦标准,16位数据长度
//        BSP_WM8978_HPvol_Set(57 - 0, 57 - 0);         // 耳机音量：左：-40dB；右：-40dB;
//        
//        BSP_WM8978_Write_Reg(3, 0x6C);                // R3,LOUT2,ROUT2输出使能(喇叭工作),RMIX,LMIX使能 
        BSP_WM8978_Write_Reg(54,50);                  // 设置LOUT2左声道音量
        BSP_WM8978_Write_Reg(55,50|(1<<8));           // 设置ROUT2右声道音量， 更新左右声道音量
    }
    else
    {
        // 录音 + 播放
        BSP_WM8978_Write_Reg(1, 0x1B);	                    // R1,MICEN设置为1(MIC使能),BIASEN设置为1(模拟器工作),VMIDSEL[1:0]设置为:11(5K)
        BSP_WM8978_Write_Reg(2, 0x1B0);	                    // R2,ROUT1,LOUT1输出使能(耳机可以工作),BOOSTENR,BOOSTENL使能
        BSP_WM8978_Write_Reg(3, 0x0C);	                    // R3,LOUT2,ROUT2输出禁止(喇叭不工作),RMIX,LMIX使能	
        BSP_WM8978_Write_Reg(6, 0x0d);	                    // R6,MCLK由外部提供
        BSP_WM8978_Write_Reg(43, 1 << 4);	                // R43,INVROUT2反向,驱动喇叭
        BSP_WM8978_Write_Reg(47, 1 << 8);	                // R47设置,PGABOOSTL,左通道MIC未获得20倍增益
        BSP_WM8978_Write_Reg(48, 1 << 8);	                // R48设置,PGABOOSTR,右通道MIC未获得20倍增益
        BSP_WM8978_Write_Reg(49, 1 << 1);	                // R49,TSDEN,开启过热保护 
        BSP_WM8978_Write_Reg(10, 1 << 3);	                // R10,SOFTMUTE关闭,128x采样,最佳SNR 
        BSP_WM8978_Write_Reg(14, 1 << 3);	                // R14,ADC 128x采样率
    
        BSP_WM8978_ADDA_Cfg(1, 1);		                // 开启ADC，开启DAC 
        BSP_WM8978_Input_Cfg(1, 0, 0);	                // 开启输入通道(MIC&LINE IN)
        BSP_WM8978_Output_Cfg(1, 0);	                // 开启DAC输出 
        BSP_WM8978_MIC_Gain(63);		                // MIC增益设置 
        BSP_WM8978_I2S_Cfg(2, 0);		                // 飞利浦标准,16位数据长度
        BSP_WM8978_HPvol_Set(57 - 10, 57 - 10);         // 耳机音量：左：-10dB；右：-10dB;
    }
    
    
    return 0;
} 

__attribute__((weak)) uint32_t BSP_WM8978_Write_Reg(uint32_t reg, uint32_t value)
{ 
	uint8_t Buf[10] = {0};

	Buf[0] = (reg << 1) | ((value >> 8) & 0x01);
	Buf[1] = value & 0xFF;
    
	HAL_I2C_Master_Transmit(&I2C4_Handle, WM8978_WIRTE_ADDRESS, Buf, 2, 100);
    
	g_reg_tab[reg] = value;	                            //保存寄存器值到本地
    
	return 0;	
}

__attribute__((weak)) uint32_t BSP_WM8978_Read_Reg(uint32_t reg)
{  
	return (uint32_t)g_reg_tab[reg];	
}

void BSP_WM8978_ADDA_Cfg(uint32_t dacen, uint32_t adcen)
{
	uint16_t value;
    
	value = BSP_WM8978_Read_Reg(3);                     //读取R3
    
	if (dacen)
        value |= 3 << 0;		                        //R3最低2个位设置为1,开启DACR&DACL
	else
        value &= ~(3 << 0);		                        //R3最低2个位清零,关闭DACR&DACL.
    
	BSP_WM8978_Write_Reg(3, value);	                    //设置R3
    
	value = BSP_WM8978_Read_Reg(2);	                    //读取R2
    
	if (adcen)
        value |= 3 << 0;		                        //R2最低2个位设置为1,开启ADCR&ADCL
	else
        value &= ~(3 << 0);		                        //R2最低2个位清零,关闭ADCR&ADCL.
    
	BSP_WM8978_Write_Reg(2, value);	                    //设置R2	
}

void BSP_WM8978_Input_Cfg(uint32_t micen, uint32_t lineinen, uint32_t auxen)
{
	uint16_t value;  
    
	value = BSP_WM8978_Read_Reg(2);	                    //读取R2
    
	if (micen)
        value |= 3 << 2;		                        //开启INPPGAENR,INPPGAENL(MIC的PGA放大)
	else
        value &= ~(3 << 2);		                        //关闭INPPGAENR,INPPGAENL.
    
 	BSP_WM8978_Write_Reg(2, value);	                    //设置R2 
	
	value = BSP_WM8978_Read_Reg(44);	                //读取R44
    
	if (micen)
        value |= 3 << 4 | 3 << 0;	                    //开启LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
	else
        value &= ~( 3 << 4 | 3 << 0);	                //关闭LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
    
	BSP_WM8978_Write_Reg(44, value);                    //设置R44
	
	if (lineinen)
        BSP_WM8978_LINEIN_Gain(5);                      //LINE IN 0dB增益
	else
        BSP_WM8978_LINEIN_Gain(0);	                    //关闭LINE IN
    
	if (auxen)
        BSP_WM8978_AUX_Gain(7);                         //AUX 6dB增益
	else
        BSP_WM8978_AUX_Gain(0);	                        //关闭AUX输入  
}

//WM8978 输出配置 
//dacen:DAC输出(放音)开启(1)/关闭(0)
//bpsen:Bypass输出(录音,包括MIC,LINE IN,AUX等)开启(1)/关闭(0) 
void BSP_WM8978_Output_Cfg(uint32_t dacen,uint32_t bpsen)
{
	uint16_t value = 0;
    
	if (dacen)
        value |= 1 << 0;	                            //DAC输出使能
    
	if(bpsen)
	{
		value |= 1 << 1;		                        //BYPASS使能
		value |= 5 << 2;		                        //0dB增益
	}
    
	BSP_WM8978_Write_Reg(50, value);                    //R50设置
	BSP_WM8978_Write_Reg(51, value);                    //R51设置 
}

//WM8978 MIC增益设置(不包括BOOST的20dB,MIC-->ADC输入部分的增益)
//gain:0~63,对应-12dB~35.25dB,0.75dB/Step
void BSP_WM8978_MIC_Gain(uint32_t gain)
{
	gain &= 0x3F;
	BSP_WM8978_Write_Reg(45, gain);		                //R45,左通道PGA设置 
	BSP_WM8978_Write_Reg(46, gain | (1<<8));	        //R46,右通道PGA设置
}

//WM8978 L2/R2(也就是Line In)增益设置(L2/R2-->ADC输入部分的增益)
//gain:0~7,0表示通道禁止,1~7,对应-12dB~6dB,3dB/Step
void BSP_WM8978_LINEIN_Gain(uint32_t gain)
{
	uint16_t value;
    
	gain &= 0x07;
	value = BSP_WM8978_Read_Reg(47);	                //读取R47
    
	value &= ~(7 << 4);			                        //清除原来的设置 
 	BSP_WM8978_Write_Reg(47, value | (gain << 4));      //设置R47
    
	value = BSP_WM8978_Read_Reg(48);	                //读取R48
    
	value &= ~(7 << 4);			                        //清除原来的设置 
 	BSP_WM8978_Write_Reg(48, value | (gain << 4));      //设置R48
}

//WM8978 AUXR,AUXL(PWM音频部分)增益设置(AUXR/L-->ADC输入部分的增益)
//gain:0~7,0表示通道禁止,1~7,对应-12dB~6dB,3dB/Step
void BSP_WM8978_AUX_Gain(uint32_t gain)
{
	uint16_t value;
    
	gain &= 0x07;
	value = BSP_WM8978_Read_Reg(47);	                //读取R47
    
	value &= ~(7 << 0);			                        //清除原来的设置 
 	BSP_WM8978_Write_Reg(47, value | (gain << 0));      //设置R47
    
	value=BSP_WM8978_Read_Reg(48);	                    //读取R48
    
	value &= ~(7 << 0);			                        //清除原来的设置 
 	BSP_WM8978_Write_Reg(48, value | (gain << 0));      //设置R48
}

//设置I2S工作模式
//fmt:0,LSB(右对齐);1,MSB(左对齐);2,飞利浦标准I2S;3,PCM/DSP;
//len:0,16位;1,20位;2,24位;3,32位;  
void BSP_WM8978_I2S_Cfg(uint32_t fmt, uint32_t len)
{
	fmt &= 0x03;
	len &= 0x03;                                        //限定范围
    
	BSP_WM8978_Write_Reg(4, (fmt << 3) | (len << 5));   //R4,WM8978工作模式设置	
}	

//设置耳机左右声道音量
//voll:左声道音量(0~63)
//volr:右声道音量(0~63)
void BSP_WM8978_HPvol_Set(uint32_t voll, uint32_t volr)
{
	voll &= 0x3F;
	volr &= 0x3F;                                       //限定范围
    
	if (voll == 0)
        voll |= 1 << 6;                                 //音量为0时,直接mute
    
	if (volr == 0)
        volr |= 1 << 6;                                 //音量为0时,直接mute
    
	BSP_WM8978_Write_Reg(52, voll);			            //R52,耳机左声道音量设置
	BSP_WM8978_Write_Reg(53, volr | (1 << 8));          //R53,耳机右声道音量设置,同步更新(HPVU=1)
}

//设置喇叭音量
//voll:左声道音量(0~63) 
void BSP_WM8978_SPKvol_Set(uint32_t volx)
{ 
	volx &= 0x3F;                                       //限定范围
    
	if (volx == 0)
        volx |= 1 << 6;                                 //音量为0时,直接mute 
    
 	BSP_WM8978_Write_Reg(54, volx);			            //R54,喇叭左声道音量设置
	BSP_WM8978_Write_Reg(55, volx | (1 << 8));	        //R55,喇叭右声道音量设置,同步更新(SPKVU=1)	
}

//设置3D环绕声
//depth:0~15(3D强度,0最弱,15最强)
void BSP_WM8978_3D_Set(uint32_t depth)
{ 
	depth &= 0xF;                                       //限定范围
    
 	BSP_WM8978_Write_Reg(41, depth);	                //R41,3D环绕设置 	
}

//设置EQ/3D作用方向
//dir:0,在ADC起作用
//    1,在DAC起作用(默认)
void BSP_WM8978_EQ_3D_Dir(uint32_t dir)
{
	uint16_t value;
    
	value = BSP_WM8978_Read_Reg(0x12);
    
	if (dir)
        value |= 1 << 8;
	else
        value &= ~(1 << 8);
    
 	BSP_WM8978_Write_Reg(18, value);                    //R18,EQ1的第9位控制EQ/3D方向
}

//设置EQ1
//cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
//gain:增益,0~24,对应-12~+12dB
void BSP_WM8978_EQ1_Set(uint32_t cfreq, uint32_t gain)
{ 
	uint16_t value;
    
	cfreq &= 0x3;                                       //限定范围
    
	if (gain > 24)
        gain = 24;
	gain = 24 - gain;
    
	value = BSP_WM8978_Read_Reg(18);
    
	value &= 0x100;
	value |= cfreq << 5;	                            //设置截止频率 
	value |= gain;		                                //设置增益
    
 	BSP_WM8978_Write_Reg(18, value);                    //R18,EQ1设置 	
}

//设置EQ2
//cfreq:中心频率,0~3,分别对应:230/300/385/500Hz
//gain:增益,0~24,对应-12~+12dB
void BSP_WM8978_EQ2_Set(uint32_t cfreq, uint32_t gain)
{ 
	uint16_t value = 0;
    
	cfreq &= 0x3;                                       //限定范围
    
	if (gain > 24)
        gain = 24;
	gain = 24 - gain;
    
	value |= cfreq << 5;	                            //设置截止频率 
	value |= gain;		                                //设置增益
    
 	BSP_WM8978_Write_Reg(19, value);                    //R19,EQ2设置 	
}

//设置EQ3
//cfreq:中心频率,0~3,分别对应:650/850/1100/1400Hz
//gain:增益,0~24,对应-12~+12dB
void BSP_WM8978_EQ3_Set(uint32_t cfreq, uint32_t gain)
{ 
	uint16_t value = 0;
    
	cfreq &= 0x3;                                       //限定范围
    
	if (gain > 24)
        gain = 24;
	gain = 24 - gain;
    
	value |= cfreq << 5;	                            //设置截止频率 
	value |= gain;		                                //设置增益
    
 	BSP_WM8978_Write_Reg(20, value);                    //R20,EQ3设置 	
}

//设置EQ4
//cfreq:中心频率,0~3,分别对应:1800/2400/3200/4100Hz
//gain:增益,0~24,对应-12~+12dB
void BSP_WM8978_EQ4_Set(uint32_t cfreq, uint32_t gain)
{ 
	uint16_t value = 0;
    
	cfreq &= 0x3;                                       //限定范围
    
	if (gain > 24)
        gain = 24;
	gain = 24 - gain;
    
	value |= cfreq << 5;	                            //设置截止频率 
	value |= gain;		                                //设置增益	
    
 	BSP_WM8978_Write_Reg(21,value);                     //R21,EQ4设置 	
}

//设置EQ5
//cfreq:中心频率,0~3,分别对应:5300/6900/9000/11700Hz
//gain:增益,0~24,对应-12~+12dB
void BSP_WM8978_EQ5_Set(uint32_t cfreq, uint32_t gain)
{ 
	uint16_t value = 0;
    
	cfreq &= 0x3;                                       //限定范围
    
	if (gain > 24)
        gain = 24;
	gain = 24 - gain; 
    
	value |= cfreq << 5;	                            //设置截止频率 
	value |= gain;		                                //设置增益
    
 	BSP_WM8978_Write_Reg(22, value);                    //R22,EQ5设置 	
}



