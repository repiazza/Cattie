/**
 * util.h
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *  
 * My util file
 *
 * Date: 10/11/2023
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
#include "consts.h"

/******************************************************************************
 *                                                                            *
 *                             Defines and macros                             *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
 *                                                                            *
 *                  Typedefs, structures, unions and enums                    *
 *                                                                            *
 ******************************************************************************/


/******************************************************************************
 *                                                                            *
 *                     Global variables and constants                         *
 *                                                                            *
 ******************************************************************************/

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

#endif /* _UTIL_H_ */

