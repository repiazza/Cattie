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

char *szTokenName[] = {
  "TRACE_FILE",
  "DEBUG_LEVEL",
  "WINDOW_HEIGHT",
  "WINDOW_WIDTH",
  NULL
};

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
  ENUM_CATTIE_PRM eCattiePrm = 0;
  
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
      case TRACE_FILE   : pDestVar = gstCmdLine.szTraceFile ; break;
      case DEBUG_LEVEL  : pDestVar = gstCmdLine.szDebugLevel; break;
      case WINDOW_HEIGHT: pDestVar = gstCmdLine.szWinHeight ; break;
      case WINDOW_WIDTH : pDestVar = gstCmdLine.szWinWidth  ; break;
      default           : break;
    }
    
    puts(pToken);

    snprintf(&(*pDestVar), _MAX_PATH, "%s", strtok(NULL, "\n"));
    printf("%s\n", pDestVar);

    pTok = strtok(NULL, "\n");
  } /* while pTok != NULL */

  return 0;
}

int iCheckCfgPrm(void)
{
  if(bStrIsEmpty(gstCmdLine.szTraceFile))
  {
    
  }

  if(bStrIsEmpty(gstCmdLine.szDebugLevel))
  {
    strcpy(gstCmdLine.szDebugLevel, "0");
  }

  if(bStrIsEmpty(gstCmdLine.szWinHeight))
  {
    strcpy(gstCmdLine.szWinHeight, "800");
  }

  if(bStrIsEmpty(gstCmdLine.szWinWidth))
  {
    strcpy(gstCmdLine.szWinWidth, "800");
  }

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

  fseek(fpFile, 0, SEEK_END);

  lSize = ftell(fpFile);

  rewind(fpFile);

  if ( (pszFileContents = (char *) malloc(lSize + 8)) == NULL )
  {
    fprintf(stderr, "E: (%s) %s", kpszFileName, strerror(errno));

    exit(EXIT_FAILURE);
  }

  if(iParseCfgFile(pszFileContents) == -1)
  {
    fprintf(stderr, "E: impossible parse .conf file\n");

    exit(EXIT_FAILURE);
  }

  if(iCheckCfgPrm() == -1)
  {
    fprintf(stderr, "E: Impossible check .conf parameters!\n");

    exit(EXIT_FAILURE);
  }

  if(!bCloseFile(&fpFile))
  {
    return FALSE;
  }

  return TRUE;
}

