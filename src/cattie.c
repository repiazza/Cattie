#include <inttypes.h>
#include <time.h>
#include <cattie.h>
#include <board.h>
#include <menu.h>
#include <action.h>
#include <button.h>
#include <consts.h>
#include <cmdline.h>
#include <util.h>

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
int giBOARD_Main[BOARD_ROWS][BOARD_COLS];
int giDeg = 0;
int gbRunning = TRUE;
int gbACTION_Check = FALSE;
int giACTION_List[_MAX_MOV_ACTION];
int giACTION_StepCtr = 0;
int giACTION_AssertedSteps = 0;
int giMENU_SelectedItem = 0;
STRUCT_PLAYER gstPlayer;

#ifdef LINUX
  char *ppszInstalledImagePath[] = {
    "/usr/share/cattie/img/cat2.png",
    "/usr/share/cattie/img/forward.png",
    "/usr/share/cattie/img/laser.png",
    "/usr/share/cattie/img/rotate2.png"
  };
#else
  char *ppszInstalledImagePath[] = {
    "C:\\Arquivos de Programas\\cattie\\img\\cat2.png",
    "C:\\Arquivos de Programas\\cattie\\img\\forward.png",
    "C:\\Arquivos de Programas\\cattie\\img\\laser.png",
    "C:\\Arquivos de Programas\\cattie\\img\\rotate2.png"
  };
#endif

char *ppszMenuOpt[MAX_MENU_OPTIONS] = {
    "option 1",
    "option 2",
    "option 3",
    "option 4",
    "option 5",
    "Sair"
};

char *ppszImagePath[] = {
    "img/cat2.png",
    "img/forward.png",
    "img/laser.png",
    "img/rotate2.png",
    "img/gear.png"
};

/* Receive the name of program */
const char *gkpszProgramName;

STRUCT_COMMAND_LINE gstCmdLine;

/**
 * 
 * Procedures and functions
 * 
 **/

void vInitRect(SDL_Rect *pSDL_RECT, int iX, int iY, int iWidth, int iHeight){
  if(DEBUG_MSGS) vTraceBegin();
  
  pSDL_RECT->x = iX;
  pSDL_RECT->y = iY;
  pSDL_RECT->w = iWidth;
  pSDL_RECT->h = iHeight;

  if(DEBUG_MORE_MSGS)
  {
    vTraceVarArgs(
  "pSDL_RECT->x = %d | pSDL_RECT->y = %d\n\t\t"
  "pSDL_RECT->w = %d | pSDL_RECT->h = %d\n",
        (pSDL_RECT->x), (pSDL_RECT->y),
        (pSDL_RECT->w), (pSDL_RECT->h)
    );
  }

  if(DEBUG_MSGS) vTraceEnd();
}

SDL_Surface *pSDL_SRFC_LoadImage(char *pszImgPath){
  // Load the image
  SDL_Surface *SDL_SRFC_Img = IMG_Load(pszImgPath);
  
  if(DEBUG_MSGS) vTraceBegin();

  if (SDL_SRFC_Img == NULL) {
    printf("Error loading image: %s\n", IMG_GetError());

    if(DEBUG_MORE_MSGS) vTraceVarArgs("%s - end return NULL", __func__);

    return NULL;
  }

  vTraceEnd();

  return SDL_SRFC_Img;
}

// SDL_Texture *pSDL_TXTR_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *pSDL_SRFC_Img){
//   // Create the texture from the image
//   SDL_Texture *SDL_TXTR_Texture = SDL_CreateTextureFromSurface(renderer, pSDL_SRFC_Img);
// 
//   if(DEBUG_MSGS) vTraceBegin();
// 
//   if (SDL_TXTR_Texture == NULL) {
//     printf("Error creating texture: %s\n", SDL_GetError());
// 
//     if(DEBUG_MORE_MSGS) vTraceVarArgs("%s - end return NULL", __func__);
// 
//     return NULL;
//   }
//   
//   if(DEBUG_MSGS) vTraceEnd();
// 
//   return SDL_TXTR_Texture;
// }

void vDrawSquareEdges(SDL_Renderer *renderer){
  int ii;
  int jj;

  if(DEBUG_MSGS) vTraceBegin();

  for (ii = 0; ii < BOARD_ROWS; ii++) {
    for (jj = 0; jj < BOARD_COLS; jj++) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_Rect SDLRECT_Edges = {ii * COL_RATIO, jj * COL_RATIO, COL_RATIO, COL_RATIO};
      SDL_RenderDrawRect(renderer, &SDLRECT_Edges);
    }
  }

  if(DEBUG_MSGS) vTraceEnd();
}

void vDrawCommandHUD(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect *pSDL_RECT_Hud){
  UNUSED(texture);

  if(DEBUG_MSGS) vTraceBegin();

  SDL_SetRenderTarget(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 17, 84, 143, 96);
  SDL_RenderFillRect(renderer, pSDL_RECT_Hud);
  SDL_SetRenderDrawColor(renderer, 17, 84, 143, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Hud);
  
  if(DEBUG_MSGS) vTraceEnd();
}

void vDrawButtonHUD(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect *pSDL_RECT_Hud){
  UNUSED(texture);

  if(DEBUG_MSGS) vTraceBegin();

  SDL_SetRenderTarget(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 128, 4, 0, 128);
  SDL_RenderFillRect(renderer, pSDL_RECT_Hud);
  SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Hud);

  if(DEBUG_MSGS) vTraceEnd();
}

void vDrawButton(SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Button, int iButtonType){
  if(DEBUG_MSGS) vTraceBegin();

  if ( iButtonType == BUTTON_DIRECTION )
  {
     if(DEBUG_MORE_MSGS) vTraceVarArgs("iButtonType == BUTTON_DIRECTION");
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
  }
  else if ( iButtonType == BUTTON_CONFIRM )
  {
    if(DEBUG_MORE_MSGS) vTraceVarArgs("iButtonType == BUTTON_CONFIRM");
    
    SDL_SetRenderDrawColor(renderer, 0, 200, 50, 255);
  }
  else if ( iButtonType == BUTTON_ERASE )
  {
    if(DEBUG_MORE_MSGS) vTraceVarArgs("iButtonType == BUTTON_ERASE");

    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
  }

  SDL_RenderFillRect(renderer, pSDL_RECT_Button);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Button);

  if(DEBUG_MSGS) vTraceEnd();
}

void vInitializeImagePosition(SDL_Rect *pSDL_Rect_Im){
  int iLocation = -2;
  int iDimensions = COL_RATIO;

  if(DEBUG_MSGS) vTraceBegin();

  vInitRect(pSDL_Rect_Im, iLocation, iLocation, iDimensions, iDimensions);
  
  if( gstPlayer.iFacingPos == SOUTH ) giDeg +=90;

  if(DEBUG_MSGS) vTraceEnd();
}

eSqType iValidateSquare(int iX, int iY){
  eSqType eSquareType = -1;
  
  if(DEBUG_MSGS) vTraceBegin();

  if ( iX < 0 || iX >= BOARD_ROWS )
    return eSquareType;

   if ( DEBUG_MSGS ) { 
    char szMsg[256];
    sprintf(szMsg,
  "iValidateSquare giBOARD_Main[%d][%d]=%d\n ", 
      iX,
      iY,
      giBOARD_Main[iX][iY]
    );
    vTraceMsg(szMsg);
  }

  if(DEBUG_MSGS) vTraceEnd();

  return eSquareType = giBOARD_Main[iX][iY]; 
}

int iWalk(){
  int iNextX = gstPlayer.iCurrX;
  int iNextY = gstPlayer.iCurrY;
  int YOffset = 0;
  int XOffset = 0;
  
  if(DEBUG_MSGS) vTraceBegin();

  if ( DEBUG_MSGS ) { 
    char szMsg[256];
    sprintf(szMsg,
  "iWalk FacingPos=%d xPos=%d yPos=%d\n ", 
      gstPlayer.iFacingPos,
      gstPlayer.iCurrX,
      gstPlayer.iCurrY
    );
    vTraceMsg(szMsg);
  }

  switch ( gstPlayer.iFacingPos ){
    case WEST:
      iNextX += 0;
      iNextY += -1;
      YOffset = -1;
      break;
    case EAST:
      iNextX += 0;
      iNextY += 1;
      YOffset = 1;
      break;
    case SOUTH:
      iNextX += 1;
      iNextY += 0;
      XOffset = 1;
      break;
    case NORTH:
      iNextX += -1;
      iNextY += 0;
      XOffset = -1;
      break;

    default:
      break;
  }
  
  if ( iBOARD_IsValidSquare(iNextX, iNextY) <= WALL_SQUARE )
  {
    if(DEBUG_MSGS) vTraceVarArgs("%s - end return -1");

    return -1;
  }

  gstPlayer.iCurrX = iNextX;
  gstPlayer.iCurrY = iNextY;
  gstPlayer.pSDL_RECT_Player->y += XOffset*gstPlayer.pSDL_RECT_Player->h;
  gstPlayer.pSDL_RECT_Player->x += YOffset*gstPlayer.pSDL_RECT_Player->w;
  if ( DEBUG_MSGS ) { 
    char szMsg[256];
    sprintf(szMsg,
  "iWalk end xPos=%d yPos=%d\n ", 
      gstPlayer.iCurrX,
      gstPlayer.iCurrY
    );
    vTraceMsg(szMsg);
  }

  if( DEBUG_MSGS ) vTraceEnd();

  return gstPlayer.iFacingPos;
}

int iTurn(void){
  if ( DEBUG_MSGS ) vTraceMsg("Turn\n");

  giDeg = (giDeg == 360) ? 90 : giDeg + 90;
  gstPlayer.iFacingPos = (gstPlayer.iFacingPos == WEST) ? NORTH : gstPlayer.iFacingPos+1;

  if(DEBUG_MSGS) vTraceEnd();

  return REDRAW_IMAGE;
}

int iFireLaser(void){
  if(DEBUG_MSGS) vTraceBegin();

  if(DEBUG_MSGS) vTraceEnd();

  return REDRAW_IMAGE;
}

void vSetButtonDimensions(SDL_Rect *pSDL_RECT_Btn, int iTrslt){
  if(DEBUG_MSGS) vTraceBegin();

  pSDL_RECT_Btn->w = 0.06*atoi(gstCmdLine.szWinWidth);
  pSDL_RECT_Btn->h = 0.06*atoi(gstCmdLine.szWinHeight);
  pSDL_RECT_Btn->x = pSDL_RECT_Btn->w + iTrslt;
  pSDL_RECT_Btn->y = atoi(gstCmdLine.szWinHeight) - pSDL_RECT_Btn->h - 20;

  if(DEBUG_MSGS) vTraceEnd();
}

void vSetHUDRectSize(SDL_Rect *pSDL_RECT_Hud){
  if(DEBUG_MSGS) vTraceBegin();

  pSDL_RECT_Hud->x = atoi(gstCmdLine.szWinWidth)/4;
  pSDL_RECT_Hud->y = 0;
  pSDL_RECT_Hud->w = 2*pSDL_RECT_Hud->x;
  pSDL_RECT_Hud->h = COL_RATIO;

  if(DEBUG_MSGS) vTraceEnd();
}

void vSetButtonHUDRectSize(SDL_Rect *pSDL_RECT_Hud){
  if(DEBUG_MSGS) vTraceBegin();

  pSDL_RECT_Hud->x = 0.06*atoi(gstCmdLine.szWinWidth) - 10;
  pSDL_RECT_Hud->y = atoi(gstCmdLine.szWinHeight) - 0.06*atoi(gstCmdLine.szWinHeight) - 30;
  pSDL_RECT_Hud->w = 2*atoi(gstCmdLine.szWinWidth)/4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;

  if(DEBUG_MSGS) vTraceEnd();
}

int iWasClicked(SDL_Event *pSDL_EVENT_Ev){
  STRUCT_BUTTON_LIST *pstWrkButtonList;

  if(DEBUG_MSGS) vTraceBegin(); 

  for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext){
    int iClickEvX = pSDL_EVENT_Ev->button.x;
    int iClickEvY = pSDL_EVENT_Ev->button.y;
    SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;

    if ( pSDL_RECT_Btn == NULL )
      break;

    if ( iClickEvX >= pSDL_RECT_Btn->x 
      && iClickEvY >= pSDL_RECT_Btn->y 
      && iClickEvX <= pSDL_RECT_Btn->x + pSDL_RECT_Btn->w 
      && iClickEvY <= pSDL_RECT_Btn->y + pSDL_RECT_Btn->h) {
      
      // if(DEBUG_MORE_MSGS) vTraceVarArgs("%s end pstWrkButtonList->iAction == %d", pstWrkButtonList->iAction);
      
      return pstWrkButtonList->iAction;
    }
  }

  if(DEBUG_MSGS) vTraceEnd();

  return FALSE;
}

SDL_Texture* createSquareTexture(SDL_Renderer* renderer)
{
  if(DEBUG_MSGS) vTraceBegin();

  SDL_Surface* surface = SDL_CreateRGBSurface(0, 800, 800, 8, 0, 0, 0, 0);
  SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(surface);

  if(DEBUG_MSGS) vTraceEnd();

  return texture;
}

int iHandleClick(SDL_Event *pSDL_EVENT_Ev){
  if(DEBUG_MSGS) vTraceBegin();

  int iAction = 0;

  switch((iAction = iWasClicked(pSDL_EVENT_Ev))){
    case FORWARD:
    case TURN:
    case FIRE_LASER:
    case ERASE:
      iACTION_AddStep2List(iAction);
      if ( DEBUG_MSGS ) vACTION_TraceList();
      break;
    case CONFIRM:
      if ( DEBUG_MSGS ) vTraceMsg("Confirm!\n");
      gbACTION_Check = TRUE;
      break;
    case CONFIGURE:
      if ( DEBUG_MSGS ) vTraceMsg("Configure!\n");
      vMENU_ToggleVisibility();
      return -10;
    default:
      break;
  }

  if(DEBUG_MSGS) vTraceEnd();

  return 0;
}

int iHandleEventKey(SDL_Event *pSDL_EVENT_Ev){
  vMENU_HandleKey(pSDL_EVENT_Ev->key.keysym.sym, ppszMenuOpt);

  if(DEBUG_MSGS) vTraceBegin();

  switch (pSDL_EVENT_Ev->key.keysym.sym) {
    case SDLK_UP:
      break;
    case SDLK_DOWN:
      break;
    case SDLK_RETURN:  
      break;
    default:
      break;
  }

  if(DEBUG_MSGS) vTraceEnd();

  return 0;
}

int iCheckMenuInteraction(SDL_Rect *pSDL_RECT_MenuData, int iXCursor, int iYCursor){
  int iInitCt = 0;
  SDL_Rect *pSDL_RECT_Wrk;

  if(DEBUG_MSGS) vTraceBegin();

  for ( pSDL_RECT_Wrk = pSDL_RECT_MenuData; 
        iInitCt < MAX_MENU_OPTIONS; 
        pSDL_RECT_Wrk++, iInitCt++){
    if ( DEBUG_MORE_MSGS ) {
      char szMsg[256];
      sprintf(szMsg,
      "SDL_MOUSEMOTION Cursor[X,Y]:[%d,%d] Rect[X,Y]:[%d,%d]\n ", 
        iXCursor,
        iYCursor,
        pSDL_RECT_Wrk->x,
        pSDL_RECT_Wrk->y
      );
      vTraceMsg(szMsg);
    }
    if (iXCursor >= pSDL_RECT_Wrk->x 
        && iXCursor <= pSDL_RECT_Wrk->x + pSDL_RECT_Wrk->w 
        && iYCursor >= pSDL_RECT_Wrk->y 
        && iYCursor <= pSDL_RECT_Wrk->y + pSDL_RECT_Wrk->h) {
      if ( DEBUG_MORE_MSGS )
        vTraceMsg("Hovered!\n");
        
      giMENU_SelectedItem = iInitCt;

      // if(DEBUG_MSGS) vTraceVarArgs("%s end - return %d", REDRAW_IMAGE);

      return REDRAW_IMAGE;
    }
  }

  if(DEBUG_MSGS) vTraceEnd();

  return 0;
}

int iHandleMouseMotion(SDL_Rect *pSDL_RECT_Menu, SDL_Event *pSDL_EVENT_Ev){
  int iX, iY;

  UNUSED(pSDL_RECT_Menu);
  
  // if(DEBUG_MORE_MSGS) vTraceBegin();

  SDL_GetMouseState(&iX, &iY);

  if ( iBUTTON_CheckInteraction(pSDL_EVENT_Ev, iX, iY) == REDRAW_IMAGE )
  {
    // if(DEBUG_MORE_MSGS) vTraceVarArgs("%s end - return %d", REDRAW_IMAGE);

    return REDRAW_IMAGE;
  }
  
  // if(DEBUG_MORE_MSGS) vTraceEnd();

  return 0;
}

/**
 *
 * main
 *  
 */
int SDL_main(int argc, char *argv[]){
  int iXTranslation = 0;
  int iRedrawAction = -1;
  uint64_t ui64ElapsedTime;
  SDL_Texture *pSDL_TXTR_ImagePlayer;
  SDL_Texture *pSDL_TXTR_ImageFoward;
  SDL_Texture *pSDL_TXTR_ImageRotate;
  SDL_Texture *pSDL_TXTR_ImageLaser;
  SDL_Texture *pSDL_TXTR_ImageConfig;
  SDL_Texture *pSDL_TXTR_Hud;
  SDL_Texture *pSDL_TXTR_ButtonHud;
  SDL_Texture *pSDL_TXTR_SquareBorder;
  SDL_Rect SDL_RECT_Player;
  SDL_Rect SDL_RECT_Hud;
  SDL_Rect SDL_RECT_ButtonHud;
  SDL_Rect SDL_RECT_ButtonArrowRight;
  SDL_Rect SDL_RECT_ButtonTurnArrow;
  SDL_Rect SDL_RECT_ButtonFireLaser;
  SDL_Rect SDL_RECT_ButtonUndoLast;
  SDL_Rect SDL_RECT_ButtonConfirm;
  SDL_Rect SDL_RECT_ButtonConfigure;
  SDL_Rect *pSDL_RECT_Menu = NULL;
  TTF_Font *pttf_Font;
  // eSDLT_Renderizable eSDLTypes;
  SDL_Window* window;
  SDL_Renderer* renderer;
  // int iRsl = 0;

  UNUSED(SDL_RECT_ButtonTurnArrow);
  UNUSED(SDL_RECT_ButtonUndoLast);
  UNUSED(SDL_RECT_ButtonFireLaser);
  UNUSED(SDL_RECT_ButtonTurnArrow);
  UNUSED(SDL_RECT_ButtonConfirm);
  UNUSED(SDL_RECT_ButtonConfigure);
  
  memset(&gstCmdLine, 0, sizeof(gstCmdLine));

  gkpszProgramName = szGetProgramName(argv[0]);
   
  if(!bLoadCfgFile("cattie.conf"))
  {
    fprintf(stderr, "E: impossible load .conf file\n");

    exit(EXIT_FAILURE);
  }
  
  vInitLogs();

  if (DEBUG_MSGS) vTraceBegin();
   
  /**
   * Check if command line is OK
   */
  if(!bCommandLineIsOK(argc, argv))
  {
    vPrintUsage();

    return -1;
  }
  
  SDL_SetMainReady();
  
  // Initialize SDL
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    if ( DEBUG_MSGS ) { 
      char szMsg[256];
      sprintf(szMsg,
    "Couldn't initialize SDL: %s\n", 
         SDL_GetError()
      );
      vTraceMsg(szMsg);
    }
    return 1;
  }
  if ( (pttf_Font = ttfSDL_InitFont()) == NULL ){
    if ( DEBUG_MSGS ) { 
      char szMsg[256];
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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // iGXRF_Init();

  // Set Hud Rect Dimensions
  vSetHUDRectSize(&SDL_RECT_Hud);

  vSetButtonHUDRectSize(&SDL_RECT_ButtonHud);

  // Generate a unique path.
  iBOARD_GenerateRandomPath();
  
  // Clear structure
  vBUTTON_InitList();
  
  // Set Button Sizes
  // Walk Forward
  vSetButtonDimensions(&SDL_RECT_ButtonArrowRight, iXTranslation);
  iXTranslation += SDL_RECT_ButtonArrowRight.w + 10;
  // Turn Arrow
  vSetButtonDimensions(&SDL_RECT_ButtonTurnArrow, iXTranslation);
  iXTranslation += SDL_RECT_ButtonTurnArrow.w + 10;
  // Fire Laser
  vSetButtonDimensions(&SDL_RECT_ButtonFireLaser, iXTranslation);
  iXTranslation += SDL_RECT_ButtonFireLaser.w + 50;
  // Undo Last
  vSetButtonDimensions(&SDL_RECT_ButtonUndoLast, iXTranslation);
  iXTranslation += SDL_RECT_ButtonUndoLast.w + 50;
  // Confirm Action
  vSetButtonDimensions(&SDL_RECT_ButtonConfirm, iXTranslation);
  iXTranslation += SDL_RECT_ButtonConfirm.w + 100;
  // Configure Action
  vSetButtonDimensions(&SDL_RECT_ButtonConfigure, iXTranslation);
  
  // Square Edges
  vDrawSquareEdges(renderer);
  
  // Add them to list
  iBUTTON_AddToList(&SDL_RECT_ButtonArrowRight   , FORWARD);
  iBUTTON_AddToList(&SDL_RECT_ButtonTurnArrow    , TURN);
  iBUTTON_AddToList(&SDL_RECT_ButtonFireLaser    , FIRE_LASER) ;
  iBUTTON_AddToList(&SDL_RECT_ButtonUndoLast     , ERASE);
  iBUTTON_AddToList(&SDL_RECT_ButtonConfirm      , CONFIRM);
  iBUTTON_AddToList(&SDL_RECT_ButtonConfigure    , CONFIGURE);
  SDL_RenderPresent(renderer);
  
  /**
   * If the game is not installed, get the images of the current directory
   */
  if(bFileExist(ppszInstalledImagePath[PLAYER_IMG_PATH_IDX]))
  {
    pSDL_TXTR_ImagePlayer  = IMG_LoadTexture(renderer  , ppszInstalledImagePath[PLAYER_IMG_PATH_IDX]); 
    pSDL_TXTR_ImageFoward  = IMG_LoadTexture(renderer  , ppszInstalledImagePath[FORWARD_IMG_PATH_IDX]); 
    pSDL_TXTR_ImageLaser   = IMG_LoadTexture(renderer  , ppszInstalledImagePath[LASER_IMG_PATH_IDX]);
    pSDL_TXTR_ImageRotate  = IMG_LoadTexture(renderer  , ppszInstalledImagePath[ROTATE_IMG_PATH_IDX]);
    pSDL_TXTR_ImageConfig  = IMG_LoadTexture(renderer  , ppszInstalledImagePath[GEAR_IMG_PATH_IDX]); 
  }
  else
  {
    pSDL_TXTR_ImagePlayer  = IMG_LoadTexture(renderer  , ppszImagePath[PLAYER_IMG_PATH_IDX]); 
    pSDL_TXTR_ImageFoward  = IMG_LoadTexture(renderer  , ppszImagePath[FORWARD_IMG_PATH_IDX]); 
    pSDL_TXTR_ImageLaser   = IMG_LoadTexture(renderer  , ppszImagePath[LASER_IMG_PATH_IDX]);
    pSDL_TXTR_ImageRotate  = IMG_LoadTexture(renderer  , ppszImagePath[ROTATE_IMG_PATH_IDX]); 
    pSDL_TXTR_ImageConfig  = IMG_LoadTexture(renderer  , ppszImagePath[GEAR_IMG_PATH_IDX]);
  }

  pSDL_TXTR_Hud          = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SDL_RECT_Hud.w, SDL_RECT_Hud.h);
  pSDL_TXTR_ButtonHud    = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SDL_RECT_ButtonHud.w, SDL_RECT_ButtonHud.h);
  pSDL_TXTR_SquareBorder = createSquareTexture(renderer); 

  vInitializeImagePosition(&SDL_RECT_Player);
    
  pSDL_RECT_Menu = (SDL_Rect *) malloc(MAX_MENU_OPTIONS*sizeof(SDL_Rect));
  
  vInitMenu(pSDL_RECT_Menu, MAX_MENU_OPTIONS);

  gstPlayer.pSDL_RECT_Player = &SDL_RECT_Player;

  vDrawButton(renderer, &SDL_RECT_ButtonArrowRight, BUTTON_DIRECTION);
  vDrawButton(renderer, &SDL_RECT_ButtonTurnArrow, BUTTON_DIRECTION);
  vDrawButton(renderer, &SDL_RECT_ButtonFireLaser, BUTTON_DIRECTION);
  vDrawButton(renderer, &SDL_RECT_ButtonUndoLast, BUTTON_ERASE);
  vDrawButton(renderer, &SDL_RECT_ButtonConfirm, BUTTON_CONFIRM); 
  vDrawButton(renderer, &SDL_RECT_ButtonConfigure, BUTTON_CONFIGURE);  
/*
  iGXRF_Add2RenderList(
    renderer,
    TRUE,
    SDL_RECT,
    &SDL_RECT_ButtonArrowRight,
    vDrawButton
  );

  iGXRF_ADD2ArgList(&SDL_RECT_ButtonArrowRight, (void *)(eSDLTypes=SDL_RECT), &SDL_RECT_ButtonArrowRight);
  iGXRF_ADD2ArgList(&SDL_RECT_ButtonArrowRight, (void *)"int", (void *)BUTTON_DIRECTION);
*/
  // Main loop
  SDL_Event event;
  while (gbRunning) {

    // The player has choose its route already?
    while (gbACTION_Check){
      // yes ...
      if (giACTION_AssertedSteps > 0)
        SDL_Delay(1000);

      if ( giACTION_AssertedSteps >= giACTION_StepCtr ){
        gbACTION_Check = FALSE;
        break;
      }

      iRedrawAction = iACTION_ExecuteStep(giACTION_AssertedSteps++);
      if  ( iRedrawAction < 0 )
        gbRunning = FALSE;

      iRedrawAction = REDRAW_IMAGE;
      if ( DEBUG_MSGS ) { 
        char szMsg[256];
        sprintf(szMsg,
      "iExecuteActionFromList Rsl(Redraw)=%d"
      " gbRunning=%d giACTION_AssertedSteps=%d giACTION_StepCtr=%d\n", 
          iRedrawAction, 
          gbRunning,
          giACTION_AssertedSteps,
          giACTION_StepCtr
        );
        vTraceMsg(szMsg);
      }
      break;
    }

    while(!gbACTION_Check && SDL_PollEvent(&event)){
      // The player hasn't choose its route yet,
      // so we must watch all interaction events... 
      switch (event.type) {
        case SDL_QUIT:
          gbRunning = FALSE;
          break;
        case SDL_MOUSEBUTTONDOWN:
          if ( (iRedrawAction = iHandleClick(&event)) > 0 ){
            gbRunning = FALSE;
            iRedrawAction = REDRAW_NONE;
            break;
          }
          if ( iRedrawAction == -10)
            iRedrawAction = REDRAW_IMAGE;
          break;
        case SDL_KEYDOWN:
          iHandleEventKey(&event);
          iRedrawAction = REDRAW_IMAGE;
          break;
        case SDL_MOUSEMOTION:
          iHandleMouseMotion(pSDL_RECT_Menu, &event);
          break;
        default:
          break;
      }
    }
    if ( iRedrawAction == ERROR_WALKING )
      break;
    // If nothing has changed, we will not redraw...
    if ( iRedrawAction != REDRAW_IMAGE && iRedrawAction != -1 ){
      continue;
    }
    //
    // Redraw 
    //
    if ( DEBUG_MSGS ) vTraceMsg("Main loop drawing section\n");

    ui64ElapsedTime = SDL_GetTicks64();
   
    SDL_RenderClear(renderer);
    
    iBOARD_Colorfy(renderer); 
    
    vSetHUDRectSize(&SDL_RECT_Hud);
    vSetButtonHUDRectSize(&SDL_RECT_ButtonHud);

    SDL_RenderCopyEx(renderer, pSDL_TXTR_ImagePlayer, NULL, &SDL_RECT_Player, giDeg, NULL, SDL_FLIP_HORIZONTAL);

    vDrawCommandHUD(renderer, pSDL_TXTR_Hud, &SDL_RECT_Hud);
    vDrawButtonHUD (renderer, pSDL_TXTR_ButtonHud, &SDL_RECT_ButtonHud);
  
    vDrawButton(renderer, &SDL_RECT_ButtonArrowRight, BUTTON_DIRECTION);
    vDrawButton(renderer, &SDL_RECT_ButtonTurnArrow, BUTTON_DIRECTION);
    vDrawButton(renderer, &SDL_RECT_ButtonFireLaser, BUTTON_DIRECTION);
    vDrawButton(renderer, &SDL_RECT_ButtonUndoLast, BUTTON_ERASE);
    vDrawButton(renderer, &SDL_RECT_ButtonConfirm, BUTTON_CONFIRM);
    vDrawButton(renderer, &SDL_RECT_ButtonConfigure, BUTTON_CONFIGURE);

    SDL_RenderCopy(renderer, pSDL_TXTR_ImageFoward, NULL, &SDL_RECT_ButtonArrowRight);
    SDL_RenderCopy(renderer, pSDL_TXTR_ImageLaser , NULL, &SDL_RECT_ButtonFireLaser);
    SDL_RenderCopy(renderer, pSDL_TXTR_ImageRotate, NULL, &SDL_RECT_ButtonTurnArrow);
    SDL_RenderCopy(renderer, pSDL_TXTR_ImageConfig, NULL, &SDL_RECT_ButtonConfigure);

    vDrawMenu(renderer, pSDL_RECT_Menu, ppszMenuOpt, MAX_MENU_OPTIONS, pttf_Font);
    // Update the screen
    SDL_RenderPresent(renderer);
    
    // Vamos sincronizar o refreshrate..
    ui64ElapsedTime -= SDL_GetTicks64();
    if (ui64ElapsedTime <= VSYNC_TIME) SDL_Delay(VSYNC_TIME - ui64ElapsedTime);
    
    // Redoing action, as we got no relevant interaction, no draw allowed.
    iRedrawAction = 0;
  }
  
  // Clean up
  TTF_CloseFont(pttf_Font);
  vBUTTON_FreeList();
  // Don't forget to destroy the texture when you're done with it
  SDL_DestroyTexture(pSDL_TXTR_Hud);
  SDL_DestroyTexture(pSDL_TXTR_ButtonHud);
  SDL_DestroyTexture(pSDL_TXTR_ImagePlayer);
  SDL_DestroyTexture(pSDL_TXTR_ImageFoward);
  SDL_DestroyTexture(pSDL_TXTR_ImageLaser );
  SDL_DestroyTexture(pSDL_TXTR_ImageRotate);
  SDL_DestroyTexture(pSDL_TXTR_ImageConfig);
  SDL_DestroyTexture(pSDL_TXTR_SquareBorder);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  if( DEBUG_MSGS ) vTraceEnd();

  // return iRsl;
  return 0;
}

