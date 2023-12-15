/**
 * util.c
 *
 * Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
 *  
 * My util file
 *
 * Date: 10/11/2023
 */

#include <util.h>

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

/******************************************************************************
 *                                                                            *
 *                              CATTIE FUNCTIONS                              *
 *                                                                            *
 ******************************************************************************/

int iValidToken(char *pTokSearch)
{
  ENUM_CATTIE_PRM eCattiePrm;
  
  while(eCattiePrm != END_PRM)
  {
    if(strcasecmp(pTokSearch, szTokenName[eCattiePrm]))
    {
      return eCattiePrm;
    }
    
    eCattiePrm++;
  }

  return TOKEN_MISMATCH;
}

int iParseCfgFile(char *pszFileContents)
{
  char *pTok = NULL;

  pTok = strtok(pszFileContents, "\n");

  /**
   * bacagine - 2023-11-10 - Reading the .conf file
   */
  while(pTok != NULL)
  {
    char *pCh = pTok;
    char *pToken = NULL;
    char *pDestVar = NULL;

    /**
     * Ignore spaces
     */
    while(isspace(*pCh));
    
    /**
     * Ignore commented lines
     */
    if(*pCh == '#' || (pToken = strtok(pCh, "=")) == NULL)
    {
      pTok = strtok(NULL, "\n");

      continue;
    }
    
    switch(iValidToken(pToken))
    {
      case TRACE_FILE   : pDestVar = stCmdLine.szTrace     ; break;
      case DEBUG_LEVEL  : pDestVar = stCmdLine.szDebugLevel; break;
      case WINDOW_HEIGTH: pDestVar = stCmdLine.szWinHeigth ; break;
      case WINDOW_WIDTH : pDestVar = stCmdLine.szWinWidth  ; break;
      default           : break;
    }

    snprintf(&(*pDestVar), _MAX_PATH, "%s", strtok(NULL, "\n"));

    pTok = strtok(NULL, "\n");
  } /* while pTok != NULL */

  return 0;
}

/**
 * Load the cattie's parameters file.
 */
bool bLoadCfgFile(const char *kpszFileName)
{
  FILE *fpFile = NULL;
  long lSize = 0L;
  char *pszFileContents = NULL;

  if(!bOpenFile(&fpFile, kpszFileName, "r"))
  {
    return FALSE;
  }

  fseek(pfFIle, 0, SEEK_END);

  lSize = ftell(pfFile);

  rewind(pfFile);

  if ( (pszFileContents = (char *) malloc(lSize + 8)) == NULL )
  {
    fprintf(stderr, "E: (%s) %s", kpszFileName, strerror(errno));

    exit(EXIT_FAILURE);
  }

  if(iParseCfgFile(pszFileContents) == -1)
  {
    fprintf(stderr, "Deu pau :(");

    exit(EXIT_FAILURE);
  }

  if(!bCloseFile(fpFileName))
  {
    return FALSE;
  }

  return TRUE;
}

