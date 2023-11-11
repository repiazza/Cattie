/**
 * util.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *  
 * My util file
 *
 * Date: 10/11/2023
 */

#include "util.h"

/******************************************************************************
 *                                                                            *
 *                               FILE FUNCTIONS                               *
 *                                                                            *
 ******************************************************************************/

bool bOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode)
{
  if((*fppFile = fopen(kpszFileName, kpszMode)) == NULL)
  {
    return FALSE;
  }

  return TRUE;
}

bool bCloseFile(FILE **fppFile)
{
  if(*fppFile != NULL)
  {
    fclose(*fppFile);
    *fppFile = NULL;

    return TRUE;
  }

  return FALSE;
}

bool bFileExist(const char *kpszFileName)
{
  FILE *fpFile;
  
  if(!bOpenFile(&fpFile, kpszFileName, "r"))
  {
    return FALSE;
  }
  
  bCloseFile(&fpFile);

  return TRUE;
}

/******************************************************************************
 *                                                                            *
 *                              STRING FUNCTIONS                              *
 *                                                                            *
 ******************************************************************************/
bool bStrIsEmpty(const char *kpszStr)
{
  if(kpszStr == NULL || !strcmp(kpszStr, "") || !strcmp(kpszStr, "\n"))
  {
    return TRUE;
  }

  return FALSE;
}

