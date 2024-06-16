/**
 * util.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in november 2023
 * Maintained by Renato Fermi <repiazza@gmail.com> since february 2024
 * 
 * Util functions and procedures header file
 * 
 */

#ifndef _UTIL_H_
  #define _UTIL_H_

  /******************************************************************************
   *                                                                            *
   *                                 Includes                                   *
   *                                                                            *
   ******************************************************************************/
  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include <consts.h>
  #include <cmdline.h>
  #ifdef LINUX
    #include <sys/stat.h>
    #include <libgen.h>
  #endif
  /******************************************************************************
   *                                                                            *
   *                             Defines and macros                             *
   *                                                                            *
   ******************************************************************************/
  #define TOKEN_MISMATCH -1

  /******************************************************************************
   *                                                                            *
   *                  Typedefs, structures, unions and enums                    *
   *                                                                            *
   ******************************************************************************/

  /**
   * Used in cattie.conf file parse
   */
  typedef enum ENUM_CATTIE_PRM
  {
    TRACE_FILE = 0,
    DEBUG_LEVEL,
    WINDOW_RESOLUTION,
    WINDOW_HEIGHT,
    WINDOW_WIDTH,
    END_PRM
  } ENUM_CATTIE_PRM, *PENUM_CATTIE_PRM;

  /******************************************************************************
   *                                                                            *
   *                     Global variables and constants                         *
   *                                                                            *
   ******************************************************************************/

  extern char *szTokenName[];

  /******************************************************************************
   *                                                                            *
   *                            Prototype functions                             *
   *                                                                            *
   ******************************************************************************/

  /******************************************************************************
   *                                                                            *
   *                               FILE FUNCTIONS                               *
   *                                                                            *
   ******************************************************************************/

  /**
   * Open a file
   *
   * fppFile: file pointer
   * kpszFileName: the name of file
   * kpszMode: mode used in fopen
   */
  int bOpenFile( FILE **fppFile, const char *kpszFileName, const char *kpszMode );

  /**
   * Close file safety
   */
  int bCloseFile( FILE **fppFile );

  /**
   * Check if file exists
   */
  int bFileExist( const char *kpszFileName );

  /******************************************************************************
   *                                                                            *
   *                              STRING FUNCTIONS                              *
   *                                                                            *
   ******************************************************************************/

  /**
   * Check if string is empty
   */
  int bStrIsEmpty( const char *kpszStr );

  /******************************************************************************
   *                                                                            *
   *                              CATTIE FUNCTIONS                              *
   *                                                                            *
   ******************************************************************************/

  /**
   * 
   */
  int iValidToken( char *pTokSearch );

  /**
   * Parse the cattie.conf file
   *
   * int iParseCfgFile( char *pszFileContents );
  */
   
  /**
   *
   */
  int iCheckCfgPrm( void );

  /**
   * Load the cattie's parameters file.
   */
  int bLoadCfgFile( const char *kpszFileName );

  /**
   * Check if your terminal have support a colored text
   */
  int bTerminalSupportColors( void );

  /**
   * Print a formatted error message
   */
  void vPrintErrorMessage( const char *kpszFmt, ... );

  /**
   *
   */
  int bAreCoordsInSDL_Rect( SDL_Rect *pSDL_RECT, int iX, int iY );

  #endif /* _UTIL_H_ */

