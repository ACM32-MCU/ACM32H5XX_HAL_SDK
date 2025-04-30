#include "fatfs_app.h"

//文件类型列表
const char *File_type_table[6][13]=
{
    {"BIN","HEX"},		 
    {"LRC"},			     
    {"NES"},			     
    {"TXT","C","H"},	 
    {"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},
    {"BMP","JPG","JPEG","GIF"},
};


#define printstrCom(str) printf("%s\r\n",str)


uint8_t char_upper(uint8_t c)
{
    if(c<'A')return c;
    if(c>='a')return c-0x20;
    else return c;
}

//获取容量大小
uint8_t mf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free)
{
    FATFS *fs1;
    uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    res = f_getfree((const TCHAR*)drv, (DWORD *)&fre_clust, &fs1);
    if(res==0)
    {
        tot_sect=(fs1->n_fatent-2)*fs1->csize;	
        fre_sect=fre_clust*fs1->csize;			
#if FF_MAX_SS != FF_MIN_SS				  				
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
#endif
        *total=tot_sect>>1;	
        *free=fre_sect>>1;	
    }
    return res;
}


uint8_t f_typetell(uint8_t *fname)
{
    uint8_t tbuf[6];
    uint8_t i=0,j;
	 
	  while((*fname++ != '\0') && i<250)i++;
		if(i==250)return 0XFF;    
	   
	  i=0;
	  while(*fname !='.')
		{
		  fname--;
			if(++i>6)return 0XFF;
		}
	
		strcpy((char *)tbuf,(const char*)(fname+1));
		for(i=0; i<strlen((const char*)tbuf); i++)tbuf[i]=char_upper(tbuf[i]); 
    
    for(i=0; i<6; i++)
    {
        for(j=0; j<13; j++)
        {
            if(*File_type_table[i][j]==0)break;
            if(strcmp((const char *)File_type_table[i][j],(const char *)tbuf)==0)
            {
                return (i<<4)|j;
            }
        }
    }
    return 0XFF;
}

/*
path:路径
ftpye：类型变化
*/
uint16_t f_getflietnum(char *path,uint8_t ftpye)
{
    uint8_t res;
    uint16_t rval=0;
    DIR tdir;	 		
    FILINFO tfileinfo;	
    res=f_opendir(&tdir,(const TCHAR*)path); 					
    if(res==FR_OK)
    {
        while(1)
        {
            res=f_readdir(&tdir,&tfileinfo);       		
            if(res != FR_OK || tfileinfo.fname[0] == 0)break;	
            res=f_typetell((uint8_t*)tfileinfo.fname);
            if(res == ftpye)
            {
                rval++;
            }
        }
    }
    return rval;
}

FATFS myfs;
void SD_Fatfs_Init()
{
	__IO uint32_t total,free;
	__IO uint16_t filenum = 0;
	char dispbuf[100];
	if(FR_OK !=SD_Init()) 
	{
		printstrCom((uint8_t*)"sd init err!!!!!!!!!!!!!!!!!!!");
		return ;
	}
	printstrCom((uint8_t*)"sd init ok!\r\n");
	if(FR_OK != f_mount(&myfs, "0:", 1)) 
	{  	 
		printstrCom((uint8_t*)"sd mount err!!!!!!!!!!!!!!!!!!!");
		return ;
	}
	mf_getfree("/",&total,&free);
	filenum = f_getflietnum("/",0x42);
	memset(dispbuf,0,sizeof(dispbuf));
	sprintf(dispbuf,"sd mount ok! disk total size:%f M,free size:%f M.\r\n",total / 1024.,free / 1024.); 
	printstrCom((uint8_t*)dispbuf); 
}

void Fatfs_TestMy(void)
{
	FATFS myfs1;
	FIL fp;
	uint32_t bw;
	char write_buff[32] = {"上海航芯科技有限公司"};
	char read_buff[32];
	if(FR_OK !=SD_Init())/* 初始化SD卡 */
	{
		printstrCom("sd init err !!!\r\n");
		return ;
	}
	if(f_mount(&myfs1,"0:",1) != FR_OK)
	{
		printstrCom("fatfs mount err !!!\r\n");
		return ;
	}
	if(f_open(&fp,"test.txt",FA_WRITE|FA_READ|FA_OPEN_ALWAYS)!= FR_OK)
	{
		printstrCom("文件打开失败\r\n");
		return ;
	}
	printstrCom("文件打开成功\r\n");

	if(f_write(&fp,write_buff,20,&bw) != FR_OK)
	{
		printstrCom("文件写入失败\r\n");
		return ;
	}
	printstrCom("文件写入成功\r\n");
	
	if(f_lseek(&fp,0) != FR_OK)/* 移动读/写指针到文件首 */
	{
		printstrCom("移动指针失败\r\n");
		return ;
	}
	if(f_read(&fp,read_buff,20,&bw) != FR_OK)
	{
		printstrCom("文件读取失败\r\n");
		return ;
	}
	printstrCom("文件读取成功\r\n");
	printf("写入内容：%s\r\n",read_buff);
	f_close(&fp);
}

