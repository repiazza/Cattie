/**
 * trace.c
 * 
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Trace functions and global variables
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <trace.h>
#include <consts.h>
#include <cmdline.h>
#include <util.h>

#ifdef LINUX
  #include <sys/time.h>
  #include <sys/types.h>  
  #include <unistd.h>  
#endif /* LINUX */

char gszTraceFile[_MAX_PATH+_MAX_PATH];
int giDebugLevel = 0;
char gszConfFile[_MAX_PATH];

int giNoNL = FALSE;

void vTraceMsgNoNL(char *szMsg){
  giNoNL = TRUE;
  vTraceMsg(szMsg);
  giNoNL = FALSE;
}

void vTraceMsg(char *szMsg){
  FILE *pfLog;
  char szDateTimeNow_us[128];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  memset(szDateTimeNow_us, 0, sizeof(szDateTimeNow_us));

  snprintf(szDateTimeNow_us, sizeof(szDateTimeNow_us),
"[%02d/%02d/%04d %02d:%02d:%02d.%03ld] ",
    (int) st_tm_Now->tm_mday, 
    (int) st_tm_Now->tm_mon+1, 
    (int) st_tm_Now->tm_year+1900,
    (int) st_tm_Now->tm_hour,
    (int) st_tm_Now->tm_min,
    (int) st_tm_Now->tm_sec,
    (long)tv.tv_usec / 1000
  );  

  if ( (pfLog=fopen(gstCmdLine.szTraceFile, "a+")) == NULL )
    return;
  if ( giNoNL == TRUE )
    fprintf(pfLog, "%s", szMsg);
  else 
    fprintf(pfLog, "%s%s\n", szDateTimeNow_us, szMsg);

  fclose(pfLog);
}

void vTracePid(char *szMsg, int iMsgLen){
  char *pszMyMsg;
  int iNewMsgLen = iMsgLen + 16;
  int iPid;
  
  iPid = getpid();
  
  if ( (pszMyMsg = (char *) malloc(iNewMsgLen)) == NULL )
    return;

  memset (pszMyMsg, 0, iNewMsgLen);
  sprintf(pszMyMsg, "%d %s", iPid, szMsg);

  vTraceMsg(pszMyMsg);

  free(pszMyMsg);
}

void _vTraceVarArgs(const char *kpszModuleName,
                    const int kiLine, 
                    const char *kpszFmt, ...)
{
  va_list args;
  FILE *pfLog = NULL;
  char szDbg[2048];
  struct tm *st_tm_Now;
  struct timeval tv;
  time_t lTime;

  time(&lTime);
  st_tm_Now = localtime(&lTime);
  gettimeofday(&tv, NULL);

  memset(szDbg, 0, sizeof(szDbg));

  if((pfLog = fopen(gszTraceFile, "a")) == NULL)
  {
    fprintf(stderr, "E: Impossible create or open file %s!\n"
                    "%s\n", gszTraceFile, strerror(errno));
    exit(EXIT_FAILURE);
  }
  
  va_start(args, kpszFmt);

  snprintf(szDbg, sizeof(szDbg), 
"[%02d/%02d/%04d %02d:%02d:%02d.%03ld] %s:%d ", 
    (int) st_tm_Now->tm_mday, 
    (int) st_tm_Now->tm_mon+1, 
    (int) st_tm_Now->tm_year+1900,
    (int) st_tm_Now->tm_hour,
    (int) st_tm_Now->tm_min,
    (int) st_tm_Now->tm_sec,
    (long)tv.tv_usec / 1000,
    kpszModuleName,
    kiLine
  );

  strcat(szDbg, kpszFmt);
  strcat(szDbg, "\n");
  vfprintf(pfLog, szDbg, args);

  va_end(args);

  fclose(pfLog);
  pfLog = NULL;
}

int iGetDebugLevel(const char *kpszConfFile)
{
  FILE *fpConfFile = NULL;
  int iDebugLevel = 0;
  char szLine[256];
  int iLineLen = sizeof(szLine);
  int bFoundDebugLevel = FALSE;
  char *pTok = NULL;
  int ii = 0;
  char szLevel[2] = "";

  memset(szLine , 0, iLineLen       );
  memset(szLevel, 0, sizeof(szLevel));
  
  if((fpConfFile = fopen(kpszConfFile, "r")) == NULL)
  {
    fprintf(stderr, "E: %s %s", kpszConfFile, strerror(errno));

    return 0;
  }
  
  /**
   * bacagine - 2023-11-10 - Reading the .conf file
   */
  while(fgets(szLine, iLineLen, fpConfFile) != NULL)
  {
    /**
     * Ignore commented lines
     */
    if(strstr(szLine, "#")) continue;

    pTok = strtok(szLine, " = ");

    if(strstr(szLine, "DEBUG_LEVEL"))
    {
      bFoundDebugLevel = TRUE;
      
      /**
       * bacagine - 2023-11-10 - Getting the value after '='
       * symbom and converting form string to char.
       */
      while(pTok != NULL)
      {
        if(ii == 1)
        {
          /**
           * bacagine - 2023-11-10 - Verify if have value after '=' symbol.
           */
          if(bStrIsEmpty(pTok))
          {
            fprintf(stderr, "E: DEBUG_LEVEL is empty in file %s!\n", kpszConfFile);
              
            fclose(fpConfFile);
            fpConfFile = NULL;
            
            return 0;
          }

          snprintf(szLevel, sizeof(szLevel), "%s", pTok);
          iDebugLevel = atoi(szLevel);

          break;
        }

        pTok = strtok(NULL, " = ");

        ii++;
      } /* while pTok */

      break;
    }
    else
    {
      continue;
    }
  } /* while fgets */
  
  /**
   * If don't found the DEBUG_LEVEL 
   * variable in file print a error 
   * message to user
   */
  if(bFoundDebugLevel == FALSE)
  {
    fprintf(stderr, "E: Not found variable DEBUG_LEVEL in file %s!\n", kpszConfFile);
        
    fclose(fpConfFile);
    fpConfFile = NULL;

    return 0;
  }
  
  /**
   * If DEBUG_LEVEL in file is incorret
   */
  if(iDebugLevel < 0 || iDebugLevel > 9)
  {
    fprintf(stderr, "E: Invalid value of log level in file %s!\n", kpszConfFile);
        
    fclose(fpConfFile);
    fpConfFile = NULL;

    return 0;
  }

  fclose(fpConfFile);
  fpConfFile = NULL;

  return iDebugLevel;
}

void vSetConfFile(void)
{
#ifdef _WIN32
  int ii = 0;
#endif /* _WIN32 */

  /**
   * bacagine - 2023-11-10 - Setting the name of the configuration file
   */

  if(!bStrIsEmpty(gstCmdLine.szConfFile))
  {
    snprintf(gszConfFile, sizeof(gszConfFile), "%s", gstCmdLine.szConfFile);
    return;
  }
#ifdef LINUX
  else if(bFileExist("/etc/cattie.conf"))
  {
    snprintf(gszConfFile, sizeof(gszConfFile), "/etc/cattie.conf");
    return;
  }
  else if(bFileExist("~/.cattie.conf"))
  {
    snprintf(gszConfFile, sizeof(gszConfFile), "~/.cattie.conf");
    return;
  }
  else
  {
    snprintf(gszConfFile, sizeof(gszConfFile), "%s.conf", gkpszProgramName);
    return;
  }
#else
  else
  {
    while(gkpszProgramName[ii] != '.')
    {
      gszConfFile[ii] = gkpszProgramName[ii];
      ii++;
    }

    strcat(gszConfFile, ".conf");
  }
#endif /* LINUX */
}

void vSetLogFile(void)
{
#ifdef _WIN32
  int ii = 0;
#endif /* _WIN32 */

  /**
   * bacagine - 2023-11-10 - Setting the name of the log file
   */

  if(!bStrIsEmpty(gstCmdLine.szTraceFile))
  {
    snprintf(gszTraceFile, (size_t) sizeof(gszTraceFile)-8, "%s", gstCmdLine.szTraceFile);
    return;
  }
#ifdef LINUX
  else
  {
    snprintf(gszTraceFile, sizeof(gszTraceFile), "%s.log", gkpszProgramName);
    return;
  }
#else
  else
  {
    while(gkpszProgramName[ii] != '.')
    {
      gszTraceFile[ii] = gkpszProgramName[ii];
      ii++;
    }
    strcat(gszTraceFile, ".log");  
  }
#endif /* LINUX */
}

void vSetDebugLevel(void)
{
  /**
   * bacagine - 2023-11-10 - Getting the debug level
   */
  if(!bStrIsEmpty(gstCmdLine.szDebugLevel))
  {
    giDebugLevel = atoi(gstCmdLine.szDebugLevel);
  }
  else
  {
    giDebugLevel = iGetDebugLevel(gszConfFile);
  }
}

void vInitLogs(void){
  memset(gszTraceFile, 0, sizeof(gszTraceFile));
  memset(gszConfFile , 0, sizeof(gszConfFile));
  
  vSetConfFile();

  vSetLogFile();
  
  vSetDebugLevel();
}

