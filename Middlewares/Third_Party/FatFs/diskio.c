/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"     /* FatFs lower layer API */

//#include "usbdisk.h"  /* Example: Header file of existing USB MSD control module */
//#include "atadrive.h" /* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"       /* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define SD    0
#define ATA     1   /* Example: Map ATA harddisk to physical drive 0 */
#define MMC     2   /* Example: Map MMC/SD card to physical drive 1 */
#define USB     3   /* Example: Map USB MSD to physical drive 2 */





/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    if (SD_Init() ==  RES_OK) {
//      printf("Init success\r\n\n");
        return RES_OK;
    } else {
        printf("Init error\r\n\n");
    }

    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    uint8_t result = RES_OK;
    uint32_t Ret;

    if (count == 1) {
        Ret = SD_ReadBlock((uint32_t *)buff, sector);
    } else {
        Ret = SD_ReadMultiBlocks((uint32_t *)buff, sector, count);
    }

    if (result == HAL_OK) {
        return RES_OK;
    } else {
        printf("Err: SD_ReadMultiBlocks(,%d,%d)\r\n", (uint32_t)sector, count);
        return RES_ERROR;
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write(
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    uint8_t result = RES_OK;
    uint32_t Ret;

    if (count == 1) {
        Ret = SD_WriteBlock((uint32_t *)buff, sector);
    } else {
        Ret = SD_WriteMultiBlocks((uint32_t *)buff, sector, count);
    }

    if (result == HAL_OK) {
        return RES_OK;
    } else {
        printf("Err: SD_ReadMultiBlocks(,%d,%d)\r\n", (uint32_t)sector, count);
        return RES_ERROR;
    }
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl(
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    return RES_OK;

}
#endif

//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
DWORD get_fattime(void)
{
    uint32_t date;
    date =
        (
            ((2015 - 1980) << 25)  |
            (7 <<  21) |
            (9 <<  16) |
            (12 << 11) |
            (4 << 5) |
            (0)
        );

    return date;
}

void *ff_memalloc(UINT msize)
{
    return pvPortMalloc(msize);  // FreeRTOS¶Ñ·ÖÅä
}

void ff_memfree(void *mblock)
{
    vPortFree(mblock);
}




