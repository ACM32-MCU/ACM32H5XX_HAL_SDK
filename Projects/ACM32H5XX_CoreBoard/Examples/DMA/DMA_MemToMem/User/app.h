/******************************************************************************
*@file  : app.h
*@brief : application
******************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "hal.h"
#include "acm32h5xx_coreboard.h" 

typedef struct
{
    uint32_t    Output; 
    char        *str;
}MCO_OutputTypedef;

void APP_Test(void);

#endif /* __APP_H */

