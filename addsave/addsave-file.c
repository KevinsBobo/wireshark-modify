/* Ñ¡ÔñIPÐÂÔö start */
#include <addsave/addsave-file.h>

void addsave_init_floder(char* szFloder)
{
  char szCmd[ ADDSAVE_MAXPATHLEN ] = { 0 };
  sprintf_s(szCmd , ADDSAVE_MAXPATHLEN ,
            "if not exist \"%s\" (md \"%s\")" , szFloder, szFloder);
  system(szCmd);
}

int addsave_init_file(char* szFile)
{
  FILE* fp = NULL;
  fopen_s(&fp , szFile, "a+");
  if(fp != NULL)
  {
    fclose(fp);
    fp = NULL;
    return 0;
  }

  return -1;
}
