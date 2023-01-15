#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <inttypes.h>
#include <time.h>

#ifdef _WIN32
  #include <windows.h>
  #define getpid _getpid 
#elif LINUX
  #include <unistd.h>
  #define SDL_main SDL_main
  #define _MAX_PATH 256
#endif


#define TILE_ADDER 6 // Level + ADDER
#define VSYNC_TIME 16.666666666 //tempo em ms para atualização em 60 FPS
#define _MAX_MOVEMENT 1000  // Level + ADDER

#define BOARD_ROWS 10
#define BOARD_COLS 10
// #define BOARD_ROWS 5
// #define BOARD_COLS 5
// #define BOARD_ROWS 20
// #define BOARD_COLS 20

#define BUTTON_DIRECTION 1
#define BUTTON_CONFIRM   2
#define BUTTON_ERASE     3

#define BOARD_SIZE (BOARD_ROWS * BOARD_COLS)

#define IMG_PATH "img.png"

#define PLAYER_IMG_PATH_IDX  0
#define FORWARD_IMG_PATH_IDX 1
#define LASER_IMG_PATH_IDX   2
#define ROTATE_IMG_PATH_IDX  3

#define _MAX_IMG_PATH 4

#define DEBUG_MSGS 1

char *ppszImagePath[] = {
  "cat2.png",
  "forward.png",
  "laser.png",
  "rotate2.png"
};

#define TRUE 1
#define FALSE 0

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define REDRAW_IMAGE 1
#define ERROR_WALKING 10

char gszLogTitle[_MAX_PATH];

typedef struct STRUCT_BUTTON_LIST{
  SDL_Rect *pSDL_RECT_Button;
  int iAction;
  struct STRUCT_BUTTON_LIST* pstNext;
}STRUCT_BUTTON_LIST;

STRUCT_BUTTON_LIST gstButtonList;

enum SquareColors{
  WALL_SQUARE = 0,
  PATH_SQUARE,
  END_SQUARE
}eSqColors;

enum MovementAction{
  FORWARD = 1,
  TURN,
  FIRE_LASER,
  ERASE,
  CONFIRM
}eMovAction;

int giActionList[_MAX_MOVEMENT];
int giActionCt = 0;
int giCheckActions = 0;
enum WindRose{
  NORTH = 1,
  EAST,
  SOUTH,
  WEST
}eWndRose;

typedef struct STRUCT_PLAYER {
  int iCurrX;
  int iCurrY;
  int iFacingPos;
  SDL_Rect *pSDL_RECT_Player;
}STRUCT_PLAYER;

STRUCT_PLAYER gstPlayer;

int giSquareSize;
int giDeg = 0;
int gbRunning = TRUE;
int gbCheckActions = FALSE;

int giBoard[BOARD_ROWS][BOARD_COLS];

int iSetTotalPlayableTiles(int iLevel){
  return iLevel + TILE_ADDER;
}

void vInitializeBoard(){
  int iX;
  int iY;
  for (iX = 0; iX < BOARD_ROWS; iX++)
    for (iY = 0; iY < BOARD_COLS; iY++)
      giBoard[iX][iY] = 0;
}

void vTraceMsg(char *szMsg){
   FILE *pfLog;
  time_t lTime;
  struct tm *st_tm_Now;
  char szDateTimeNow_us[128];
  // struct timeval tv;
  time (&lTime);
  st_tm_Now = localtime(&lTime);
  // mingw_gettimeofday(&tv, NULL);

  memset(szDateTimeNow_us, 0, sizeof(szDateTimeNow_us));
  sprintf(szDateTimeNow_us,
    "[%02d/%02d/%04d %02d:%02d:%02d] ",
    (int)st_tm_Now->tm_mday, 
    (int)st_tm_Now->tm_mon+1, 
    (int)st_tm_Now->tm_mday,
    (int)st_tm_Now->tm_hour,
    (int)st_tm_Now->tm_min,
    (int)st_tm_Now->tm_sec
  );  

  if ( (pfLog=fopen(gszLogTitle, "a+")) == NULL )
    return;

  fprintf(pfLog, "%s", szMsg);

  fclose(pfLog);
}
void vTracePid(char *szMsg, int iMsgLen){
  char *pszMyMsg;
  int iNewMsgLen = iMsgLen + 16;
  int iPid;
  iPid = getpid();
  
  if ( (pszMyMsg = (char *) malloc(iNewMsgLen)) == NULL)
    return;

  memset(pszMyMsg, 0, iNewMsgLen);
  sprintf(pszMyMsg, "%d %s", iPid, szMsg);

  vTraceMsg(pszMyMsg);

  free(pszMyMsg);
}
void vTraceBoard(){
  int ii;
  int jj;
  
  vTraceMsg("========================\n");
  for (ii = 0; ii < BOARD_ROWS; ii++) {
    for (jj = 0; jj < BOARD_COLS; jj++) {
      char szWrk[8];
      memset(szWrk, 0, sizeof(szWrk));
      sprintf(szWrk, "[%d] ", giBoard[ii][jj]);
      vTraceMsg(szWrk);
    }
    vTraceMsg("\n");
  }
  vTraceMsg("========================\n");
}

int iAddButtonToList(SDL_Rect *pSDL_RECT_Btm, int iAction){
  STRUCT_BUTTON_LIST *pstWrkButtonList;

  eMovAction = iAction;

  for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList->pstNext != NULL; pstWrkButtonList = pstWrkButtonList->pstNext );
  
  if ( pstWrkButtonList->iAction == 0 && pstWrkButtonList == &gstButtonList){
      pstWrkButtonList->pSDL_RECT_Button = pSDL_RECT_Btm;
    pstWrkButtonList->iAction = eMovAction;
    pstWrkButtonList->pstNext = NULL;
    return 0;
  }
  if ( (pstWrkButtonList->pstNext = (STRUCT_BUTTON_LIST *) malloc (sizeof(STRUCT_BUTTON_LIST))) == NULL )
    return -1;
  
  memset(pstWrkButtonList->pstNext, 0, sizeof(STRUCT_BUTTON_LIST));
  pstWrkButtonList = pstWrkButtonList->pstNext;
  pstWrkButtonList->pSDL_RECT_Button = pSDL_RECT_Btm;
  pstWrkButtonList->iAction = eMovAction;
  pstWrkButtonList->pstNext = NULL;

  return 0;
}

void vFreeButtonList(){
  STRUCT_BUTTON_LIST *pstWrkButtonList;

  for ( pstWrkButtonList = gstButtonList.pstNext; pstWrkButtonList != NULL;  ){ 
    STRUCT_BUTTON_LIST *pstLastButton = pstWrkButtonList;
    pstWrkButtonList = pstWrkButtonList->pstNext;
    free(pstLastButton);
  }
}

void vInitButtonList(){
  memset(&gstButtonList, 0, sizeof(STRUCT_BUTTON_LIST));
  gstButtonList.pstNext = NULL;
}

int iGenerateRandomPath(){
  int iRow = 0;
  int iCol = 0;
  int bFirstSq = TRUE;
  int iLastRow = 0;
  int iLastCol = 0;
  int iDirection;
  
  vInitializeBoard();

  while (iRow < BOARD_ROWS && iCol < BOARD_COLS) {
    giBoard[iRow][iCol] = 1;
    iLastRow = iRow;
    iLastCol = iCol;
    iDirection = rand() % 2; // Generate a random number (0 or 1)
    if (iDirection == 0) {
      // Go down
      iRow++;
    } else {
      // Go right
      iCol++;
    }
    if ( bFirstSq ) {
      gstPlayer.iCurrX = 0;
      gstPlayer.iCurrY = 0;
      if ( iDirection == 0 ){
        gstPlayer.iFacingPos = SOUTH;
        giDeg = 90;
      }
      else{
        gstPlayer.iFacingPos = EAST;
      }
      bFirstSq = FALSE;
    }
  }

  giBoard[iLastRow][iLastCol] = 2;
  
  vTraceBoard();

  return 0;
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

// Dinamizar as cores...
int iColorBoard(SDL_Renderer *renderer){
  int ii;
  int jj;
  for (ii = 0; ii < BOARD_ROWS; ii++) {
    for (jj = 0; jj < BOARD_COLS; jj++) {
      const int iSquare = giBoard[ii][jj];
      switch(iSquare){
        // Cinza claro
        case WALL_SQUARE:
          SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
          break;
        // amarelo claro
        case PATH_SQUARE:
          SDL_SetRenderDrawColor(renderer, 255, 255, 224, 255); 
          break;
        // vermelhão
        case END_SQUARE:
          SDL_SetRenderDrawColor(renderer, 235, 150, 70, 255);
          break;
        default:
          // Se não é nada, branco...
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          break;
      }

      SDL_Rect SDLRECT_Square = {jj * giSquareSize, ii * giSquareSize, giSquareSize, giSquareSize};
      SDL_RenderFillRect(renderer, &SDLRECT_Square);
      
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_Rect SDLRECT_Edges = {jj * giSquareSize, ii * giSquareSize, giSquareSize, giSquareSize};
      SDL_RenderDrawRect(renderer, &SDLRECT_Edges);
    }
  }

  return 0;
}

void vSetBoardSquareBorder(SDL_Renderer *renderer){
  int ii;
  int jj;
  for (ii = 0; ii < BOARD_ROWS; ii++) {
    for (jj = 0; jj < BOARD_COLS; jj++) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_Rect SDLRECT_Edges = {ii * giSquareSize, jj * giSquareSize, giSquareSize, giSquareSize};
      SDL_RenderDrawRect(renderer, &SDLRECT_Edges);
    }
  }
}

void vDrawCommandHUD(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect *pSDL_RECT_Hud){
  SDL_SetRenderTarget(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 17, 84, 143, 96);
  SDL_RenderFillRect(renderer, pSDL_RECT_Hud);
  SDL_SetRenderDrawColor(renderer, 17, 84, 143, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Hud);
}
void vDrawButtonHUD(SDL_Renderer *renderer, SDL_Texture* texture, SDL_Rect *pSDL_RECT_Hud){
  SDL_SetRenderTarget(renderer, NULL);
  SDL_SetRenderDrawColor(renderer, 128, 4, 0, 128);
  SDL_RenderFillRect(renderer, pSDL_RECT_Hud);
  SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Hud);
}

void vDrawButton(SDL_Renderer *renderer, SDL_Rect *pSDL_RECT_Button, int iButtonType){
  if ( iButtonType == BUTTON_DIRECTION )
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
  else if ( iButtonType == BUTTON_CONFIRM)
    SDL_SetRenderDrawColor(renderer, 0, 200, 50, 255);
  else if ( iButtonType == BUTTON_ERASE)
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    
  SDL_RenderFillRect(renderer, pSDL_RECT_Button);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer, pSDL_RECT_Button);
}

void vInitializeImagePosition(SDL_Rect *pSDL_Rect_Im){
  pSDL_Rect_Im->w = WINDOW_WIDTH/BOARD_COLS;
  pSDL_Rect_Im->h = WINDOW_WIDTH/BOARD_ROWS;
  pSDL_Rect_Im->x = -2;
  pSDL_Rect_Im->y = -2;
}


int iMovementIsOutOfBounds(int iNextX, int iNextY){
  if ( iNextX < 0 || iNextX >= BOARD_COLS )
    return TRUE;
  if ( iNextY < 0 || iNextY >= BOARD_ROWS )
    return TRUE;

  return FALSE;
}

int iValidateSquare(int iX, int iY){
   if ( DEBUG_MSGS ) { 
    char szMsg[256];
    sprintf(szMsg,
  "iValidateSquare giBoard[%d][%d]=%d\n ", 
      iX,
      iY,
      giBoard[iX][iY]
    );
    vTraceMsg(szMsg);
  }
  return (eSqColors = giBoard[iX][iY]); 
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
  
  if ( iMovementIsOutOfBounds(iNextX, iNextY) )
    return -1;

  if ( iValidateSquare(iNextX, iNextY) == WALL_SQUARE )
    return -2;
    
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
void vSetButtonDimensions(SDL_Rect *pSDL_RECT_Btn, int iTrslt){
  pSDL_RECT_Btn->w = 0.06*WINDOW_WIDTH;
  pSDL_RECT_Btn->h = 0.06*WINDOW_HEIGHT;
  pSDL_RECT_Btn->x = pSDL_RECT_Btn->w + iTrslt;
  pSDL_RECT_Btn->y = WINDOW_HEIGHT - pSDL_RECT_Btn->h - 20;
}
void vSetHUDRectSize(SDL_Rect *pSDL_RECT_Hud){
  pSDL_RECT_Hud->x = WINDOW_WIDTH/4;
  pSDL_RECT_Hud->y = 0;
  pSDL_RECT_Hud->w = 2*WINDOW_WIDTH/4;
  pSDL_RECT_Hud->h = WINDOW_WIDTH/BOARD_ROWS;
}
void vSetButtonHUDRectSize(SDL_Rect *pSDL_RECT_Hud){
  pSDL_RECT_Hud->x = 0.06*WINDOW_WIDTH - 10;
  pSDL_RECT_Hud->y = WINDOW_HEIGHT - 0.06*WINDOW_HEIGHT - 30;
  pSDL_RECT_Hud->w = 2*WINDOW_WIDTH/4 - 20;
  pSDL_RECT_Hud->h = WINDOW_WIDTH/BOARD_ROWS - 10;
}

int iWasClicked(SDL_Event *pSDL_EVENT_Ev){
  STRUCT_BUTTON_LIST *pstWrkButtonList;
  for ( pstWrkButtonList = &gstButtonList; pstWrkButtonList != NULL; pstWrkButtonList = pstWrkButtonList->pstNext){
    int iClickEvX = pSDL_EVENT_Ev->button.x;
    int iClickEvY = pSDL_EVENT_Ev->button.y;
    SDL_Rect *pSDL_RECT_Btn = pstWrkButtonList->pSDL_RECT_Button;
    if (iClickEvX >= pSDL_RECT_Btn->x 
      && iClickEvX < pSDL_RECT_Btn->x + pSDL_RECT_Btn->w 
      && iClickEvY >= pSDL_RECT_Btn->y 
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

int iTurn(){
  if ( DEBUG_MSGS ) vTraceMsg("Turn\n");
  giDeg = (giDeg == 360) ? 90 : giDeg + 90;
  gstPlayer.iFacingPos = (gstPlayer.iFacingPos == WEST) ? NORTH : gstPlayer.iFacingPos+1;
  return REDRAW_IMAGE;
}
void vLogActionList();

int iAddCommandToActionList(int iCmd){
  if ( iCmd == ERASE ){
    if ( giActionCt <= 0 )
      return 0;

    giActionList[giActionCt--] = 0;
    return 0;
  }
  if ( giActionCt >= _MAX_MOVEMENT )
    return -1;
  
  giActionList[giActionCt++] = iCmd;
  return 0;
}
void vLogActionList(){
  int ii;
  vTraceMsg("==========Action List========");
  for ( ii=0; ii<giActionCt; ii++){
    char szWrk[32];
    memset(szWrk, 0, sizeof(szWrk));
    sprintf(szWrk, "[%d] ", giActionList[ii]);
    if ( (ii % 20) == 0 )
      vTraceMsg("\n\t");

    vTraceMsg(szWrk);
  }
     
  vTraceMsg("\n========================\n");

}

int iHandleClick(SDL_Event *pSDL_EVENT_Ev){
  int iAction = 0;
  switch((iAction = iWasClicked(pSDL_EVENT_Ev))){
    case FORWARD:
    case TURN:
    case FIRE_LASER:
    case ERASE:
      iAddCommandToActionList(iAction);
      vLogActionList();
      break;
    case CONFIRM:
      if ( DEBUG_MSGS ) vTraceMsg("Confirm!\n");
      gbCheckActions = TRUE;
      break;
    default:
      break;
  }

  return 0;
}


int iExecuteActionFromList(int iAction){
  int iRsl = 0;
  if ( iAction < 0 )
    return 0;

  if ( iAction >= _MAX_MOVEMENT )
    return 0;

  switch(giActionList[iAction]){
  case FORWARD:
    iRsl = iWalk();
    break;
  case TURN:
    iRsl = iTurn();
    break;
  case FIRE_LASER:
    break;
  default:
    break;
  }

  return iRsl;   
}

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
  SDL_Rect SDL_RECT_Rect;
  SDL_Rect SDL_RECT_Hud;
  SDL_Rect SDL_RECT_ButtonHud;
  SDL_Rect SDL_RECT_ButtonArrowRight;
  SDL_Rect SDL_RECT_ButtonTurnArrow;
  SDL_Rect SDL_RECT_ButtonFireLaser;
  SDL_Rect SDL_RECT_ButtonUndoLast;
  SDL_Rect SDL_RECT_ButtonConfirmAction;
  SDL_Window* window;
  SDL_Renderer* renderer;
  char *pTok;

  sprintf(gszLogTitle, "%s", argv[0]);
  if ( (pTok = strstr(gszLogTitle, ".exe")) != NULL ){
    *pTok = 0;
  }
  strcat(gszLogTitle, ".log");

  vTraceMsg("SDL_Main --- Init\n");

  SDL_SetMainReady();
  
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
    "Cattie",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 
    WINDOW_WIDTH, 
    WINDOW_HEIGHT, 
    SDL_WINDOW_SHOWN
  );


  // Create a renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Set Hud Rect Dimensions
  vSetHUDRectSize(&SDL_RECT_Hud);

  vSetButtonHUDRectSize(&SDL_RECT_ButtonHud);

  // Calculate the size of the squares
  giSquareSize = WINDOW_WIDTH / BOARD_COLS;

  // Generate a unique path.
  iGenerateRandomPath();

  // Clear structure
  vInitButtonList();
  
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
  vSetButtonDimensions(&SDL_RECT_ButtonConfirmAction, iXTranslation);
  // Square Edges
  vSetBoardSquareBorder(renderer);
  
  // Add them to list
  iAddButtonToList(&SDL_RECT_ButtonArrowRight   , FORWARD);
  iAddButtonToList(&SDL_RECT_ButtonTurnArrow    , TURN);
  iAddButtonToList(&SDL_RECT_ButtonFireLaser    , FIRE_LASER) ;
  iAddButtonToList(&SDL_RECT_ButtonUndoLast     , ERASE);
  iAddButtonToList(&SDL_RECT_ButtonConfirmAction, CONFIRM);

  SDL_RenderPresent(renderer);

  pSDL_TXTR_ImagePlayer  = IMG_LoadTexture(renderer  , ppszImagePath[PLAYER_IMG_PATH_IDX]); 
  pSDL_TXTR_ImageFoward  = IMG_LoadTexture(renderer  , ppszImagePath[FORWARD_IMG_PATH_IDX]); 
  pSDL_TXTR_ImageLaser   = IMG_LoadTexture(renderer  , ppszImagePath[LASER_IMG_PATH_IDX]); ;
  pSDL_TXTR_ImageRotate  = IMG_LoadTexture(renderer  , ppszImagePath[ROTATE_IMG_PATH_IDX]); 
  pSDL_TXTR_Hud          = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SDL_RECT_Hud.w, SDL_RECT_Hud.h);
  pSDL_TXTR_ButtonHud    = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SDL_RECT_ButtonHud.w, SDL_RECT_ButtonHud.h);
  pSDL_TXTR_SquareBorder = createSquareTexture(renderer); 

  vInitializeImagePosition(&SDL_RECT_Rect);

  gstPlayer.pSDL_RECT_Player = &SDL_RECT_Rect;
  
  SDL_Event event;
  while (gbRunning) {
    while (gbCheckActions){    

      if (giCheckActions > 0)
        SDL_Delay(2000);

      if ( giCheckActions >= giActionCt ){
        gbCheckActions = FALSE;
        break;
      }

      iRedrawAction = iExecuteActionFromList(giCheckActions++);
      if  ( iRedrawAction < 0 )
        gbRunning = FALSE;

      iRedrawAction = REDRAW_IMAGE;
      if ( DEBUG_MSGS ) { 
        char szMsg[256];
        sprintf(szMsg,
      "iExecuteActionFromList Rsl(Redraw)=%d"
      " gbRunning=%d giCheckActions=%d giActionCt=%d\n", 
          iRedrawAction, 
          gbRunning,
          giCheckActions,
          giActionCt
        );
        vTraceMsg(szMsg);
      }
      break;
    }
    while(!gbCheckActions && SDL_PollEvent(&event))
    {
      switch (event.type) {
        case SDL_QUIT:
          gbRunning = FALSE;
          break;
        case SDL_MOUSEBUTTONDOWN:
          if ( iHandleClick(&event) ){
            gbRunning = FALSE;
          }
          break;
        default:
          break;
      }
      
    }
    if ( iRedrawAction == ERROR_WALKING )
      break;

    if ( iRedrawAction != REDRAW_IMAGE && iRedrawAction != -1 ){
      continue;
    }
    vTraceMsg("Draw Scope\n");
    ui64ElapsedTime = SDL_GetTicks64();
    SDL_RenderClear(renderer);

    iColorBoard(renderer);
    
    vSetHUDRectSize(&SDL_RECT_Hud);
    vSetButtonHUDRectSize(&SDL_RECT_ButtonHud);

    SDL_RenderCopyEx(renderer, pSDL_TXTR_ImagePlayer, NULL, &SDL_RECT_Rect, giDeg, NULL, SDL_FLIP_HORIZONTAL);

    vDrawCommandHUD(renderer, pSDL_TXTR_Hud, &SDL_RECT_Hud);
    vDrawButtonHUD(renderer, pSDL_TXTR_ButtonHud, &SDL_RECT_ButtonHud);
    
    vDrawButton(renderer, &SDL_RECT_ButtonArrowRight, BUTTON_DIRECTION);
    vDrawButton(renderer, &SDL_RECT_ButtonTurnArrow, BUTTON_DIRECTION);
    vDrawButton(renderer, &SDL_RECT_ButtonFireLaser, BUTTON_DIRECTION);
    vDrawButton(renderer, &SDL_RECT_ButtonUndoLast, BUTTON_ERASE);
    vDrawButton(renderer, &SDL_RECT_ButtonConfirmAction, BUTTON_CONFIRM);

    SDL_RenderCopy(renderer, pSDL_TXTR_ImageFoward, NULL, &SDL_RECT_ButtonArrowRight);
    SDL_RenderCopy(renderer, pSDL_TXTR_ImageLaser , NULL, &SDL_RECT_ButtonFireLaser);
    SDL_RenderCopy(renderer, pSDL_TXTR_ImageRotate, NULL, &SDL_RECT_ButtonTurnArrow);

    // Update the screen
    SDL_RenderPresent(renderer);
    ui64ElapsedTime -= SDL_GetTicks64();
    if (ui64ElapsedTime <= VSYNC_TIME) SDL_Delay(VSYNC_TIME - ui64ElapsedTime);
    
    iRedrawAction = 0;
  }
  
  // Clean up
  vFreeButtonList();
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
