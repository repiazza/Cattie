#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <trace.h>
#include "consts.h"
#include "cmdline.h"

#ifdef LINUX
  #include <sys/time.h>
  #include <sys/types.h>  
  #include <unistd.h>  
#endif /* LINUX */

char gszTraceFile[_MAX_PATH+_MAX_PATH];

int giNoNL = FALSE;

bool bStrIsEmpty(const char *kpszStr)
{
  if(kpszStr == NULL || !strcmp(kpszStr, "") || !strcmp(kpszStr, "\n"))
  {
    return TRUE;
  }

  return FALSE;
}


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
  sprintf(szDateTimeNow_us,
"[%02d/%02d/%04d %02d:%02d:%02d.%3.3ld] ",
    (int) st_tm_Now->tm_mday, 
    (int) st_tm_Now->tm_mon+1, 
    (int) st_tm_Now->tm_year+1900,
    (int) st_tm_Now->tm_hour,
    (int) st_tm_Now->tm_min,
    (int) st_tm_Now->tm_sec,
    (long)tv.tv_sec
  );  

  if ( (pfLog=fopen(stCmdLine.szTraceFile, "a+")) == NULL )
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

  memset(szDbg , 0, sizeof(szDbg));

  if((pfLog = fopen(gszTraceFile, "a")) == NULL)
  {
    fprintf(stderr, "E: Impossible create or open file %s!\n"
                    "%s\n", gszTraceFile, strerror(errno));
    exit(EXIT_FAILURE);
  }
  
  va_start(args, kpszFmt);

  sprintf(szDbg, "[%02d/%02d/%04d %02d:%02d:%02d.%3.3ld] %s:%d ", (int) st_tm_Now->tm_mday, 
                                                                  (int) st_tm_Now->tm_mon+1, 
                                                                  (int) st_tm_Now->tm_year+1900,
                                                                  (int) st_tm_Now->tm_hour,
                                                                  (int) st_tm_Now->tm_min,
                                                                  (int) st_tm_Now->tm_sec,
                                                                  (long)tv.tv_sec,
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

void vInitLogs(void){
  char *pTok;
  
  memset  (gszTraceFile, 0, sizeof(gszTraceFile));

  if(!bStrIsEmpty(stCmdLine.szTraceFile))
  {
    snprintf(gszTraceFile, (size_t) sizeof(gszTraceFile)-8, "%s", stCmdLine.szTraceFile);
    return;
  }
  
  snprintf(gszTraceFile, (size_t) sizeof(gszTraceFile)-8, "%s", gkpszProgramName);

  if ( (pTok = strstr(gkpszProgramName, ".exe")) != NULL )
  {
    *pTok = 0;
  }

  strcat(gszTraceFile, ".log");
}

