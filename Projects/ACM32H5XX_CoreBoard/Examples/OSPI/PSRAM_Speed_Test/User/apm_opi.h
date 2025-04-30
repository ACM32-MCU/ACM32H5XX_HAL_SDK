
#ifndef __APM_OPI_H
#define __APM_OPI_H

#include "hal.h"


#define OTX_BUFF_LEN            (256)
#define OTX_HALF_BUFF_LEN       (OTX_BUFF_LEN/2)
#define OTX_QUARTER_UFF_LEN     (OTX_BUFF_LEN/4)
#define OTX_DATA_16BIT          (0x1616)
#define OTX_DATA_32BIT          (0x32323232)


/* APM OPI PSRAM Command */
#define APMOPI_SYNC_READ                   (0x00)
#define APMOPI_SYNC_WRITE                  (0x80)
#define APMOPI_SYNC_READ_LINEAR_BURST      (0x20)
#define APMOPI_SYNC_WRITE_LINEAR_BURST     (0xA0)
#define APMOPI_MODE_REGISTER_READ          (0x40)
#define APMOPI_MODE_REGISTER_WRITE         (0xC0)
#define APMOPI_GLOBAL_RESET                (0xFF)

/* Control Registers Address */
#define APMOPI_MA_ADDR_0                   (0x00)
#define APMOPI_MA_ADDR_1                   (0x01)
#define APMOPI_MA_ADDR_2                   (0x02)
#define APMOPI_MA_ADDR_3                   (0x03)
#define APMOPI_MA_ADDR_4                   (0x04)
#define APMOPI_MA_ADDR_8                   (0x08)


void APS6408L_OTX_BJ_Fifo_Test(void);
void APS6408L_OTX_OSPI_DTR_Memory_Test(void);



#endif /* __APP_H */

