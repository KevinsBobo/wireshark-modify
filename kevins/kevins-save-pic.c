#include <kevins/kevins-save-pic.h>

int kevins_g_is_pic = 0;
char kevins_g_src_ip[ KEVINS_MAXPATHLEN ];

void kevins_save_pic(epan_dissect_t * edt)
{
  if(edt == NULL)
  {
    return ;
  }

  char szPicPath[ MAXPATHLEN ] = { 0 };
  tvbuff_t * tvb = NULL;
  u_char* pData = NULL;
  unsigned long long *pVerify = NULL;
  FILE* fpPic = NULL;
  static int g_kevins_save_pic_time = 0;
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
    if(((kevins_g_is_pic == KEVINS_PIC_JPG || kevins_g_is_pic == KEVINS_PIC_GIF)
         && tvb->next != NULL))
    {
      continue;
    }

    if(tvb->real_data == NULL)
    {
      return ;
    }

    // jpg 数据首地址在最后一个数据包地址的前两个字节的位置，png 和 gif 则正常
    pData = (unsigned char*)(tvb->real_data) - 2;
    pVerify = (unsigned long long*)tvb->real_data;
    // 再次判断，匹配则跳出循环，按照逻辑只有png才会多次判断
    if((*(unsigned short*)pData == 0xD8FF && *pVerify == 0x4649464A1000E0FF)                 // jpg
       || (*(unsigned long long*)(pData + 2) == 0x0A1A0A0D474E5089)                          // png
       || (((*(unsigned long long*)(pData + 2)) & 0x0000FFFFFFFFFFFF) == 0x0000613938464947) // gif
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

  if(kevins_g_is_pic == KEVINS_PIC_JPG)
  {
    sprintf_s(szPicPath , MAXPATHLEN , "%s%s\\jpg\\" ,
              KEVINS_PIC_FLODER_NAME , kevins_g_src_ip);
    // 创建文件夹
    kevins_init_floder(szPicPath);

    sprintf_s(szPicPath , MAXPATHLEN , "%s%s\\jpg\\%s%d.jpg" ,
              KEVINS_PIC_FLODER_NAME , kevins_g_src_ip, KEVINS_PIC_FILE_NAME, g_kevins_save_pic_time++);
  }
  else if(kevins_g_is_pic == KEVINS_PIC_PNG)
  {
    // 偏移指针
    pData += 2;
    sprintf_s(szPicPath , MAXPATHLEN , "%s%s\\png\\" ,
              KEVINS_PIC_FLODER_NAME , kevins_g_src_ip);
    // 创建文件夹
    kevins_init_floder(szPicPath);

    sprintf_s(szPicPath , MAXPATHLEN , "%s%s\\png\\%s%d.png" ,
              KEVINS_PIC_FLODER_NAME , kevins_g_src_ip, KEVINS_PIC_FILE_NAME, g_kevins_save_pic_time++);
  }
  else if(kevins_g_is_pic == KEVINS_PIC_GIF)
  {
    // 偏移指针
    pData += 2;
    sprintf_s(szPicPath , MAXPATHLEN , "%s%s\\gif\\" ,
              KEVINS_PIC_FLODER_NAME , kevins_g_src_ip);
    // 创建文件夹
    kevins_init_floder(szPicPath);

    sprintf_s(szPicPath , MAXPATHLEN , "%s%s\\gif\\%s%d.gif" ,
              KEVINS_PIC_FLODER_NAME , kevins_g_src_ip, KEVINS_PIC_FILE_NAME, g_kevins_save_pic_time++);
  }

  // 创建文件
  if(kevins_init_file(szPicPath))
  {
    return ;
  }

  // 打开文件
  fopen_s(&fpPic , szPicPath , "wb");
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
