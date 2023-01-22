#include <time.h>

#ifndef TRACE_h_
  #define TRACE_h_

  #ifdef _WIN32
    #include <windows.h>
    #define getpid _getpid 
    #define gettimeofday mingw_gettimeofday
  #elif LINUX
    #include <unistd.h>
    #include <sys/time.h>
    #define SDL_main main
    #define _MAX_PATH 256
  #endif

  void vTraceMsg(char *szMsg);
  void vTracePid(char *szMsg, int iMsgLen);
  void vInitLogs(char *argv0);
  
#endif
