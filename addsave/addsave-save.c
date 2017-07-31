#include <addsave/addsave-save.h>

int addsave_g_is_savefile = 0;
char addsave_g_src_ip[ ADDSAVE_MAXPATHLEN ];

void addsave_save_pic(epan_dissect_t * edt)
{
  if(edt == NULL)
  {
    return ;
  }

  char szFilePath[ MAXPATHLEN ] = { 0 };
  char* pszFileType = NULL;
  tvbuff_t * tvb = NULL;
  u_char* pData = NULL;
  unsigned long long *pVerify = NULL;
  FILE* fpPic = NULL;
  time_t save_time = 0;
  time(&save_time);
  static int nTime = 0;
  const guchar *cp = NULL;
  guint         length = 0;

  // gboolean      multiple_sources;
  GSList       *src_le = NULL;
  struct data_source *src = NULL;


  // 将 tvb 指针移动到合适的位置 
  for( tvb = edt->tvb ; tvb != NULL; tvb = tvb->next)
  {
    // 可以确定 jgp/gif 图片肯定在最后一个数据包
    // PNG 图片在倒数第六个数据包
    // 所以直接将 jpg/gif 的指针移到最后一个数据包的位置
    if(((addsave_g_is_savefile == ADDSAVE_PIC_JPG
      || addsave_g_is_savefile == ADDSAVE_PIC_GIF
      || addsave_g_is_savefile == ADDSAVE_FILE_AUDIO)
         && tvb->next != NULL))
    {
      continue;
    }

    if(tvb->real_data == NULL)
    {
      return ;
    }

    // jpg 数据首地址在最后一个数据包地址的前两个字节的位置，png 和 gif 则正常
    pData = (unsigned char*)(tvb->real_data);
    pVerify = (unsigned long long*)tvb->real_data;
    // 再次判断，匹配则跳出循环，按照逻辑只有png才会多次判断
    if((*(unsigned short*)(pData - 2) == 0xD8FF && *pVerify == 0x4649464A1000E0FF)       // jpg
       || (*(unsigned long long*)(pData) == 0x0A1A0A0D474E5089)                          // png
       || (((*(unsigned long long*)(pData)) & 0x0000FFFFFFFFFFFF) == 0x0000613938464947) // gif
       || addsave_g_is_savefile == ADDSAVE_FILE_AUDIO   // audio 类型直接跳出
       )
    {
      break;
    }
  }


  /* 参考自 print.c -> print_hex_data 函数 */
  // 获取数据长度 和 http 数据包首部指针
  for(src_le = edt->pi.data_src; src_le != NULL;
      src_le = src_le->next)
  {
    if(src_le->next != NULL)
    {
      continue;
    }

    src = (struct data_source *)src_le->data;
    tvb = get_data_source_tvb(src);
    length = tvb_captured_length(tvb);
    if(length == 0)
      return ;
    // 获取http数据首部指针
    cp = tvb_get_ptr(tvb , 0 , length);
    
    if(cp == NULL)
    {
      return ;
    }

    break;
  }

  if(addsave_g_is_savefile == ADDSAVE_PIC_JPG)
  {
    // 偏移指针
    pData -= 2;
    pszFileType = "jpg";
  }
  else if(addsave_g_is_savefile == ADDSAVE_PIC_PNG)
  {
    pszFileType = "png";
  }
  else if(addsave_g_is_savefile == ADDSAVE_PIC_GIF)
  {
    pszFileType = "gif";
  }
  else if(addsave_g_is_savefile == ADDSAVE_FILE_AUDIO)
  {
    pszFileType = "mp3";
  }

  if(pszFileType == NULL)
  {
    return ;
  }

  sprintf_s(szFilePath , MAXPATHLEN , "%s%s\\%s\\" ,
            ADDSAVE_FILE_FLODER_NAME , addsave_g_src_ip, pszFileType);
  // 创建文件夹
  addsave_init_floder(szFilePath);

  sprintf_s(szFilePath ,
            MAXPATHLEN ,
            "%s%s\\%s\\%s_%lld.%s" ,
            ADDSAVE_FILE_FLODER_NAME ,
            addsave_g_src_ip,
            pszFileType ,
            ADDSAVE_FILE_NAME,
            save_time + nTime++,
            pszFileType);

  // 创建文件
  if(addsave_init_file(szFilePath))
  {
    return ;
  }

  // 打开文件
  fopen_s(&fpPic , szFilePath , "wb");
  if(fpPic == NULL)
  {
    return ;
  }

  // 获取文件长度
  u_int pic_length = length - (pData - cp);
  
  // 写文件
  fwrite(pData , pic_length , 1 , fpPic);
  
  // 关闭文件
  fclose(fpPic);
}
