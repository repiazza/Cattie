/**
 * trace.h
 * 
 * Written by Renato Fermi <repiazza@gmail.com>
 *
 * Trace functions and global variables
 */

#ifndef TRACE_h_
  #define TRACE_h_

/******************************************************************************
 *                                                                            *
 *                                 Includes                                   *
 *                                                                            *
 ******************************************************************************/ 
  #include <time.h>
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
  
/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/
  #define DEBUG_MSGS      giDebugLevel > 0
  #define DEBUG_MORE_MSGS giDebugLevel > 8

  #define vTraceVarArgs(FORMAT, ...) _vTraceVarArgs(__FILE__, __LINE__, FORMAT, ##__VA_ARGS__)
  #define vTraceBegin() vTraceVarArgs("%s - begin", __func__)
  #define vTraceEnd() vTraceVarArgs("%s - end", __func__)

/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

  extern char gszTraceFile[_MAX_PATH+_MAX_PATH];
  extern int giDebugLevel;
  extern char gszConfFile[_MAX_PATH];

/******************************************************************************
 *                                                                            *
 *                                 Prototypes                                 *
 *                                                                            *
 ******************************************************************************/ 
  /**
   * bacagine - 2023-11-10 - Read a .conf file and
   * get the value of DEBUG_LEVEL.
   * 
   * Return:
   *  If not found DEBUG_LEVEL or can't read .conf file, return 0.
   *  If can read .conf file return the value of DEBUG_LEVEL
   */
  int iGetDebugLevel(const char *kpszConfFile);

  /**
   * bacagine - 2023-11-10 - Set the name of the .conf file
   */
  void vSetConfFile(void);

  /**
   * bacagine - 2023-11-10 - Set the name of the .log file
   */
  void vSetLogFile(void);
  
  /**
   * bacagine - 2023-11-10 - Set the value of giDebugLevel
   */
  void vSetDebugLevel(void);

  void vTraceMsg(char *szMsg);
  void vTracePid(char *szMsg, int iMsgLen);
  void vTraceMsgNoNL(char *szMsg);
  void vInitLogs(void);

  /**
   * bacagine - 2023-mm-dd - Advanced trace function
   */
  void _vTraceVarArgs(const char *kpszModuleName,
                      const int kiLine,
                      const char *kpszFmt, ...);
  
  /**
   * bacagine - 2023-mm-dd A detailed and excessive sample of what was passed on the command line
   */
  void vTraceCommandLine(int argc, char **argv);

  /**
   * bacagine - 2023-mm-dd - A detailed and excessive sampling of the system's environment variables
   */
  void vTraceEnvp(char **envp);
  
#endif /* TRACE_h_*/

