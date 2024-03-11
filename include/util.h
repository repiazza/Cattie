/**
 * util.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com> in november 2023
 * Maintained by Renato Fermi <repiazza@gmail.com> since february 2024
 * 
 * Util functions and procedures header file
 * 
 */

#ifndef _UTIL_H
  #define _UTIL_H

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
  bool bOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode);

  /**
   * Close file safety
   */
  bool bCloseFile(FILE **fppFile);

  /**
   * Check if file exists
   */
  bool bFileExist(const char *kpszFileName);

  /******************************************************************************
   *                                                                            *
   *                              STRING FUNCTIONS                              *
   *                                                                            *
   ******************************************************************************/

  /**
   * Check if string is empty
   */
  bool bStrIsEmpty(const char *kpszStr);

  /******************************************************************************
   *                                                                            *
   *                              CATTIE FUNCTIONS                              *
   *                                                                            *
   ******************************************************************************/

  /**
   * 
   */
  int iValidToken(char *pTokSearch);

  /**
   * Parse the cattie.conf file
   */
  int iParseCfgFile(char *pszFileContents);

  /**
   *
   */
  int iCheckCfgPrm(void);

  /**
   * Load the cattie's parameters file.
   */
  bool bLoadCfgFile(const char *kpszFileName);

  /**
   * Check if your terminal have support a colored text
   */
  bool bTerminalSupportColors(void);

  /**
   * Print a formatted error message
   */
  void vPrintErrorMessage(const char *kpszFmt, ...);

  #endif /* _UTIL_H */

