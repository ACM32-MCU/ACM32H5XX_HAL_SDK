#include "lcd8080.h"  

//根据液晶扫描方向而变化的XY像素宽度
//调用ILI9341_GramScan函数设置方向时会自动更改
uint16_t glcdXLength = ILI9341_LESS_PIXEL;
uint16_t glcdYLength = ILI9341_MORE_PIXEL;

//液晶屏扫描模式，本变量主要用于方便选择触摸屏的计算参数
//参数可选值为0-7
//调用ILI9341_GramScan函数设置方向时会自动更改
//LCD刚初始化完成时会使用本默认值
uint8_t glcdScanMode = 6;



//保存液晶屏驱动ic的 ID
uint16_t glcdId = 0;


static FONT_SelectTypeDef *LCD_Currentfonts = &gfont8x16;  //英文字体
static uint16_t CurrentTextColor   = BLACK;//前景色
static uint16_t CurrentBackColor   = WHITE;//背景色



/******************************************************************************
* @brief : Write command to ILI9341.
* @param : Cmd : Command to write (table register address).
* @return: none
******************************************************************************/  
void ILI9341_Write_Cmd(uint16_t Cmd)
{
    * (__IO uint16_t *) (FMC_Addr_ILI9341_CMD) = Cmd;
}

/******************************************************************************
* @brief : Write data to ILI9341.
* @param : Data : data to write (table register address).
* @return: none
******************************************************************************/  	
void ILI9341_Write_Data(uint16_t Data)
{
    * (__IO uint16_t *) (FMC_Addr_ILI9341_DATA) = Data;
}


/******************************************************************************
* @brief : Read data from ILI9341.
* @param : None.
* @return: Data read
******************************************************************************/  
uint16_t ILI9341_Read_Data(void)
{
    return (*(__IO uint16_t *) (FMC_Addr_ILI9341_DATA) );	
}

/******************************************************************************
* @brief : For ILI9341 simple delay function.
* @param : Count: Delay count value.
* @return: None
******************************************************************************/  
static void ILI9341_Delay(__IO uint32_t Count)
{
    for ( ; Count != 0; Count -- );
}




/******************************************************************************
* @brief : Read LCD driver chip ID function.
* @param : None.
* @return: LCDID_UNKNOWN or LCD driver chip ID
******************************************************************************/  
uint16_t ILI9341_ReadID(void)
{
    uint16_t id = 0;

    ILI9341_Write_Cmd(0x04);
    ILI9341_Read_Data();
    ILI9341_Read_Data();
    id = ILI9341_Read_Data();
    id <<= 8;
    id |= ILI9341_Read_Data();

    if(id == LCDID_ST7789V)
    {
        return id;
    }
    else
    {
        ILI9341_Write_Cmd(0xD3);
        ILI9341_Read_Data();
        ILI9341_Read_Data();
        id = ILI9341_Read_Data();
        id <<= 8;
        id |= ILI9341_Read_Data();
        if(id == LCDID_ILI9341)
        {
            return id;
        }
    }

    return LCDID_UNKNOWN;
}


/******************************************************************************
* @brief : ILI9341 register initialization.
* @param : None.
* @return: None
******************************************************************************/  
static void ILI9341_REG_Config(void)
{
    glcdId = ILI9341_ReadID();
    printfS("LCD ID = 0x%0.4x\r\n", glcdId);
    if(glcdId == LCDID_ILI9341)
    {
        /*  Power control B (CFh)  */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xCF);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x81);
        ILI9341_Write_Data(0x30);

        /*  Power on sequence control (EDh) */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xED);
        ILI9341_Write_Data(0x64);
        ILI9341_Write_Data(0x03);
        ILI9341_Write_Data(0x12);
        ILI9341_Write_Data(0x81);

        /*  Driver timing control A (E8h) */
        DEBUG_DELAY ();
        ILI9341_Write_Cmd(0xE8);
        ILI9341_Write_Data(0x85);
        ILI9341_Write_Data(0x10);
        ILI9341_Write_Data(0x78);

        /*  Power control A (CBh) */
        DEBUG_DELAY ();
        ILI9341_Write_Cmd(0xCB);
        ILI9341_Write_Data(0x39);
        ILI9341_Write_Data(0x2C);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x34);
        ILI9341_Write_Data(0x02);

        /* Pump ratio control (F7h) */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xF7);
        ILI9341_Write_Data(0x20);

        /* Driver timing control B */
        DEBUG_DELAY ();
        ILI9341_Write_Cmd(0xEA);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x00);

        /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xB1);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x1B);

        /*  Display Function Control (B6h) */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xB6);
        ILI9341_Write_Data(0x0A);
        ILI9341_Write_Data(0xA2);

        /* Power Control 1 (C0h) */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xC0);
        ILI9341_Write_Data(0x35);

        /* Power Control 2 (C1h) */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0xC1);
        ILI9341_Write_Data(0x11);

        /* VCOM Control 1 (C5h) */
        ILI9341_Write_Cmd(0xC5);
        ILI9341_Write_Data(0x45);
        ILI9341_Write_Data(0x45);

        /*  VCOM Control 2 (C7h)  */
        ILI9341_Write_Cmd(0xC7);
        ILI9341_Write_Data(0xA2);

        /* Enable 3G (F2h) */
        ILI9341_Write_Cmd(0xF2);
        ILI9341_Write_Data(0x00);

        /* Gamma Set (26h) */
        ILI9341_Write_Cmd(0x26);
        ILI9341_Write_Data(0x01);
        DEBUG_DELAY();

        /* Positive Gamma Correction */
        ILI9341_Write_Cmd(0xE0); //Set Gamma
        ILI9341_Write_Data(0x0F);
        ILI9341_Write_Data(0x26);
        ILI9341_Write_Data(0x24);
        ILI9341_Write_Data(0x0B);
        ILI9341_Write_Data(0x0E);
        ILI9341_Write_Data(0x09);
        ILI9341_Write_Data(0x54);
        ILI9341_Write_Data(0xA8);
        ILI9341_Write_Data(0x46);
        ILI9341_Write_Data(0x0C);
        ILI9341_Write_Data(0x17);
        ILI9341_Write_Data(0x09);
        ILI9341_Write_Data(0x0F);
        ILI9341_Write_Data(0x07);
        ILI9341_Write_Data(0x00);

        /* Negative Gamma Correction (E1h) */
        ILI9341_Write_Cmd(0XE1); //Set Gamma
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x19);
        ILI9341_Write_Data(0x1B);
        ILI9341_Write_Data(0x04);
        ILI9341_Write_Data(0x10);
        ILI9341_Write_Data(0x07);
        ILI9341_Write_Data(0x2A);
        ILI9341_Write_Data(0x47);
        ILI9341_Write_Data(0x39);
        ILI9341_Write_Data(0x03);
        ILI9341_Write_Data(0x06);
        ILI9341_Write_Data(0x06);
        ILI9341_Write_Data(0x30);
        ILI9341_Write_Data(0x38);
        ILI9341_Write_Data(0x0F);

        /* memory access control set */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0x36); 	
        ILI9341_Write_Data(0xC8);    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
        DEBUG_DELAY();

        /* column address control set */
        ILI9341_Write_Cmd(CMD_SetCoordinateX ); 
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0xEF);

        /* page address control set */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(CMD_SetCoordinateY ); 
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x00);
        ILI9341_Write_Data(0x01);
        ILI9341_Write_Data(0x3F);

        /*  Pixel Format Set (3Ah)  */
        DEBUG_DELAY();
        ILI9341_Write_Cmd(0x3a); 
        ILI9341_Write_Data(0x55);

        /* Sleep Out (11h)  */
        ILI9341_Write_Cmd(0x11);	
        ILI9341_Delay(0xAFFf<<2);
        DEBUG_DELAY();

        /* Display ON (29h) */
        ILI9341_Write_Cmd(0x29); 
    }
}






/******************************************************************************
* @brief : ILI9341 backlight LED control.
* @param : enumState: Enable or Disable backlight LED
* @return: None
******************************************************************************/  
void ILI9341_BackLed_Control(FunctionalState enumState)
{
    if (enumState)
    {
        HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_SET);
    }

}



/******************************************************************************
* @brief : ILI9341 softwore reset.
* @param : None
* @return: None
******************************************************************************/  
void ILI9341_Rst (void)
{			
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);	 //低电平复位

    HAL_Delay(200); 					   

    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);		 	 

    HAL_Delay(200); 	
}

/******************************************************************************
* @brief : Set the scan direction of ILI9341's GRAM.
* @param : Option: Select the scan direction of GRAM
* @return: None
******************************************************************************/  
void ILI9341_GramScan(uint8_t Option)
{	
    //参数检查，只可输入0-7
    if(Option >7)
    {
        return;
    }

    //根据模式更新glcdScanMode的值，主要用于触摸屏选择计算参数
    glcdScanMode = Option;

    //根据模式更新XY方向的像素宽度
    if(Option%2 == 0)	
    {
        //0 2 4 6模式下X方向像素宽度为240，Y方向为320
        glcdXLength = ILI9341_LESS_PIXEL;
        glcdYLength =	ILI9341_MORE_PIXEL;
    }
    else				
    {
        //1 3 5 7模式下X方向像素宽度为320，Y方向为240
        glcdXLength = ILI9341_MORE_PIXEL;
        glcdYLength =	ILI9341_LESS_PIXEL; 
    }

    //0x36命令参数的高3位可用于设置GRAM扫描方向	
    ILI9341_Write_Cmd(0x36); 
    ILI9341_Write_Data(0x08 |(Option<<5));//根据ucOption的值设置LCD参数，共0-7种模式
    ILI9341_Write_Cmd(CMD_SetCoordinateX ); 
    ILI9341_Write_Data(0x00);		/* x 起始坐标高8位 */
    ILI9341_Write_Data(0x00);		/* x 起始坐标低8位 */
    ILI9341_Write_Data(((glcdXLength-1)>>8)&0xFF); /* x 结束坐标高8位 */	
    ILI9341_Write_Data((glcdXLength-1)&0xFF);				/* x 结束坐标低8位 */

    ILI9341_Write_Cmd(CMD_SetCoordinateY ); 
    ILI9341_Write_Data(0x00);		/* y 起始坐标高8位 */
    ILI9341_Write_Data(0x00);		/* y 起始坐标低8位 */
    ILI9341_Write_Data(((glcdYLength-1)>>8)&0xFF);	/* y 结束坐标高8位 */	 
    ILI9341_Write_Data((glcdYLength-1)&0xFF);				/* y 结束坐标低8位 */

    /* write gram start */
    ILI9341_Write_Cmd(CMD_SetPixel);	
}




/******************************************************************************
* @brief : Create a window on the ILI9341 display.
* @param : x: X coordinate of the starting point of the window in a specific scanning direction
* @param : y: y coordinate of the starting point of the window in a specific scanning direction
* @param : width: Width of the window
* @param : height: Window height
* @return: None
******************************************************************************/ 
void ILI9341_OpenWindow (uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{	
    ILI9341_Write_Cmd (CMD_SetCoordinateX); 				 /* 设置X坐标 */
    ILI9341_Write_Data (x >> 8);	 /* 先高8位，然后低8位 */
    ILI9341_Write_Data (x & 0xff);	 /* 设置起始点和结束点*/
    ILI9341_Write_Data ((x + width - 1) >> 8);
    ILI9341_Write_Data ((x + width - 1) & 0xff);

    ILI9341_Write_Cmd (CMD_SetCoordinateY); 			     /* 设置Y坐标*/
    ILI9341_Write_Data (y >> 8);
    ILI9341_Write_Data (y & 0xff);
    ILI9341_Write_Data ((y + height - 1) >> 8);
    ILI9341_Write_Data ((y + height - 1) & 0xff);

}


/******************************************************************************
* @brief : Set the cursor coordinates of ILI9341.
* @param : x: X coordinate of cursor in specific scanning direction
* @param : y: Y coordinate of cursor in specific scanning direction

* @return: None
******************************************************************************/ 
static void ILI9341_SetCursor (uint16_t x, uint16_t y)	
{
    ILI9341_OpenWindow (x, y, 1, 1 );
}



/******************************************************************************
* @brief : Fill pixels with a certain color on the ILI9341 display.
* @param : amoutPoint: Total number of pixels to fill with color
* @param : color: color
* @return: None
******************************************************************************/ 
static __inline void ILI9341_FillColor (uint32_t amoutPoint, uint16_t color)
{
    uint32_t i = 0;
    /* memory write */
    ILI9341_Write_Cmd(CMD_SetPixel);	

    for (i = 0; i < amoutPoint; i ++)
    {
        ILI9341_Write_Data(color);
    }
}



/******************************************************************************
* @brief : Clear a window of the ILI9341 display with a certain color.
* @param : x: x coordinate of the starting point of the window in a specific scanning direction
* @param : y: y coordinate of the starting point of the window in a specific scanning direction
* @param : width: Width of the window
* @param : height: Window height
* @return: None
******************************************************************************/ 
void ILI9341_Clear ( uint16_t x, uint16_t y, uint16_t width, uint16_t height )
{
    ILI9341_OpenWindow (x, y, width, height);

    ILI9341_FillColor (width * height, CurrentBackColor );		
}



/******************************************************************************
* @brief : Clear a window of the ILI9341 display with a certain color.
* @param : x: x coordinate of the point in a specific scanning direction
* @param : y: y coordinate of the point in a specific scanning direction
* @return: None
******************************************************************************/ 
void ILI9341_SetPointPixel (uint16_t x, uint16_t y)	
{	
    if ((x < glcdXLength) && (y < glcdYLength))
    {
        ILI9341_SetCursor (x, y);

        ILI9341_FillColor (1, CurrentTextColor);
    }
}



/******************************************************************************
* @brief : Read one pixel data of ILI9341 GRAN.
* @param : None
* @return: pixel data
******************************************************************************/ 
static uint16_t ILI9341_Read_PixelData(void)	
{	
    uint16_t usR=0, usG=0, usB=0 ;


    ILI9341_Write_Cmd (0x2E);   /* 读数据 */

    usR = ILI9341_Read_Data(); 	/*FIRST READ OUT DUMMY DATA*/

    usR = ILI9341_Read_Data();  	/*READ OUT RED DATA  */
    usB = ILI9341_Read_Data();  	/*READ OUT BLUE DATA*/
    usG = ILI9341_Read_Data();  	/*READ OUT GREEN DATA*/	

    return ((((usR >> 11 ) << 11) | ( usG >> 10 ) << 5) | (usB >> 11 ));

}



/******************************************************************************
* @brief : Obtain pixel data of a coordinate point on the ILI9341 display.
* @param : x: x coordinate of the point in a specific scanning direction
* @param : y: y coordinate of the point in a specific scanning direction
* @return: None
******************************************************************************/ 
uint16_t ILI9341_GetPointPixel (uint16_t x, uint16_t y)
{ 
    uint16_t usPixelData;


    ILI9341_SetCursor (x, y);

    usPixelData = ILI9341_Read_PixelData ();

    return usPixelData;

}



/******************************************************************************
* @brief : Use Bresenham algorithm to draw line segments on ILI9341 display.
* @param : x1: x coordinate of an end point of a line segment in a specific scanning direction
* @param : y1: y coordinate of an end point of a line segment in a specific scanning direction
* @param : x2: x coordinate of the other end point of the line segment in a specific scanning direction
* @param : y3: y coordinate of the other end point of the line segment in a specific scanning direction
* @return: None
******************************************************************************/ 
void ILI9341_DrawLine (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint16_t us; 
    uint16_t usX_Current, usY_Current;

    int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
    int32_t lIncrease_X, lIncrease_Y; 	


    lDelta_X = x2 - x1; //计算坐标增量 
    lDelta_Y = y2 - y1; 

    usX_Current = x1; 
    usY_Current = y1; 


    if(lDelta_X > 0)
    {    
        lIncrease_X = 1; //设置单步方向 
    }

    else if(lDelta_X == 0)
    {    
        lIncrease_X = 0;//垂直线 
    }
    else 
    { 
        lIncrease_X = -1;
        lDelta_X = - lDelta_X;
    } 


    if(lDelta_Y > 0)
    {
        lIncrease_Y = 1; 
    }
    else if(lDelta_Y == 0)
    {
        lIncrease_Y = 0;//水平线 
    }
    else 
    {
        lIncrease_Y = -1;
        lDelta_Y = - lDelta_Y;
    } 


    if (lDelta_X > lDelta_Y)
    {
        lDistance = lDelta_X; //选取基本增量坐标轴 
    }
    else
    {    
        lDistance = lDelta_Y; 
    }

    for (us = 0; us <= lDistance + 1; us ++)//画线输出 
    {  
        ILI9341_SetPointPixel (usX_Current, usY_Current);//画点 

        lError_X += lDelta_X ; 
        lError_Y += lDelta_Y ; 

    if (lError_X > lDistance) 
    { 
        lError_X -= lDistance; 
        usX_Current += lIncrease_X; 
    }  

    if (lError_Y > lDistance) 
    { 
        lError_Y -= lDistance; 
        usY_Current += lIncrease_Y; 
    } 

    }  
}   



/******************************************************************************
* @brief : Draw a rectangle on the ILI9341 display.
* @param : x: x coordinate of the starting point of the rectangle in a specific scanning direction
* @param : y: y coordinate of the starting point of the rectangle in a specific scanning direction
* @param : width: Width of the rectangle in pixels
* @param : height: Height of rectangle in pixels
* @param : filled: Select whether to fill the rectangle
* @return: None
******************************************************************************/ 
void ILI9341_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t filled)
{
    if(filled )
    {
        ILI9341_OpenWindow (x, y, width, height);
        ILI9341_FillColor (width * height, CurrentTextColor);	
    }
    else
    {
        ILI9341_DrawLine (x, y, x + width - 1, y );
        ILI9341_DrawLine (x, y + height - 1, x + width - 1, y + height - 1);
        ILI9341_DrawLine (x, y, x, y + height - 1 );
        ILI9341_DrawLine (x + width - 1, y, x + width - 1, y + height - 1);		
    }

}



/******************************************************************************
* @brief : Use Bresenham algorithm to draw circle on ILI9341 display.
* @param : x: x coordinate of the circle center in a specific scanning direction
* @param : y: y coordinate of the circle center in a specific scanning direction
* @param : radius: Radius of circle in pixels
* @param : filled: Select whether to fill the rectangle
* @return: None
******************************************************************************/ 
void ILI9341_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint8_t filled)
{
    int16_t sCurrentX, sCurrentY;
    int16_t sError;


    sCurrentX = 0; sCurrentY = radius;	  

    sError = 3 - ( radius << 1 );     //判断下个点位置的标志


    while ( sCurrentX <= sCurrentY )
    {
        int16_t sCountY;
        if(filled)
        {    
            for(sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++) 
            {                      
                ILI9341_SetPointPixel(x + sCurrentX, y + sCountY );           //1，研究对象 
                ILI9341_SetPointPixel(x - sCurrentX, y + sCountY );           //2       
                ILI9341_SetPointPixel(x - sCountY,   y + sCurrentX );           //3
                ILI9341_SetPointPixel(x - sCountY,   y - sCurrentX );           //4
                ILI9341_SetPointPixel(x - sCurrentX, y - sCountY );           //5    
                ILI9341_SetPointPixel(x + sCurrentX, y - sCountY );           //6
                ILI9341_SetPointPixel(x + sCountY,   y - sCurrentX );           //7 	
                ILI9341_SetPointPixel(x + sCountY,   y + sCurrentX );           //0				
            }
        }
        else
        {          
            ILI9341_SetPointPixel(x + sCurrentX, y + sCurrentY );             //1，研究对象
            ILI9341_SetPointPixel(x - sCurrentX, y + sCurrentY );             //2      
            ILI9341_SetPointPixel(x - sCurrentY, y + sCurrentX );             //3
            ILI9341_SetPointPixel(x - sCurrentY, y - sCurrentX );             //4
            ILI9341_SetPointPixel(x - sCurrentX, y - sCurrentY );             //5       
            ILI9341_SetPointPixel(x + sCurrentX, y - sCurrentY );             //6
            ILI9341_SetPointPixel(x + sCurrentY, y - sCurrentX );             //7 
            ILI9341_SetPointPixel(x + sCurrentY, y + sCurrentX );             //0
        }			


        sCurrentX ++;


        if (sError < 0)
        {        
            sError += 4 * sCurrentX + 6;	  
        }
        else
        {
            sError += 10 + 4 * ( sCurrentX - sCurrentY );   
            sCurrentY --;
        } 	


    }


}


/******************************************************************************
* @brief : Display an English character on the ILI9341 display.
* @param : x: The starting X coordinate of a character in a specific scanning direction
* @param : y: The starting Y coordinate of the point in a specific scanning direction
* @param : display: English characters to be displayed
* @return: None
******************************************************************************/ 
void ILI9341_DispChar_EN ( uint16_t x, uint16_t y, const char display )
{
    uint8_t  byteCount, bitCount,fontLength;	
    uint16_t ucRelativePositon;
    uint8_t *Pfont;

    //对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
    ucRelativePositon = display - ' ';

    //每个字模的字节数
    fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;

    //字模首地址
    /*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
    Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];

    //设置显示窗口
    ILI9341_OpenWindow(x, y, LCD_Currentfonts->Width, LCD_Currentfonts->Height);

    ILI9341_Write_Cmd(CMD_SetPixel);			

    //按字节读取字模数据
    //由于前面直接设置了显示窗口，显示数据会自动换行
    for (byteCount = 0; byteCount < fontLength; byteCount++ )
    {
        //一位一位处理要显示的颜色
        for(bitCount = 0; bitCount < 8; bitCount++)
        {
            if(Pfont[byteCount] & (0x80>>bitCount))
            {
                ILI9341_Write_Data (CurrentTextColor);
            }			
            else
            {
                ILI9341_Write_Data (CurrentBackColor);
            }
        }	
    }	
}


/******************************************************************************
* @brief : Display English strings on the ILI9341 display.
* @param : Line: The starting Y coordinate of the string in a specific scanning direction

* @return: None
******************************************************************************/ 
void ILI9341_DispStringLine_EN(uint16_t line, char * pStr)
{
    uint16_t x = 0;

    while (* pStr != '\0')
    {
        if((x - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > glcdXLength)
        {
            x = ILI9341_DispWindow_X_Star;
            line += LCD_Currentfonts->Height;
        }

        if((line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > glcdYLength)
        {
            x = ILI9341_DispWindow_X_Star;
            line = ILI9341_DispWindow_Y_Star;
        }

        ILI9341_DispChar_EN (x, line, * pStr);

        pStr ++;

        x += LCD_Currentfonts->Width;

    }

}


/******************************************************************************
* @brief : Display English strings on the ILI9341 display.
* @param : x: The starting X coordinate of a character in a specific scanning direction
* @param : y: The starting Y coordinate of a character in a specific scanning direction
* @param : pStr: The first address of the English string to be displayed
* @return: None
******************************************************************************/ 
void ILI9341_DispString_EN(uint16_t x ,uint16_t y, char * pStr)
{
    while (* pStr != '\0')
    {
        if ((x - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > glcdXLength)
        {
            x = ILI9341_DispWindow_X_Star;
            y += LCD_Currentfonts->Height;
        }

        if ((y - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > glcdYLength)
        {
            x = ILI9341_DispWindow_X_Star;
            y = ILI9341_DispWindow_Y_Star;
        }

        ILI9341_DispChar_EN (x, y, * pStr);

        pStr ++;

        x += LCD_Currentfonts->Width;

    }

}

/******************************************************************************
* @brief : Display English strings on the ILI9341 display (along the Y axis).
* @param : x: The starting X coordinate of a character in a specific scanning direction
* @param : y: The starting Y coordinate of a character in a specific scanning direction
* @param : pStr: The first address of the English string to be displayed
* @return: None
******************************************************************************/ 
void ILI9341_DispString_EN_YDir(uint16_t x, uint16_t y, char * pStr)
{	
    while (* pStr != '\0')
    {
        if ((y - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) >glcdYLength)
        {
            y = ILI9341_DispWindow_Y_Star;
            x += LCD_Currentfonts->Width;
        }

        if ((x - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) >  glcdXLength)
        {
            x = ILI9341_DispWindow_X_Star;
            y = ILI9341_DispWindow_Y_Star;
        }

        ILI9341_DispChar_EN (x, y, * pStr);

        pStr ++;

        y += LCD_Currentfonts->Height;		
    }	
}

/******************************************************************************
* @brief : Set English font type.
* @param : fonts: Specify the font to select
* @return: None
******************************************************************************/
void LCD_SetFont(FONT_SelectTypeDef *fonts)
{
    LCD_Currentfonts = fonts;
}

/******************************************************************************
* @brief : Get the current font type.
* @param : None
* @return: Get the current font type
******************************************************************************/
FONT_SelectTypeDef *LCD_GetFont(void)
{
    return LCD_Currentfonts;
}

/******************************************************************************
* @brief : Set LCD foreground (font) and background color, RGB565.
* @param : TextColor: specify foreground (font) color
* @param : BackColor: specify background color
* @return: none
******************************************************************************/
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
    CurrentTextColor = TextColor; 
    CurrentBackColor = BackColor;
}

/******************************************************************************
* @brief : Get the foreground (font) and background color of LCD, RGB565.
* @param : TextColor: pointer variable used to store foreground (font) color
* @param : BackColor: pointer variable used to store background color
* @return: none
******************************************************************************/
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
    *TextColor = CurrentTextColor;
    *BackColor = CurrentBackColor;
}


/******************************************************************************
* @brief : Set LCD foreground (font) color, RGB565.
* @param : Color: specify foreground (font) color
* @return: none
******************************************************************************/
void LCD_SetTextColor(uint16_t Color)
{
    CurrentTextColor = Color;
}

/******************************************************************************
* @brief : Set LCD background color, RGB565t.
* @param : Color: specify the background color
* @return: none
******************************************************************************/
void LCD_SetBackColor(uint16_t Color)
{
    CurrentBackColor = Color;
}


/******************************************************************************
* @brief : Clear a line of text.
* @param : Line: Specify rows to delete
* @return: none
******************************************************************************/
void LCD_ClearLine(uint16_t Line)
{
    ILI9341_Clear(0,Line,glcdXLength,((FONT_SelectTypeDef *)LCD_GetFont())->Height);	/* 清屏，显示全黑 */
}


/******************************************************************************
* @brief : LCD8080 IO Configuration
* @param : None
* @return: none
******************************************************************************/
static void LCD8080_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable GPIOs clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*-- GPIO 配置 -----------------------------------------------------*/

    /* 通用 GPIO 配置 */
    GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;       //配置为复用功能
    GPIO_InitStructure.Pull  = GPIO_PULLUP;
    GPIO_InitStructure.Drive = GPIO_DRIVE_LEVEL3;    
 
    /*DQ数据信号线 针对引脚配置*/
    GPIO_InitStructure.Alternate = GPIO_FUNCTION_10;//AF10  
    GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

    /*控制信号线*/
    /* 配置FSMC相对应的控制线
	 * FMC_NOE   :LCD-RD
	 * FMC_NWE   :LCD-WR
	 * FMC_NE3   :LCD-CS
	 * FMC_A0    :LCD-DC/RS
	 */
    GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_OE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_OE_GPIO_PORT, &GPIO_InitStructure);  

    GPIO_InitStructure.Alternate = GPIO_FUNCTION_11;//AF11  
    GPIO_InitStructure.Pin = FMC_LCD_CS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_LCD_CS_GPIO_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);    

  
    /* 配置LCD复位RST/RSE控制管脚-A10 */
    GPIO_InitStructure.Pin            = ILI9341_RST_PIN;
    GPIO_InitStructure.Mode           = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull           = GPIO_PULLUP;
    GPIO_InitStructure.Alternate      = GPIO_FUNCTION_0;//AF0 
    HAL_GPIO_Init(ILI9341_RST_PORT, &GPIO_InitStructure); 
	
	/* 配置LCD背光控制管脚BK-A1 */
	GPIO_InitStructure.Pin            = ILI9341_BK_PIN; 
    GPIO_InitStructure.Mode           = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull           = GPIO_PULLUP;
    GPIO_InitStructure.Alternate      = GPIO_FUNCTION_0;//AF0 
	HAL_GPIO_Init (ILI9341_BK_PORT, &GPIO_InitStructure);  
    HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_SET);

}


/******************************************************************************
* @brief : FMC LCD8080 initialization function.
* @param : None
* @return: none
******************************************************************************/
void FMC_LCD_Init(void)
{
   FMC_NORSRAMInitTypeDef hlcd_Init;
    FMC_NORSRAMTimingInitTypeDef hlcd_Timing;

    /* 配置FMC接口相关的 GPIO*/
    LCD8080_GPIO_Config();
 
    /* 配置 FMC 相关参数 ---------------------------------------------------------*/
    hlcd_Init.NSBank                 = FMC_NORSRAM_BANK3;//use CS3 control，Addr=0x68000000
    hlcd_Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_DISABLE;
    hlcd_Init.MemoryType             = FMC_MEMORY_TYPE_NOR;
    hlcd_Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hlcd_Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
    hlcd_Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hlcd_Init.WrapMode               = FMC_WRAP_MODE_DISABLE;
    hlcd_Init.WaitSignalActive       = FMC_WAITSIGNALACTIVE_BEFOREWAITSTATE;
    hlcd_Init.WriteOperation         = FMC_WRITEOPERATION_ENABLE;
    hlcd_Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
    hlcd_Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
    hlcd_Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hlcd_Init.WriteMode              = FMC_WRITE_ASY_MODE;
    hlcd_Init.CRAMPageSize           = FMC_CRAMPAGE_SIZE_AUTO;

    /* NOR device configuration */ 
    hlcd_Timing.AddressSetupTime       = 0;
    hlcd_Timing.AddressHoldTime        = 8;
    hlcd_Timing.DataSetupTime          = 8;
    
    hlcd_Timing.BusTurnAroundDuration  = 0;
    hlcd_Timing.CLKDivision            = 0;
    hlcd_Timing.DataLatency            = 0;
    hlcd_Timing.AccessMode             = FMC_ACCESS_MODE_B;

    HAL_FMC_NORSRAM_Init(&hlcd_Init);
    HAL_FMC_NORSRAM_Timing_Init(&hlcd_Timing, hlcd_Init.NSBank);
}

/******************************************************************************
* @brief : ILI9341 initialization function.
* @param : None
* @return: none
******************************************************************************/
void ILI9341_Init(void)
{
    ILI9341_BackLed_Control(ENABLE);      //点亮LCD背光灯
    ILI9341_Rst();
    ILI9341_REG_Config();

    //设置默认扫描方向，其中 6 模式为大部分液晶例程的默认显示方向  
    ILI9341_GramScan(glcdScanMode);
}


/******************************************************************************
* @brief : ILI9341 LCD display.
* @param : None
* @return: none
******************************************************************************/
void LCD_Display(void)
{
    /*演示显示变量*/
    static uint8_t testCNT = 0;	
    char dispBuff[100];

    testCNT++;	

    LCD_SetFont(&gfont8x16);
    LCD_SetColors(RED,BLACK);

    ILI9341_Clear(0,0,glcdXLength,glcdYLength);	/* 清屏，显示全黑 */
    /********显示字符串示例*******/
    ILI9341_DispStringLine_EN(LINE(0),"Welcome to use Aisinochip:");
    ILI9341_DispStringLine_EN(LINE(1),"Using FMC to drive LCD8080");
    ILI9341_DispStringLine_EN(LINE(2),"Using ILI9341 LCD");
    ILI9341_DispStringLine_EN(LINE(3),"Only display and draw");

    /********显示变量示例*******/
    LCD_SetFont(&gfont16x24);
    LCD_SetTextColor(GREEN);

    /*使用c标准库把变量转化成字符串*/
    sprintf(dispBuff,"Count : %d ",testCNT);
    LCD_ClearLine(LINE(4));	/* 清除单行文字 */

    /*然后显示该字符串即可，其它变量也是这样处理*/
    ILI9341_DispStringLine_EN(LINE(4),dispBuff);

    /*******显示图形示例******/
    LCD_SetFont(&gfont24x32);
    /* 画直线 */

    LCD_ClearLine(LINE(4));/* 清除单行文字 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN(LINE(4),"Draw line:");

    LCD_SetTextColor(RED);
    ILI9341_DrawLine(50,170,210,230);  
    ILI9341_DrawLine(50,200,210,240);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawLine(100,170,200,230);  
    ILI9341_DrawLine(200,200,220,240);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawLine(110,170,110,230);  
    ILI9341_DrawLine(130,200,220,240);

    HAL_Delay(1000);

    ILI9341_Clear(0,16*8,glcdXLength,glcdYLength-16*8);	/* 清屏，显示全黑 */


    /*画矩形*/

    LCD_ClearLine(LINE(4));	/* 清除单行文字 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");

    LCD_SetTextColor(RED);
    ILI9341_DrawRectangle(50,200,100,30,1);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawRectangle(160,200,20,40,0);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawRectangle(170,200,50,20,1);


    HAL_Delay(1000);

    ILI9341_Clear(0,16*8,glcdXLength,glcdYLength-16*8);	/* 清屏，显示全黑 */

    /* 画圆 */
    LCD_ClearLine(LINE(4));	/* 清除单行文字 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");

    LCD_SetTextColor(RED);
    ILI9341_DrawCircle(100,200,20,0);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawCircle(100,200,10,1);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawCircle(140,200,20,0);

    HAL_Delay(1000);

    ILI9341_Clear(0,16*8,glcdXLength,glcdYLength-16*8);	/* 清屏，显示全黑 */

}



/******************************************************************************
* @brief : LCD8080 test.
* @param : None
* @return: none
******************************************************************************/
void FMC_LCD8080_Test(void)
{  
    printf("Start LCD8080 test!\r\n");  
    FMC_LCD_Init();
    
    ILI9341_Init();

    //其中0、3、5、6 模式适合从左至右显示文字，
    //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
    //其中 6 模式为大部分液晶例程的默认显示方向  
    ILI9341_GramScan (6);
    
    while (1)
    {
        LCD_Display();
    }
}


/*********************end of file*************************/








