/* Ñ¡ÔñIPÐÂÔö start */
#include <kevins/kevins-file.h>

void kevins_init_floder(char* szFloder)
{
  char szCmd[ KEVINS_MAXPATHLEN ] = { 0 };
  sprintf_s(szCmd , KEVINS_MAXPATHLEN ,
            "if not exist \"%s\" (md \"%s\")" , szFloder, szFloder);
  system(szCmd);
}

int kevins_init_file(char* szFile)
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
