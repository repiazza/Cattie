#ifndef TRACE_h_
  #define TRACE_h_

  void vTraceMsg(char *szMsg);
  void vTracePid(char *szMsg, int iMsgLen);
  void vInitLogs(char *argv0);
  #define _MAX_PATH 256
  extern char gszLogTitle[_MAX_PATH];
  

#endif