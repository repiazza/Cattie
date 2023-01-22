#ifndef TRACE_h_
  #define TRACE_h_
  
  #ifndef _WIN32
    #define _MAX_PATH 256
  #endif
  void vTraceMsg(char *szMsg);
  void vTracePid(char *szMsg, int iMsgLen);
  void vInitLogs(char *argv0);
  extern char gszLogTitle[_MAX_PATH];
  
#endif