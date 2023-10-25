#include <time.h>

#ifndef TRACE_h_
  #define TRACE_h_
  
  #include <errno.h>
  #include <stdarg.h>

  #ifdef _WIN32
    #include <windows.h>
    #include <process.h>
  #elif LINUX
    #include <unistd.h>
    #include <sys/time.h>
  #endif
  
  #ifndef _CONSTS_H_
    #include "consts.h"
  #endif
  
  #define DEBUG_MSGS      1
  #define DEBUG_MORE_MSGS 9

  #define vTraceVarArgs(FORMAT, ...) _vTraceVarArgs(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__)
  #define vTraceBegin() vTraceVarArgs("%s - begin", __func__)
  #define vTraceEnd() vTraceVarArgs("%s - end", __func__)

  extern char gszTraceFile[_MAX_PATH+_MAX_PATH];
  
  bool bStrIsEmpty(const char *kpszStr);
  void vTraceMsg(char *szMsg);
  void vTracePid(char *szMsg, int iMsgLen);
  void vTraceMsgNoNL(char *szMsg);
  void vInitLogs(void);

  /**
   * 
   */
  void _vTraceVarArgs(const char *kpszModuleName,
                      const int kiLine,
                      const char *kpszFmt, ...);
  
  /**
   * A detailed and excessive sample of what was passed on the command line
   */
  void vTraceCommandLine(int argc, char **argv);

  /**
   * \brief A detailed and excessive sampling of the system's environment variables
   */
  void vTraceEnvp(char **envp);
  
#endif

