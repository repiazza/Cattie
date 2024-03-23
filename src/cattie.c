/**
 * cattie.c
 *
 * Written by Renato Fermi <repizza@gmail.com>
 */

#include <inttypes.h>
#include <time.h>
#include <cattie.h>
#include <board.h>
#include <menu.h>
#include <action.h>
#include <button.h>
#include <consts.h>
#include <texture.h>
#include <cmdline.h>
#include <util.h>
#include <image.h>
#include <sl.h>

#ifdef _WIN32
  #include <windows.h>
#elif LINUX
  #include <unistd.h>
#endif

/**
 * 
 * Globals
 * 
 **/

STRUCT_BUTTON_LIST gstButtonList;
STRUCT_TEXTURE_LIST gstTextureList;
int giBOARD_Main[BOARD_ROWS][BOARD_COLS];
int giDeg = 0;
int gbRunning = TRUE;
int gbACTION_Check = FALSE;
int giACTION_List[_MAX_MOV_ACTION];
int giACTION_StepCtr = 0;
int giACTION_AssertedSteps = 0;
int giMENU_SelectedItem = 0;
STRUCT_PLAYER gstPlayer;

/* Receive the name of program */
const char *gkpszProgramName;

STRUCT_COMMAND_LINE gstCmdLine;

SDL_Rect *pSDL_RECT_Menu = NULL;

/**
 * 
 * Procedures and functions
 * 
 **/
void vInitRect( SDL_Rect *pSDL_RECT, int iX, int iY, int iWidth, int iHeight ) {
  if( DEBUG_MSGS ) vTraceBegin();
  
  pSDL_RECT->x = iX;
  pSDL_RECT->y = iY;
  pSDL_RECT->w = iWidth;
  pSDL_RECT->h = iHeight;

  if( DEBUG_MORE_MSGS )
  {
    vTraceVarArgs(
      "\n\t\tpSDL_RECT->x = %d | pSDL_RECT->y = %d"
      "\n\t\tpSDL_RECT->w = %d | pSDL_RECT->h = %d",
      (pSDL_RECT->x), (pSDL_RECT->y),
      (pSDL_RECT->w), (pSDL_RECT->h)
    );
  }

  if ( DEBUG_MSGS ) vTraceEnd();
} /* vInitRect */

SDL_Surface *pSDL_SRFC_LoadImage( char *pszImgPath ) {
  // Load the image
  SDL_Surface *SDL_SRFC_Img = IMG_Load(pszImgPath);
  
  if ( DEBUG_MSGS ) vTraceBegin();

  if ( SDL_SRFC_Img == NULL ) {
    printf("Error loading image: %s\n", IMG_GetError());

    if ( DEBUG_MORE_MSGS ) vTraceVarArgs("%s - end return NULL", __func__);

    return NULL;
  }

  vTraceEnd();

  return SDL_SRFC_Img;
} /* pSDL_SRFC_LoadImage */

void vDrawCommandHUD( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Hud ) {
  if( DEBUG_MSGS ) vTraceBegin();

  SDL_SetRenderTarget( renderer, NULL );
  SDL_SetRenderDrawColor( renderer, 17, 84, 143, 96 );
  SDL_RenderFillRect( renderer, pSDL_RECT_Hud );
  SDL_SetRenderDrawColor( renderer, 17, 84, 143, 255 );
  SDL_RenderDrawRect( renderer, pSDL_RECT_Hud );
  
  if( DEBUG_MSGS ) vTraceEnd();
} /* vDrawCommandHUD */

void vDrawButtonHUD( SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Hud ) {
  if( DEBUG_MSGS ) vTraceBegin();

  SDL_SetRenderTarget( renderer, NULL );
  SDL_SetRenderDrawColor( renderer, 128, 4, 0, 128 );
  SDL_RenderFillRect( renderer, pSDL_RECT_Hud );
  SDL_SetRenderDrawColor( renderer, 128, 0, 0, 255 );
  SDL_RenderDrawRect( renderer, pSDL_RECT_Hud );

  if( DEBUG_MSGS ) vTraceEnd();
} /* vDrawButtonHUD */

void vInitializeImagePosition( SDL_Rect *pSDL_Rect_Im ) {
  int iLocation = -2;
  int iDimensions = COL_RATIO;

  if( DEBUG_MSGS ) vTraceBegin();

  vInitRect( pSDL_Rect_Im, iLocation, iLocation, iDimensions, iDimensions );
  
  if( gstPlayer.iFacingPos == SOUTH ) giDeg += 90;

  if( DEBUG_MSGS ) vTraceEnd();
} /* vInitializeImagePosition */

eSqType iValidateSquare( int iX, int iY ) {
  eSqType eSquareType = -1;
  
  if( DEBUG_MSGS ) vTraceBegin();

  if ( iX < 0 || iX >= BOARD_ROWS )
    return eSquareType;

  if ( DEBUG_MSGS ) { 
    char szMsg[256] = "";

    memset( szMsg, 0x00, sizeof( szMsg ) );

    sprintf( szMsg,
      "iValidateSquare giBOARD_Main[%d][%d]=%d\n ",
      iX,
      iY,
      giBOARD_Main[iX][iY]
    );

    vTraceMsg( szMsg );
  }

  if( DEBUG_MSGS ) vTraceEnd();

  return eSquareType = giBOARD_Main[iX][iY];
} /* iValidateSquare */

int iWalk( void ) {
  int iNextX = gstPlayer.iCurrX;
  int iNextY = gstPlayer.iCurrY;
  int YOffset = 0;
  int XOffset = 0;
  
  if( DEBUG_MSGS ) vTraceBegin();

  if ( DEBUG_MSGS ) { 
    char szMsg[256] = "";

    memset( szMsg, 0x00, sizeof( szMsg ) );

    sprintf( szMsg,
  "iWalk FacingPos=%d xPos=%d yPos=%d\n ", 
      gstPlayer.iFacingPos,
      gstPlayer.iCurrX,
      gstPlayer.iCurrY
    );

    vTraceMsg( szMsg );
  }

  switch ( gstPlayer.iFacingPos ) {
    case WEST: {
      iNextX += 0;
      iNextY += -1;
      YOffset = -1;
      break;
    }
    case EAST: {
      iNextX += 0;
      iNextY += 1;
      YOffset = 1;
      break;
    }
    case SOUTH: {
      iNextX += 1;
      iNextY += 0;
      XOffset = 1;
      break;
    }
    case NORTH: {
      iNextX += -1;
      iNextY += 0;
      XOffset = -1;
      break;
    }
    default: break;
  }
  
  if ( iBOARD_IsValidSquare( iNextX, iNextY ) <= WALL_SQUARE ) {
    if( DEBUG_MSGS ) vTraceVarArgs("%s - end return -1");

    return -1;
  }

  gstPlayer.iCurrX = iNextX;
  gstPlayer.iCurrY = iNextY;
  gstPlayer.pSDL_RECT_Player->y += XOffset*gstPlayer.pSDL_RECT_Player->h;
  gstPlayer.pSDL_RECT_Player->x += YOffset*gstPlayer.pSDL_RECT_Player->w;

  if ( DEBUG_MSGS ) {
    char szMsg[256] = "";

    memset( szMsg, 0x00, sizeof( szMsg ) );

    sprintf( szMsg,
      "iWalk end xPos=%d yPos=%d\n ",
      gstPlayer.iCurrX,
      gstPlayer.iCurrY
    );

    vTraceMsg( szMsg );
  }

  if( DEBUG_MSGS ) vTraceEnd();

  return gstPlayer.iFacingPos;
} /* iWalk */

int iTurn( void ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  if ( giDeg != FULL_TURN )
   giDeg += SINGLE_TURN;
  else 
   giDeg = SINGLE_TURN;
  
  if ( gstPlayer.iFacingPos != WEST )
    gstPlayer.iFacingPos++;
  else 
    gstPlayer.iFacingPos = NORTH;

  if( DEBUG_MSGS ) vTraceEnd();

  return REDRAW_IMAGE;
} /* iTurn */

int iFireLaser( void ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  if ( DEBUG_MSGS ) vTraceEnd();

  return REDRAW_IMAGE;
} /* iFireLaser */

void vSetButtonDimensions( SDL_Rect *pSDL_RECT_Btn, int iTrslt ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Btn->w = 0.06 * atoi( gstCmdLine.szWinWidth );
  pSDL_RECT_Btn->h = 0.06 * atoi( gstCmdLine.szWinHeight );
  pSDL_RECT_Btn->x = pSDL_RECT_Btn->w + iTrslt;
  pSDL_RECT_Btn->y = atoi( gstCmdLine.szWinHeight ) - pSDL_RECT_Btn->h - 20;

  if ( DEBUG_MSGS ) vTraceEnd();
} /* vSetButtonDimensions */

void vSetTmpHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->w = 2 * atoi( gstCmdLine.szWinWidth ) / 4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;
  pSDL_RECT_Hud->x = 0.06 * atoi( gstCmdLine.szWinWidth ) - 10;
  pSDL_RECT_Hud->y = atoi( gstCmdLine.szWinHeight ) - 0.06 * atoi( gstCmdLine.szWinHeight ) - ( 30 + pSDL_RECT_Hud->h );

  if ( DEBUG_MSGS ) vTraceEnd();
} /* vSetTmpHUDRect */

void vSetCmdHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->x = atoi( gstCmdLine.szWinWidth ) / 4;
  pSDL_RECT_Hud->y = 0;
  pSDL_RECT_Hud->w = 2 * pSDL_RECT_Hud->x;
  pSDL_RECT_Hud->h = COL_RATIO;

  if ( DEBUG_MSGS ) vTraceEnd();
} /* vSetCmdHUDRect */

void vSetButtonHUDRect( SDL_Rect *pSDL_RECT_Hud ) {
  if ( DEBUG_MSGS ) vTraceBegin();

  pSDL_RECT_Hud->x = 0.06 * atoi( gstCmdLine.szWinWidth ) - 10;
  pSDL_RECT_Hud->y = atoi( gstCmdLine.szWinHeight ) - 0.06 * atoi( gstCmdLine.szWinHeight ) - 30;
  pSDL_RECT_Hud->w = 2 * atoi( gstCmdLine.szWinWidth ) / 4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;

  if( DEBUG_MSGS ) vTraceEnd();
} /* vSetButtonHUDRect */

int iWasClicked( SDL_Event *pSDL_EVENT_Ev ) {
  STRUCT_BUTTON_LIST *pstWrkButtonList;
  int iRsl = 0;

  if( DEBUG_MSGS ) vTraceBegin();

  for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext ) {
    int iClickEvX = pSDL_EVENT_Ev->button.x;
    int iClickEvY = pSDL_EVENT_Ev->button.y;
    SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;

    if ( pSDL_RECT_Btn == NULL )
      break;

    if ( bAreCoordsInSDL_Rect(pSDL_RECT_Btn, iClickEvX, iClickEvY) ) {
      // if(DEBUG_MORE_MSGS) vTraceVarArgs("%s end pstWrkButtonList->iAction == %d", pstWrkButtonList->iAction);
      return pstWrkButtonList->iAction;
    }
  }

  iRsl = iMENU_HandleMouseClick( pSDL_RECT_Menu, pSDL_RECT_Menu->x, pSDL_RECT_Menu->y, ppszMenuOpt );

  if( DEBUG_MSGS ) vTraceEnd();

  return iRsl != -1 ? iRsl : FALSE;
} /* iWasClicked */

SDL_Texture* createSquareTexture( SDL_Renderer* renderer ) {
  if( DEBUG_MSGS ) vTraceBegin();

  SDL_Surface* surface = SDL_CreateRGBSurface( 0, 800, 800, 8, 0, 0, 0, 0 );
  SDL_SetSurfaceBlendMode( surface, SDL_BLENDMODE_BLEND );
  SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
  SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
  SDL_FreeSurface( surface );

  if( DEBUG_MSGS ) vTraceEnd();

  return texture;
} /* createSquareTexture */

int iHandleClick( SDL_Event *pSDL_EVENT_Ev, SDL_Texture *pSDL_TXTR_CmdListHud ) {
  int iAction = 0;

  if( DEBUG_MSGS ) vTraceBegin();
  
  UNUSED( pSDL_TXTR_CmdListHud );

  switch( ( iAction = iWasClicked(pSDL_EVENT_Ev) ) ) {
    case FORWARD:
    case TURN:
    case FIRE_LASER:
    case ERASE: {
      iACTION_AddStep2List( iAction );
      // vUpdateCmdTmpList(iAction, pSDL_TXTR_CmdListHud);
      if ( DEBUG_MSGS ) vACTION_TraceList();
      break;
    }
    case CONFIRM: {
      if ( DEBUG_MSGS ) vTraceMsg("Confirm!\n");
      gbACTION_Check = TRUE;
      break;
    }
    case CONFIGURE: {
      if ( DEBUG_MSGS ) vTraceMsg("Configure!\n");
      vMENU_ToggleVisibility();
      return REDRAW_MENU_CLKD;
    }
    case MENU_OPT_1:
    case MENU_OPT_2:
    case MENU_OPT_3:
    case MENU_OPT_4:
    case MENU_OPT_5:
    case MENU_OPT_EXIT: {
      return REDRAW_MENU_CLKD;
    }
    default: break;
  }

  if ( DEBUG_MSGS ) vTraceEnd();

  return 0;
} /* iHandleClick */

#if 0
void vUpdateCmdTmpList( int iAct, SDL_Rect *pSDL_Rect, SDL_Texture *pSDL_TXTR_CmdListHud, SDL_Renderer *pSDL_Rndr ) {
  if ( pSDL_TXTR_CmdListHud == NULL ) {
    pSDL_TXTR_CmdListHud = IMG_LoadTexture( pSDL_Rndr, ppszInstalledImagePath[( iAct == 0 ? iAct : ( iAct - 1 ) )] );
  }
  SDL_SetTextureAlphaMod( pSDL_TXTR_CmdListHud, (Uint8)(255) );
  // SDL_Rect dstRect = { x ,y, w, h };
  SDL_RenderCopy( pSDL_Rndr, pSDL_TXTR_CmdListHud, NULL, pSDL_Rect );
  
  return;
} /* vUpdateCmdTmpList */
#endif

int iHandleEventKey( SDL_Event *pSDL_EVENT_Ev ) {
  vMENU_HandleKey( pSDL_EVENT_Ev->key.keysym.sym, ppszMenuOpt );

  if ( DEBUG_MSGS ) vTraceBegin();

  switch (pSDL_EVENT_Ev->key.keysym.sym) {
    case SDLK_UP: {
      break;
    }
    case SDLK_DOWN: {
      break;
    }
    case SDLK_RETURN: {
      break;
    }
    default: break;
  }

  if( DEBUG_MSGS ) vTraceEnd();

  return 0;
} /* iHandleEventKey */

int iCheckMenuInteraction( SDL_Rect *pSDL_RECT_MenuData, int iXCursor, int iYCursor ) {
  int iInitCt = 0;
  SDL_Rect *pSDL_RECT_Wrk;

  if( DEBUG_MSGS ) vTraceBegin();

  for ( pSDL_RECT_Wrk = pSDL_RECT_MenuData; 
        iInitCt < MAX_MENU_OPTIONS; 
        pSDL_RECT_Wrk++, iInitCt++ ) {

    if ( DEBUG_MORE_MSGS ) {
      char szMsg[256] = "";

      memset( szMsg, 0x00, sizeof( szMsg ) );

      sprintf( szMsg,
      "SDL_MOUSEMOTION Cursor[X,Y]:[%d,%d] Rect[X,Y]:[%d,%d]\n ", 
        iXCursor,
        iYCursor,
        pSDL_RECT_Wrk->x,
        pSDL_RECT_Wrk->y
      );
      vTraceMsg( szMsg );
    }

    if ( bAreCoordsInSDL_Rect( pSDL_RECT_Wrk, iXCursor, iYCursor ) ) {
      if ( DEBUG_MORE_MSGS )
        vTraceMsg("Hovered!\n");
        
      giMENU_SelectedItem = iInitCt;

      // if(DEBUG_MSGS) vTraceVarArgs("%s end - return %d", REDRAW_IMAGE);

      return REDRAW_IMAGE;
    }
  } /* for */

  if ( DEBUG_MSGS ) vTraceEnd();

  return 0;
} /* iCheckMenuInteraction */

int iHandleMouseMotion( SDL_Rect *pSDL_RECT_Menu, SDL_Event *pSDL_EVENT_Ev ) {
  int iX;
  int iY;

  UNUSED(pSDL_RECT_Menu);
  
  // if(DEBUG_MORE_MSGS) vTraceBegin();

  SDL_GetMouseState( &iX, &iY );

  if ( iBUTTON_CheckInteraction( pSDL_EVENT_Ev, iX, iY ) == REDRAW_IMAGE ) {
    // if(DEBUG_MORE_MSGS) vTraceVarArgs("%s end - return %d", REDRAW_IMAGE);

    return REDRAW_IMAGE;
  }

  if ( iMENU_HandleMouseMotion( pSDL_RECT_Menu, iX, iY ) ) {
    return REDRAW_IMAGE;
  }
  
  // if(DEBUG_MORE_MSGS) vTraceEnd();

  return 0;
} /* iHandleMouseMotion */

/**
 * Print the version of the software
 */
static void vPrintVersion( void )
{
  printf("%s %s\n"
         "Build in %s %s\n"
         "%s %s\n"
         "For reporting bugs, send a email to:\n"
         "<%s>\n"
         "<%s>\n", gkpszProgramName,
                   VERSION,
                   __DATE__,
                   __TIME__,
                   COPYRIGHT,
                   DEVELOPER,
                   RFERMI_MAIL,
                   BACAGINE_MAIL
  );
} /* vPrintVersion */

/**
 * Print the help message for the user
 */
static void vPrintUsage( void )
{
  int iReallocBuffer = 0;
  char *pszMsg = NULL;

  iReallocBuffer = snprintf(
    NULL,
    0,
    "Usage %s [options]<arguments>\n\n"
    "%s\n\n"
    "Options:\n", gkpszProgramName,
                  DESCRIPTION
  );

  pszMsg = ( char * ) malloc( sizeof( char ) * ( iReallocBuffer + 1 ) );
  snprintf(
    pszMsg,
    iReallocBuffer+1,
    "Usage %s [options]<arguments>\n\n"
    "%s\n\n"
    "Options:\n", gkpszProgramName,
                  DESCRIPTION
  );

  vShowSyntax( pszMsg, astCmdOpt );

  free( pszMsg );
  pszMsg = NULL;

  exit( EXIT_FAILURE );
} /* vPrintUsage */

/**
 *
 * main
 *  
 */
int SDL_main( int argc, char *argv[] ) {
  int iXTranslation = 0;
  int iRedrawAction = -1;
  int iImageIx;
  uint64_t ui64ElapsedTime;
  SDL_Texture *pSDL_TXTR_TemporaryCmdList = NULL;
  SDL_Rect SDL_RECT_Player;
  SDL_Rect SDL_RECT_Hud;
  SDL_Rect SDL_RECT_ButtonHud;
  SDL_Rect SDL_RECT_ButtonArrowRight;
  SDL_Rect SDL_RECT_ButtonTurnArrow;
  SDL_Rect SDL_RECT_ButtonFireLaser;
  SDL_Rect SDL_RECT_ButtonUndoLast;
  SDL_Rect SDL_RECT_ButtonConfirm;
  SDL_Rect SDL_RECT_ButtonConfigure;
  TTF_Font *pttf_Font;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Event event;

  memset( &gstCmdLine, 0x00, sizeof( gstCmdLine ) );

  gkpszProgramName = szGetProgramName( argv[0] );

  if ( !bLoadCfgFile("cattie.conf") ) {
    fprintf(stderr, "E: impossible load .conf file\n");

    exit( EXIT_FAILURE );
  }

  /**
   * Check if command line is OK
   */
  if ( !bCommandLineIsOK( argc, argv, astCmdOpt ) ) {
    vPrintUsage();

    return -1;
  }

  if( astCmdOpt[0].bSet ) {
    vPrintUsage();

    return 0;
  }

  if( astCmdOpt[1].bSet ) {
    vPrintVersion();

    return 0;
  }

  if ( astCmdOpt[5].bSet ) {
    vShowTrain();
    exit( EXIT_SUCCESS );
  }

  vInitLogs();

  if ( DEBUG_MSGS ) vTraceBegin();

  SDL_SetMainReady();

  // Initialize SDL
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    if ( DEBUG_MSGS ) {
      char szMsg[256] = "";

      memset( szMsg, 0x00, sizeof( szMsg ) );

      sprintf(szMsg,
        "Couldn't initialize SDL: %s\n",
        SDL_GetError()
      );

      vTraceMsg( szMsg );
    }

    return 1;
  }

  if ( ( pttf_Font = ttfSDL_InitFont() ) == NULL ) {
    if ( DEBUG_MSGS ) {
      char szMsg[256] = "";

      memset( szMsg, 0x00, sizeof( szMsg ) );

      sprintf(szMsg,
        "Couldn't initialize Font: %s",
        TTF_FONT_TITLE
      );

      vTraceMsg(szMsg);
    }

    return 1;
  }

  // Create a window
  window = SDL_CreateWindow(
    WINDOW_TITLE,
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    atoi(gstCmdLine.szWinWidth),
    atoi(gstCmdLine.szWinHeight),
    SDL_WINDOW_SHOWN
  );

  if ( DEBUG_MSGS ) vTraceMsg("SDL_CreateWindow --- Ok\n");

  // Create a renderer
  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
  SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );

  // Not rdy yet
  // iGXRF_Init();
  //

  //
  // HUDs
  //
  // Set Hud Rect Dimensions
  vSetCmdHUDRect( &SDL_RECT_Hud );
  vSetButtonHUDRect( &SDL_RECT_ButtonHud );

  //
  // Board
  //
  iBOARD_GenerateRandomPath();

  //
  // Buttons
  //

  // Clear structure
  vBUTTON_InitList();

  // Set Button Sizes
  vSetButtonDimensions( &SDL_RECT_ButtonArrowRight, iXTranslation );
  iXTranslation += SDL_RECT_ButtonArrowRight.w + 10;
  vSetButtonDimensions( &SDL_RECT_ButtonTurnArrow, iXTranslation );
  iXTranslation += SDL_RECT_ButtonTurnArrow.w + 10;
  vSetButtonDimensions( &SDL_RECT_ButtonFireLaser, iXTranslation );
  iXTranslation += SDL_RECT_ButtonFireLaser.w + 50;
  vSetButtonDimensions( &SDL_RECT_ButtonUndoLast, iXTranslation );
  iXTranslation += SDL_RECT_ButtonUndoLast.w + 50;
  vSetButtonDimensions( &SDL_RECT_ButtonConfirm, iXTranslation );
  iXTranslation += SDL_RECT_ButtonConfirm.w + 100;
  vSetButtonDimensions( &SDL_RECT_ButtonConfigure, iXTranslation );

  // Square Edges
  vBOARD_DrawEdges( renderer );

  // Add them to list
  iBUTTON_AddToList( &SDL_RECT_ButtonArrowRight   , FORWARD    );
  iBUTTON_AddToList( &SDL_RECT_ButtonTurnArrow    , TURN       );
  iBUTTON_AddToList( &SDL_RECT_ButtonFireLaser    , FIRE_LASER );
  iBUTTON_AddToList( &SDL_RECT_ButtonUndoLast     , ERASE      );
  iBUTTON_AddToList( &SDL_RECT_ButtonConfirm      , CONFIRM    );
  iBUTTON_AddToList( &SDL_RECT_ButtonConfigure    , CONFIGURE  );

  SDL_RenderPresent( renderer );

  vBUTTON_DrawList( renderer );

  gstPlayer.pSDL_RECT_Player = &SDL_RECT_Player;

  //
  // TXTR
  //
  vTXTR_InitList();


  for ( iImageIx = PLAYER_IMG_PATH_IDX; iImageIx != END_PATH_LIST; iImageIx += NEXT_IMG_PATH ) {
    pSDL_TXTR_AddToList(
      renderer,
      pBUTTON_GetRectByImg( iImageIx ),
      ASSERT_IMG_PATH( iImageIx, ppszInstalledImagePath, ppszImagePath ),
      BLENDED_SURFACE_NONE
    );

  }

  pSDL_TXTR_AddToList( renderer, &SDL_RECT_Hud, NULL, BLENDED_SURFACE_NONE );
  pSDL_TXTR_AddToList( renderer, &SDL_RECT_ButtonHud, NULL, BLENDED_SURFACE_NONE );
  pSDL_TXTR_AddToList( renderer, NULL, NULL, BLENDED_SURFACE_TXTR );

  vInitializeImagePosition( &SDL_RECT_Player );

  pSDL_RECT_Menu = ( SDL_Rect * ) malloc( MAX_MENU_OPTIONS * sizeof( SDL_Rect ) );
  vInitMenu( pSDL_RECT_Menu, MAX_MENU_OPTIONS );

/*
  iGXRF_Add2RenderList(
    renderer,
    TRUE,
    SDL_RECT,
    &SDL_RECT_ButtonArrowRight,
    vDrawButton
  );

  iGXRF_ADD2ArgList( &SDL_RECT_ButtonArrowRight, ( void * ) ( eSDLTypes=SDL_RECT ), &SDL_RECT_ButtonArrowRight );
  iGXRF_ADD2ArgList( &SDL_RECT_ButtonArrowRight, ( void * ) "int", ( void * ) BUTTON_DIRECTION );
*/
  // Main loop
  while ( gbRunning ) {
    // The player has choose its route already?
    while ( gbACTION_Check ) {
      // yes ...
      if ( giACTION_AssertedSteps > 0 )
        SDL_Delay( 1000 );

      if ( giACTION_AssertedSteps >= giACTION_StepCtr ) {
        gbACTION_Check = FALSE;
        break;
      }

      iRedrawAction = iACTION_ExecuteStep( giACTION_AssertedSteps++ );
      if  ( iRedrawAction < 0 )
        gbRunning = FALSE;

      iRedrawAction = REDRAW_IMAGE;
      if ( DEBUG_MSGS ) { 
        char szMsg[256] = "";

        memset( szMsg, 0x00, sizeof( szMsg ) );

        sprintf(szMsg,
          "iExecuteActionFromList Rsl(Redraw)=%d"
          " gbRunning=%d giACTION_AssertedSteps=%d giACTION_StepCtr=%d\n",
          iRedrawAction, 
          gbRunning,
          giACTION_AssertedSteps,
          giACTION_StepCtr
        );

        vTraceMsg( szMsg );
      }
      break;
    } /* while */

    while ( !gbACTION_Check && SDL_PollEvent(&event) ) {
      // The player hasn't choose its route yet,
      // so we must watch all interaction events... 
      switch (event.type) {
        case SDL_QUIT: {
          gbRunning = FALSE;
          break;
        }
        case SDL_MOUSEBUTTONDOWN: {
          if ( (iRedrawAction = iHandleClick( &event, pSDL_TXTR_TemporaryCmdList ) ) > 0 ) {
            gbRunning = FALSE;
            iRedrawAction = REDRAW_NONE;
            break;
          }

          if ( iRedrawAction == REDRAW_MENU_CLKD )
            iRedrawAction = REDRAW_IMAGE;
            
          break;
        }
        case SDL_KEYDOWN: {
          iHandleEventKey(&event);
          iRedrawAction = REDRAW_IMAGE;
          break;
        }
        case SDL_MOUSEMOTION: {
          iRedrawAction = iHandleMouseMotion(pSDL_RECT_Menu, &event);
          break;
        }
        default: break;
      } /* switch */
    } /* while */

    if ( iRedrawAction == ERROR_WALKING )
      break;

    // If nothing has changed, we will not redraw...
    if ( iRedrawAction != REDRAW_IMAGE && iRedrawAction != -1 ) {
      continue;
    }

    //
    // Redraw 
    //
    if ( DEBUG_MSGS ) vTraceMsg("---  Main loop Redraw ---");

    ui64ElapsedTime = SDL_GetTicks64();
   
    SDL_RenderClear( renderer );
    
    iBOARD_Colorfy( renderer );
    
    vSetCmdHUDRect( &SDL_RECT_Hud );
    vSetButtonHUDRect( &SDL_RECT_ButtonHud );

    vDrawCommandHUD( renderer, &SDL_RECT_Hud );
    vDrawButtonHUD ( renderer, &SDL_RECT_ButtonHud );
  
    vBUTTON_DrawList( renderer );

    vTXTR_RenderCopy( renderer, giDeg );

    vDrawMenu( renderer, pSDL_RECT_Menu, ppszMenuOpt, MAX_MENU_OPTIONS, pttf_Font );

    // Update the screen
    SDL_RenderPresent( renderer );
    
    // Vamos sincronizar o refreshrate..
    ui64ElapsedTime -= SDL_GetTicks64();
    if ( ui64ElapsedTime <= VSYNC_TIME ) SDL_Delay( VSYNC_TIME - ui64ElapsedTime );
    
    // Redoing action, as we got no relevant interaction, no draw allowed.
    iRedrawAction = 0;
  } /* while */
  
  // Clean up
  TTF_CloseFont( pttf_Font );
  vBUTTON_FreeList();
  vTXTR_FreeList( DESTROY_TEXTURES );
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
  
  if( DEBUG_MSGS ) vTraceEnd();

  // return iRsl;
  return 0;
} /* SDL_main */

