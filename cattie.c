#include <inttypes.h>
#include <time.h>
#include <cattie.h>
#include <board.h>
#include <menu.h>
#include <action.h>
#include <button.h>

#ifdef _WIN32
  #include <windows.h>
  #define getpid _getpid 
  #define gettimeofday mingw_gettimeofday
#elif LINUX
  #include <unistd.h>
  #define SDL_main main
  #define _MAX_PATH 256
#endif

typedef int bool;

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

static const char *kszOptStr = "hvt:d:cC:";

/* Receive the name of program */
static const char *kpszProgramName;
STRUCT_COMMAND_LINE stCmdLine;

/**
 * 
 * Procedures and functions
 * 
 **/

void vInitRect(SDL_Rect *pSDL_RECT, int iX, int iY, int iWidth, int iHeight){
  pSDL_RECT->x = iX;
  pSDL_RECT->y = iY;
  pSDL_RECT->w = iWidth;
  pSDL_RECT->h = iHeight;
}

SDL_Surface *pSDL_SRFC_LoadImage(char *pszImgPath){
  // Load the image
  SDL_Surface *SDL_SRFC_Img = IMG_Load(pszImgPath);
  if (SDL_SRFC_Img == NULL) {
    printf("Error loading image: %s\n", IMG_GetError());
    return NULL;
  }
  return SDL_SRFC_Img;
}

SDL_Texture *pSDL_TXTR_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *pSDL_SRFC_Img){
  // Create the texture from the image
  SDL_Texture *SDL_TXTR_Texture = SDL_CreateTextureFromSurface(renderer, pSDL_SRFC_Img);
  if (SDL_TXTR_Texture == NULL) {
    printf("Error creating texture: %s\n", SDL_GetError());
    return NULL;
  }

  return SDL_TXTR_Texture;
}

void vDrawSquareEdges(SDL_Renderer *renderer){
  int ii;
  int jj;
  for (ii = 0; ii < BOARD_ROWS; ii++) {
    for (jj = 0; jj < BOARD_COLS; jj++) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_Rect SDLRECT_Edges = {ii * COL_RATIO, jj * COL_RATIO, COL_RATIO, COL_RATIO};
      SDL_RenderDrawRect(renderer, &SDLRECT_Edges);
    }
  }
}

void vDrawCommandHUD(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect *pSDL_RECT_Hud){
  (void) texture;

  SDL_SetRenderTarget(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 17, 84, 143, 96);
  SDL_RenderFillRect(renderer, pSDL_RECT_Hud);
  SDL_SetRenderDrawColor(renderer, 17, 84, 143, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Hud);
}

void vDrawButtonHUD(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect *pSDL_RECT_Hud){
  (void) texture;

  SDL_SetRenderTarget(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 128, 4, 0, 128);
  SDL_RenderFillRect(renderer, pSDL_RECT_Hud);
  SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Hud);
}

void vDrawButton(SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Button, int iButtonType){
  if ( iButtonType == BUTTON_DIRECTION )
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
  else if ( iButtonType == BUTTON_CONFIRM )
    SDL_SetRenderDrawColor(renderer, 0, 200, 50, 255);
  else if ( iButtonType == BUTTON_ERASE )
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    
  SDL_RenderFillRect(renderer, pSDL_RECT_Button);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Button);
}

void vInitializeImagePosition(SDL_Rect *pSDL_Rect_Im){
  int iLocation = -2;
  int iDimensions = COL_RATIO;
  vInitRect(pSDL_Rect_Im, iLocation, iLocation, iDimensions, iDimensions);
}

eSqType iValidateSquare(int iX, int iY){
  eSqType eSquareType = -1;

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
  return eSquareType = giBOARD_Main[iX][iY]; 
}

int iWalk(){
  int iNextX = gstPlayer.iCurrX;
  int iNextY = gstPlayer.iCurrY;
  int YOffset = 0;
  int XOffset = 0;
  
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
    return -1;
    
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
  return gstPlayer.iFacingPos;
}

int iTurn(){
  if ( DEBUG_MSGS ) vTraceMsg("Turn\n");
  giDeg = (giDeg == 360) ? 90 : giDeg + 90;
  gstPlayer.iFacingPos = (gstPlayer.iFacingPos == WEST) ? NORTH : gstPlayer.iFacingPos+1;
  return REDRAW_IMAGE;
}

int iFireLaser(){
  return REDRAW_IMAGE;
}

void vSetButtonDimensions(SDL_Rect *pSDL_RECT_Btn, int iTrslt){
  pSDL_RECT_Btn->w = 0.06*WINDOW_WIDTH;
  pSDL_RECT_Btn->h = 0.06*WINDOW_HEIGHT;
  pSDL_RECT_Btn->x = pSDL_RECT_Btn->w + iTrslt;
  pSDL_RECT_Btn->y = WINDOW_HEIGHT - pSDL_RECT_Btn->h - 20;
}

void vSetHUDRectSize(SDL_Rect *pSDL_RECT_Hud){
  pSDL_RECT_Hud->x = WINDOW_WIDTH/4;
  pSDL_RECT_Hud->y = 0;
  pSDL_RECT_Hud->w = 2*pSDL_RECT_Hud->x;
  pSDL_RECT_Hud->h = COL_RATIO;
}

void vSetButtonHUDRectSize(SDL_Rect *pSDL_RECT_Hud){
  pSDL_RECT_Hud->x = 0.06*WINDOW_WIDTH - 10;
  pSDL_RECT_Hud->y = WINDOW_HEIGHT - 0.06*WINDOW_HEIGHT - 30;
  pSDL_RECT_Hud->w = 2*WINDOW_WIDTH/4 - 20;
  pSDL_RECT_Hud->h = COL_RATIO - 10;
}

int iWasClicked(SDL_Event *pSDL_EVENT_Ev){
  STRUCT_BUTTON_LIST *pstWrkButtonList;
  for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext){
    int iClickEvX = pSDL_EVENT_Ev->button.x;
    int iClickEvY = pSDL_EVENT_Ev->button.y;
    SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;
    if ( pSDL_RECT_Btn == NULL )
      break;

    if ( iClickEvX >= pSDL_RECT_Btn->x 
      && iClickEvY >= pSDL_RECT_Btn->y 
      && iClickEvX < pSDL_RECT_Btn->x + pSDL_RECT_Btn->w 
      && iClickEvY < pSDL_RECT_Btn->y + pSDL_RECT_Btn->h) {
      return pstWrkButtonList->iAction;
    }
  }
  return FALSE;
}

SDL_Texture* createSquareTexture(SDL_Renderer* renderer)
{
  SDL_Surface* surface = SDL_CreateRGBSurface(0, 800, 800, 8, 0, 0, 0, 0);
  SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(surface);
  return texture;
}

int iHandleClick(SDL_Event *pSDL_EVENT_Ev){
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
    default:
      break;
  }

  return 0;
}

int iHandleEventKey(SDL_Event *pSDL_EVENT_Ev){
  vMENU_HandleKey(pSDL_EVENT_Ev->key.keysym.sym);

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
  return 0;
}

int iCheckMenuInteraction(SDL_Rect *pSDL_RECT_MenuData, int iXCursor, int iYCursor){
  int iInitCt = 0;
  SDL_Rect *pSDL_RECT_Wrk;
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
      return REDRAW_IMAGE;
    }
  }
  return 0;
}

int iHandleMouseMotion(SDL_Rect *pSDL_RECT_Menu, SDL_Event *pSDL_EVENT_Ev){
  int iX, iY;

  (void) pSDL_RECT_Menu;

  SDL_GetMouseState(&iX, &iY);

  if ( iBUTTON_CheckInteraction(pSDL_EVENT_Ev, iX, iY) == REDRAW_IMAGE )
    return REDRAW_IMAGE;

  return 0;
}

void vPrintUsage(void)
{
  int ii = 0;
  
  printf("Usage %s [options] <arguments>\n\n"
         "%s\n\n"
         "Options:\n", kpszProgramName, DESCRIPTION);
  while(astCmdOpt[ii].name)
  {
    if(astCmdOpt[ii].has_arg == required_argument)
    {
      printf("  --%s=<%s>, -%c <%s>\n"
             "    %s\n\n", astCmdOpt[ii].name, pszCmdArguments[ii],
                           astCmdOpt[ii].val, pszCmdArguments[ii],
                           pszCmdMessages[ii]);
    }
    else
    {
      printf("  --%s, -%c\n"
             "    %s\n\n", astCmdOpt[ii].name, astCmdOpt[ii].val,
                           pszCmdMessages[ii]);
    }

    ii++;
  }
}

void vPrintVersion(void)
{
  printf("%s %s\n"
         "Build in %s %s\n"
         "%s %s\n"
         "For reporting bugs, send a email to:\n"
         "<%s>\n" 
         "<%s>\n", kpszProgramName, 
                   VERSION,
                   __DATE__,
                   __TIME__,
                   COPYRIGHT,
                   DEVELOPER,
                   RFERMI_MAIL,
                   BACAGINE_MAIL
  );
}

bool bCommandLineIsOK(int argc, char **argv)
{
  int iCmdLineOpt = 0;
  
  /**
   * Used to get the final of
   * conversion of strtol
   */
  char *pchEndPtr; 

  while((iCmdLineOpt = getopt_long(argc, argv, kszOptStr, astCmdOpt, NULL )) != -1)
  {
    switch(iCmdLineOpt)
    {
      case 'h':
        vPrintUsage();
        exit(EXIT_SUCCESS);
      case 'v':
        vPrintVersion();
        exit(EXIT_SUCCESS);
      case 't':
        sprintf(stCmdLine.szLogFileName, "%s", optarg);
        break;
      case 'd':
        sprintf(stCmdLine.szDebugLevel, "%s", optarg);

        strtol(stCmdLine.szDebugLevel, &pchEndPtr, 10);

        if(*pchEndPtr != '\0')
        {
          return FALSE;
        }

        break;
      case '?':
      default:
        return FALSE;
    }
  }

  return TRUE;
}

/**
 *
 * main
 *  
 **/
int SDL_main(int argc, char *argv[]){
  int iXTranslation = 0;
  int iRedrawAction = -1;
  uint64_t ui64ElapsedTime;
  SDL_Texture *pSDL_TXTR_ImagePlayer;
  SDL_Texture *pSDL_TXTR_ImageFoward;
  SDL_Texture *pSDL_TXTR_ImageRotate;
  SDL_Texture *pSDL_TXTR_ImageLaser;
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
  SDL_Rect SDL_RECT_ButtonConfirmAction;
  SDL_Rect *pSDL_RECT_Menu = NULL;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Renderer* pSDL_Renderer;
  // int iVArgsCt;
  // va_list pvlst_ArgList;
  
  (void) SDL_RECT_ButtonTurnArrow;
  (void) pSDL_Renderer;
  (void) SDL_RECT_ButtonUndoLast;
  (void) SDL_RECT_ButtonFireLaser;
  (void) SDL_RECT_ButtonTurnArrow;
  (void) SDL_RECT_ButtonConfirmAction;
  
  kpszProgramName = argv[0];

  if(!bCommandLineIsOK(argc, argv))
  {
    vPrintUsage();
    return -1;
  }

  vInitLogs(argv[0]);
   
  if ( DEBUG_MSGS ) vTraceMsg("SDL_Main --- Init\n");

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
  
  // Create a window
  window = SDL_CreateWindow(
    WINDOW_TITLE,
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 
    WINDOW_WIDTH, 
    WINDOW_HEIGHT, 
    SDL_WINDOW_SHOWN
  );

  if ( DEBUG_MSGS ) vTraceMsg("SDL_CreateWindow --- Ok\n");

  // Create a renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  pSDL_Renderer = renderer;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  iGXRF_Init();

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
  // {
  //   char szMsg[256];
  //   sprintf(szMsg,"%d-%d-%d-%d", SDL_RECT_ButtonArrowRight.h, SDL_RECT_ButtonArrowRight.w,  SDL_RECT_ButtonArrowRight.x,  SDL_RECT_ButtonArrowRight.y);
  //   vTraceMsg(szMsg);
  // }
  // iXTranslation += SDL_RECT_ButtonArrowRight.w + 10;
  // // Turn Arrow
  // vSetButtonDimensions(&SDL_RECT_ButtonTurnArrow, iXTranslation);
  // iXTranslation += SDL_RECT_ButtonTurnArrow.w + 10;
  // // Fire Laser
  // vSetButtonDimensions(&SDL_RECT_ButtonFireLaser, iXTranslation);
  // iXTranslation += SDL_RECT_ButtonFireLaser.w + 50;
  // // Undo Last
  // vSetButtonDimensions(&SDL_RECT_ButtonUndoLast, iXTranslation);
  // iXTranslation += SDL_RECT_ButtonUndoLast.w + 50;
  // // Confirm Action
  // vSetButtonDimensions(&SDL_RECT_ButtonConfirmAction, iXTranslation);
  
  // Square Edges
  vDrawSquareEdges(renderer);
  
  // Add them to list
  // iBUTTON_AddToList(&SDL_RECT_ButtonArrowRight   , FORWARD);
  // iBUTTON_AddToList(&SDL_RECT_ButtonTurnArrow    , TURN);
  // iBUTTON_AddToList(&SDL_RECT_ButtonFireLaser    , FIRE_LASER) ;
  // iBUTTON_AddToList(&SDL_RECT_ButtonUndoLast     , ERASE);
  // iBUTTON_AddToList(&SDL_RECT_ButtonConfirmAction, CONFIRM);
  SDL_RenderPresent(renderer);

  pSDL_TXTR_ImagePlayer  = IMG_LoadTexture(renderer  , ppszImagePath[PLAYER_IMG_PATH_IDX]); 
  pSDL_TXTR_ImageFoward  = IMG_LoadTexture(renderer  , ppszImagePath[FORWARD_IMG_PATH_IDX]); 
  pSDL_TXTR_ImageLaser   = IMG_LoadTexture(renderer  , ppszImagePath[LASER_IMG_PATH_IDX]);
  pSDL_TXTR_ImageRotate  = IMG_LoadTexture(renderer  , ppszImagePath[ROTATE_IMG_PATH_IDX]); 
  pSDL_TXTR_Hud          = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SDL_RECT_Hud.w, SDL_RECT_Hud.h);
  pSDL_TXTR_ButtonHud    = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SDL_RECT_ButtonHud.w, SDL_RECT_ButtonHud.h);
  pSDL_TXTR_SquareBorder = createSquareTexture(renderer); 

  vInitializeImagePosition(&SDL_RECT_Player);

  //pSDL_RECT_Menu = (SDL_Rect *) malloc(MAX_MENU_OPTIONS*sizeof(SDL_Rect));
  
  // vInitMenu(pSDL_RECT_Menu, MAX_MENU_OPTIONS);

  gstPlayer.pSDL_RECT_Player = &SDL_RECT_Player;

  // vDrawButton(renderer, &SDL_RECT_ButtonArrowRight, BUTTON_DIRECTION);
  // vDrawButton(renderer, &SDL_RECT_ButtonTurnArrow, BUTTON_DIRECTION);
  // vDrawButton(renderer, &SDL_RECT_ButtonFireLaser, BUTTON_DIRECTION);
  // vDrawButton(renderer, &SDL_RECT_ButtonUndoLast, BUTTON_ERASE);
  // vDrawButton(renderer, &SDL_RECT_ButtonConfirmAction, BUTTON_CONFIRM);  
  // 
  /*iGXRF_Add2RenderList(
    renderer,
    TRUE,
    SDL_RECT,
    &SDL_RECT_ButtonArrowRight,
    //pszFmt,
    vDrawButton,
    2,
    &SDL_RECT_ButtonArrowRight,
    BUTTON_DIRECTION
  );*/
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
          if ( iHandleClick(&event) ){
            gbRunning = FALSE;
          }
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
    
    // vSetHUDRectSize(&SDL_RECT_Hud);
    // vSetButtonHUDRectSize(&SDL_RECT_ButtonHud);

    SDL_RenderCopyEx(renderer, pSDL_TXTR_ImagePlayer, NULL, &SDL_RECT_Player, giDeg, NULL, SDL_FLIP_HORIZONTAL);

    vDrawCommandHUD(renderer, pSDL_TXTR_Hud, &SDL_RECT_Hud);
    vDrawButtonHUD (renderer, pSDL_TXTR_ButtonHud, &SDL_RECT_ButtonHud);
      
    // vDrawButtons(renderer);
    
    // vGXRF_RenderAll();
    //vGXRF_RenderObject(&SDL_RECT_ButtonArrowRight);
    //  vDrawButton(renderer, &SDL_RECT_ButtonArrowRight, BUTTON_DIRECTION);
    // vDrawButton(renderer, &SDL_RECT_ButtonTurnArrow, BUTTON_DIRECTION);
    // vDrawButton(renderer, &SDL_RECT_ButtonFireLaser, BUTTON_DIRECTION);
    // vDrawButton(renderer, &SDL_RECT_ButtonUndoLast, BUTTON_ERASE);
    // vDrawButton(renderer, &SDL_RECT_ButtonConfirmAction, BUTTON_CONFIRM);

    // SDL_RenderCopy(renderer, pSDL_TXTR_ImageFoward, NULL, &SDL_RECT_ButtonArrowRight);
    // SDL_RenderCopy(renderer, pSDL_TXTR_ImageLaser , NULL, &SDL_RECT_ButtonFireLaser);
    // SDL_RenderCopy(renderer, pSDL_TXTR_ImageRotate, NULL, &SDL_RECT_ButtonTurnArrow);

    // vDrawMenu(renderer, pSDL_RECT_Menu, MAX_MENU_OPTIONS);
    // Update the screen
    SDL_RenderPresent(renderer);
    
    // Vamos sincronizar o refreshrate..d
    ui64ElapsedTime -= SDL_GetTicks64();
    if (ui64ElapsedTime <= VSYNC_TIME) SDL_Delay(VSYNC_TIME - ui64ElapsedTime);
    
    // Redoing action, as we got no relevant interaction, no draw allowed.
    iRedrawAction = 0;
  }
  
  // Clean up
  iGXRF_End();
  vBUTTON_FreeList();
  // Don't forget to destroy the texture when you're done with it
  SDL_DestroyTexture(pSDL_TXTR_Hud);
  SDL_DestroyTexture(pSDL_TXTR_ButtonHud);
  SDL_DestroyTexture(pSDL_TXTR_ImagePlayer);
  SDL_DestroyTexture(pSDL_TXTR_ImageFoward);
  SDL_DestroyTexture(pSDL_TXTR_ImageLaser );
  SDL_DestroyTexture(pSDL_TXTR_ImageRotate);
  SDL_DestroyTexture(pSDL_TXTR_SquareBorder);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
