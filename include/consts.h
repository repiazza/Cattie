/**
 * consts.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in November 2023
 *  
 * Constants and OS env variables.
 *
 */

#ifndef _CONSTS_H_
  #define _CONSTS_H_

  #ifdef _WIN32
    #include <windows.h>
    #define getpid _getpid 
    #define gettimeofday mingw_gettimeofday
  #elif LINUX
    #include <unistd.h>
    #define SDL_main main
    #define _MAX_PATH 256
  #endif

  #ifndef FALSE
    #define FALSE 0
  #endif /* FALSE */

  #ifndef TRUE
    #define TRUE  1
  #endif /* TRUE */

  /**
   * This macro is used to remove
   * unused warnings during the
   * compilation.
   */
  #define UNUSED(X) (void) X
  
  #define CATTIE_INSTALL_PATH "/usr/share/cattie"

  #define MILLISECOND 1
  #define SECOND (1000 * MILLISECOND)

  /******************************************************************************
   *                                                                            *
   *                  Typedefs, structures, unions and enums                    *
   *                                                                            *
   ******************************************************************************/
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
  typedef int bool;
#endif

  /******************************************************************************
   *                                                                            *
   *                     Global variables and constants                         *
   *                                                                            *
   ******************************************************************************/

  /* Receive the name of program */
  extern const char *gkpszProgramName;

#endif /* _CONSTS_H_ */

