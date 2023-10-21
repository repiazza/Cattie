#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <trace.h>

#ifndef TRUE
  #define TRUE 1
#endif /* TRUE */

#ifndef FALSE
  #define FALSE 0
#endif /* FALSE */

#ifdef LINUX
  #include <sys/time.h>
  #include <sys/types.h>  
  #include <unistd.h>  
#endif /* LINUX */

char gszLogTitle[_MAX_PATH];

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

  if ( (pfLog=fopen(gszLogTitle, "a+")) == NULL )
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

void vInitLogs(char *argv0){
  char *pTok;
  
  memset  (gszLogTitle, 0, sizeof(gszLogTitle));
  snprintf(gszLogTitle, (size_t)sizeof(gszLogTitle)-8, "%s", argv0);
  if ( (pTok = strstr(gszLogTitle, ".exe")) != NULL ){
    *pTok = 0;
  }
  strcat(gszLogTitle, ".log");
}

