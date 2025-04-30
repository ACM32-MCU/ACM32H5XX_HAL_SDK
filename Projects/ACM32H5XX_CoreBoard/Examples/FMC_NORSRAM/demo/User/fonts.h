#ifndef __FONT_H
#define __FONT_H       

#include "stdint.h"


typedef struct Font
{    
    const uint8_t *table;
    uint16_t Width;
    uint16_t Height;
}FONT_SelectTypeDef;

extern FONT_SelectTypeDef gfont24x32;
extern FONT_SelectTypeDef gfont16x24;
extern FONT_SelectTypeDef gfont8x16;




#endif 
