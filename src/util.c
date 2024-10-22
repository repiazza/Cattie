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

char *szTokenName[] = {
  "TRACE_FILE",
  "DEBUG_LEVEL",
  "WINDOW_RESOLUTION",
  "WINDOW_HEIGHT",
  "WINDOW_WIDTH",
  NULL
};

/******************************************************************************
 *                                                                            *
 *                               FILE FUNCTIONS                               *
 *                                                                            *
 ******************************************************************************/

#ifdef _WIN32
  // Return 0 if error
  int iDIR_MkDir(char *szDir)
  {
    return CreateDirectory(szDir, NULL);
  }
  int iDIR_SplitFilename(char* szFilename, char* szPath, char* szName, char* szExt)
  {
    char szDrive[_MAX_DRIVE];
    char szDir[_MAX_DIR];
    _splitpath(
      szFilename,
      szDrive,
      szDir,
      szName,
      szExt
      );
    strcpy(szPath, szDrive);
    strcat(szPath, szDir);
    return 0;
  }
  int iDIR_IsDir(char* szDir)
  {
    HANDLE hArquivo;
    WIN32_FIND_DATA wfdArquivo;
    hArquivo = FindFirstFile(szDir, &wfdArquivo);
    if ( hArquivo == INVALID_HANDLE_VALUE )
      return 0;
    FindClose(hArquivo);
    if ( wfdArquivo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
      return 1;
    return 0;
  }
#else
  /*
   * Is directory: 1
   * Exists, not directory: 0
   * Does not exist: -1
   * **/ 
  int iDIR_IsDir(char* szDir)
  {
    struct stat stStat;
    if ( stat(szDir, &stStat) != 0 )
      return 0;
    if ( S_ISDIR(stStat.st_mode) )
      return 1;
    return 0;
  }
  int iDIR_SplitFilename(char* szFilename, char* szPath, char* szName, char* szExt)
  {
    char szWrk[_MAX_PATH];
    char szBase[_MAX_PATH];
    char* pszBase;
    int ii;
    strcpy(szWrk, szFilename);
    pszBase = dirname(szWrk);
    if ( pszBase == NULL ) {
      szPath[0] = 0;
    }
    else {
      strcpy(szPath, pszBase);
    }
    strcpy(szWrk, szFilename);
    pszBase = basename(szWrk);
    strcpy(szBase, pszBase);
    for ( ii = strlen(szBase)-1;  ii && szBase[ii] != '.';  ii-- ) {
      if ( szBase[ii] == '\n' || szBase[ii] == '\r' )
        szBase[ii] = 0;
    }
    if ( ii > 0 ) { // found dot
      strcpy(szExt, &szBase[ii]);
      szBase[ii] = 0;
      strcpy(szName, szBase);
    }
    else {
      strcpy(szName, szBase);
      *szExt = 0;
    }
    if ( DEBUG_MORE_MSGS ) {
      char szDbg[1024];
      sprintf(szDbg, "iDIR_SplitFilename('%s', ...) szPath=[%s] szName=[%s] szExt=[%s]",
        szFilename,
        szPath,
        szName,
        szExt
      );
      vTraceMsg(szDbg);
    }
    return 0;
  }
    // Return 0 if error
  int iDIR_MkDir(char *szDir) //linux
  {
    int iRsl;
    if ( DEBUG_LEVEL)
      vTracePid("iDIR_MkDir(%s)", strlen(szDir));
    iRsl = mkdir(szDir,
      S_IRUSR | S_IWUSR | S_IXUSR |
      S_IRGRP | S_IWGRP | S_IXGRP |
      S_IROTH
    );
    return (iRsl == 0);
  }

#endif

int bOpenFile( FILE **fppFile, const char *kpszFileName, const char *kpszMode ) {
  if ( ( *fppFile = fopen( kpszFileName, kpszMode ) ) == NULL ) {
    return FALSE;
  }

  return TRUE;
} /* bOpenFile */

int bCloseFile( FILE **fppFile ) {
  if ( *fppFile != NULL ) {
    fclose( *fppFile );
    *fppFile = NULL;

    return TRUE;
  }

  return FALSE;
} /* bCloseFile */

int bFileExist(const char *kpszFileName) {
  FILE *fpFile = NULL;
  
  if ( !bOpenFile( &fpFile, kpszFileName, "r" ) ) {
    return FALSE;
  }
  
  bCloseFile( &fpFile );

  return TRUE;
} /* bFileExist */

/******************************************************************************
 *                                                                            *
 *                              STRING FUNCTIONS                              *
 *                                                                            *
 ******************************************************************************/
int bStrIsEmpty( const char *kpszStr ) {
  if (kpszStr == NULL || !strcmp( kpszStr, "" ) || !strcmp( kpszStr, "\n" ) ) {
    return TRUE;
  }

  return FALSE;
} /* bStrIsEmpty */

/******************************************************************************
 *                                                                            *
 *                              CATTIE FUNCTIONS                              *
 *                                                                            *
 ******************************************************************************/

int iValidToken( char *pTokSearch ) {
  ENUM_CATTIE_PRM eCattiePrm = 0;

  while ( eCattiePrm != END_PRM ) {
    if ( memcmp( pTokSearch, szTokenName[eCattiePrm], strlen( szTokenName[eCattiePrm] ) ) == 0 ) {
      return eCattiePrm;
    }
    
    eCattiePrm++;
  } /* while */
  
  return TOKEN_MISMATCH;
} /* iValidToken */

int iParseCfgLine( char *pszLineContents ) {
  char *pTok     = NULL;
  char *pCh      = NULL;
  char *pToken   = NULL;
  char *pDestVar = NULL;
  int iTokenRsl  =    0;

  if ( ( pTok = strtok( pszLineContents, "\n" ) ) == NULL ) return 0;
  
  pCh = pTok;
  
  /**
   * bacagine - 2023-11-10 - Reading the .conf file
   */

  /**
   * Ignore commented lines
   */ 
  if ( *pCh == '#' ) return 0;
  
  /**
   * Ignore spaces
   */
  while ( isspace( *pCh ) ) {
    if ( *pCh == EOF ) return 0;

    pCh++;
  } /* while */

  if ( bStrIsEmpty( pCh ) ) return 0;
  
  switch ( ( iTokenRsl = iValidToken( pCh ) ) ) {
    case TRACE_FILE       : pDestVar = gstCmdLine.szTraceFile ; break;
    case DEBUG_LEVEL      : pDestVar = gstCmdLine.szDebugLevel; break;
    case WINDOW_RESOLUTION: 
    case WINDOW_HEIGHT    : 
    case WINDOW_WIDTH     : break; 
    default               : return 0;
  } /* switch */
  
  pToken = strchr( pCh, '=' );
  pToken++;
  
  /**
   * Ignore spaces
   */
  while ( isspace( *pToken ) ) pToken++;

  if ( bStrIsEmpty( pToken ) ) return 0;
  
  if ( iTokenRsl == WINDOW_RESOLUTION ) {
    strcpy( gstCmdLine.szWinWidth, strtok( pToken, "x" ) );
    strcpy( gstCmdLine.szWinHeight, strtok (NULL, "\n" ) );
  }
  else {
    snprintf( &(*pDestVar), strlen( pToken ) + 1, "%s", pToken );
  }

  return 0;
} /* iParseCfgLine */

int iCheckCfgPrm( void ) {
  if ( bStrIsEmpty( gstCmdLine.szTraceFile ) ) {

  }

  if ( bStrIsEmpty( gstCmdLine.szDebugLevel ) ) {
    strcpy( gstCmdLine.szDebugLevel, "0" );
  }

  if ( bStrIsEmpty( gstCmdLine.szWinHeight ) ) {
    strcpy ( gstCmdLine.szWinHeight, "800" );
  }

  if ( bStrIsEmpty( gstCmdLine.szWinWidth ) ) {
    strcpy(gstCmdLine.szWinWidth, "800");
  }

  return 0;
} /* iCheckCfgPrm */

/**
 * Load the cattie's parameters file.
 */
int bLoadCfgFile( const char *kpszFileName ) {
  FILE *fpFile = NULL;
  char szLine[2048];
  char szFullPath[256];

  memset( szLine, 0x00, sizeof( szLine ) );
  memset( szFullPath, 0x00, sizeof( szFullPath ) );

  sprintf(szFullPath, "%s/%s", ROOT_PATH_FROM_BIN, kpszFileName);
  
  if(!bOpenFile( &fpFile, kpszFileName, "r" ) ) {
    return FALSE;
  }
  
  while ( fgets( szLine, sizeof( szLine ), fpFile ) ) {
    if ( iParseCfgLine( szLine ) == -1 ) {
      fprintf(stderr, "E: impossible parse .conf file\n");
      
      exit(EXIT_FAILURE);
    }

    memset( szLine, 0x00, sizeof( szLine ) );
  } // while

  if ( iCheckCfgPrm() == -1 ) {
    fprintf(stderr, "E: Impossible check .conf parameters!\n");

    exit(EXIT_FAILURE);
  }

  if ( !bCloseFile( &fpFile ) ) {
    return FALSE;
  }

  return TRUE;
} /* bLoadCfgFile */

int bTerminalSupportColors( void ) {
  char *szTerm = getenv("TERM");

  if (bStrIsEmpty( szTerm ) || !strcmp( szTerm, "dumb" ) ) {
    return FALSE;
  }

  return TRUE;
} /* bTerminalSupportColors */

void vPrintWarningMessage( const char* kpszFmt, ... ) {
  va_list args;
  char *pszMsg = NULL;
  int iReallocBuffer = 0;

  va_start(args, kpszFmt);

  /**
   * Check if the terminal suport colors
   */
  if ( bTerminalSupportColors() == FALSE ) {
    iReallocBuffer = snprintf(NULL, 0, "W: %s\n", kpszFmt);

    pszMsg = ( char * ) malloc( sizeof( char ) * ( iReallocBuffer + 1 ) );

    snprintf( pszMsg, iReallocBuffer + 1, "W: %s\n", kpszFmt );

    vfprintf( stderr, pszMsg, args );

    free( pszMsg );
    pszMsg = NULL;

    va_end( args );

    return;
  }

  iReallocBuffer = snprintf( NULL, 0, "\033[1;35mW:\033[m %s\n", kpszFmt );

  pszMsg = (char *) malloc(sizeof(char) * (iReallocBuffer+1));

  snprintf( pszMsg, iReallocBuffer + 1, "\033[1;35mW:\033[m %s\n", kpszFmt );

  vfprintf( stderr, pszMsg, args );

  free( pszMsg );
  pszMsg = NULL;

  va_end( args );
} /* vPrintWarningMessage */

int bAreCoordsInSDL_Rect( SDL_Rect *pSDL_RECT, int iX, int iY ) {
  if ( pSDL_RECT == NULL ) return FALSE;

  if ( iX < 0 ||
        iY < 0 ||
        iX > INT_WINDOW_WIDTH ||
        iY > INT_WINDOW_HEIGHT ) return FALSE;

  if ( iX >= pSDL_RECT->x
        && iY >= pSDL_RECT->y
        && iX <= pSDL_RECT->x + pSDL_RECT->w
        && iY <= pSDL_RECT->y + pSDL_RECT->h ) {

    return TRUE;
  }

  return FALSE;
} /* bAreCoordsInSDL_Rect */
